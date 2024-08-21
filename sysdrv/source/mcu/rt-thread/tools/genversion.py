import os
import datetime

def genversion(Env) :
    try:
        BSP_ROOT = Env['BSP_ROOT']
        rtt_commit = os.popen('git rev-parse --verify --short HEAD').readlines()[0]
        hal_path = os.path.join(Env['RTT_ROOT'], 'bsp/rockchip/common/hal')
        hal_commit = os.popen('(cd ' + hal_path + '; git rev-parse --verify --short HEAD)').readlines()[0]
        battery_ipc_path = os.path.join(Env['RTT_ROOT'], 'applications/battery-ipc/')
        battery_ipc_commit = os.popen('(cd ' + battery_ipc_path + '; git rev-parse --verify --short HEAD)').readlines()[0]
        f = open(os.path.join(BSP_ROOT, 'auto_version.h'), 'w')
        f.write('#ifndef __AUTO_VERSION_H\n')
        f.write('#define __AUTO_VERSION_H\n\n')
        f.write('/* This file is auto generate, please do not edit it. */\n')
        f.write('#define FIRMWARE_AUTO_VERSION "{timestamp} {rtt}(rtt) {hal}(hal) {battery_ipc}(battery-ipc)"\n'.format(timestamp = str(datetime.datetime.now()), rtt = rtt_commit.replace('\n', ''), hal = hal_commit.replace('\n', ''), battery_ipc = battery_ipc_commit.replace('\n', '')))
        f.write('\n#endif\n')
        f.close()
    except:
        print("Generate auto_version.h failed.")
