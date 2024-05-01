#ifndef __SAPP_FUNC_H__
#define __SAPP_FUNC_H__


#include <Arduino.h>
#include "string.h"
#include "cJSON.h"
#include <SPI.h>

#include "main.h"

#include "Freenove_WS2812_Lib_for_ESP32.h"

#include "sDRV_Si24R1.h"



#define WS2812_COUNT   4
#define WS2812_PIN	   26
#define WS2812_CH	   0



#define ADC_CURR_PIN   35
#define ADC_VOLT_PIN   34






void sAPP_Func_InitOLED();

void sAPP_Func_InitWS2812();
void sAPP_Func_SetWS2812Brightness(uint8_t brightness);
void sAPP_Func_SetWS2812();


void sAPP_Func_ADCInit();
float sAPP_Func_GetVoltMV();
float sAPP_Func_GetCurrMA();


void sAPP_Func_RTC_Init();
void sAPP_Func_RTC_Get12HTime(char* str);
uint8_t sAPP_Func_GetBatPercent();




#endif
