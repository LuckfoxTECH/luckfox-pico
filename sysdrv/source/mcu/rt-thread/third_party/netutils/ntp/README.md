# NTP：网络时间协议

## 1、介绍

[NTP](https://baike.baidu.com/item/NTP) 是网络时间协议(Network Time Protocol)，它是用来同步网络中各个计算机时间的协议。

在 RT-Thread 上实现了 NTP 客户端，连接上网络后，可以获取当前 UTC 时间，并更新至 RTC 中。

## 2、使用

首先打开 meunconfig ，配置并开启 netutils 软件包。在配置选项中，默认提供了 3 个 NTP 服务器，保证了 NTP 功能的可靠性。

### 2.1 获取 UTC 时间

[UTC 时间](https://baike.baidu.com/item/%E5%8D%8F%E8%B0%83%E4%B8%96%E7%95%8C%E6%97%B6/787659?fromtitle=UTC&fromid=5899996) 又称世界统一时间、世界标准时间、国际协调时间。北京时间为 UTC+8 时间，比 UTC 时间多 8 小时，或者理解为早 8 小时。

API: `time_t ntp_get_time(void)`

|参数                                    |描述|
|:-----                                  |:----|
|return                                  |`>0`: 当前 UTC 时间，`=0`：获取时间失败|


示例代码：

```C
#include <ntp.h>

void main(void)
{
    time_t cur_time;

    cur_time = ntp_get_time();
    
    if (cur_time)
    {
        rt_kprintf("NTP Server Time: %s", ctime((const time_t*) &cur_time));
    }
}
```

### 2.2 获取本地时间

本地时间比 UTC 时间多了时区的概念，例如：北京时间为东八区，比 UTC 时间多 8 个小时。

在 `menuconfig` 中可以设置当前时区，默认为 `8`

API: `time_t ntp_get_local_time(void)`

|参数                                    |描述|
|:-----                                  |:----|
|return                                  |`>0`: 当前本地时间，`=0`：获取时间失败|

该 API 使用方法与 `ntp_get_time()` 类似

### 2.3 同步本地时间至 RTC

如果开启 RTC 设备，还可以使用下面的命令及 API 同步 NTP 的本地时间至 RTC 设备。

Finsh/MSH 命令效果如下：

```
msh />ntp_sync
Get local time from NTP server: Sat Feb 10 15:22:33 2018
The system time is updated. Timezone is 8.
msh />
```

API: `time_t ntp_sync_to_rtc(void)`

|参数                                    |描述|
|:-----                                  |:----|
|return                                  |`>0`: 当前本地时间，`=0`：同步时间失败|

## 3、注意事项

- 1、NTP API 方法执行时会占用较多的线程堆栈，使用时保证堆栈空间充足（≥1.5K）；
- 2、NTP API 方法 **不支持可重入** ，并发使用时，请注意加锁。
