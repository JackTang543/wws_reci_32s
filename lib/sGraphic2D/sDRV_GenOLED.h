#ifndef __SDRV_GEN_OLED_H__
#define __SDRV_GEN_OLED_H__

#include "Arduino.h"



//sDRV_GenOLED.c
//SH1106 通用OLED显示驱动
//Sightseer's General OLED Driver


//引用外部GRAM
extern uint8_t GRAM[128][8];

int8_t sDRV_GenOLED_Init();
void sDRV_GenOLED_SetBrightness(uint8_t bl);
void sDRV_GenOLED_SetShowEN(uint8_t is_show);
void sDRV_GenOLED_SetDisRev(uint8_t is_reverse);
void sDRV_GenOLED_SetHorizontalFlip(uint8_t is_flip);
void sDRV_GenOLED_SetVerticalFlip(uint8_t is_flip);

void sDRV_GenOLED_SetBrightness(uint8_t bl);
void sDRV_GenOLED_UpdateScreen();


#endif
