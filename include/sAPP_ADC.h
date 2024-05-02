#ifndef __SAPP_ADC_H__
#define __SAPP_ADC_H__

#include <Arduino.h>
#include "main.h"


void sAPP_ADC_Init();
float sAPP_ADC_GetVoltMV();
float sAPP_ADC_GetCurrMA();
uint8_t sAPP_ADC_GetBatPercent();



#endif

