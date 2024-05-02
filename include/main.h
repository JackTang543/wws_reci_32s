#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>

#include "sAPP_Task.h"
#include "sAPP_Func.h"

#include "sGraphic2D.h"
#include "cot_menu.h"

#include "sAPP_Btns.h"
#include "sAPP_Menu.h"
#include "sAPP_2D4GHz.h"
#include "sAPP_NVS.h"
#include "sAPP_WS2812.h"
#include "sAPP_ADC.h"
#include "sAPP_UART.h"

#include "sDRV_Buzzer.h"



typedef struct{
    float aht10_humi;
    float aht10_temp;
    float bmp280_pres;
    float bmp280_temp;
    float temt_mv;
    float vbat;
    uint16_t status;
}data_packet_t;


//全局变量
extern QueueHandle_t g_si24r1_data_queue;
extern QueueHandle_t g_sgbd2_ev_mq;
extern SemaphoreHandle_t g_sapp_menu_item_selected_sem;
extern SemaphoreHandle_t g_sapp_menu_item_unselect_sem;

extern data_packet_t g_data_packet_p1;


//按键事件消息队列,5个元素
extern QueueHandle_t btn_ev_data_queue;
//电池电压电流
extern float bat_mv;
extern float bat_ma;
extern uint8_t g_oled_bl_percent;
extern uint8_t g_ws2812_bl_percent;

extern Si24R1_Data_t si24r1_data_p1;



extern uint32_t page;


extern Si24R1_Conf_t g_si24r1_conf;

extern char* WiFi_SSID;
extern char* WiFi_PWD;



#endif
