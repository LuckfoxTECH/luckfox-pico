/*
 * memtester version 4
 *
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2004-2012 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file COPYING for details.
 *
 */

#define __version__ "4.3.0"

#include "types.h"
#include "sizes.h"
#include "tests.h"
#include "drv_heap.h"

#include <stdint.h>
#include <stdlib.h>

#define EXIT_FAIL_NONSTARTER    0x01
#define EXIT_FAIL_ADDRESSLINES  0x02
#define EXIT_FAIL_OTHERTEST     0x04

struct test tests[] =
{
    { "Random Value", test_random_value },
    { "Compare XOR", test_xor_comparison },
    { "Compare SUB", test_sub_comparison },
    { "Compare MUL", test_mul_comparison },
    { "Compare DIV", test_div_comparison },
    { "Compare OR", test_or_comparison },
    { "Compare AND", test_and_comparison },
    { "Sequential Increment", test_seqinc_comparison },
    { "Solid Bits", test_solidbits_comparison },
    { "Block Sequential", test_blockseq_comparison },
    { "Checkerboard", test_checkerboard_comparison },
    { "Bit Spread", test_bitspread_comparison },
    { "Bit Flip", test_bitflip_comparison },
    { "Walking Ones", test_walkbits1_comparison },
    { "Walking Zeroes", test_walkbits0_comparison },
#ifdef TEST_NARROW_WRITES
    { "8-bit Writes", test_8bit_wide_random },
    { "16-bit Writes", test_16bit_wide_random },
#endif
    { NULL, NULL }
};

/* Global vars - so tests have access to this information */
int use_phys = 0;
off_t physaddrbase = 0;

#if 0
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

/* Sanity checks and portability helper macros. */
#ifdef _SC_VERSION
void check_posix_system(void)
{
    if (sysconf(_SC_VERSION) < 198808L)
    {
        fprintf(stderr, "A POSIX system is required.  Don't be surprised if "
                "this craps out.\n");
        fprintf(stderr, "_SC_VERSION is %lu\n", sysconf(_SC_VERSION));
    }
}
#else
#define check_posix_system()
#endif

#ifdef _SC_PAGE_SIZE
int memtester_pagesize(void)
{
    int pagesize = sysconf(_SC_PAGE_SIZE);
    if (pagesize == -1)
    {
        perror("get page size failed");
        exit(EXIT_FAIL_NONSTARTER);
    }
    printf("pagesize is %ld\n", (long) pagesize);
    return pagesize;
}
#else
int memtester_pagesize(void)
{
    printf("sysconf(_SC_PAGE_SIZE) not supported; using pagesize of 8192\n");
    return 8192;
}
#endif

/* Some systems don't define MAP_LOCKED.  Define it to 0 here
   so it's just a no-op when ORed with other constants. */
#ifndef MAP_LOCKED
#define MAP_LOCKED 0
#endif

/* Function declarations */
void usage(char *me);

/* Function definitions */
void usage(char *me)
{
    fprintf(stderr, "\n"
            "Usage: %s [-p physaddrbase [-d device]] <mem>[B|K|M|G] [loops]\n",
            me);
    exit(EXIT_FAIL_NONSTARTER);
}

int test_main(int argc, char **argv)
{
    ul loops, loop, i;
    size_t pagesize, wantraw, wantmb, wantbytes, wantbytes_orig, bufsize,
           halflen, count;
    char *memsuffix, *addrsuffix, *loopsuffix;
    ptrdiff_t pagesizemask;
    void volatile *buf, *aligned;
    ulv *bufa, *bufb;
    int do_mlock = 1, done_mem = 0;
    int exit_code = 0;
    int memfd, opt, memshift;
    size_t maxbytes = -1; /* addressable memory, in bytes */
    size_t maxmb = (maxbytes >> 20) + 1; /* addressable memory, in MB */
    /* Device to mmap memory from with -p, default is normal core */
    char *device_name = "/dev/mem";
    struct stat statbuf;
    int device_specified = 0;
    char *env_testmask = 0;
    ul testmask = 0;

    printf("memtester version " __version__ " (%d-bit)\n", UL_LEN);
    printf("Copyright (C) 2001-2012 Charles Cazabon.\n");
    printf("Licensed under the GNU General Public License version 2 (only).\n");
    printf("\n");
    check_posix_system();
    pagesize = memtester_pagesize();
    pagesizemask = (ptrdiff_t) ~(pagesize - 1);
    printf("pagesizemask is 0x%tx\n", pagesizemask);

    /* If MEMTESTER_TEST_MASK is set, we use its value as a mask of which
       tests we run.
     */
    if (env_testmask = getenv("MEMTESTER_TEST_MASK"))
    {
        errno = 0;
        testmask = strtoul(env_testmask, 0, 0);
        if (errno)
        {
            fprintf(stderr, "error parsing MEMTESTER_TEST_MASK %s: %s\n",
                    env_testmask, strerror(errno));
            usage(argv[0]); /* doesn't return */
        }
        printf("using testmask 0x%lx\n", testmask);
    }

    while ((opt = getopt(argc, argv, "p:d:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            errno = 0;
            physaddrbase = (off_t) strtoull(optarg, &addrsuffix, 16);
            if (errno != 0)
            {
                fprintf(stderr,
                        "failed to parse physaddrbase arg; should be hex "
                        "address (0x123...)\n");
                usage(argv[0]); /* doesn't return */
            }
            if (*addrsuffix != '\0')
            {
                /* got an invalid character in the address */
                fprintf(stderr,
                        "failed to parse physaddrbase arg; should be hex "
                        "address (0x123...)\n");
                usage(argv[0]); /* doesn't return */
            }
            if (physaddrbase & (pagesize - 1))
            {
                fprintf(stderr,
                        "bad physaddrbase arg; does not start on page "
                        "boundary\n");
                usage(argv[0]); /* doesn't return */
            }
            /* okay, got address */
            use_phys = 1;
            break;
        case 'd':
            if (stat(optarg, &statbuf))
            {
                fprintf(stderr, "can not use %s as device: %s\n", optarg,
                        strerror(errno));
                usage(argv[0]); /* doesn't return */
            }
            else
            {
                if (!S_ISCHR(statbuf.st_mode))
                {
                    fprintf(stderr, "can not mmap non-char device %s\n",
                            optarg);
                    usage(argv[0]); /* doesn't return */
                }
                else
                {
                    device_name = optarg;
                    device_specified = 1;
                }
            }
            break;
        default: /* '?' */
            usage(argv[0]); /* doesn't return */
        }
    }

    if (device_specified && !use_phys)
    {
        fprintf(stderr,
                "for mem device, physaddrbase (-p) must be specified\n");
        usage(argv[0]); /* doesn't return */
    }

    if (optind >= argc)
    {
        fprintf(stderr, "need memory argument, in MB\n");
        usage(argv[0]); /* doesn't return */
    }

    errno = 0;
    wantraw = (size_t) strtoul(argv[optind], &memsuffix, 0);
    if (errno != 0)
    {
        fprintf(stderr, "failed to parse memory argument");
        usage(argv[0]); /* doesn't return */
    }
    switch (*memsuffix)
    {
    case 'G':
    case 'g':
        memshift = 30; /* gigabytes */
        break;
    case 'M':
    case 'm':
        memshift = 20; /* megabytes */
        break;
    case 'K':
    case 'k':
        memshift = 10; /* kilobytes */
        break;
    case 'B':
    case 'b':
        memshift = 0; /* bytes*/
        break;
    case '\0':  /* no suffix */
        memshift = 20; /* megabytes */
        break;
    default:
        /* bad suffix */
        usage(argv[0]); /* doesn't return */
    }
    wantbytes_orig = wantbytes = ((size_t) wantraw << memshift);
    wantmb = (wantbytes_orig >> 20);
    optind++;
    if (wantmb > maxmb)
    {
        fprintf(stderr, "This system can only address %llu MB.\n", (ull) maxmb);
        exit(EXIT_FAIL_NONSTARTER);
    }
    if (wantbytes < pagesize)
    {
        fprintf(stderr, "bytes %ld < pagesize %ld -- memory argument too large?\n",
                wantbytes, pagesize);
        exit(EXIT_FAIL_NONSTARTER);
    }

    if (optind >= argc)
    {
        loops = 0;
    }
    else
    {
        errno = 0;
        loops = strtoul(argv[optind], &loopsuffix, 0);
        if (errno != 0)
        {
            fprintf(stderr, "failed to parse number of loops");
            usage(argv[0]); /* doesn't return */
        }
        if (*loopsuffix != '\0')
        {
            fprintf(stderr, "loop suffix %c\n", *loopsuffix);
            usage(argv[0]); /* doesn't return */
        }
    }

    printf("want %lluMB (%llu bytes)\n", (ull) wantmb, (ull) wantbytes);
    buf = NULL;

    if (use_phys)
    {
        memfd = open(device_name, O_RDWR | O_SYNC);
        if (memfd == -1)
        {
            fprintf(stderr, "failed to open %s for physical memory: %s\n",
                    device_name, strerror(errno));
            exit(EXIT_FAIL_NONSTARTER);
        }
        buf = (void volatile *) mmap(0, wantbytes, PROT_READ | PROT_WRITE,
                                     MAP_SHARED | MAP_LOCKED, memfd,
                                     physaddrbase);
        if (buf == MAP_FAILED)
        {
            fprintf(stderr, "failed to mmap %s for physical memory: %s\n",
                    device_name, strerror(errno));
            exit(EXIT_FAIL_NONSTARTER);
        }

        if (mlock((void *) buf, wantbytes) < 0)
        {
            fprintf(stderr, "failed to mlock mmap'ed space\n");
            do_mlock = 0;
        }

        bufsize = wantbytes; /* accept no less */
        aligned = buf;
        done_mem = 1;
    }

    while (!done_mem)
    {
        while (!buf && wantbytes)
        {
            buf = (void volatile *) malloc(wantbytes);
            if (!buf) wantbytes -= pagesize;
        }
        bufsize = wantbytes;
        printf("got  %lluMB (%llu bytes)", (ull) wantbytes >> 20,
               (ull) wantbytes);
        fflush(stdout);
        if (do_mlock)
        {
            printf(", trying mlock ...");
            fflush(stdout);
            if ((size_t) buf % pagesize)
            {
                /* printf("aligning to page -- was 0x%tx\n", buf); */
                aligned = (void volatile *)((size_t) buf & pagesizemask) + pagesize;
                /* printf("  now 0x%tx -- lost %d bytes\n", aligned,
                 *      (size_t) aligned - (size_t) buf);
                 */
                bufsize -= ((size_t) aligned - (size_t) buf);
            }
            else
            {
                aligned = buf;
            }
            /* Try mlock */
            if (mlock((void *) aligned, bufsize) < 0)
            {
                switch (errno)
                {
                case EAGAIN: /* BSDs */
                    printf("over system/pre-process limit, reducing...\n");
                    free((void *) buf);
                    buf = NULL;
                    wantbytes -= pagesize;
                    break;
                case ENOMEM:
                    printf("too many pages, reducing...\n");
                    free((void *) buf);
                    buf = NULL;
                    wantbytes -= pagesize;
                    break;
                case EPERM:
                    printf("insufficient permission.\n");
                    printf("Trying again, unlocked:\n");
                    do_mlock = 0;
                    free((void *) buf);
                    buf = NULL;
                    wantbytes = wantbytes_orig;
                    break;
                default:
                    printf("failed for unknown reason.\n");
                    do_mlock = 0;
                    done_mem = 1;
                }
            }
            else
            {
                printf("locked.\n");
                done_mem = 1;
            }
        }
        else
        {
            done_mem = 1;
            printf("\n");
        }
    }

    if (!do_mlock) fprintf(stderr, "Continuing with unlocked memory; testing "
                               "will be slower and less reliable.\n");

    halflen = bufsize / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *)((size_t) aligned + halflen);

    for (loop = 1; ((!loops) || loop <= loops); loop++)
    {
        printf("Loop %lu", loop);
        if (loops)
        {
            printf("/%lu", loops);
        }
        printf(":\n");
        printf("  %-20s: ", "Stuck Address");
        fflush(stdout);
        if (!test_stuck_address(aligned, bufsize / sizeof(ul)))
        {
            printf("ok\n");
        }
        else
        {
            exit_code |= EXIT_FAIL_ADDRESSLINES;
        }
        for (i = 0;; i++)
        {
            if (!tests[i].name) break;
            /* If using a custom testmask, only run this test if the
               bit corresponding to this test was set by the user.
             */
            if (testmask && (!((1 << i) & testmask)))
            {
                continue;
            }
            printf("  %-20s: ", tests[i].name);
            if (!tests[i].fp(bufa, bufb, count))
            {
                printf("ok\n");
            }
            else
            {
                exit_code |= EXIT_FAIL_OTHERTEST;
            }
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
    if (do_mlock) munlock((void *) aligned, bufsize);
    printf("Done.\n");
    fflush(stdout);
    exit(exit_code);
}

#else
/*extern int _sdata, __bss_end, _sidata, _stext;

void mem_cmp(void)
{
    uint32_t *icache_val, *dcache_val, offset, end;

    end = 1024 * 1024;
    offset = (uint32_t)(&_sdata) - 0x20000000;

    rt_kprintf("mem_cmp, offset=0x%08x, _sdata: %p, stext: %p, dcache flush all first\n",
        offset, &_sdata, &_stext);
    CacheFlushAll();
    for (; offset < end; ) {
        icache_val = (uint32_t *)(0x04000000 + offset);
        dcache_val = (uint32_t *)(0x20000000 + offset);
        if (*icache_val != *dcache_val)
            rt_kprintf("offset=0x%08x, ival=0x%08x, dval=0x%08x\n",
            offset, *icache_val, *dcache_val);
        offset += sizeof(uint32_t);
    }
    rt_kprintf("mem_cmp finish\n");
    while(1);
}

void mem_dump(uint32_t base)
{
    uint32_t *val, offset, end;

    end = 1024 * 1024;
    offset = (uint32_t)(&_sdata) - (uint32_t)(&_stext);

    rt_kprintf("mem_dump, offset=0x%04x, _sdata: %p, stext: %p, dcache flush all first\n",
        offset, &_sdata, &_stext);
    CacheFlushAll();
    for (; offset < end; ) {
        val = (uint32_t *)(base + offset);
        rt_kprintf("offset=0x%08x, val=0x%08x\n", offset, *val);
        offset += sizeof(uint32_t);
    }
    rt_kprintf("mem_dump finish\n");
    while(1);
}*/

static void usage(void)
{
    printf("memtester addr size loop\n");
    printf("ex: memtester 0x1c000000 0x800000 1\n");
}

void memtester(int argc, char **argv)
{
    ul loops, loop, i;
    size_t bufsize, halflen, count;
    void volatile *aligned;
    ulv *bufa, *bufb;
    ul testmask = 0;

    if (argc != 4)
    {
        usage();
        return;
    }

    physaddrbase = strtol(argv[1], NULL, 16);
    bufsize = strtol(argv[2], NULL, 16);
    loops = atoi(argv[3]);
    use_phys = 1;
    aligned = (void volatile *)(physaddrbase & (~0xF));

    rt_kprintf("addr=%p, size=%d, loops=%d\n", physaddrbase, bufsize, loops);

    halflen = bufsize / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *)((size_t) aligned + halflen);

    for (loop = 1; ((!loops) || loop <= loops); loop++)
    {
        printf("Loop %lu", loop);
        if (loops)
        {
            printf("/%lu", loops);
        }
        printf(":\n");
        printf("  %-20s: ", "Stuck Address");

        if (!test_stuck_address(aligned, bufsize / sizeof(ul)))
        {
            printf("ok\n");
        }
        else
        {
            printf("test_stuck_address fail\n");
        }
        for (i = 0;; i++)
        {
            if (!tests[i].name) break;
            /* If using a custom testmask, only run this test if the
               bit corresponding to this test was set by the user.
             */
            if (testmask && (!((1 << i) & testmask)))
            {
                continue;
            }
            printf("  %-20s: ", tests[i].name);
            if (!tests[i].fp(bufa, bufb, count))
            {
                printf("ok\n");
            }
            else
            {
                printf("test %s fail\n", tests[i].name);
            }

        }
        printf("\n");

    }
}

MSH_CMD_EXPORT(memtester, memory tester);
#endif
