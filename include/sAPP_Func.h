#ifndef __SAPP_FUNC_H__
#define __SAPP_FUNC_H__


#include <Arduino.h>
#include "string.h"
#include "cJSON.h"
#include <SPI.h>

#include "main.h"

#include "Freenove_WS2812_Lib_for_ESP32.h"

#include "sDRV_Si24R1.h"
#include "sGenBtnDrv2.h"



#define WS2812_COUNT   4
#define WS2812_PIN	   26
#define WS2812_CH	   0

#define BTN_UP_PIN     36
#define BTN_DOWN_PIN   33
#define BTN_ENTER_PIN  39
#define BTN_BACK_PIN   32

#define BTN_UP_ID      0
#define BTN_DOWN_ID    1
#define BTN_ENTER_ID   2
#define BTN_BACK_ID    3

#define ADC_CURR_PIN   35
#define ADC_VOLT_PIN   34


typedef struct{
    float aht10_humi;
    float aht10_temp;
    float bmp280_pres;
    float bmp280_temp;
    float temt_mv;
    float vbat;
    uint16_t status;
}data_packet_t;



void sAPP_Func_InitOLED();
void sAPP_Func_InitBtns();



void sAPP_Func_InitWS2812();
void sAPP_Func_SetWS2812Brightness(uint8_t brightness);
void sAPP_Func_SetWS2812();


void sAPP_Func_ADCInit();
float sAPP_Func_GetVoltMV();
float sAPP_Func_GetCurrMA();



void sAPP_Func_DataHandler();
void sAPP_Func_Init2D4GHz();





#endif
