#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/clk.h>
#include <linux/pwm.h>
#include <linux/file.h>
#include <linux/list.h>
#include <linux/gpio.h>
#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/kthread.h>
#include <linux/mempolicy.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include "motor_24byj48.h"

static void motor_set_default(struct motor_device *mdev)
{
	int index = 0;
	struct motor_device_attribute *motor = NULL;

	for (index = 0; index < HAS_MOTOR_CNT; index++) {
		motor =  &mdev->motors[index];
		motor->motor_direction = 'R';
		motor->cur_steps = 0;
		motor->input_move_steps = 0;
		motor->real_move_steps = 0;
		motor->timer_count = 0;
		motor->motor_speed = MOTOR_MIN_SPEED;
		motor->motor_current_status = MOTOR_IS_STOP;
		motor->reset_point = motor->max_steps >> 1;
	}
}

void motor_off(struct motor_device_attribute motor)
{
	gpio_set_value(motor.motor_gpio[0], 0);
	gpio_set_value(motor.motor_gpio[1], 0);
	gpio_set_value(motor.motor_gpio[2], 0);
	gpio_set_value(motor.motor_gpio[3], 0);
}

static long motor_ops_move(struct motor_device *mdev, int x, int y)
{
	struct motor_device_attribute *motors = mdev->motors;
	unsigned long flags;
	char x_dir = 'R';
	char y_dir = 'R';
	int x1 = 0;
	int y1 = 0;

	if (mdev->motors[HORIZONTAL_MOTOR].motor_current_status
		== MOTOR_IS_RUNNING)
		return -1;
	if (mdev->motors[VERTICAL_MOTOR].motor_current_status
		== MOTOR_IS_RUNNING)
		return -1;
	/* check x value */

	if (x > 0) {
		if (motors[HORIZONTAL_MOTOR].cur_steps + x >
			motors[HORIZONTAL_MOTOR].max_steps)
			x = motors[HORIZONTAL_MOTOR].max_steps -
				motors[HORIZONTAL_MOTOR].cur_steps;
	} else {
		if (motors[HORIZONTAL_MOTOR].cur_steps <= 0)
			x = 0;
		else
			if (-x > motors[HORIZONTAL_MOTOR].cur_steps + 1)
				x = -motors[HORIZONTAL_MOTOR].cur_steps;
	}

	/* check y value */
	if (y > 0) {
		if (motors[VERTICAL_MOTOR].cur_steps + y >
			motors[VERTICAL_MOTOR].max_steps)
			y = motors[VERTICAL_MOTOR].max_steps -
				motors[VERTICAL_MOTOR].cur_steps;
	} else {
		if (motors[VERTICAL_MOTOR].cur_steps <= 0)
			y = 0;
		else
			if (-y > motors[VERTICAL_MOTOR].cur_steps + 1)
				y = -motors[VERTICAL_MOTOR].cur_steps;
	}

	x_dir = x > 0 ? 'R' : 'L';
	y_dir = y > 0 ? 'R' : 'L';
	x1 = x < 0 ? 0 - x : x;
	y1 = y < 0 ? 0 - y : y;
	if (x1 + y1 == 0)
		return 0;

	mutex_lock(&mdev->dev_mutex);
	spin_lock_irqsave(&mdev->slock, flags);
	if (x1 > 0) {
		mdev->motors[HORIZONTAL_MOTOR].motor_current_status =
			MOTOR_IS_RUNNING;
		mdev->motors[HORIZONTAL_MOTOR].input_move_steps = x1;
		mdev->motors[HORIZONTAL_MOTOR].real_move_steps = 0;
		mdev->motors[HORIZONTAL_MOTOR].motor_direction = x_dir;
	}
	if (y1 > 0) {
		mdev->motors[VERTICAL_MOTOR].motor_current_status =
			MOTOR_IS_RUNNING;
		mdev->motors[VERTICAL_MOTOR].input_move_steps = y1;
		mdev->motors[VERTICAL_MOTOR].real_move_steps = 0;
		mdev->motors[VERTICAL_MOTOR].motor_direction = y_dir;
	}
	spin_unlock_irqrestore(&mdev->slock, flags);
	mutex_unlock(&mdev->dev_mutex);
	if (x1 > 0) {
		mdev->motors[HORIZONTAL_MOTOR].m_kt =
			ktime_set(0, mdev->motors[HORIZONTAL_MOTOR].motor_speed);
		hrtimer_cancel(&mdev->horizontal_motor_timer);
		hrtimer_start(&mdev->horizontal_motor_timer,
			mdev->motors[HORIZONTAL_MOTOR].m_kt, HRTIMER_MODE_REL);
	}
	if (y1 > 0) {
		mdev->motors[VERTICAL_MOTOR].m_kt =
			ktime_set(0, mdev->motors[VERTICAL_MOTOR].motor_speed);
		hrtimer_cancel(&mdev->vertical_motor_timer);
		hrtimer_start(&mdev->vertical_motor_timer,
			mdev->motors[VERTICAL_MOTOR].m_kt, HRTIMER_MODE_REL);
	}
	return 0;
}

static void motor_ops_stop(struct motor_device *mdev)
{
	unsigned long flags;
	int index;
	struct motor_device_attribute *motors = mdev->motors;

	if (mdev->motors[HORIZONTAL_MOTOR].motor_current_status ==
			MOTOR_IS_STOP &&
		mdev->motors[VERTICAL_MOTOR].motor_current_status ==
			MOTOR_IS_STOP)
		return;
	if (mdev->motors[HORIZONTAL_MOTOR].motor_current_status !=
			MOTOR_IS_STOP) {
		hrtimer_cancel(&mdev->horizontal_motor_timer);
		motor_off(mdev->motors[HORIZONTAL_MOTOR]);
	}
	if (mdev->motors[VERTICAL_MOTOR].motor_current_status !=
			MOTOR_IS_STOP) {
		hrtimer_cancel(&mdev->vertical_motor_timer);
		motor_off(mdev->motors[VERTICAL_MOTOR]);
	}
	mutex_lock(&mdev->dev_mutex);
	spin_lock_irqsave(&mdev->slock, flags);

	for (index = 0; index < HAS_MOTOR_CNT; index++) {
		motors[index].motor_current_status = MOTOR_IS_STOP;
		if (mdev->motors[index].motor_direction == 'R')
			mdev->motors[index].cur_steps = mdev->motors[index].cur_steps +
			motors[index].real_move_steps;
		else if (mdev->motors[index].motor_direction == 'L')
			mdev->motors[index].cur_steps = mdev->motors[index].cur_steps -
			motors[index].real_move_steps;
		motors[index].input_move_steps = 0;
		motors[index].real_move_steps = 0;
	}

	spin_unlock_irqrestore(&mdev->slock, flags);
	mutex_unlock(&mdev->dev_mutex);
}

static int motor_speed(struct motor_device *mdev, struct motors_input_speed speed)
{
	if ((speed.input_x_speed <= MOTOR_MIN_SPEED) &&
		(speed.input_x_speed >= MOTOR_MAX_SPEED)) {
		mdev->motors[HORIZONTAL_MOTOR].motor_speed = speed.input_x_speed;
		if (mdev->motors[HORIZONTAL_MOTOR].motor_current_status ==
			MOTOR_IS_RUNNING) {
			mdev->motors[HORIZONTAL_MOTOR].m_kt =
				ktime_set(0, mdev->motors[HORIZONTAL_MOTOR].motor_speed);
			hrtimer_cancel(&mdev->horizontal_motor_timer);
			hrtimer_start(&mdev->horizontal_motor_timer,
				mdev->motors[HORIZONTAL_MOTOR].m_kt, HRTIMER_MODE_REL);
		}
	}

	if ((speed.input_y_speed <= MOTOR_MIN_SPEED) &&
		(speed.input_y_speed >= MOTOR_MAX_SPEED)) {
		mdev->motors[VERTICAL_MOTOR].motor_speed = speed.input_y_speed;
		if (mdev->motors[VERTICAL_MOTOR].motor_current_status ==
			MOTOR_IS_RUNNING) {
			mdev->motors[VERTICAL_MOTOR].m_kt =
				ktime_set(0, mdev->motors[VERTICAL_MOTOR].motor_speed);
			hrtimer_cancel(&mdev->vertical_motor_timer);
			hrtimer_start(&mdev->vertical_motor_timer,
				mdev->motors[VERTICAL_MOTOR].m_kt, HRTIMER_MODE_REL);
		}
	}
	return 0;
}

static long motor_ops_goback(struct motor_device *mdev)
{
	struct motor_device_attribute *motors = mdev->motors;
	int sx, sy;
	int cx, cy;

	printk("%s enter\n", __func__);
	if (mdev->motors[HORIZONTAL_MOTOR].motor_current_status ==
		MOTOR_IS_RUNNING)
		return -1;
	sx = motors[HORIZONTAL_MOTOR].reset_point;
	sy = motors[VERTICAL_MOTOR].reset_point;
	cx = motors[HORIZONTAL_MOTOR].cur_steps;
	cy = motors[VERTICAL_MOTOR].cur_steps;
	printk("sx=%d, sy=%d, cx=%d, cy=%d\n", sx, sy, cx, cy);
	motor_ops_move(mdev, sx-cx, sy-cy);
	return 0;
}

static void motor_get_message(struct motor_device *mdev, struct motor_message *msg)
{
	struct motor_device_attribute *motors = mdev->motors;

	msg->x_cur_steps = motors[HORIZONTAL_MOTOR].cur_steps;
	msg->y_cur_steps = motors[VERTICAL_MOTOR].cur_steps;
	msg->x_cur_speed = motors[HORIZONTAL_MOTOR].motor_speed;
	msg->y_cur_speed = motors[VERTICAL_MOTOR].motor_speed;
	msg->x_cur_status = motors[HORIZONTAL_MOTOR].motor_current_status;
	msg->y_cur_status = motors[VERTICAL_MOTOR].motor_current_status;
}

static int motor_set_message(struct motor_device *mdev, struct motors_init_data msg)
{
	struct motor_device_attribute *motors = mdev->motors;

	printk("%s enter\n", __func__);
	if (mdev->motors[HORIZONTAL_MOTOR].motor_current_status == MOTOR_IS_RUNNING ||
		mdev->motors[VERTICAL_MOTOR].motor_current_status == MOTOR_IS_RUNNING)
		return -1;
	if (msg.motor_data.x_max_steps < mdev->motors[HORIZONTAL_MOTOR].cur_steps ||
		msg.motor_data.y_max_steps < mdev->motors[VERTICAL_MOTOR].cur_steps)
		return -1;
	if (msg.motor_data.reset_x < 0 || msg.motor_data.reset_x > msg.motor_data.x_max_steps ||
		msg.motor_data.reset_y < 0 || msg.motor_data.reset_y > msg.motor_data.y_max_steps)
		return -1;
	if (msg.motor_speed.input_x_speed > MOTOR_MIN_SPEED || msg.motor_speed.input_x_speed < MOTOR_MAX_SPEED ||
		msg.motor_speed.input_y_speed > MOTOR_MIN_SPEED || msg.motor_speed.input_y_speed < MOTOR_MAX_SPEED)
		return -1;
	motors[HORIZONTAL_MOTOR].max_steps = msg.motor_data.x_max_steps;
	motors[HORIZONTAL_MOTOR].motor_speed = msg.motor_speed.input_x_speed;
	motors[HORIZONTAL_MOTOR].reset_point = msg.motor_data.reset_x;
	motors[VERTICAL_MOTOR].max_steps = msg.motor_data.y_max_steps;
	motors[VERTICAL_MOTOR].motor_speed = msg.motor_speed.input_y_speed;
	motors[VERTICAL_MOTOR].reset_point = msg.motor_data.reset_y;

	return 0;
}

static long motor_ops_reset(struct motor_device *mdev, struct motor_reset_data *rdata)
{
	unsigned long flags;
	int index = 0;
	long ret = 0;
	int times = 0;
	struct motor_message msg;
	struct motor_device_attribute *motor = NULL;

	if (mdev == NULL || rdata == NULL) {
		printk("ERROR: the parameters of %s is wrong!!\n", __func__);
		return -EPERM;
	}
	if (rdata->x_max_steps <= 0 || rdata->y_max_steps <= 0)
		return -1;
	if (rdata->reset_x < 0 || rdata->reset_x > rdata->x_max_steps ||
		rdata->reset_y < 0 || rdata->reset_y > rdata->y_max_steps)
		return -1;

	mutex_lock(&mdev->dev_mutex);
	spin_lock_irqsave(&mdev->slock, flags);
	for (index = 0; index < HAS_MOTOR_CNT; index++) {
		motor =  &mdev->motors[index];
		motor->motor_speed = MOTOR_MAX_SPEED;
		motor->motor_current_status = MOTOR_IS_STOP;
	}
	mdev->motors[HORIZONTAL_MOTOR].max_steps = rdata->x_max_steps;
	mdev->motors[VERTICAL_MOTOR].max_steps = rdata->y_max_steps;
	mdev->motors[HORIZONTAL_MOTOR].cur_steps = rdata->x_max_steps;
	mdev->motors[VERTICAL_MOTOR].cur_steps = rdata->y_max_steps;
	spin_unlock_irqrestore(&mdev->slock, flags);
	mutex_unlock(&mdev->dev_mutex);
	ret = motor_ops_move(mdev, -(rdata->x_max_steps + 1), -(rdata->y_max_steps + 1));
	do {
		msleep(15);
		motor_get_message(mdev, &msg);
		times++;
		if (times > 1000) {
			printk("ERROR:wait motor timeout %s%d\n", __func__, __LINE__);
			ret = -ETIMEDOUT;
			goto exit;
		}
	} while (msg.x_cur_status == MOTOR_IS_RUNNING || msg.y_cur_status == MOTOR_IS_RUNNING);
	mutex_lock(&mdev->dev_mutex);
	spin_lock_irqsave(&mdev->slock, flags);
	mdev->motors[HORIZONTAL_MOTOR].cur_steps = 0;
	mdev->motors[VERTICAL_MOTOR].cur_steps = 0;
	mdev->motors[HORIZONTAL_MOTOR].reset_point = rdata->reset_x;
	mdev->motors[VERTICAL_MOTOR].reset_point = rdata->reset_y;
	spin_unlock_irqrestore(&mdev->slock, flags);
	mutex_unlock(&mdev->dev_mutex);
	ret = motor_ops_goback(mdev);
	do {
		msleep(10);
		motor_get_message(mdev, &msg);
		times++;
		if (times > 1000) {
			printk("ERROR:wait motor timeout %s%d\n", __func__, __LINE__);
			ret = -ETIMEDOUT;
			goto exit;
		}
	} while (msg.x_cur_status == MOTOR_IS_RUNNING || msg.y_cur_status == MOTOR_IS_RUNNING);
	/* sync data */
	rdata->x_max_steps	= mdev->motors[HORIZONTAL_MOTOR].max_steps;
	rdata->y_max_steps	= mdev->motors[VERTICAL_MOTOR].max_steps;
	for (index = 0; index < HAS_MOTOR_CNT; index++) {
		motor =  &mdev->motors[index];
		motor->motor_direction = 'R';
		motor->input_move_steps = 0;
		motor->real_move_steps = 0;
		motor->timer_count = 0;
		motor->motor_speed = MOTOR_MIN_SPEED;
	}

exit:
	msleep(10);
	return 0;
}

static int motor_open(struct inode *inode, struct file *file)
{
	struct miscdevice *dev = file->private_data;
	struct motor_device *mdev = container_of(dev, struct motor_device, misc_dev);
	int ret = 0;

	if (mdev->motor_open_flag) {
		ret = -EBUSY;
		dev_err(mdev->dev, "Motor driver busy now!\n");
	} else {
		printk("open motor is success!\n");
		mdev->motor_open_flag = 1;
	}

	return ret;
}

static int motor_release(struct inode *inode, struct file *file)
{
	struct miscdevice *dev = file->private_data;
	struct motor_device *mdev = container_of(dev, struct motor_device, misc_dev);

	motor_ops_stop(mdev);
	mdev->motor_open_flag = 0;
	printk("release motor is success!\n");
	return 0;
}

static int meter_turn(struct motor_device_attribute motor)
{

	switch (motor.timer_count) {
	case 0:
		gpio_set_value(motor.motor_gpio[0], 1);
		gpio_set_value(motor.motor_gpio[1], 0);
		gpio_set_value(motor.motor_gpio[2], 0);
		gpio_set_value(motor.motor_gpio[3], 0);
		break;
	case 1:
		gpio_set_value(motor.motor_gpio[0], 1);
		gpio_set_value(motor.motor_gpio[1], 1);
		gpio_set_value(motor.motor_gpio[2], 0);
		gpio_set_value(motor.motor_gpio[3], 0);
		break;
	case 2:
		gpio_set_value(motor.motor_gpio[0], 0);
		gpio_set_value(motor.motor_gpio[1], 1);
		gpio_set_value(motor.motor_gpio[2], 0);
		gpio_set_value(motor.motor_gpio[3], 0);
		break;
	case 3:
		gpio_set_value(motor.motor_gpio[0], 0);
		gpio_set_value(motor.motor_gpio[1], 1);
		gpio_set_value(motor.motor_gpio[2], 1);
		gpio_set_value(motor.motor_gpio[3], 0);
		break;
	case 4:
		gpio_set_value(motor.motor_gpio[0], 0);
		gpio_set_value(motor.motor_gpio[1], 0);
		gpio_set_value(motor.motor_gpio[2], 1);
		gpio_set_value(motor.motor_gpio[3], 0);
		break;
	case 5:
		gpio_set_value(motor.motor_gpio[0], 0);
		gpio_set_value(motor.motor_gpio[1], 0);
		gpio_set_value(motor.motor_gpio[2], 1);
		gpio_set_value(motor.motor_gpio[3], 1);
		break;
	case 6:
		gpio_set_value(motor.motor_gpio[0], 0);
		gpio_set_value(motor.motor_gpio[1], 0);
		gpio_set_value(motor.motor_gpio[2], 0);
		gpio_set_value(motor.motor_gpio[3], 1);
		break;
	case 7:
		gpio_set_value(motor.motor_gpio[0], 1);
		gpio_set_value(motor.motor_gpio[1], 0);
		gpio_set_value(motor.motor_gpio[2], 0);
		gpio_set_value(motor.motor_gpio[3], 1);
		break;
	}

	return 0;
}

static enum hrtimer_restart  motor_horizontal_timer_poll(struct hrtimer *timer)
{
	unsigned long flags;
	struct motor_device *mdev = container_of(timer, struct motor_device, horizontal_motor_timer);

	if  (mdev->motors[HORIZONTAL_MOTOR].motor_direction == 'R') {
		mdev->motors[HORIZONTAL_MOTOR].timer_count++;
		if (mdev->motors[HORIZONTAL_MOTOR].timer_count > 7)
			mdev->motors[HORIZONTAL_MOTOR].timer_count = 0;
	} else {
		if (mdev->motors[HORIZONTAL_MOTOR].timer_count == 0)
			mdev->motors[HORIZONTAL_MOTOR].timer_count = 8;
		mdev->motors[HORIZONTAL_MOTOR].timer_count--;
	}
	meter_turn(mdev->motors[HORIZONTAL_MOTOR]);
	mdev->motors[HORIZONTAL_MOTOR].real_move_steps++;
	mdev->motors[HORIZONTAL_MOTOR].input_move_steps--;
	if (mdev->motors[HORIZONTAL_MOTOR].input_move_steps == 0) {
		mutex_lock(&mdev->dev_mutex);
		spin_lock_irqsave(&mdev->slock, flags);
		mdev->motors[HORIZONTAL_MOTOR].motor_current_status = MOTOR_IS_STOP;
		if (mdev->motors[HORIZONTAL_MOTOR].motor_direction == 'R')
			mdev->motors[HORIZONTAL_MOTOR].cur_steps = mdev->motors[HORIZONTAL_MOTOR].cur_steps +
			mdev->motors[HORIZONTAL_MOTOR].real_move_steps;
		else if (mdev->motors[HORIZONTAL_MOTOR].motor_direction == 'L')
			mdev->motors[HORIZONTAL_MOTOR].cur_steps = mdev->motors[HORIZONTAL_MOTOR].cur_steps -
			mdev->motors[HORIZONTAL_MOTOR].real_move_steps;
		mdev->motors[HORIZONTAL_MOTOR].input_move_steps = 0;
		mdev->motors[HORIZONTAL_MOTOR].real_move_steps = 0;
		spin_unlock_irqrestore(&mdev->slock, flags);
		mutex_unlock(&mdev->dev_mutex);
		printk("the x steps after stop：%d\n", mdev->motors[HORIZONTAL_MOTOR].cur_steps);
		motor_off(mdev->motors[HORIZONTAL_MOTOR]);
		return HRTIMER_NORESTART;
	}

	hrtimer_forward(timer, timer->base->get_time(), mdev->motors[HORIZONTAL_MOTOR].m_kt);
	return HRTIMER_RESTART;
}

static enum hrtimer_restart  motor_vertical_timer_poll(struct hrtimer *timer)
{
	unsigned long flags;
	struct motor_device *mdev = container_of(timer, struct motor_device, vertical_motor_timer);

	if (mdev->motors[VERTICAL_MOTOR].motor_direction == 'R') {
		mdev->motors[VERTICAL_MOTOR].timer_count++;
		if (mdev->motors[VERTICAL_MOTOR].timer_count > 7)
			mdev->motors[VERTICAL_MOTOR].timer_count = 0;
	} else {
		if (mdev->motors[VERTICAL_MOTOR].timer_count == 0)
			mdev->motors[VERTICAL_MOTOR].timer_count = 8;
		mdev->motors[VERTICAL_MOTOR].timer_count--;
	}
	meter_turn(mdev->motors[VERTICAL_MOTOR]);
	mdev->motors[VERTICAL_MOTOR].real_move_steps++;
	mdev->motors[VERTICAL_MOTOR].input_move_steps--;
	if (mdev->motors[VERTICAL_MOTOR].input_move_steps == 0) {
		mutex_lock(&mdev->dev_mutex);
		spin_lock_irqsave(&mdev->slock, flags);
		mdev->motors[VERTICAL_MOTOR].motor_current_status = MOTOR_IS_STOP;
		if (mdev->motors[VERTICAL_MOTOR].motor_direction == 'R')
			mdev->motors[VERTICAL_MOTOR].cur_steps = mdev->motors[VERTICAL_MOTOR].cur_steps +
			mdev->motors[VERTICAL_MOTOR].real_move_steps;
		else if (mdev->motors[VERTICAL_MOTOR].motor_direction == 'L')
			mdev->motors[VERTICAL_MOTOR].cur_steps = mdev->motors[VERTICAL_MOTOR].cur_steps -
			mdev->motors[VERTICAL_MOTOR].real_move_steps;
		mdev->motors[VERTICAL_MOTOR].input_move_steps = 0;
		mdev->motors[VERTICAL_MOTOR].real_move_steps = 0;
		spin_unlock_irqrestore(&mdev->slock, flags);
		mutex_unlock(&mdev->dev_mutex);
		printk("the y steps after stop：%d\n", mdev->motors[VERTICAL_MOTOR].cur_steps);
		motor_off(mdev->motors[VERTICAL_MOTOR]);
		return HRTIMER_NORESTART;
	}

	hrtimer_forward(timer, timer->base->get_time(), mdev->motors[VERTICAL_MOTOR].m_kt);
	return HRTIMER_RESTART;
}

static long motor_ioctl(struct file *file, unsigned int cmd, unsigned long value)
{
	struct miscdevice *dev = file->private_data;
	struct motor_device *mdev = container_of(dev, struct motor_device, misc_dev);
	long ret = 0;

	if (mdev->motor_open_flag == 0) {
		printk("Please Open /dev/motor Firstly\n");
		return -EPERM;
	}

	switch (cmd) {
	case MOTOR_STOP: {
		motor_ops_stop(mdev);
		//printk("MOTOR_STOP!!!!!!!!!!!!!!!!!!!\n");
		break;
	}
	case MOTOR_RESET: {
		struct motor_reset_data rdata;

		if (value == 0) {
			ret = -EPERM;
			break;
		}
		if (copy_from_user(&rdata, (void __user *)value,
					sizeof(rdata))) {
			dev_err(mdev->dev, "[%s][%d] copy from user error\n",
					__func__, __LINE__);
			return -EFAULT;
		}
		ret = motor_ops_reset(mdev, &rdata);
		if (!ret) {
			if (copy_to_user((void __user *)value, &rdata,
				sizeof(rdata))) {
				dev_err(mdev->dev, "[%s][%d] copy to user error\n",
					__func__, __LINE__);
				return -EFAULT;
			}
		}
		/*printk("MOTOR_RESET!!!!!!!!!!!!!!!!!!!\n");*/
		break;
	}
	case MOTOR_MOVE: {
		struct motors_input_steps dst;

		if (copy_from_user(&dst, (void __user *)value,
			sizeof(struct motors_input_steps))) {
			dev_err(mdev->dev, "[%s][%d] copy from user error\n",
			__func__, __LINE__);
			return -EFAULT;
		}
		motor_ops_move(mdev, dst.input_x_steps, dst.input_y_steps);
		printk("MOTOR_MOVE!!!!!!!!!!!!!!!!!!!\n");
		break;
	}
	case MOTOR_GET_STATUS: {
		struct motor_message msg;

		motor_get_message(mdev, &msg);
		if (copy_to_user((void __user *)value, &msg,
			sizeof(struct motor_message))) {
			dev_err(mdev->dev, "[%s][%d] copy to user error\n",
				__func__, __LINE__);
			return -EFAULT;
		}
		/*printk("MOTOR_GET_STATUS!!!!!!!!!!!!!!!!!!\n");*/
		break;
	}
	case MOTOR_SPEED:{
		struct motors_input_speed speed;

		if (copy_from_user(&speed, (void __user *)value, sizeof(struct motors_input_speed))) {
			dev_err(mdev->dev, "[%s][%d] copy to user error\n", __func__, __LINE__);
			return -EFAULT;
		}

		ret = motor_speed(mdev, speed);
		if (ret) {
			printk("set motor_speed fail\n");
			ret = -1;
		}
		break;
		//printk("MOTOR_SPEED!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
	case MOTOR_GOBACK: {
		motor_ops_stop(mdev);
		ret = motor_ops_goback(mdev);
		//printk("MOTOR_GOBACK!!!!!!!!!!!!!!!!!!!!!!!\n");
		break;
	}
	case MOTOR_CRUISE: {
		//printk("MOTOR_CRUISE!!!!!!!!!!!!!!!!!!!!!!!\n");
		break;
	}
	case MOTOR_SET_STATUS: {
		struct motors_init_data data;

		if (copy_from_user(&data, (void __user *)value,
			sizeof(struct motors_init_data))) {
			dev_err(mdev->dev, "[%s][%d] copy from user error\n",
			__func__, __LINE__);
			return -EFAULT;
		}
		printk("set data:max_x:%d, max_y:%d, x_speed:%d, y_speed:%d,x_reset:%d, y_reset:%d\n",
		data.motor_data.x_max_steps, data.motor_data.y_max_steps, data.motor_speed.input_x_speed,
		data.motor_speed.input_y_speed, data.motor_data.reset_x, data.motor_data.reset_y);
		ret = motor_set_message(mdev, data);
		if (ret) {
			printk("motor_set_message fail\n");
			ret = -1;
		}
		/*printk("MOTOR_SET_STATUS!!!!!!!!!!!!!!!!!!\n");*/
		break;
	}
	default:
		return -EINVAL;
	}

	return ret;

}

static struct file_operations motor_fops = {
	.owner = THIS_MODULE,
	.open = motor_open,
	.release = motor_release,
	.unlocked_ioctl = motor_ioctl,
};

static int motor_info_show(struct seq_file *m, void *v)
{
	printk("%s enter!\n", __func__);
	return 0;
}

static int motor_info_open(struct inode *inode, struct file *file)
{
	return single_open_size(file, motor_info_show, PDE_DATA(inode), 1024);
}

static const struct proc_ops motor_info_fops = {
	.proc_read = seq_read,
	.proc_open = motor_info_open,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int motor_request_gpio(struct platform_device *pdev, struct motor_device *mdev)
{
	int ret;
	int GPIO_ID_A = -1;
	int GPIO_ID_B = -1;
	int GPIO_ID_C = -1;
	int GPIO_ID_D = -1;
	int GPIO_ID_E = -1;
	int GPIO_ID_F = -1;
	int GPIO_ID_G = -1;
	int GPIO_ID_H = -1;

	GPIO_ID_A = of_get_named_gpio(pdev->dev.of_node, "motorA-gpios", 0);
	if (GPIO_ID_A < 0) {
		printk("get gpioA named is error!\n");
		return -1;
	}
	GPIO_ID_B = of_get_named_gpio(pdev->dev.of_node, "motorB-gpios", 0);
	if (GPIO_ID_B < 0) {
		printk("get gpioB named is error!\n");
		return -1;
	}
	GPIO_ID_C = of_get_named_gpio(pdev->dev.of_node, "motorC-gpios", 0);
	if (GPIO_ID_C < 0) {
		printk("get gpioC named is error!\n");
		return -1;
	}
	GPIO_ID_D = of_get_named_gpio(pdev->dev.of_node, "motorD-gpios", 0);
	if (GPIO_ID_D < 0) {
		printk("get gpioD named is error!\n");
		return -1;
	}
	GPIO_ID_E = of_get_named_gpio(pdev->dev.of_node, "motorE-gpios", 0);
	if (GPIO_ID_E < 0) {
		printk("get gpioA named is error!\n");
		return -1;
	}
	GPIO_ID_F = of_get_named_gpio(pdev->dev.of_node, "motorF-gpios", 0);
	if (GPIO_ID_F < 0) {
		printk("get gpioB named is error!\n");
		return -1;
	}
	GPIO_ID_G = of_get_named_gpio(pdev->dev.of_node, "motorG-gpios", 0);
	if (GPIO_ID_G < 0) {
		printk("get gpioC named is error!\n");
		return -1;
	}
	GPIO_ID_H = of_get_named_gpio(pdev->dev.of_node, "motorH-gpios", 0);
	if (GPIO_ID_H < 0) {
		printk("get gpioD named is error!\n");
		return -1;
	}


	ret = gpio_request(GPIO_ID_A, "motor_A_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		return -1;
	}
	ret = gpio_request(GPIO_ID_B, "motor_B_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		gpio_free(GPIO_ID_A);
		GPIO_ID_A = -1;
		return -1;
	}
	ret = gpio_request(GPIO_ID_C, "motor_C_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		gpio_free(GPIO_ID_A);
		gpio_free(GPIO_ID_B);
		GPIO_ID_A = -1;
		GPIO_ID_B = -1;
		return -1;
	}
	ret = gpio_request(GPIO_ID_D, "motor_D_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		gpio_free(GPIO_ID_A);
		gpio_free(GPIO_ID_B);
		gpio_free(GPIO_ID_C);
		GPIO_ID_A = -1;
		GPIO_ID_B = -1;
		GPIO_ID_C = -1;
		return -1;
	}
	ret = gpio_request(GPIO_ID_E, "motor_E_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		gpio_free(GPIO_ID_A);
		gpio_free(GPIO_ID_B);
		gpio_free(GPIO_ID_C);
		gpio_free(GPIO_ID_D);
		GPIO_ID_A = -1;
		GPIO_ID_B = -1;
		GPIO_ID_C = -1;
		GPIO_ID_D = -1;
		return -1;
	}
	ret = gpio_request(GPIO_ID_F, "motor_F_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		gpio_free(GPIO_ID_A);
		gpio_free(GPIO_ID_B);
		gpio_free(GPIO_ID_C);
		gpio_free(GPIO_ID_D);
		gpio_free(GPIO_ID_E);
		GPIO_ID_A = -1;
		GPIO_ID_B = -1;
		GPIO_ID_C = -1;
		GPIO_ID_D = -1;
		GPIO_ID_E = -1;
		return -1;
	}
	ret = gpio_request(GPIO_ID_G, "motor_G_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		gpio_free(GPIO_ID_A);
		gpio_free(GPIO_ID_B);
		gpio_free(GPIO_ID_C);
		gpio_free(GPIO_ID_D);
		gpio_free(GPIO_ID_E);
		gpio_free(GPIO_ID_F);
		GPIO_ID_A = -1;
		GPIO_ID_B = -1;
		GPIO_ID_C = -1;
		GPIO_ID_D = -1;
		GPIO_ID_E = -1;
		GPIO_ID_F = -1;
		return -1;
	}
	ret = gpio_request(GPIO_ID_H, "motor_H_GPIO");
	if (ret != 0) {
		printk("motor gpio request is error!\n");
		gpio_free(GPIO_ID_A);
		gpio_free(GPIO_ID_B);
		gpio_free(GPIO_ID_C);
		gpio_free(GPIO_ID_D);
		gpio_free(GPIO_ID_E);
		gpio_free(GPIO_ID_F);
		gpio_free(GPIO_ID_G);
		GPIO_ID_A = -1;
		GPIO_ID_B = -1;
		GPIO_ID_C = -1;
		GPIO_ID_D = -1;
		GPIO_ID_E = -1;
		GPIO_ID_F = -1;
		GPIO_ID_G = -1;
		return -1;
	}

	ret = gpio_direction_output(GPIO_ID_A, 0);
	if (ret) {
		printk("motor gpioA set direction fail%d\n", ret);
		goto free_gpio;
	}
	ret = gpio_direction_output(GPIO_ID_B, 0);
	if (ret) {
		printk("motor gpioB set direction fail%d\n", ret);
		goto free_gpio;
	}
	ret = gpio_direction_output(GPIO_ID_C, 0);
	if (ret) {
		printk("motor gpioC set direction fail%d\n", ret);
		goto free_gpio;
	}
	ret = gpio_direction_output(GPIO_ID_D, 0);
	if (ret) {
		printk("motor gpioD set direction fail%d\n", ret);
		goto free_gpio;
	}
	ret = gpio_direction_output(GPIO_ID_E, 0);
	if (ret) {
		printk("motor gpioE set direction fail%d\n", ret);
		goto free_gpio;
	}
	ret = gpio_direction_output(GPIO_ID_F, 0);
	if (ret) {
		printk("motor gpioF set direction fail%d\n", ret);
		goto free_gpio;
	}
	ret = gpio_direction_output(GPIO_ID_G, 0);
	if (ret) {
		printk("motor gpioG set direction fail%d\n", ret);
		goto free_gpio;
	}
	ret = gpio_direction_output(GPIO_ID_H, 0);
	if (ret) {
		printk("motor gpioH set direction fail%d\n", ret);
		goto free_gpio;
	}
	mdev->motors[HORIZONTAL_MOTOR].motor_gpio[0] = GPIO_ID_A;
	mdev->motors[HORIZONTAL_MOTOR].motor_gpio[1] = GPIO_ID_B;
	mdev->motors[HORIZONTAL_MOTOR].motor_gpio[2] = GPIO_ID_C;
	mdev->motors[HORIZONTAL_MOTOR].motor_gpio[3] = GPIO_ID_D;
	mdev->motors[VERTICAL_MOTOR].motor_gpio[0] = GPIO_ID_E;
	mdev->motors[VERTICAL_MOTOR].motor_gpio[1] = GPIO_ID_F;
	mdev->motors[VERTICAL_MOTOR].motor_gpio[2] = GPIO_ID_G;
	mdev->motors[VERTICAL_MOTOR].motor_gpio[3] = GPIO_ID_H;
	return 0;
free_gpio:
	if (GPIO_ID_A  != -1)
		gpio_free(GPIO_ID_A);
	if (GPIO_ID_B  != -1)
		gpio_free(GPIO_ID_B);
	if (GPIO_ID_C  != -1)
		gpio_free(GPIO_ID_C);
	if (GPIO_ID_D  != -1)
		gpio_free(GPIO_ID_D);
	if (GPIO_ID_E  != -1)
		gpio_free(GPIO_ID_E);
	if (GPIO_ID_F  != -1)
		gpio_free(GPIO_ID_F);
	if (GPIO_ID_G  != -1)
		gpio_free(GPIO_ID_G);
	if (GPIO_ID_H  != -1)
		gpio_free(GPIO_ID_H);
	return -1;
}

static void motor_release_gpio(struct motor_device *mdev)
{
	int i = 0, index = 0;
	struct motor_device_attribute *motor = NULL;
	int GPIO_ID_A = mdev->motors[HORIZONTAL_MOTOR].motor_gpio[0];
	int GPIO_ID_B = mdev->motors[HORIZONTAL_MOTOR].motor_gpio[1];
	int GPIO_ID_C = mdev->motors[HORIZONTAL_MOTOR].motor_gpio[2];
	int GPIO_ID_D = mdev->motors[HORIZONTAL_MOTOR].motor_gpio[3];
	int GPIO_ID_E = mdev->motors[VERTICAL_MOTOR].motor_gpio[0];
	int GPIO_ID_F = mdev->motors[VERTICAL_MOTOR].motor_gpio[1];
	int GPIO_ID_G = mdev->motors[VERTICAL_MOTOR].motor_gpio[2];
	int GPIO_ID_H = mdev->motors[VERTICAL_MOTOR].motor_gpio[3];

	if (GPIO_ID_A  != -1)
		gpio_free(GPIO_ID_A);
	if (GPIO_ID_B  != -1)
		gpio_free(GPIO_ID_B);
	if (GPIO_ID_C  != -1)
		gpio_free(GPIO_ID_C);
	if (GPIO_ID_D  != -1)
		gpio_free(GPIO_ID_D);
	if (GPIO_ID_E  != -1)
		gpio_free(GPIO_ID_E);
	if (GPIO_ID_F  != -1)
		gpio_free(GPIO_ID_F);
	if (GPIO_ID_G  != -1)
		gpio_free(GPIO_ID_G);
	if (GPIO_ID_H  != -1)
		gpio_free(GPIO_ID_H);


	for (index = 0; index < HAS_MOTOR_CNT; index++) {
		motor =  &mdev->motors[index];
		for (i = 0; i < MAX_GPIO_NUM; i++)
			motor->motor_gpio[i] = -1;
	}
}

static int motor_probe(struct platform_device *pdev)
{
	int ret;
	int i = 0, index = 0;
	struct motor_device_attribute *motor = NULL;
	struct motor_device *mdev;
	struct proc_dir_entry *proc = NULL;
	struct proc_dir_entry *motor_info = NULL;

	printk("%s enter\n", __func__);
	mdev = devm_kzalloc(&pdev->dev, sizeof(struct motor_device), GFP_KERNEL);
	if (!mdev) {
		ret = -ENOENT;
		dev_err(&pdev->dev, "kzalloc motor device memery error\n");
		goto error_devm_kzalloc;
	}

	mdev->dev = &pdev->dev;
	mutex_init(&mdev->dev_mutex);
	spin_lock_init(&mdev->slock);

	platform_set_drvdata(pdev, mdev);

	mdev->misc_dev.minor = MISC_DYNAMIC_MINOR;
	mdev->misc_dev.name = "motor";
	mdev->misc_dev.fops = &motor_fops;
	ret = misc_register(&mdev->misc_dev);
	if (ret < 0) {
		ret = -ENOENT;
		dev_err(&pdev->dev, "misc_register failed\n");
	}

	hrtimer_init(&mdev->horizontal_motor_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	mdev->horizontal_motor_timer.function = motor_horizontal_timer_poll;
	hrtimer_init(&mdev->vertical_motor_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	mdev->vertical_motor_timer.function = motor_vertical_timer_poll;

	proc = proc_mkdir("motor", NULL);
	if (!proc) {
		mdev->proc = NULL;
		printk("create motor dir failed!\n");
		ret = -1;
		goto proc_mkdir_fail;
	} else {
		mdev->proc = proc;
	}

	motor_info = proc_create_data("motor_info", S_IRUGO, proc, &motor_info_fops, (void *)mdev);
	if (!motor_info)
		printk("create motor_info failed!\n");

	motor_set_default(mdev);
	mdev->motors[HORIZONTAL_MOTOR].max_steps = HORIZONTAL_MAX_STEPS;
	mdev->motors[VERTICAL_MOTOR].max_steps = VERTICAL_MAX_STEPS;
	for (index = 0; index < HAS_MOTOR_CNT; index++) {
		motor =  &mdev->motors[index];
		for (i = 0; i < MAX_GPIO_NUM; i++)
			motor->motor_gpio[i] = -1;
	}
	mdev->motor_open_flag = 0;

	ret = motor_request_gpio(pdev, mdev);
	if (ret) {
		printk("motor_request_gpio fail\n");
		ret = -1;
		goto motor_request_gpio_fail;
	}

	printk("%s leave\n", __func__);
	return 0;
motor_request_gpio_fail:
	if (mdev->proc)
		proc_remove(mdev->proc);
proc_mkdir_fail:
	mutex_destroy(&mdev->dev_mutex);
	misc_deregister(&mdev->misc_dev);
	kfree(mdev);
error_devm_kzalloc:
	return ret;
}

static int motor_remove(struct platform_device *pdev)
{
	struct motor_device *mdev = platform_get_drvdata(pdev);

	mutex_destroy(&mdev->dev_mutex);
	motor_release_gpio(mdev);
	if (mdev->proc)
		proc_remove(mdev->proc);
	misc_deregister(&mdev->misc_dev);
	kfree(mdev);
	return 0;
}

struct of_device_id of_match_table = {
	.compatible = "motor"
};

static struct platform_driver motor_platform_driver = {
	.probe = motor_probe,
	.remove = motor_remove,
	.driver = {
		.name = "motor",
		.owner = THIS_MODULE,
		.of_match_table = &of_match_table,
	}
};

static int __init motor_init(void)
{
	int ret;

	ret = platform_driver_register(&motor_platform_driver);
	return ret;
}

static void __exit motor_exit(void)
{
	platform_driver_unregister(&motor_platform_driver);
}

module_init(motor_init);
module_exit(motor_exit);

MODULE_LICENSE("GPL");

