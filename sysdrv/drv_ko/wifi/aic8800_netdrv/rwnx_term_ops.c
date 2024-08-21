/*
 ******************************************************************************
 *
 * rwnx_term_ops.c
 *
 * File operations definition for rwnx/term device
 * Copyright (C) RivieraWaves 2017-2018
 *
 ******************************************************************************
 */

#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#include "rwnx_defs.h"
//#include "ipc_fhost.h"

#define RWNX_TERM_IOC_MAGIC  0x7b
#define IPC_FHOST_MSG_BUF_SIZE 512
/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": switch G and S atomically
 * H means "sHift": switch T and Q atomically
 */
#define RWNX_TERM_IOCQCMDSIZE _IO(RWNX_TERM_IOC_MAGIC,  0)
#define RWNX_TERM_IOCGCMD     _IOR(RWNX_TERM_IOC_MAGIC,  1, int)
#define RWNX_TERM_IOC_MAXNR 1

/// Command result
enum rwnx_msg_res {
    RWNX_CMD_SUCCESS,
    RWNX_CMD_ERROR,
    RWNX_CMD_UNKWN_CMD,
    RWNX_CMD_NO_RESP,
};

/**
 * struct rwnx_term_cmd - Command definition
 *
 * @exec: Command processing function
 * @name: Command name
 * @params: Command parameters
 */
struct rwnx_term_cmd {
    int (*exec) (struct rwnx_term *term, char *params);
    char *name;
    char *params;
};

static struct rwnx_term_cmd commands[];
static int commands_size = 0;
static char *fw_commands = NULL;
static int fw_commands_size = 0;
static char ipc_resp[IPC_FHOST_MSG_BUF_SIZE + 1];

/**
 * rwnx_term_stream_read_mod_user() - copy from stream buffer to user buffer
 *
 * @stream: Stream pointer
 * @to: User buffer
 * @size: Size to copy
 *
 * It is assumed that user buffer is at least @size long and stream buffer
 * contains at least @size data.
 */
static size_t rwnx_term_stream_read_mod_user(struct rwnx_term_stream *stream,
                                             char *to, size_t size)
{
    size_t read = size;

    stream->avail -= size;

    if (stream->write <= stream->read) {
        unsigned int avail = stream->buf + sizeof(stream->buf) - stream->read;

        if (avail <= size) {
            read -= copy_to_user(to, stream->read, avail);
            stream->read = stream->buf;
            to += avail;
            size -= avail;
        }
    }

    read -= copy_to_user(to, stream->read, size);
    stream->read += size;

    return read;
}

/**
 * rwnx_term_stream_write_mod() - write inside stream buffer
 *
 * @stream: Stream pointer
 * @src: Pointer to copy
 * @size: Size to copy
 *
 * It is assumed that stream buffer contains at least @size free space.
 */
void rwnx_term_stream_write_mod(struct rwnx_term_stream *stream,
                                char *src, unsigned int size)
{
    unsigned int remain = stream->buf + sizeof(stream->buf) - stream->write;

    if (src && (size == 0)) {
        size = strlen(src);
    }

    stream->avail += size;

    if (remain <= size) {
        memcpy(stream->write, src, remain);
        stream->write = stream->buf;
        src += remain;
        size -= remain;
    }

    memcpy(stream->write, src, size);
    stream->write += size;
}

/**
 * rwnx_term_stream_read_shared() - Copy data from shared buffer to stream buffer
 *
 * @stream: Stream pointer
 *
 * Copy as much data as possible from shared buffer. It never overrides memory
 * in stream buffer that hasn't been read.
 * If missing data is detected, add trace in the stream buffer so that user
 * is informed.
 */
static bool rwnx_term_stream_read_shared(struct rwnx_term_stream *stream)
{
    unsigned int to_read;
    unsigned int free_space = sizeof(stream->buf) - stream->avail;
    char *read;

    spin_lock_bh(&stream->term->buf_lock);

    to_read = stream->term->buf_idx - stream->last_idx;
    if (!to_read)
        goto end;

    if (to_read > TERM_BUFFER_SIZE) {
        int w, miss = to_read - TERM_BUFFER_SIZE;
        char tmp[32];

        w = scnprintf(tmp, sizeof(tmp), "\nLost %d bytes\n", miss);

        if (free_space < w)
            goto end;

        rwnx_term_stream_write_mod(stream, tmp, w);
        free_space -= w;
        stream->last_idx = stream->term->buf_idx - TERM_BUFFER_SIZE;
        to_read = TERM_BUFFER_SIZE;
    }

    if (to_read > free_space)
        to_read = free_space;

    read = stream->term->buf + (stream->last_idx % TERM_BUFFER_SIZE);
    stream->last_idx += to_read;

    if (read + to_read >= stream->term->buf + TERM_BUFFER_SIZE) {
        int to_read1 = (stream->term->buf + TERM_BUFFER_SIZE) - read;

        rwnx_term_stream_write_mod(stream, read, to_read1);
        read = stream->term->buf;
        to_read -= to_read1;
    }

    rwnx_term_stream_write_mod(stream, read, to_read);

  end:
    spin_unlock_bh(&stream->term->buf_lock);

    return (stream->avail > 0);
}

/**
 * rwnx_term_write_shared() - Copy data in the shared buffer
 *
 * @term: Point to &struct rwnx_term to access shared buffer
 * @data: Data to write
 * @size: Size to write
 *
 * Once copy is done all process waiting on &struct rwnx_term.queue are
 * woken up.
 */
void rwnx_term_write_shared(struct rwnx_term *term, char *data, size_t size)
{
    char *write, *write_end;
    size_t w;

    write_end = term->buf + TERM_BUFFER_SIZE;

    if (size > TERM_BUFFER_SIZE) {
        data += size - TERM_BUFFER_SIZE;
        w = TERM_BUFFER_SIZE;
    } else {
        w = size;
    }

    spin_lock_bh(&term->buf_lock);
    write = term->buf + (term->buf_idx % TERM_BUFFER_SIZE);

    if ((write + w) >= write_end) {
        int w_ = write_end - write;
        memcpy(write, data, w_);
        write = term->buf;
        data += w_;
        w -= w_;
    }

    memcpy(write, data, w);
    term->buf_idx += size;
    spin_unlock_bh(&term->buf_lock);
    wake_up_interruptible(&term->queue);
}

/**
 * rwmx_term_cmd_parse_args - Extract parameters form command line
 *
 * @args: Command line to parse
 * @argc: Maximum number to parse.
 * @argv: Table, of size @argc, to store parameter.
 * @remain: Updated with the remaining part of the command line not parsed.
 *
 * Extract parameters from @cmd_line, that will be modified.
 *
 * Return: Number of parameter found
 */
static int rwmx_term_cmd_parse_args(char *cmd_line, int argc, char **argv,
                                    char **remain)
{
    int arg_idx = 0;

    while(1) {
        char *token;

        if (!cmd_line)
            break;

        if (cmd_line[0] == '"') {
            cmd_line++;
            token = strsep(&cmd_line, "\"");
        } else if (cmd_line[0] == '\'') {
            cmd_line++;
            token = strsep(&cmd_line, "'");
        } else {
            token = strsep(&cmd_line, " \t\n");
        }

        if (!token) {
            break;
        } else if (token[0] == '\0') {
            continue;
        } else {
            argv[arg_idx++] = token;
        }

        if (arg_idx == argc)
            break;
    }

    *remain = cmd_line;
    return arg_idx;
}

/**
 * rwmx_term_cmd_write() - Process function for write command
 *
 * @term: Pointer to &struct rwnx_sterm
 * @params: Buffer to write in shared buffer
 *
 * Write buffer provided in argument to the shared buffer.
 * (for test purpose only)
 */
static int rwmx_term_cmd_write(struct rwnx_term *term, char *params)
{
    if (params)
        rwnx_term_write_shared(term, params, strlen(params));
    return 0;
}

/**
 * rwmx_term_cmd_clear() - Process function for clear command
 *
 * @term: Pointer to &struct rwnx_sterm
 * @params: Buffer to write in shared buffer
 *
 * Reset number of bytes written in share buffer since driver creation
 */
static int rwmx_term_cmd_clear(struct rwnx_term *term, char *params)
{
    struct rwnx_term_stream *stream;
    term->buf_idx = 0;
    list_for_each_entry(stream, &term->stream_list, list) {
        stream->last_idx = 0;
    }
    return 0;
}

/**
 * rwmx_term_cmd_reload() - Process function for reload command
 *
 * @term: Pointer to &struct rwnx_sterm
 * @params: Unused
 *
 * Reload and restart the firmware
 */
static int rwmx_term_cmd_reload(struct rwnx_term *term, char *params)
{
    struct rwnx_hw *rwnx_hw = container_of(term, struct rwnx_hw, term);
    void *config = NULL;

//    if (rwnx_trace_pre_reload(rwnx_hw))
  //      return -EIO;

    //rwnx_platform_off(rwnx_hw, &config);
    if (fw_commands) {
        kfree(fw_commands);
        fw_commands = NULL;
    }
    fw_commands_size = commands_size = 0;
    rwmx_term_cmd_clear(term, params);
    rwnx_platform_on(rwnx_hw, config);
    if (config)
        kfree(config);
    //rwnx_trace_post_reload(rwnx_hw);
    return 0;
}

static struct rwnx_term_cmd commands[] = {
    {rwmx_term_cmd_reload, "reload", ""},
    {rwmx_term_cmd_clear, "clear", ""},
    {rwmx_term_cmd_write, "write", "<pattern to write on driver buffer>"},
    {NULL,"",""}
};

/**
 * rwmx_term_fw_cmd() - Default action, forward command to fw
 *
 * @stream: Stream pointer used to print command output
 * @command: command
 *
 * Forward command to FW.
 */
static int rwmx_term_fw_cmd(struct rwnx_term_stream *stream, char *command)
{
   // struct rwnx_hw *rwnx_hw = container_of(stream->term, struct rwnx_hw, term);
   // return rwnx_fhost_send_msg(rwnx_hw, stream, command, ipc_resp);
       return 0;
}

/**
 * rwnx_term_open() - Open a stream on rwnx/term device
 *
 */
static int rwnx_term_open(struct inode *inodep, struct file *filep)
{
    struct rwnx_term *term;
    struct rwnx_term_stream *stream;

    stream = kmalloc(sizeof(*stream), GFP_KERNEL);
    if (!stream)
        return -ENOMEM;

    term = container_of(inodep->i_cdev, struct rwnx_term, cdev);
    stream->term = term;
    stream->read = stream->buf;
    stream->write = stream->buf;
    stream->avail = 0;
    stream->last_idx = 0;
    rwnx_term_stream_read_shared(stream);
    filep->private_data = stream;

    list_add(&stream->list, &term->stream_list);
    return 0;
}

/**
 * rwnx_term_release() - Close a stream
 *
 */
static int rwnx_term_release(struct inode *inodep, struct file *filep)
{
    struct rwnx_term_stream *stream = filep->private_data;

    if (filep->private_data)
    {
        list_del(&stream->list);
        kfree(filep->private_data);
    }

    return 0;
}

/**
 * rwnx_term_read() - Read data from stream buffer
 *
 */
static ssize_t rwnx_term_read(struct file *filep, char *buffer,
                              size_t len, loff_t *offset)
{
    struct rwnx_term_stream *stream = filep->private_data;
    size_t read;

    while (! stream->avail ) {
        if (filep->f_flags & O_NONBLOCK)
            return -EAGAIN;

        if (wait_event_interruptible(stream->term->queue,
                                     rwnx_term_stream_read_shared(stream)))
            return -ERESTARTSYS;
    }

    read = min_t(size_t, stream->avail, len);
    read = rwnx_term_stream_read_mod_user(stream, buffer, read);
    rwnx_term_stream_read_shared(stream);

    return read;
}

/**
 * rwnx_term_poll() - Poll data from stream buffer
 *
 */
static unsigned int rwnx_term_poll (struct file *filep, poll_table *wait)
{
    struct rwnx_term_stream *stream = filep->private_data;
    unsigned int mask = 0;

    poll_wait(filep, &stream->term->queue, wait);
    rwnx_term_stream_read_shared(stream);
    if (stream->avail)
        mask |= POLLIN | POLLRDNORM;
    mask |= POLLOUT | POLLWRNORM;   /* always writable */

    return mask;
}

/**
 * rwnx_term_write() - Send a command to rwnx/term device
 *
 */
static ssize_t rwnx_term_write(struct file *filep, const char *buffer,
                               size_t len, loff_t *offset)
{
    struct rwnx_term_stream *stream = filep->private_data;
    char *cmd_buf = NULL;
    char *command = NULL;
    char *params  = NULL;
    size_t ret = len;
    bool found = false;
    int i;
    int res;
    size_t resp_len;

    cmd_buf = kmalloc(len + 1, GFP_KERNEL);
    if (! cmd_buf)
        return -ENOMEM;

    if (copy_from_user(cmd_buf, buffer, len)) {
        ret = -EFAULT;
        goto end;
    }
    cmd_buf[len] = '\0';

    if (!rwmx_term_cmd_parse_args(cmd_buf, 1, &command, &params))
        goto end;

    i = 0;
    while (commands[i].exec)
    {
        if (!strcmp(commands[i].name, command)) {
            found = true;
            if (down_interruptible(&stream->term->sem)) {
                ret = -ERESTARTSYS;
                goto end;
            }
            res = commands[i].exec(stream->term, params);
            up(&stream->term->sem);
            if (res < 0) {
                resp_len = scnprintf(ipc_resp, sizeof(ipc_resp),
                                     "Error: %d\n", res);
                rwnx_term_stream_write_mod(stream, ipc_resp, resp_len);
            }
            break;
        }
        i++;
    }

    if (!found) {
        /* forward command to FW */
        ipc_resp[0] = '\0';

        /* rewrite original command */
        if (params)
            scnprintf(cmd_buf, len + 1, "%s %s", command, params);
        else
            scnprintf(cmd_buf, len + 1, "%s", command);

        res = rwmx_term_fw_cmd(stream, cmd_buf);

        if (res < 0) {
            resp_len = scnprintf(ipc_resp, sizeof(ipc_resp),
                                "Error: %d\n", res);
        } else {
            u32_l status = *((u32_l *) ipc_resp);
            if (status != RWNX_CMD_SUCCESS) {
                resp_len = scnprintf(ipc_resp, sizeof(status),
                                     "Error: %d\n", status);

                rwnx_term_stream_write_mod(stream, ipc_resp, resp_len);
            }
        }
    }

  end:
    if (cmd_buf)
        kfree(cmd_buf);

    return ret;
}

/**
 * rwnx_term_ioctl() - Handles ioctl commands
 *
 */
static long rwnx_term_ioctl(struct file *filep, unsigned int cmd,
                            unsigned long arg)
{
    struct rwnx_term_stream *stream = filep->private_data;
    //struct rwnx_hw *rwnx_hw = container_of(stream->term, struct rwnx_hw, term);
    char *buf;
    long ret = 0;
    int i, w = 0;
    int /* resp, */ nb_read;

    if (_IOC_TYPE(cmd) != RWNX_TERM_IOC_MAGIC)
        return -ENOTTY;
    if (_IOC_NR(cmd) > RWNX_TERM_IOC_MAXNR)
        return -ENOTTY;

    switch(cmd) {
        case RWNX_TERM_IOCQCMDSIZE:
            /* Query size of the buffer to retrieve command list */
            ret = commands_size;
            if (!commands_size) {
                char *src;
                /* get driver commands size */
                i = 0;
                while (commands[i].exec)
                {
                    commands_size += strlen(commands[i].name) +
                        strlen(commands[i].params) + 2;
                    i++;
                }
                commands_size += 1;

                /* get FW commands size */
                fw_commands_size = 0;
                nb_read = stream->avail;

                src = stream->write;
                //ret = rwnx_fhost_send_msg(rwnx_hw, stream, "list_cmd", (char*) &resp);
                if (ret)
                    break;

                nb_read = stream->avail - nb_read;
                stream->write = src;

                fw_commands_size += nb_read;

                if (fw_commands_size) {
                    unsigned int remain = stream->buf + sizeof(stream->buf) - src;
                    char *to;

                    fw_commands_size ++;
                    fw_commands = kmalloc(fw_commands_size, GFP_KERNEL);
                    if (!fw_commands) {
                        ret = -ENOMEM;
                        break;
                    }

                    stream->avail -= nb_read;
                    to = fw_commands;
                    if (remain <= nb_read) {
                        memcpy(to, src, remain);
                        src = stream->buf;
                        to += remain;
                        nb_read -= remain;
                    }
                    memcpy(to, src, nb_read);
                    fw_commands[fw_commands_size - 1] = '\0';
                    commands_size += fw_commands_size;
                }

                ret = commands_size;
            }
            break;
        case RWNX_TERM_IOCGCMD:
            /* Get command list */
            if (!commands_size)
                return -EINVAL;

            buf = kmalloc(commands_size, GFP_KERNEL);
            if (!buf)
                return -ENOMEM;

            i = 0;
            w = 0;
            while (commands[i].exec)
            {
                w += scnprintf(&buf[w], commands_size - w, "%s %s\n",
                               commands[i].name, commands[i].params);
                i++;
            }

            if (fw_commands_size) {
                /* sanity check */
                if (fw_commands_size + w > commands_size) {
                    pr_err("w=%d fw_commands_size=%d commands_size=%d\n",
                            w, fw_commands_size, commands_size);
                } else {
                    memcpy(&buf[w], fw_commands, fw_commands_size);
                }
            }

            if (copy_to_user((void *)arg, buf, commands_size))
                ret = -EINVAL;
            kfree(buf);
            break;
        default:  /* redundant, as cmd was checked against MAXNR */
            return -ENOTTY;
    }

    return ret;
}

/**
 * rwnx_term_ops_exit() - Free memory allocated
 */
void rwnx_term_ops_exit(void)
{
    if (fw_commands) {
        kfree(fw_commands);
        fw_commands = NULL;
    }
}

struct file_operations rwnx_term_ops = {
    .open = rwnx_term_open,
    .read = rwnx_term_read,
    .write = rwnx_term_write,
    .release = rwnx_term_release,
    .poll = rwnx_term_poll,
    .unlocked_ioctl = rwnx_term_ioctl,
};
