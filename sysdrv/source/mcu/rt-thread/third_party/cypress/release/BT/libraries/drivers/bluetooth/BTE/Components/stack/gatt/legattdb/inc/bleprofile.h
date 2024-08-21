#ifndef _BLEPROFILE_H_
#define _BLEPROFILE_H_
/*******************************************************************************
* THIS INFORMATION IS PROPRIETARY TO BROADCOM CORP
*
* ------------------------------------------------------------------------------
*
* Copyright (c) 2011 Broadcom Corp.
*
*          ALL RIGHTS RESERVED
*
********************************************************************************
*
* File Name: bleprofile.h
*
* Abstract: This file implements the BLE generic profile.
*
* Functions:
*
*******************************************************************************/
#include "bleappcfa.h"
#include "blecm.h"
#include "bleapp.h"
#include "blebat.h"
#include "lesmp.h"
#include "legattdb.h"
#include "leatt.h"
#include "emconinfo.h"
#include "ble_uuid.h"
#include "stacknvram.h"
#include "error.h"
#include "bt_rtos.h"

//////////////////////////////////////////////////////////////////////////////
//                      public data type definition.
//////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <pshpack1.h>
#endif
// GHS syntax.
#pragma pack(1)

#define BD_ADDR_LEN     6                   // Device address length 
#define ADV_LEN_MAX   31
#define FLAGS_LEN  1
#define COD_LEN   3
#define UUID_LEN 2
#define LOCAL_NAME_LEN_MAX       (ADV_LEN_MAX-2-FLAGS_LEN-2-COD_LEN-2-UUID_LEN-2) // additional 1 byte len, 1 byte value for each field //this will be 17 bytes
#define TX_POWER_LEN 1

#define VERSION_LEN 6

#define TIMESTAMP_LEN 7

#define READ_UART_LEN 15

#define SECURITY_ENABLED   0x01
#define SECURITY_REQUEST   0x02

#define BLEPROFILE_GENERIC_APP_TIMER 0

//GATT related definition
#define HANDLE_NUM_MAX 5

//GPIO flag bit (this is different than definition that used in gpio driver source code (GPIO_INPUT_ENABLE, etc)
#define GPIO_NUM_MAX  16
#define GPIO_INPUT        0x0000
#define GPIO_OUTPUT     0x0001
#define GPIO_INIT_LOW  0x0000
#define GPIO_INIT_HIGH 0x0002
#define GPIO_POLL          0x0000
#define GPIO_INT            0x0004   // Interrupt
#define GPIO_BOTHEDGE_INT       0x0008   // Both Edge

#define GPIO_WP             0x0010   //EEPROM write protect
#define GPIO_BAT            0x0020

#define GPIO_BUTTON     0x0100
#define GPIO_BUTTON1   0x0100
#define GPIO_BUTTON2   0x0200
#define GPIO_BUTTON3   0x0400

#define GPIO_LED            0x1000
#define GPIO_BUZ            0x2000


#ifdef BLE_PWM
#if defined(BCM20732)
#define PWM_START_GPIO 26
#define PWM_END_GPIO (PWM_START_GPIO + MAX_PWMS)
#else
#define PWM_START_GPIO 0
#define PWM_END_GPIO 0
#endif
#else
#define PWM_START_GPIO 0
#define PWM_END_GPIO 0
#endif


// ADV flag values
enum ble_adv_flag_value
{
    ADV_FLAGS = 0x01,
    ADV_SERVICE_UUID16_MORE = 0x02,
    ADV_SERVICE_UUID16_COMP = 0x03,
    ADV_SERVICE_UUID32_MORE = 0x04,
    ADV_SERVICE_UUID32_COMP = 0x05,
    ADV_SERVICE_UUID128_MORE = 0x06,
    ADV_SERVICE_UUID128_COMP = 0x07,
    ADV_LOCAL_NAME_SHORT = 0x08,
    ADV_LOCAL_NAME_COMP = 0x09,
    ADV_TX_POWER_LEVEL = 0x0A,
    ADV_CLASS_OF_DEVICE = 0x0D,
    ADV_SIMPLE_PAIRING_HASH_C = 0x0E,
    ADV_SIMPLE_PAIRING_RANDOMIZER_R = 0x0F,
    ADV_TK_VALUE = 0x10,
    ADV_OOB_FLAGS = 0x11,
    ADV_SLAVE_CONNECTION_INTERVAL_RANGE = 0x12,
    ADV_SERVICE_UUID16 = 0x14,
    ADV_SERVICE_UUID128 = 0x15,
    ADV_SERVICE_DATA = 0x16,
    ADV_SERVICE_TARGET_PUBLIC_ADR = 0x17,
    ADV_SERVICE_TARGET_RANDOM_ADR = 0x18,
    ADV_MANUFACTURER_DATA = 0xFF,
};

#define LE_LIMITED_DISCOVERABLE        0x01 // LE Limited Discoverable Mode
#define LE_GENERAL_DISCOVERABLE       0x02 // LE General Discoverable Mode
#define BR_EDR_NOT_SUPPORTED            0x04 // BR/EDR Not Supported
#define SIMUL_LE_BR_EDR_CONTROLLER 0x08 // Simultaneous LE and BR/EDR to Same Device Capable (Controller)
#define SIMUL_LE_BR_EDR_HOST             0x10 // Simultaneous LE and BR/EDR to Same Device Capable (Host)


enum bleprofile_puart_rx_int
{
    PUART_RX_INT_ENABLE = 0x01,
    PUART_RX_FIFO_INT_ENABLE = 0x02,
    PUART_RX_CTS_ENABLE = 0x04,
};

typedef UINT8 BD_ADDR[BD_ADDR_LEN];         /* Device address */
typedef UINT8 *BD_ADDR_PTR;                 /* Pointer to Device Address */

typedef PACKED struct
{
    UINT8 len; //length of field
    UINT8 val; //value of field
    UINT8 data[ADV_LEN_MAX - 2]; // This is the data and the biggest one is 31-2 = 29
} BLE_ADV_FIELD;

typedef PACKED struct
{
    UINT16 fine_timer_interval; //ms
    UINT8 default_adv; //default adv
    UINT8 button_adv_toggle; //pairing button make adv toggle (if 1) or always on (if 0)
    UINT16 high_undirect_adv_interval; //slots
    UINT16 low_undirect_adv_interval; //slots
    UINT16 high_undirect_adv_duration; //seconds
    UINT16 low_undirect_adv_duration; //seconds
    UINT16 high_direct_adv_interval; //seconds
    UINT16 low_direct_adv_interval; //seconds
    UINT16 high_direct_adv_duration; //seconds
    UINT16 low_direct_adv_duration; //seconds
    char local_name[LOCAL_NAME_LEN_MAX];
    char cod[COD_LEN];
    char ver[VERSION_LEN];
    UINT8 encr_required; // if 1, encryption is needed before sending indication/notification
    UINT8 disc_required;// if 1, disconnection after confirmation
    UINT8 test_enable;
    UINT8 tx_power_level; //dbm
    UINT8 con_idle_timeout; //second
    UINT8 powersave_timeout; //second
    UINT16 hdl[HANDLE_NUM_MAX];   //GATT HANDLE number
    UINT16 serv[HANDLE_NUM_MAX];  //GATT service UUID
    UINT16 cha[HANDLE_NUM_MAX];   // GATT characteristic UUID
    UINT8 findme_locator_enable; //if 1 Find me locator is enable
    UINT8 findme_alert_level; //alert level of find me
    UINT8 client_grouptype_enable; // if 1 grouptype read can be used
    UINT8 linkloss_button_enable; //if 1 linkloss button is enable
    UINT8 pathloss_check_interval; //second
    UINT8 alert_interval; //interval of alert
    UINT8 high_alert_num;     //number of alert for each interval
    UINT8 mild_alert_num;     //number of alert for each interval
    UINT8 status_led_enable;    //if 1 status LED is enable
    UINT8 status_led_interval; //second
    UINT8 status_led_con_blink; //blink num of connection
    UINT8 status_led_dir_adv_blink; //blink num of dir adv
    UINT8 status_led_un_adv_blink; //blink num of undir adv
    UINT16 led_on_ms;  //led blink on duration in ms
    UINT16 led_off_ms; //led blink off duration in ms
    UINT16 buz_on_ms; //buzzer on duration in ms
    UINT8 button_power_timeout; // seconds
    UINT8 button_client_timeout; // seconds
    UINT8 button_discover_timeout; // seconds
    UINT8 button_filter_timeout; // seconds
#ifdef BLE_UART_LOOPBACK_TRACE
    UINT8 button_uart_timeout; // seconds
#endif
} BLE_PROFILE_CFG;

typedef PACKED struct
{
    UINT32 baudrate;
    UINT8  txpin; //GPIO pin number
    UINT8  rxpin; //GPIO pin number
} BLE_PROFILE_PUART_CFG;

typedef PACKED struct
{
    INT8 gpio_pin[GPIO_NUM_MAX];  //pin number of gpio
    UINT16 gpio_flag[GPIO_NUM_MAX]; //flag of gpio
} BLE_PROFILE_GPIO_CFG;

//GATT_PDU
typedef PACKED struct
{
    UINT8 len;
    UINT8 header;
    UINT8 pdu[LEATT_ATT_MTU - 1];
} BLEPROFILE_DB_PDU;


//Timestamp
typedef UINT8 TIMESTAMP[TIMESTAMP_LEN];

// Timer control
typedef PACKED struct
{
    UINT8 count;
    UINT8 interval;
    UINT8 blinknum;
    UINT8 duration;
    UINT8 timeout;
}  BLE_PROFILE_TIMER_CONTROL;

//host information for NVRAM
typedef PACKED struct
//typedef struct
{
    // BD address of the bonded host
    BD_ADDR  bdAddr;
    UINT16 serv;
    UINT16 cha;
    UINT16 cli_cha_desc;
}  BLEPROFILE_HOSTINFO;


// pwm control
typedef PACKED struct
{
    UINT8 freq;
    UINT16 init_value;
    UINT16 toggle_val;
}  BLE_PROFILE_PWM_CFG;

typedef UINT16 SFLOAT;
typedef UINT32 FLOAT32;

//Discoverable mode
enum ble_discover_mode
{
    NO_DISCOVERABLE = 0,
    LOW_DIRECTED_DISCOVERABLE = 1,
    HIGH_DIRECTED_DISCOVERABLE = 2,
    LOW_UNDIRECTED_DISCOVERABLE = 3,
    HIGH_UNDIRECTED_DISCOVERABLE = 4,
    MANDATORY_DISCOVERABLE = 0xFF, //without NVRAM checking and start high undirected adv
};

//Find me client state
enum ble_findme_state
{
    FINDME_IDLE = 0,
    FINDME_WAIT_READ_PRIMARY_SERVICE_DEFER = 1,
    FINDME_WAIT_READ_PRIMARY_SERVICE = 2,
    FINDME_WAIT_READ_CHARACTERISTIC_DEFER = 3,
    FINDME_WAIT_READ_CHARACTERISTIC = 4,
    FINDME_WAIT_WRITE_ALERTLEVEL_DEFER = 5,
};

//time client state
enum ble_timeclient_state
{
    TIMECLIENT_IDLE = 0,
    TIMECLIENT_WAIT_READ_PRIMARY_SERVICE_DEFER = 1,
    TIMECLIENT_WAIT_READ_PRIMARY_SERVICE = 2,
    TIMECLIENT_WAIT_READ_CHARACTERISTIC_DEFER = 3,
    TIMECLIENT_WAIT_READ_CHARACTERISTIC = 4,
    TIMECLIENT_WAIT_READ_DESC_DEFER = 5,
    TIMECLIENT_WAIT_READ_DESC = 6,
    TIMECLIENT_WAIT_READ_CURRENTTIME_DEFER = 7,
    TIMECLIENT_WAIT_READ_CURRENTTIME = 8,
};

//client state
enum ble_client_state
{
    CLIENT_IDLE = 0,
    CLIENT_WAIT_READ_PRIMARY_SERVICE_DEFER = 1,
    CLIENT_WAIT_READ_PRIMARY_SERVICE = 2,
    CLIENT_WAIT_READ_CHARACTERISTIC_DEFER = 3,
    CLIENT_WAIT_READ_CHARACTERISTIC = 4,
    CLIENT_WAIT_READ_DESC_DEFER = 5,
    CLIENT_WAIT_READ_DESC = 6,
    CLIENT_WAIT_ACTION_DEFER = 7,
    CLIENT_WAIT_ACTION = 8,
    CLIENT_WAIT_READ = 9,
    CLIENT_WAIT_WRITE = 10,
};

//client action
enum ble_client_action
{
    CLIENT_READREQ = 0,
    CLIENT_WRITECMD = 1,
    CLIENT_WRITEREQ = 2,
};

//Client Characteristic Configuration mode
enum ble_ccc_mode
{
    CCC_NONE = 0x00,
    CCC_NOTIFICATION = 0x01,
    CCC_INDICATION = 0x02,
    CCC_RESERVED = 0x04,
};

//Server Characteristic Configuration mode
enum ble_scc_mode
{
    SCC_NONE = 0x00,
    SCC_BROADCAST = 0x01,
};

enum ble_disc_setting
{
    DISC_NONE = 0x00,
    DISC_AFTER_CONFIRM = 0x01,
    DISC_ATT_TIMEOUT = 0x02,
    DISABLE_ATT_TIMEOUT = 0x04,
};

//Button Function list
enum ble_button_function
{
    BUTTON_PRESS = 0,
    BUTTON_RELEASE = 1,
    BUTTON_POWER = 2,
    BUTTON_CLIENT = 3,
    BUTTON_DISCOVER = 4,
    BUTTON_FILTER = 5,
#ifdef BLE_UART_LOOPBACK_TRACE
    BUTTON_UART = 6,
#endif
    TIMEOUT_POWER = 10, // This is not related to button, but notify when powersave is started
};

enum ble_puart_disable
{
    PUARTENABLE = 0x00,
    PUARTDISABLE = 0x80,
};

enum ble_pwmbuz_freq
{
    PWMBUZ_125 = 0,
    PWMBUZ_250 = 1,
    PWMBUZ_500 = 2,
    PWMBUZ_1000 = 3,
    PWMBUZ_2000 = 4,
    PWMBUZ_4000 = 5,
    PWMBUZ_8000 = 6,
    PWMBUZ_FREQ_MAX = 7,
    PWMBUZ_FREQ_MANUAL = 0xFF,
};

typedef UINT32(*BLEPROFILE_QUERY_PARAM_CB)(UINT32, UINT32);
typedef UINT8(*BLEPROFILE_DOUBLE_PARAM_CB)(UINT8 *, UINT8 *);
typedef UINT32(*BLEPROFILE_SINGLE_PARAM_CB)(UINT32);
typedef UINT8(*BLEPROFILE_NO_PARAM_CB)(void);

#ifdef _WIN32
#include <poppack.h>
#endif
// GHS syntax.
#pragma pack()

//////////////////////////////////////////////////////////////////////////////
//                      extern global variable
//////////////////////////////////////////////////////////////////////////////
extern BLE_PROFILE_CFG bleprofile_cfg;
extern BLE_PROFILE_PUART_CFG bleprofile_puart_cfg;
extern BLE_PROFILE_GPIO_CFG bleprofile_gpio_cfg;
extern BLE_PROFILE_PWM_CFG bleprofile_pwm_cfg;

extern UINT8 *bleprofile_p_db;
extern UINT16 bleprofile_db_size;
extern BLE_PROFILE_PUART_CFG *bleprofile_puart_p_cfg;
extern BLE_PROFILE_GPIO_CFG *bleprofile_gpio_p_cfg;
extern BLE_PROFILE_CFG *bleprofile_p_cfg;
extern BD_ADDR bleprofile_remote_addr;

extern BLEPROFILE_DOUBLE_PARAM_CB bleprofile_handleUARTCb;

extern UINT8 bleprofile_PUART_RxInt_Enable;
extern UINT8 bleprofile_puart_tx_waterlevel;

//////////////////////////////////////////////////////////////////////////////
//                      public interface declaration
//////////////////////////////////////////////////////////////////////////////
void bleprofile_GenerateADVData(BLE_ADV_FIELD *p_adv, UINT8 num);
void bleprofile_GenerateScanRspData(BLE_ADV_FIELD *p_adv, UINT8 num);

void bleprofile_DBRegister(UINT8 *p_db, UINT16 db_size);
void bleprofile_DBInit(UINT8 **startPtr, UINT8 **endPtr);
void bleprofile_CFGRegister(BLE_PROFILE_CFG *p_cfg);
void bleprofile_PUARTRegister(BLE_PROFILE_PUART_CFG *p_puart_cfg);
void bleprofile_GPIORegister(BLE_PROFILE_GPIO_CFG *p_gpio_cfg);

void bleprofile_Init(BLE_PROFILE_CFG *p_cfg);
void bleprofile_GPIOWPInit(BLE_PROFILE_GPIO_CFG *p_gpio_cfg);
void bleprofile_GPIOInit(BLE_PROFILE_GPIO_CFG *p_gpio_cfg);

//NVRAM related functions
UINT8 bleprofile_ReadNVRAM(UINT8 vsID, UINT8 itemLength, UINT8 *payload);
UINT8 bleprofile_WriteNVRAM(UINT8 vsID, UINT8 itemLength, UINT8 *payload);
BOOL32 bleprofile_DeleteNVRAM(UINT8 vsID);
void bleprofile_NVRAMCheck(void);

void bleprofile_ReadUART(char *data);

//local GATT DB related function
int bleprofile_ReadHandle(UINT16 hdl, BLEPROFILE_DB_PDU *p_pdu);
int bleprofile_WriteHandle(UINT16 hdl, BLEPROFILE_DB_PDU *p_pdu);

//GATT client related function
void bleprofile_sendWriteReq(UINT16 attrHandle, UINT8 *attr, int len);
void bleprofile_sendWriteCmd(UINT16 attrHandle, UINT8 *data, int len);
void bleprofile_sendReadReq(UINT16 Handle);
void bleprofile_sendReadByTypeReq(UINT16 startHandle, UINT16 endHandle, UINT16 uuid16);
void bleprofile_sendReadByGroupTypeReq(UINT16 startHandle, UINT16 endHandle, UINT16 uuid16);
void bleprofile_sendHandleValueConf(void);


//GATT server related function
void bleprofile_sendIndication(UINT16 attrHandle, UINT8 *attr, int len, LEATT_NO_PARAM_CB cb);
void bleprofile_sendNotification(UINT16 attrHandle, UINT8 *attr, int len);

void bleprofile_WPEnable(UINT8 wp_enable);

void bleprofile_LEDOn(void);
void bleprofile_LEDOff(void);
void bleprofile_LEDBlink(UINT16 on_ms, UINT16 off_ms, UINT8 num);

void bleprofile_BUZOn(void);
void bleprofile_BUZOff(void);
void bleprofile_BUZBeep(UINT16 duration_ms);
void bleprofile_PWMBUZFreq(UINT8 preset, UINT16 init_value, UINT16 toggle_val);

void bleprofile_BatOn(void);
void bleprofile_BatOff(void);

void bleprofile_UARTOn(void);
void bleprofile_UARTOff(void);

void bleprofile_PUARTTxOn(void);
void bleprofile_PUARTTxOff(void);
void bleprofile_PUARTRxOn(void);
void bleprofile_PUARTRxOff(void);

INT8 bleprofile_ReadBut(void);
INT8 bleprofile_ReadBut1(void);
INT8 bleprofile_ReadBut2(void);
INT8 bleprofile_ReadBut3(void);

SFLOAT bleprofile_UINT16toSFLOAT(UINT16 uin);
UINT16 bleprofile_SFLOATtoUINT16(SFLOAT sfl);
SFLOAT bleprofile_UINT8_UINT8toSFLOAT(UINT8 minus, UINT8 uin, UINT8 bdp);
void bleprofile_SFLOATTest(void);
FLOAT32 bleprofile_UINT32toFLOAT32(UINT32 uin);
UINT32 bleprofile_FLOAT32toUINT32(FLOAT32 fl);
FLOAT32 bleprofile_UINT16_UINT16toFLOAT32(UINT8 minus, UINT16 uin, UINT16 bdp);
void bleprofile_FLOAT32Test(void);

UINT8 bleprofile_GetDiscoverable(void);
void bleprofile_Discoverable(UINT8 Discoverable, BD_ADDR paired_addr);
void bleprofile_enteringDiscLowPowerMode(UINT32 lowPowerMode);
void bleprofile_abortingDiscLowPowerMode(UINT32 lowPowerMode);
void bleprofile_PrepareHidOff(void);
//callback register function for UART data
void bleprofile_regHandleUARTCb(BLEPROFILE_DOUBLE_PARAM_CB cb);

//callback register function for GPIO Interrupt
void bleprofile_regIntCb(BLEPROFILE_SINGLE_PARAM_CB cb);

// Generic usage of Button
void bleprofile_regButtonFunctionCb(BLEPROFILE_SINGLE_PARAM_CB cb);
void bleprofile_ReadButton(void);

void bleprofile_regTimerCb(BLEAPP_TIMER_CB fine_cb, BLEAPP_TIMER_CB normal_cb);
void bleprofile_StartTimer(void);
void bleprofile_KillTimer(void);

void bleprofile_regAppEvtHandler(BLECM_APP_EVT_ENUM idx, BLECM_NO_PARAM_FUNC func);

void bleprofile_InitPowersave(void);
UINT8 bleprofile_pollPowersave(void);
void bleprofile_StartPowersave(void);
void bleprofile_StopPowersave(void);
void bleprofile_PreparePowersave(void);

void bleprofile_setupTargetAdrInScanRsp(void);
void bleprofile_appTimerCb(UINT32 arg);
void bleprofile_setidletimer_withNotification(void);

void bleprofile_Sleep(UINT8 hund_us);

int bleprofile_PUARTTx(char *data, UINT8 len);
int bleprofile_PUARTTxMaxWait(char *data, UINT8 len, UINT8 ms);
int bleprofile_PUARTTxEchoMaxWait(char *data, UINT8 len, UINT8 tx_ms, UINT8 rx_ms, UINT8 flush);
int bleprofile_PUARTRx(char *data, UINT8 len);
int bleprofile_PUARTRxMaxFail(char *data, UINT8 len, UINT8 max);
int bleprofile_PUARTRxMaxWait(char *data, UINT8 len, UINT8 ms);

void bleprofile_PUARTInitTxWaterlevel(UINT8 puart_tx_waterlevel);
void bleprofile_PUARTSetTxWaterlevel(UINT8 puart_tx_waterlevel);
void bleprofile_PUART_EnableRxInt(UINT8 rxpin, void (*userfn)(void *));
void bleprofile_PUART_EnableRxFIFOInt(void (*userfn)(void *));
void bleprofile_PUARTSetRxWaterlevel(UINT8 puart_rx_waterlevel);
void bleprofile_PUARTRegisterRxHandler(void (*RxHandler)(void));
void bleprofile_PUARTRegisterCSARxHandler(void (*userfn)(void *));
void bleprofile_PUARTEnableAFFInt(void);
void bleprofile_PUART_deassertCts(void);
void bleprofile_PUART_assertCts(void);
void bleprofile_PUART_ConfigRTS(BOOL enabled, UINT8 rtsPortPin, UINT8 assertvalue);
void bleprofile_PUART_EnableRxFIFOCTSInt(UINT8 ctsPortPin, UINT8 waterLevel, UINT8 assertvalue, UINT8 disableINT);
INT32 bleprofile_PUARTRxCTS(char *data, UINT8 len);
INT32 bleprofile_PUARTRxMaxWaitCTS(char *data, UINT8 len, UINT16 us);
void bleprofile_PUART_RxIntCb(void *ptr, UINT8 portPin);
void bleprofile_regAppEvtHandler_DirectedAdvStop(BLEPROFILE_NO_PARAM_CB handler);
void bleprofile_regAppEvtHandler_leConnUpdateComplete(BLEPROFILE_NO_PARAM_CB handler);
#endif
