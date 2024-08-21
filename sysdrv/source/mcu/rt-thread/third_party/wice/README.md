# WICE使用说明

## 1、介绍

wice 是RTOS上的一个WIFI框架，目前支持AP系列芯片，并且支持AIRKISS

## 2、KCONFIG配置
![wice_kconfig](Z:\rt-thread\third_party\wice\images\wice_kconfig.png)

​     WICE会默认打开SDIO， WLAN,   LWIP,  其中LWIP需要重新配置

![lwip_kconfig](Z:\rt-thread\third_party\wice\images\lwip_kconfig.png)


## 3、使用说明

### 1、WICE命令说明

wice command

wice [-c | -d | -a | -s | -e | -q]

    -c         create wice driver
    -d         delete wice driver
    -a         auto create and delete wice
    -q         quit create and delete wice
    -s         start airkiss
    -e         end airkiss
    -g         get airkiss result
Examples:
    wice                  start wice
    wice -s               start wice

### 2、WL测试命令

​	该命令继承rkos 的命令格式， 格式如下

wl "rkos命令"， 例如：wl wl.start

        start           start wifi mfg test
        stop            stop wifi mfg test
        down            wl down
        up              wl up
        mpc             wl mpc 0
        country         wl country All
        band            wl band a/b/auto 2.4g, 5g
        rate            wl rate -r/h/v %d -b %d
        channel         wl channel
        watchdog        wl phy_watchdog 0 
        suppress        wl scansuppress 1
        forcecal        wl phy_phy_forcecal 1
        txpwrctr        wl phy_txpwrctrl 1
        txpwr1          wl txpwr1 %d(-1)
        pkstop          wl pkteng_stop tx/rx
        pkstart         wl pkteng_start mac tx/rx %d %d %d
        chanspec        wl chanspec %d/%d
        mimo_txbw       wl mimo_txbw 4
        out             wl out
        fqacurcy        wl fqacurcy 1
        reset_cnts      wl reset_cnts
        bi              wl bi 65535
        rateset         wl rateset %db(54b)
        nrate           wl.nrate -m 7
        counters        wl.counters
