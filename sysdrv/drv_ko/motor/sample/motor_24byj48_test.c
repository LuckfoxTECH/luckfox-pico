#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#define MOTOR_DEV "/dev/motor"

/* ioctl cmd */
#define MOTOR_STOP          _IOW('M',0,long)
#define MOTOR_RESET         _IOW('M',1,long)
#define MOTOR_MOVE          _IOW('M',2,long)
#define MOTOR_GET_STATUS    _IOW('M',3,long)
#define MOTOR_SPEED         _IOW('M',4,long)
#define MOTOR_GOBACK        _IOW('M',5,long)
#define MOTOR_CRUISE        _IOW('M',6,long)
#define MOTOR_SET_STATUS    _IOW('M',7,long)

enum motor_cnt {
    HORIZONTAL_MOTOR,
    VERTICAL_MOTOR,
    HAS_MOTOR_CNT,
};

struct motors_input_steps{
    int input_x_steps;
    int input_y_steps;
};

struct motors_input_speed{
    int input_x_speed;
    int input_y_speed;
};

struct motor_reset_data {
    unsigned int x_max_steps;
    unsigned int y_max_steps;
    unsigned int reset_x;
    unsigned int reset_y;
};

enum motor_status {
    MOTOR_IS_STOP,
    MOTOR_IS_RUNNING,
};

struct motor_message {
    int x_cur_steps;
    int y_cur_steps;
    enum motor_status x_cur_status;
    enum motor_status y_cur_status;
    int x_cur_speed;
    int y_cur_speed;
};

struct motors_init_data {
    struct motor_reset_data motor_data;
    struct motors_input_speed motor_speed;
};

int main(int argc, char * argv[])
{
    int fd;
    int a,b,c,d,e,f;
    struct motors_input_steps motors_steps;
    struct motor_message get_motor_message;
    struct motors_input_speed motors_speed;
    struct motor_reset_data reset_data;
    struct motors_init_data init_data;

    fd = open(MOTOR_DEV, O_RDWR);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    while(1)
    {
        printf("action：\n");
        printf("1:MOTOR_STOP 2:MOTOR_RESET 3:MOTOR_MOVE 4:MOTOR_GET_STATUS 5:MOTOR_SPEED 6:MOTOR_GOBACK 7:MOTOR_CRUISE 8:MOTOR_SET_STATUS 9:QUIT\n");
        printf("choose:");
        scanf("%d",&d);
        switch(d)
        {
            case 1:
                ioctl(fd, MOTOR_STOP, 0);
                break;
            case 2:
                printf("reset max steps x：");
                scanf("%d",&a);
                printf("reset max steps y：");
                scanf("%d",&b);
                printf("reset x：");
                scanf("%d",&c);
                printf("reset y：");
                scanf("%d",&d);
                printf("x=%d,y=%d,reset_x=%d,reset_y=%d\n",a,b,c,d);
                reset_data.x_max_steps = a;
                reset_data.y_max_steps = b;
                reset_data.reset_x = c;
                reset_data.reset_y = d;
                ioctl(fd, MOTOR_RESET, (unsigned long)&reset_data);
                break;
            case 3:
                printf("steps x：");
                scanf("%d",&a);
                printf("steps y：");
                scanf("%d",&b);
                printf("x=%d,y=%d\n",a,b);
                motors_steps.input_x_steps = a;
                motors_steps.input_y_steps = b;
                ioctl(fd, MOTOR_MOVE, (unsigned long)&motors_steps);
                break;
            case 4:
                ioctl(fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
                printf("x_current_steps=%d\n", get_motor_message.x_cur_steps);
                printf("x_cur_status=%d\n", get_motor_message.x_cur_status);
                printf("x_cur_speed=%d\n", get_motor_message.x_cur_speed);
                printf("y_current_steps=%d\n", get_motor_message.y_cur_steps);
                printf("y_cur_status=%d\n", get_motor_message.y_cur_status);
                printf("y_cur_speed=%d\n", get_motor_message.y_cur_speed);
                break;
            case 5:
                printf("speed x：");
                scanf("%d",&a);
                printf("speed y：");
                scanf("%d",&b);
                printf("x_speed=%d,y_speed=%d\n",a,b);
                motors_speed.input_x_speed = a;
                motors_speed.input_y_speed = b;
                ioctl(fd, MOTOR_SPEED, (unsigned long)&motors_speed);
                break;
            case 6:
                ioctl(fd, MOTOR_GOBACK, 0);
                break;
            case 7:
                ioctl(fd, MOTOR_CRUISE, 0);
                break;
            case 8:
                printf("reset max steps x：");
                scanf("%d",&a);
                printf("reset max steps y：");
                scanf("%d",&b);
                printf("reset x：");
                scanf("%d",&c);
                printf("reset y：");
                scanf("%d",&d);
                printf("speed x：");
                scanf("%d",&e);
                printf("speed y：");
                scanf("%d",&f);
                init_data.motor_data.x_max_steps = a;
                init_data.motor_data.y_max_steps = b;
                init_data.motor_data.reset_x = c;
                init_data.motor_data.reset_y = d;
                init_data.motor_speed.input_x_speed = e;
                init_data.motor_speed.input_y_speed = f;
                ioctl(fd, MOTOR_SET_STATUS, (unsigned long)&init_data);
                break;
            default:
                close(fd);
                return 0;
        }
    }
    return 0;
}

