# Ping

## 1、介绍

[ping](https://baike.baidu.com/item/ping/6235) 是一种网络工具，用来测试数据包能否通过 IP 协议到达特定主机。估算与主机间的丢失数据包率（丢包率）和数据包往返时间（网络时延，Round-trip delay time）。

## 2、使用

ping 支持访问 `IP 地址` 或 `域名` ，使用 Finsh/MSH 命令进行测试，大致使用效果如下：

### 2.1 Ping 域名

```
msh />ping rt-thread.org
60 bytes from 116.62.244.242 icmp_seq=0 ttl=49 time=11 ms
60 bytes from 116.62.244.242 icmp_seq=1 ttl=49 time=10 ms
60 bytes from 116.62.244.242 icmp_seq=2 ttl=49 time=12 ms
60 bytes from 116.62.244.242 icmp_seq=3 ttl=49 time=10 ms
msh />
```

### 2.2 Ping IP

```
msh />ping 192.168.10.12
60 bytes from 192.168.10.12 icmp_seq=0 ttl=64 time=5 ms
60 bytes from 192.168.10.12 icmp_seq=1 ttl=64 time=1 ms
60 bytes from 192.168.10.12 icmp_seq=2 ttl=64 time=2 ms
60 bytes from 192.168.10.12 icmp_seq=3 ttl=64 time=3 ms
msh />

```

## 3、常见问题

- netdev 组件中会默认导出 ping 功能支持，如果使用系统中包含并开启 netdev 组件，该配置选项在 ENV 中将不会被显示。

