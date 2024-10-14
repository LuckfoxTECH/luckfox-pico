# tcpdump

## 1、介绍
这是一个基于 RT-Thread 的捕获IP报文的小工具， 抓包的数据可以通过文件系统保存，或者通过 rdb 工具导入PC，利用 wireshark 软件解析。



### 1.1、依赖

- 依赖 [optparse](https://github.com/liu2guang/optparse) 软件包 
- 依赖 [dfs](https://www.rt-thread.org/document/site/rtthread-development-guide/rtthread-manual-doc/zh/1chapters/12-chapter_filesystem/) 文件系统
- 依赖 [env](https://www.rt-thread.org/document/site/rtthread-development-guide/rtthread-tool-manual/env/env-user-manual/) 工具
- RT-Thread 3.0+，对 bsp 无依赖

### 1.2、获取方式
- 使用 menuconfig 使能 tcpdump，详情如下： 

```
  RT-Thread online packages --->
      IOT internet of things --->
          [*] netutils: Networking utilities for RT-Thread  --->
          [*]   Enable tcpdump tool
          [ ]     Enable tcpdump data to print on the console
          [*]     Enable tcpdump debug log output
```
保存 menuconfig 配置后使用 `pkgs --update` 命令下载软件包

> 注：调试信息建议不关闭



## 2、使用

### 2.1、tcpdump命令含义

```
-i: 指定监听的网络接口
-m: 选择保存模式（文件系统 或 rdb）
-w: 用户指定的文件名 xx.pcap
-p: 停止抓包
-h: 帮助信息
```

### 2.2、命令详情
```
msh />tcpdump -h

|>------------------------- help -------------------------<|
| tcpdump [-p] [-h] [-i interface] [-m mode] [-w file]     |
|                                                          |
| -h: help                                                 |
| -i: specify the network interface for listening          |
| -m: choose what mode(file-system or rdb) to save the file|
| -w: write the captured packets into an xx.pcap file      |
| -p: stop capturing packets                               |
|                                                          |
| e.g.:                                                    |
| specify network interface and select save mode \         |
| and specify filename                                     |
| tcpdump -ie0 -mfile -wtext.pcap                          |
| tcpdump -ie0 -mrdb -wtext.pcap                           |
|                                                          |
| -m: file-system mode                                     |
| tcpdump -mfile                                           |
|                                                          |
| -m: rdb mode                                             |
| tcpdump -mrdb                                            |
|                                                          |
| -w: file                                                 |
| tcpdump -wtext.pcap                                      |
|                                                          |
| -p: stop                                                 |
| tcpdump -p                                               |
|                                                          |
| -h: help                                                 |
| tcpdump -h                                               |
|                                                          |
| write commands but no arguments are illegal!!            |
| e.g.: tcpdump -i / -i -mfile  / -i -mfile -wtext.pcap    |
|>------------------------- help -------------------------<|

msh />
```



## 3、使用文件系统保存抓包的数据

> 我们这里是把 sd-card 挂载文件系统

### 3.1、抓包前准备

开发板上电前，插入 sd-card

- 挂载成功，则提示：

```
SD card capacity 31023104 KB
probe mmcsd block device!
found part[0], begin: 10485760, size: 29.580GB
File System initialized!
```

- 挂载失败，则提示：

```
sdcard init fail or timeout: -2!
```

- 挂载成功，输入 `list_device` 可以看到 `sd0` 设备，详情如下：

```
msh />list_device
device         type         ref count
------ -------------------- ---------
sd0    Block Device         1       
e0     Network Interface    0             
usbd   USB Slave Device     0                   
rtc    RTC                  1       
spi4   SPI Bus              0       
pin    Miscellaneous Device 0       
uart1  Character Device     3       
msh />
```

### 3.2、抓包前检查

> 抓包前请确认板子的 IP 地址


- msh />里，输入 `ifconfig` 查看，详情如下：

```
msh />
network interface: e0 (Default)
MTU: 1500
MAC: 00 04 9f 05 44 e5 
FLAGS: UP LINK_UP ETHARP BROADCAST
ip address: 192.168.1.137
gw address: 192.168.1.1
net mask  : 255.255.255.0
dns server #0: 192.168.1.1
dns server #1: 0.0.0.0
msh />
```

### 3.3、开始抓包

- msh />里，输入 `tcpdump -ie0 -mfile -wtext.pcap`，详情如下：

```
msh />tcpdump -ie0 -msd -wtext.pcap
[TCPDUMP]select  [e0] network card device
[TCPDUMP]select  [file-system] mode
[TCPDUMP]save in [text.pcap]
[TCPDUMP]tcpdump start!
msh />
```

- 使用抓包命令会创建一个线程，线程的优先级是12。
- 输入 `list_thread` 命令查看运行中的线程，线程名字是 `tdth`，详情如下：

```
thread   pri  status      sp     stack size max used left tick  error
-------- ---  ------- ---------- ----------  ------  ---------- ---
tdth      12  suspend 0x000000ac 0x00000800    08%   0x0000000a 000
tshell    20  ready   0x00000070 0x00001000    22%   0x00000003 000
rp80       8  suspend 0x0000009c 0x00000400    15%   0x0000000a 000
phy       30  suspend 0x00000070 0x00000200    28%   0x00000001 000
usbd       8  suspend 0x00000098 0x00001000    03%   0x00000014 000
tcpip     10  suspend 0x000000b4 0x00000400    39%   0x00000014 000
etx       12  suspend 0x00000084 0x00000400    12%   0x00000010 000
erx       12  suspend 0x00000084 0x00000400    34%   0x00000010 000
mmcsd_de  22  suspend 0x0000008c 0x00000400    49%   0x00000013 000
tidle     31  ready   0x00000054 0x00000100    32%   0x0000001a 000
main      10  suspend 0x00000064 0x00000800    35%   0x00000010 000
msh />
```


### 3.4、抓包测试

> 使用 [ping ](https://github.com/RT-Thread-packages/netutils/blob/master/ping/README.md) 命令来进行抓包测试，`ping` 命令需要在 menuconfig 配置使能，详情如下：

```
  RT-Thread online packages --->
      IOT internet of things --->
          [*] Enable Ping utility
```
保存 menuconfig 配置后使用 `pkgs --update` 命令下载软件包

#### 3.4.1、ping 域名

- msh />里输入 `ping rt-thread.org`，详情如下：

```
msh />ping rt-thread.org
60 bytes from 116.62.244.242 icmp_seq=0 ttl=49 time=11 ticks
60 bytes from 116.62.244.242 icmp_seq=1 ttl=49 time=10 ticks
60 bytes from 116.62.244.242 icmp_seq=2 ttl=49 time=12 ticks
60 bytes from 116.62.244.242 icmp_seq=3 ttl=49 time=10 ticks
msh />
```

#### 3.4.2、ping IP

- msh />里输入 `ping 192.168.1.121`，详情如下： 

```
msh />ping 192.168.1.121
60 bytes from 192.168.10.121 icmp_seq=0 ttl=64 time=5 ticks
60 bytes from 192.168.10.121 icmp_seq=1 ttl=64 time=1 ticks
60 bytes from 192.168.10.121 icmp_seq=2 ttl=64 time=2 ticks
60 bytes from 192.168.10.121 icmp_seq=3 ttl=64 time=3 ticks
msh />
```

### 3.5、停止抓包

- msh />里，输入 `tcpdump -p`，详情如下：

```
msh />tcpdump -p
[TCPDUMP]tcpdump stop and tcpdump thread exit!
msh />
```

### 3.6、查看结果

- msh />里，输入 `ls` 查看保存结果，详情如下：

```
msh />ls
Directory /:
System Volume Information<DIR>                    
text.pcap         1012                     
msh />
```

### 3.7、抓包后处理

使用读卡器将保存在 sd-card 里的 xx.pcap 文件拷贝到 PC，使用抓包软件 wireshark 直接进行网络流的分析



## 4、抓包文件通过 rdb 工具导入PC

### 4.1、开启抓包

- msh />里，输入 `tcpdump -ie0 -mrdb -wtext.pcap`，详情如下：

```
msh />tcpdump -ie0 -mrdb -wtext.pcap
[TCPDUMP]select  [e0] network card device
[TCPDUMP]select  [rdb] mode
[TCPDUMP]save in [text.pcap]
[TCPDUMP]tcpdump start!
msh />
```

### 4.2、抓包测试

- 请参考 3.4 的操作

### 4.3、停止抓包

- msh />里，输入 `tcpdump -p`，详情如下：

```
msh />tcpdump -p
[TCPDUMP]tcpdump stop and tcpdump thread exit!
msh />
```

### 4.4、查看结果

- msh />里，输入 `ls` 查看保存结果，详情如下：

```
msh />ls
Directory /:
System Volume Information<DIR>                    
text.pcap         1012                     
msh />
```

### 4.5、抓包后处理

使用 rdb 工具将 xx.pcap 文件导入到PC，使用抓包软件 wireshark 直接进行网络流的分析



## 5、注意事项

- tcpdump 工具是需要开启 lwip 的 发送、接收线程的
- 抓包结束或者不想抓包了，请输入 `tcpdump -p` 结束抓包



## 6、联系方式 & 感谢

* 感谢：[liu2guang](https://github.com/liu2guang) 制作了 optprase 软件包
* 感谢：[uestczyh222](https://github.com/uestczyh222) 制作了 rdb 工具 & rdb 上位机
* 维护：[never](https://github.com/neverxie)
* 主页：https://github.com/RT-Thread-packages/netutils
