#ifndef __SAPP_FUNC_H__
#define __SAPP_FUNC_H__


#include <Arduino.h>
#include "string.h"
#include "cJSON.h"
#include <SPI.h>

#include "main.h"

#include "sDRV_Si24R1.h"



#define ADC_CURR_PIN   35
#define ADC_VOLT_PIN   34




void sAPP_Func_RTC_Init();
void sAPP_Func_RTC_Get12HTime(char* str);




#endif
