#include "board_wifibt.h"

typedef uint32_t uint32;

uint32 wice_bt_rst(void)
{
    rk_bt_rst();
    return 0;
}

uint32 wice_bt_power_down(void)
{
    rk_bt_power_down();
    return 1;
}

void wice_init_bt_gpio(void)
{
    rk_bt_init_gpio();
}
