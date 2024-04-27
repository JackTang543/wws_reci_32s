#ifndef __MAIN_H__
#define __MAIN_H__



#include <Arduino.h>

#include "sAPP_Func.h"

#include "sGraphic2D.h"

#include "sAPP_Task.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>

#include "sBSP_SPI.h"

#include "sAPP_Task.h"
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
//2.4G数据消息队列,只有一个元素
extern QueueHandle_t si24r1_data_queue;
//按键事件消息队列,5个元素
extern QueueHandle_t btn_ev_data_queue;
//电池电压电流
extern float bat_mv;
extern float bat_ma;

extern data_packet_t data_packet_p1;

extern uint32_t page;

extern int16_t y_pos;



#endif
