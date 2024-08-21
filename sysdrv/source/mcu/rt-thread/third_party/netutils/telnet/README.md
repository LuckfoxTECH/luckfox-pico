# Telnet：远程登录 RT-Thread

## 1、介绍

[Telnet](https://baike.baidu.com/item/Telnet) 协议是一种应用层协议，使用于互联网及局域网中，使用虚拟终端机的形式，提供双向、以文字字符串为主的交互功能。属于 TCP/IP 协议族的其中之一，是 Internet 远程登录服务的标准协议和主要方式，常用于网页服务器的远程控制，可供用户在本地主机运行远程主机上的工作。

RT-Thread 目前支持的是 Telnet 服务器， Telnet 客户端连接成功后，将会远程连接到设备的 Finsh/MSH ，实现设备的远程控制。

## 2、使用

### 2.1 启动 Telnet 服务器

需要在 RT-Thread 上使用 Finsh/MSH 命令来启动 Telnet 服务器，大致效果如下：

```
msh />telnet_server
Telnet server start successfully
telnet: waiting for connection
msh />
```

### 2.2 远程登录到 Telnet 服务器

本地电脑上安装支持 Telnet 客户端的终端，例如：putty、Xshell、SecureCRT，这里使用 putty 进行演示：

- 1、打开 putty ，选择 `Telnet` 模式；
- 2、输入 Telnet 服务器地址，点击 Open；
- 3、连接成功后，可以在终端中使用 RT-Thread Finsh/MSH；

![telnet_connect_cfg](../images/telnet_connect_cfg.png)

![telnet_connected](../images/telnet_connected.png)

### 2.3 注意事项

- 1、与 Telnet 服务器连接成功后，设备本地的 Finsh/MSH 将无法使用。如果需要使用，断开已连接的 Telnet 客户端即可；
- 2、Telnet 不支持 `TAB` 自动补全快捷键、`Up`/`Down` 查阅历史等快捷键；
- 3、目前 Telnet 服务器只支持连接到 **1** 个客户端。
