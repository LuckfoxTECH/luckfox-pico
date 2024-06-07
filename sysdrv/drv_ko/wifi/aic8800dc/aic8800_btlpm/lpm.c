/*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 */

#define DEBUG

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>

#include <linux/irq.h>
#include <linux/param.h>
#include <linux/bitops.h>
#include <linux/termios.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
#include <linux/pm_wakeirq.h>
#endif
#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#include <linux/serial_core.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
#include <linux/wakelock.h>
#endif

/*
 * #define BT_SLEEP_DBG
 */
#define BT_SLEEP_DBG
#undef BT_DBG
#undef BT_ERR
#ifdef BT_SLEEP_DBG
#define BT_DBG(fmt, arg...) pr_debug("[BT_LPM] %s: " fmt "\n",\
				__func__, ## arg)
#else
#define BT_DBG(fmt, arg...)
#endif
#define BT_ERR(fmt, arg...) pr_debug("[BT_LPM] %s: " fmt "\n",\
				__func__, ## arg)

/*
 * Defines
 */

#define VERSION		"1.3.3"
#define PROC_DIR	"bluetooth/sleep"

#define DEFAULT_UART_INDEX   1
#define BT_BLUEDROID_SUPPORT 1
static int bluesleep_start(void);
static void bluesleep_stop(void);

struct bluesleep_info {
	unsigned int wakeup_enable;
	unsigned host_wake;
	unsigned ext_wake;
	unsigned host_wake_irq;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	struct wakeup_source *ws;
#else
	struct wake_lock wake_lock;
#endif
	struct uart_port *uport;
	unsigned host_wake_assert:1;
	unsigned ext_wake_assert:1;
	struct platform_device *pdev;
};

/* work function */
static void bluesleep_sleep_work(struct work_struct *work);
static void bluesleep_tx_allow_sleep(void);

/* work queue */
DECLARE_DELAYED_WORK(sleep_workqueue, bluesleep_sleep_work);

/* Macros for handling sleep work */
#define bluesleep_rx_busy()     schedule_delayed_work(&sleep_workqueue, 0)
#define bluesleep_tx_busy()     schedule_delayed_work(&sleep_workqueue, 0)
#define bluesleep_rx_idle()     schedule_delayed_work(&sleep_workqueue, 0)
#define bluesleep_tx_idle()     schedule_delayed_work(&sleep_workqueue, 0)

/* 1 second timeout */
#define RX_TIMER_INTERVAL  1

/* state variable names and bit positions */
#define BT_PROTO	0x01
#define BT_TXDATA	0x02
#define BT_ASLEEP	0x04
#define BT_RXTIMER	0x20
#define BT_TXIDLE	0x08

#if BT_BLUEDROID_SUPPORT
static bool has_lpm_enabled;
#else
/* global pointer to a single hci device. */
static struct hci_dev *bluesleep_hdev;
#endif

#if BT_BLUEDROID_SUPPORT
static struct platform_device *bluesleep_uart_dev;
#endif
static struct bluesleep_info *bsi;

/* module usage */
static atomic_t open_count = ATOMIC_INIT(1);

/*
 * Local function prototypes
 */

#if !BT_BLUEDROID_SUPPORT
static int bluesleep_hci_event(struct notifier_block *this,
				unsigned long event, void *data);
#endif

/*
 * Global variables
 */

/** Global state flags */
static unsigned long flags;

/** Tasklet to respond to change in hostwake line */
static struct tasklet_struct hostwake_task;

/** Reception timer */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
static void bluesleep_rx_timer_expire(struct timer_list *t);
#else
static void bluesleep_rx_timer_expire(unsigned long data);
#endif
static struct timer_list rx_timer;

/** Lock for state transitions */
static spinlock_t rw_lock;

#if !BT_BLUEDROID_SUPPORT
/** Notifier block for HCI events */
struct notifier_block hci_event_nblock = {
	.notifier_call = bluesleep_hci_event,
};
#endif

struct proc_dir_entry *bluetooth_dir, *sleep_dir;

/*
 * Local functions
 */

/*
 * bt go to sleep will call this function tell uart stop data interactive
 */
static void hsuart_power(int on)
{
	if (bsi->uport != NULL) {
		if (on)
			bsi->uport->ops->set_mctrl(bsi->uport, TIOCM_RTS);
		else
			bsi->uport->ops->set_mctrl(bsi->uport, 0);
	} else {
		BT_ERR("bsi->uport = NULL, has_lpm_enabled = %d", has_lpm_enabled);
	}
}

/**
 * @return 1 if the Host can go to sleep, 0 otherwise.
 */
static inline int bluesleep_can_sleep(void)
{
	/* check if HOST_WAKE_BT_GPIO and BT_WAKE_HOST_GPIO
	 * are both deasserted
	 */
	return (gpio_get_value(bsi->ext_wake) != bsi->ext_wake_assert) &&
		(gpio_get_value(bsi->host_wake) != bsi->host_wake_assert) &&
		(!test_bit(BT_RXTIMER, &flags)) && (bsi->uport != NULL);
}

/**
 * @brief@  main sleep work handling function which update the flags
 * and activate and deactivate UART ,check FIFO.
 */
static void bluesleep_sleep_work(struct work_struct *work)
{
	if (!has_lpm_enabled)
		return;

	if (bluesleep_can_sleep()) {
		/* already asleep, this is an error case */
		if (test_bit(BT_ASLEEP, &flags)) {
			BT_DBG("already asleep");
			return;
		}
		if (bsi->uport->ops->tx_empty(bsi->uport)) {
			BT_DBG("going to sleep...");
			set_bit(BT_ASLEEP, &flags);
			/*Deactivating UART */
			hsuart_power(0);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
			__pm_wakeup_event(bsi->ws, HZ / 2);
#else
			wake_lock_timeout(&bsi->wake_lock, HZ / 2);
#endif
		} else {
			BT_DBG("This should never happen.\n");
			return;
		}
	} else if (test_bit(BT_ASLEEP, &flags)) {
		BT_DBG("hold wake locks for rx_task.");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
		__pm_stay_awake(bsi->ws);
#else
		wake_lock(&bsi->wake_lock);
#endif
		clear_bit(BT_ASLEEP, &flags);

		/* Add a timer to make sure that UART
		 * would not be turned on&off very frequentently
		 */
		mod_timer(&rx_timer, jiffies + (RX_TIMER_INTERVAL * HZ));

		set_bit(BT_RXTIMER, &flags);
		hsuart_power(1);
	} else {
		mod_timer(&rx_timer, jiffies + (RX_TIMER_INTERVAL * HZ));
		if (gpio_get_value(bsi->ext_wake) != bsi->ext_wake_assert
					&& !test_bit(BT_TXIDLE, &flags)) {
			BT_DBG("force retrigger bt wake:%lx", flags);
			gpio_set_value(bsi->ext_wake, bsi->ext_wake_assert);
			msleep(20);
			gpio_set_value(bsi->ext_wake, !bsi->ext_wake_assert);
		}
	}
}

/**
 * A tasklet function that runs in tasklet context and reads the value
 * of the HOST_WAKE GPIO pin and further defer the work.
 * @param data Not used.
 */
static void bluesleep_hostwake_task(unsigned long data)
{
	BT_DBG("hostwake line change");
	spin_lock(&rw_lock);

	if (gpio_get_value(bsi->host_wake) == bsi->host_wake_assert)
		bluesleep_rx_busy();
	else
		bluesleep_rx_idle();

	spin_unlock(&rw_lock);
}

/**
 * Handles proper timer action when outgoing data is delivered to the
 * HCI line discipline. Sets BT_TXDATA.
 */
static void bluesleep_outgoing_data(void)
{
	unsigned long irq_flags;
	int power_on_uart = 0;

	spin_lock_irqsave(&rw_lock, irq_flags);

	/* if the tx side is sleeping... */
	if (gpio_get_value(bsi->ext_wake) != bsi->ext_wake_assert) {
		BT_DBG("tx was sleeping, wakeup it");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
		__pm_stay_awake(bsi->ws);
#else
		wake_lock(&bsi->wake_lock);
#endif
		gpio_set_value(bsi->ext_wake, bsi->ext_wake_assert);
		clear_bit(BT_ASLEEP, &flags);
		clear_bit(BT_TXIDLE, &flags);
		power_on_uart = 1;
	}

	spin_unlock_irqrestore(&rw_lock, irq_flags);
	if (power_on_uart == 1)
		hsuart_power(1);
}

#if BT_BLUEDROID_SUPPORT
static struct uart_port *bluesleep_get_uart_port(void)
{
	struct uart_port *uport = NULL;

	if (bluesleep_uart_dev) {
		uport = platform_get_drvdata(bluesleep_uart_dev);
		if (uport)
			BT_DBG(
			"%s get uart_port from blusleep_uart_dev: %s, port irq: %d",
			__func__, bluesleep_uart_dev->name, uport->irq);
	}
	return uport;
}

static int bluesleep_lpm_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "lpm enable: %d\n", has_lpm_enabled);
	return 0;
}

static int bluesleep_lpm_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, bluesleep_lpm_proc_show, NULL);
}

static ssize_t bluesleep_write_proc_lpm(struct file *file,
				const char __user *buffer,
				size_t count, loff_t *pos)
{
	char b;

	if (count < 1)
		return -EINVAL;

	if (copy_from_user(&b, buffer, 1))
		return -EFAULT;

	if (b == '0') {
		/* HCI_DEV_UNREG */
		bluesleep_stop();
		has_lpm_enabled = false;
		bsi->uport = NULL;
	} else {
		/* HCI_DEV_REG */
		if (!has_lpm_enabled) {
			has_lpm_enabled = true;
			if (bluesleep_uart_dev)
				bsi->uport = bluesleep_get_uart_port();

			/* if bluetooth started, start bluesleep*/
			bluesleep_start();
		}
	}

	return count;
}

static int bluesleep_btwrite_proc_show(struct seq_file *m, void *v)
{
	seq_puts(m, "it's not support\n");
	return 0;
}

static int bluesleep_btwrite_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, bluesleep_btwrite_proc_show, NULL);
}

static ssize_t bluesleep_write_proc_btwrite(struct file *file,
				const char __user *buffer,
				size_t count, loff_t *pos)
{
	char b;

	if (count < 1)
		return -EINVAL;

	if (copy_from_user(&b, buffer, 1))
		return -EFAULT;

	/* HCI_DEV_WRITE */
	if (b != '0')
		bluesleep_outgoing_data();
	else
		bluesleep_tx_allow_sleep();

	return count;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
static const struct proc_ops lpm_fops = {
	.proc_open	= bluesleep_lpm_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
	.proc_write	= bluesleep_write_proc_lpm,
};
static const struct proc_ops btwrite_fops = {
	.proc_open	= bluesleep_btwrite_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
	.proc_write	= bluesleep_write_proc_btwrite,
};
#else
static const struct file_operations lpm_fops = {
	.owner		= THIS_MODULE,
	.open		= bluesleep_lpm_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= bluesleep_write_proc_lpm,
};
static const struct file_operations btwrite_fops = {
	.owner		= THIS_MODULE,
	.open		= bluesleep_btwrite_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= bluesleep_write_proc_btwrite,
};
#endif
#else
/**
 * Handles HCI device events.
 * @param this Not used.
 * @param event The event that occurred.
 * @param data The HCI device associated with the event.
 * @return <code>NOTIFY_DONE</code>.
 */
static int bluesleep_hci_event(struct notifier_block *this,
				unsigned long event, void *data)
{
	struct hci_dev *hdev = (struct hci_dev *) data;
	struct hci_uart *hu;
	struct uart_state *state;

	if (!hdev)
		return NOTIFY_DONE;

	switch (event) {
	case HCI_DEV_REG:
		if (!bluesleep_hdev) {
			bluesleep_hdev = hdev;
			hu  = (struct hci_uart *) hdev->driver_data;
			state = (struct uart_state *) hu->tty->driver_data;
			bsi->uport = state->uart_port;
		}
		break;
	case HCI_DEV_UNREG:
		bluesleep_hdev = NULL;
		bsi->uport = NULL;
		break;
	case HCI_DEV_WRITE:
		bluesleep_outgoing_data();
		break;
	}

	return NOTIFY_DONE;
}
#endif

/**
 * Function to check wheather bluetooth can sleep when btwrite was deasserted
 * by bluedroid.
 */
static void bluesleep_tx_allow_sleep(void)
{
	unsigned long irq_flags;
	BT_DBG("Tx has been idle\n");
	spin_lock_irqsave(&rw_lock, irq_flags);
	gpio_set_value(bsi->ext_wake, !bsi->ext_wake_assert);
	set_bit(BT_TXIDLE, &flags);
	bluesleep_tx_idle();
	spin_unlock_irqrestore(&rw_lock, irq_flags);
}


/* Handles reception timer expiration.
 * Clear BT_RXTIMER.
 * @param data Not used.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
static void bluesleep_rx_timer_expire(struct timer_list *t)
#else
static void bluesleep_rx_timer_expire(unsigned long data)
#endif
{
	BT_DBG("bluesleep_rx_timer_expire");
	clear_bit(BT_RXTIMER, &flags);
	bluesleep_rx_idle();
}

/**
 * Schedules a tasklet to run when receiving an interrupt on the
 * <code>HOST_WAKE</code> GPIO pin.
 * @param irq Not used.
 * @param dev_id Not used.
 */
static irqreturn_t bluesleep_hostwake_isr(int irq, void *dev_id)
{
	/* schedule a tasklet to handle the change in the host wake line */
	tasklet_schedule(&hostwake_task);
	return IRQ_HANDLED;
}

/**
 * Starts the Sleep-Mode Protocol on the Host.
 * @return On success, 0. On error, -1, and <code>errno</code> is set
 * appropriately.
 */
static int bluesleep_start(void)
{
	int retval;
	unsigned long irq_flags;

	spin_lock_irqsave(&rw_lock, irq_flags);

	if (test_bit(BT_PROTO, &flags)) {
		spin_unlock_irqrestore(&rw_lock, irq_flags);
		return 0;
	}

	spin_unlock_irqrestore(&rw_lock, irq_flags);

	if (!atomic_dec_and_test(&open_count)) {
		atomic_inc(&open_count);
		return -EBUSY;
	}

	/* start the timer */
	mod_timer(&rx_timer, jiffies + (RX_TIMER_INTERVAL*HZ));
	/*deassert BT_WAKE first*/
	gpio_set_value(bsi->ext_wake, !bsi->ext_wake_assert);
	msleep(20);

	/* assert BT_WAKE */
	gpio_set_value(bsi->ext_wake, bsi->ext_wake_assert);
	retval = request_irq(bsi->host_wake_irq, bluesleep_hostwake_isr,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"bluetooth hostwake", &bsi->pdev->dev);
	if (retval  < 0) {
		BT_ERR("Couldn't acquire BT_HOST_WAKE IRQ");
		goto fail;
	}

	set_bit(BT_PROTO, &flags);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	__pm_stay_awake(bsi->ws);
#else
	wake_lock(&bsi->wake_lock);
#endif

	return 0;
fail:
	del_timer(&rx_timer);
	atomic_inc(&open_count);

	return retval;
}

/**
 * Stops the Sleep-Mode Protocol on the Host.
 */
static void bluesleep_stop(void)
{
	unsigned long irq_flags;

	spin_lock_irqsave(&rw_lock, irq_flags);

	if (!test_bit(BT_PROTO, &flags)) {
		spin_unlock_irqrestore(&rw_lock, irq_flags);
		return;
	}

	/* assert BT_WAKE */
	gpio_set_value(bsi->ext_wake, bsi->ext_wake_assert);

	del_timer(&rx_timer);
	clear_bit(BT_PROTO, &flags);

	if (test_bit(BT_ASLEEP, &flags)) {
		clear_bit(BT_ASLEEP, &flags);
		hsuart_power(1);
	}

	atomic_inc(&open_count);

	spin_unlock_irqrestore(&rw_lock, irq_flags);
	free_irq(bsi->host_wake_irq, &bsi->pdev->dev);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	__pm_wakeup_event(bsi->ws, HZ / 2);
#else
	wake_lock_timeout(&bsi->wake_lock, HZ / 2);
#endif
}
#if 0
/**
 * Read the <code>BT_WAKE</code> GPIO pin value via the proc interface.
 * When this function returns, <code>page</code> will contain a 1 if the
 * pin is high, 0 otherwise.
 * @param page Buffer for writing data.
 * @param start Not used.
 * @param offset Not used.
 * @param count Not used.
 * @param eof Whether or not there is more data to be read.
 * @param data Not used.
 * @return The number of bytes written.
 */
static int bluepower_read_proc_btwake(char *page, char **start, off_t offset,
					int count, int *eof, void *data)
{
	*eof = 1;
	return sprintf(page, "btwake:%u\n",
		(gpio_get_value(bsi->ext_wake) == bsi->ext_wake_assert));
}

/**
 * Write the <code>BT_WAKE</code> GPIO pin value via the proc interface.
 * @param file Not used.
 * @param buffer The buffer to read from.
 * @param count The number of bytes to be written.
 * @param data Not used.
 * @return On success, the number of bytes written. On error, -1, and
 * <code>errno</code> is set appropriately.
 */
static int bluepower_write_proc_btwake(struct file *file, const char *buffer,
					unsigned long count, void *data)
{
	char *buf;

	if (count < 1)
		return -EINVAL;

	buf = kmalloc(count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (copy_from_user(buf, buffer, count)) {
		kfree(buf);
		return -EFAULT;
	}

	if (buf[0] == '0') {
		gpio_set_value(bsi->ext_wake, !bsi->ext_wake_assert);
	} else if (buf[0] == '1') {
		gpio_set_value(bsi->ext_wake, bsi->ext_wake_assert);
	} else {
		kfree(buf);
		return -EINVAL;
	}

	kfree(buf);
	return count;
}

/**
 * Read the <code>BT_HOST_WAKE</code> GPIO pin value via the proc interface.
 * When this function returns, <code>page</code> will contain a 1 if the pin
 * is high, 0 otherwise.
 * @param page Buffer for writing data.
 * @param start Not used.
 * @param offset Not used.
 * @param count Not used.
 * @param eof Whether or not there is more data to be read.
 * @param data Not used.
 * @return The number of bytes written.
 */
static int bluepower_read_proc_hostwake(char *page, char **start, off_t offset,
					int count, int *eof, void *data)
{
	*eof = 1;
	return sprintf(page, "hostwake: %u\n",
		(gpio_get_value(bsi->host_wake) == bsi->host_wake_assert));
}


/**
 * Read the low-power status of the Host via the proc interface.
 * When this function returns, <code>page</code> contains a 1 if the Host
 * is asleep, 0 otherwise.
 * @param page Buffer for writing data.
 * @param start Not used.
 * @param offset Not used.
 * @param count Not used.
 * @param eof Whether or not there is more data to be read.
 * @param data Not used.
 * @return The number of bytes written.
 */
static int bluesleep_read_proc_asleep(char *page, char **start, off_t offset,
					int count, int *eof, void *data)
{
	unsigned int asleep;

	asleep = test_bit(BT_ASLEEP, &flags) ? 1 : 0;
	*eof = 1;
	return sprintf(page, "asleep: %u\n", asleep);
}

/**
 * Read the low-power protocol being used by the Host via the proc interface.
 * When this function returns, <code>page</code> will contain a 1 if the Host
 * is using the Sleep Mode Protocol, 0 otherwise.
 * @param page Buffer for writing data.
 * @param start Not used.
 * @param offset Not used.
 * @param count Not used.
 * @param eof Whether or not there is more data to be read.
 * @param data Not used.
 * @return The number of bytes written.
 */
static int bluesleep_read_proc_proto(char *page, char **start, off_t offset,
					int count, int *eof, void *data)
{
	unsigned int proto;

	proto = test_bit(BT_PROTO, &flags) ? 1 : 0;
	*eof = 1;
	return sprintf(page, "proto: %u\n", proto);
}

/**
 * Modify the low-power protocol used by the Host via the proc interface.
 * @param file Not used.
 * @param buffer The buffer to read from.
 * @param count The number of bytes to be written.
 * @param data Not used.
 * @return On success, the number of bytes written. On error, -1, and
 * <code>errno</code> is set appropriately.
 */
static int bluesleep_write_proc_proto(struct file *file, const char *buffer,
					unsigned long count, void *data)
{
	char proto;

	if (count < 1)
		return -EINVAL;

	if (copy_from_user(&proto, buffer, 1))
		return -EFAULT;

	if (proto == '0')
		bluesleep_stop();
	else
		bluesleep_start();

	/* claim that we wrote everything */
	return count;
}
#endif

static int assert_level = -1;
module_param(assert_level, int, S_IRUGO);
MODULE_PARM_DESC(assert_level, "BT_LPM hostwake/btwake assert level");

#if 1
static struct platform_device *sw_uart_get_pdev(int id)
{
	struct device_node *np;
	char match[20];
	sprintf(match, "uart%d", id);
	np = of_find_node_by_type(NULL, match);
	return of_find_device_by_node(np);
}
#endif

static int bluesleep_probe(struct platform_device *pdev)
{
#if 1
	struct device_node *np = of_find_compatible_node(NULL, NULL, "allwinner,sunxi-btlpm");
	struct device *dev = &pdev->dev;
	enum of_gpio_flags config;
	int ret, uart_index;
	u32 val;

	bsi = devm_kzalloc(&pdev->dev, sizeof(struct bluesleep_info),
			GFP_KERNEL);
	if (!bsi)
		return -ENOMEM;

	bsi->host_wake = of_get_named_gpio_flags(np, "bt_hostwake", 0, &config);
	if (!gpio_is_valid(bsi->host_wake)) {
		BT_ERR("get gpio bt_hostwake failed\n");
		ret = -EINVAL;
		goto err0;
	}

	/* set host_wake_assert */
	bsi->host_wake_assert = (config == OF_GPIO_ACTIVE_LOW) ? 0 : 1;
	BT_DBG("bt_hostwake gpio=%d assert=%d\n", bsi->host_wake, bsi->host_wake_assert);

	if (assert_level != -1) {
		bsi->host_wake_assert = (assert_level & 0x02) > 0;
		BT_DBG("override host_wake assert to %d", bsi->host_wake_assert);
	}

	ret = devm_gpio_request(dev, bsi->host_wake, "bt_hostwake");
	if (ret < 0) {
		BT_ERR("can't request bt_hostwake gpio %d\n",
			bsi->host_wake);
		goto err0;
	}
	ret = gpio_direction_input(bsi->host_wake);
	if (ret < 0) {
		BT_ERR("can't request input direction bt_wake gpio %d\n",
			bsi->host_wake);
		goto err1;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
	if (!of_property_read_bool(np, "wakeup-source")) {
#else
	if (!of_property_read_u32(np, "wakeup-source", &bsi->wakeup_enable) &&
		(bsi->wakeup_enable == 0)) {
#endif
		BT_DBG("wakeup source is disabled!\n");
	} else {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
		ret = device_init_wakeup(dev, true);
		if (ret < 0) {
			BT_ERR("device init wakeup failed!\n");
			goto err1;
		}
		ret = dev_pm_set_wake_irq(dev, gpio_to_irq(bsi->host_wake));
		if (ret < 0) {
			BT_ERR("can't enable wakeup src for bt_hostwake %d\n",
				bsi->host_wake);
			goto err2;
		}
		bsi->wakeup_enable = 1;
#else
			BT_ERR("%s kernel unsupport this feature!\r\n", __func__);
#endif
	}

	bsi->ext_wake = of_get_named_gpio_flags(np, "bt_wake", 0, &config);
	if (!gpio_is_valid(bsi->ext_wake)) {
		BT_ERR("get gpio bt_wake failed\n");
		ret = -EINVAL;
		goto err2;
	}

	ret = devm_gpio_request(dev, bsi->ext_wake, "bt_wake");
	if (ret < 0) {
		BT_ERR("can't request bt_wake gpio %d\n",
			bsi->ext_wake);
		goto err2;
	}

	/* set ext_wake_assert */
	bsi->ext_wake_assert = (config == OF_GPIO_ACTIVE_LOW) ? 0 : 1;
	BT_DBG("bt_wake gpio=%d assert=%d\n", bsi->ext_wake, bsi->ext_wake_assert);

	if (assert_level != -1) {
		bsi->ext_wake_assert = (assert_level & 0x01) > 0;
		BT_DBG("override ext_wake assert to %d", bsi->ext_wake_assert);
	}

	/* 1.set bt_wake as output and the level is assert, assert bt wake */
	ret = gpio_direction_output(bsi->ext_wake, bsi->ext_wake_assert);
	if (ret < 0) {
		BT_ERR("can't request output direction bt_wake gpio %d\n",
			bsi->ext_wake);
		goto err3;
	}
	/*set ext_wake deassert as default*/
	gpio_set_value(bsi->ext_wake, !bsi->ext_wake_assert);

	/* 2.get bt_host_wake gpio irq */
	bsi->host_wake_irq = gpio_to_irq(bsi->host_wake);
	if (bsi->host_wake_irq < 0) {
		BT_ERR("map gpio [%d] to virq failed, errno = %d\n",
				bsi->host_wake, bsi->host_wake_irq);
		ret = -ENODEV;
		goto err3;
	}

	uart_index = DEFAULT_UART_INDEX;
	if (!of_property_read_u32(np, "uart_index", &val)) {
		switch (val) {
		case 0:
		case 1:
		case 2:
			uart_index = val;
			break;
		default:
			BT_ERR("unsupported uart_index (%u)\n", val);
		}
	}
	BT_DBG("uart_index (%u)\n", uart_index);
	bluesleep_uart_dev = sw_uart_get_pdev(uart_index);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
    bsi->ws = wakeup_source_register(dev, "bluesleep");
#else
	bsi->ws = wakeup_source_register("bluesleep");
#endif

#else
	wake_lock_init(&bsi->wake_lock, WAKE_LOCK_SUSPEND, "bluesleep");
#endif
	bsi->pdev = pdev;

	return 0;

err3:
	devm_gpio_free(dev, bsi->ext_wake);
err2:
	device_init_wakeup(dev, false);
err1:
	devm_gpio_free(dev, bsi->host_wake);
err0:
	devm_kfree(dev, bsi);

	BT_ERR("probe fail, err: %d", ret);
	return ret;
#endif
	return 0;
}

static int bluesleep_remove(struct platform_device *pdev)
{
	/* assert bt wake */
	gpio_set_value(bsi->ext_wake, bsi->ext_wake_assert);
	if (test_bit(BT_PROTO, &flags)) {
		if (disable_irq_wake(bsi->host_wake_irq))
			BT_ERR("Couldn't disable hostwake IRQ wakeup mode\n");
		free_irq(bsi->host_wake_irq, &bsi->pdev->dev);
		del_timer(&rx_timer);
		if (test_bit(BT_ASLEEP, &flags))
			hsuart_power(1);
	}
	gpio_free(bsi->host_wake);
	gpio_free(bsi->ext_wake);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	wakeup_source_unregister(bsi->ws);
#else
	wake_lock_destroy(&bsi->wake_lock);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
	if (bsi->wakeup_enable) {
		BT_DBG("Deinit wakeup source");
		device_init_wakeup(&pdev->dev, false);
		dev_pm_clear_wake_irq(&pdev->dev);
	}
#else
	BT_ERR("%s kernel unsupport this feature!\r\n", __func__);
#endif
	return 0;
}

/**
 * Initializes the module.
 * @return On success, 0. On error, -1, and <code>errno</code> is set
 * appropriately.
 */
int bluesleep_init(struct platform_device *pdev)
{
	int retval;
	struct proc_dir_entry *ent;

	BT_DBG("BlueSleep Mode Driver Ver %s", VERSION);

#if 1
	retval = bluesleep_probe(pdev);
	if (retval)
		return retval;
#endif

#if !BT_BLUEDROID_SUPPORT
	bluesleep_hdev = NULL;
#endif

	bluetooth_dir = proc_mkdir("bluetooth", NULL);
	if (bluetooth_dir == NULL) {
		BT_ERR("Unable to create /proc/bluetooth directory");
		return -ENOMEM;
	}

	sleep_dir = proc_mkdir("sleep", bluetooth_dir);
	if (sleep_dir == NULL) {
		BT_ERR("Unable to create /proc/%s directory", PROC_DIR);
		return -ENOMEM;
	}
#if 0
	/* Creating read/write "btwake" entry */
	ent = create_proc_entry("btwake", 0, sleep_dir);
	if (ent == NULL) {
		BT_ERR("Unable to create /proc/%s/btwake entry", PROC_DIR);
		retval = -ENOMEM;
		goto fail;
	}
	ent->read_proc = bluepower_read_proc_btwake;
	ent->write_proc = bluepower_write_proc_btwake;

	/* read only proc entries */
	if (create_proc_read_entry("hostwake", 0, sleep_dir,
				bluepower_read_proc_hostwake, NULL) == NULL) {
		BT_ERR("Unable to create /proc/%s/hostwake entry", PROC_DIR);
		retval = -ENOMEM;
		goto fail;
	}

	/* read/write proc entries */
	ent = create_proc_entry("proto", 0666, sleep_dir);
	if (ent == NULL) {
		BT_ERR("Unable to create /proc/%s/proto entry", PROC_DIR);
		retval = -ENOMEM;
		goto fail;
	}
	ent->read_proc = bluesleep_read_proc_proto;
	ent->write_proc = bluesleep_write_proc_proto;

	/* read only proc entries */
	if (create_proc_read_entry("asleep", 0,
			sleep_dir, bluesleep_read_proc_asleep, NULL) == NULL) {
		BT_ERR("Unable to create /proc/%s/asleep entry", PROC_DIR);
		retval = -ENOMEM;
		goto fail;
	}
#endif
#if BT_BLUEDROID_SUPPORT
	/* read/write proc entries */
	ent = proc_create("lpm", 0660, sleep_dir, &lpm_fops);
	if (ent == NULL) {
		BT_ERR("Unable to create /proc/%s/lpm entry", PROC_DIR);
		retval = -ENOMEM;
		goto fail;
	}

	ent = proc_create("btwrite", 0660, sleep_dir, &btwrite_fops);
	if (ent == NULL) {
		BT_ERR("Unable to create /proc/%s/btwrite entry", PROC_DIR);
		retval = -ENOMEM;
		goto fail;
	}
#endif

	flags = 0; /* clear all status bits */

	/* Initialize spinlock. */
	spin_lock_init(&rw_lock);

	/* Initialize timer */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	timer_setup(&rx_timer, bluesleep_rx_timer_expire, 0);
#else
	init_timer(&rx_timer);
	rx_timer.function = bluesleep_rx_timer_expire;
	rx_timer.data = 0;
#endif

	/* initialize host wake tasklet */
	tasklet_init(&hostwake_task, bluesleep_hostwake_task, 0);

#if !BT_BLUEDROID_SUPPORT
	hci_register_notifier(&hci_event_nblock);
#endif

	return 0;

fail:
#if BT_BLUEDROID_SUPPORT
	remove_proc_entry("btwrite", sleep_dir);
	remove_proc_entry("lpm", sleep_dir);
#endif
#if 0
	remove_proc_entry("asleep", sleep_dir);
	remove_proc_entry("proto", sleep_dir);
	remove_proc_entry("hostwake", sleep_dir);
	remove_proc_entry("btwake", sleep_dir);
#endif
	remove_proc_entry("sleep", bluetooth_dir);
	remove_proc_entry("bluetooth", 0);
	return retval;
}

/**
 * Cleans up the module.
 */
int bluesleep_exit(struct platform_device *dev)
{
#if !BT_BLUEDROID_SUPPORT
	hci_unregister_notifier(&hci_event_nblock);
#endif

#if BT_BLUEDROID_SUPPORT
	remove_proc_entry("btwrite", sleep_dir);
	remove_proc_entry("lpm", sleep_dir);
#endif
#if 0
	remove_proc_entry("asleep", sleep_dir);
	remove_proc_entry("proto", sleep_dir);
	remove_proc_entry("hostwake", sleep_dir);
	remove_proc_entry("btwake", sleep_dir);
#endif
	remove_proc_entry("sleep", bluetooth_dir);
	remove_proc_entry("bluetooth", 0);
	bluesleep_remove(dev);
	return 0;
}

