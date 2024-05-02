#ifndef __SAPP_NVS_H__
#define __SAPP_NVS_H__


#include <Arduino.h>
#include <Preferences.h>
#include "main.h"

extern Preferences nvs;


#define SAPP_NVS_KEY_OLED_BL_PERCENT    ((const char*)"oled_bl")
#define SAPP_NVS_KEY_WS2812_BL_PERCENT  ((const char*)"ws2812_bl")


void sAPP_NVS_Init();
void sAPP_NVS_SaveInt(char* key, int value);
int sAPP_NVS_GetInt(char* key);
void sAPP_NVS_SaveUInt(char* key, uint value);
uint sAPP_NVS_GetUInt(char* key);
void sAPP_NVS_SaveFloat(char* key, float value);
float sAPP_NVS_GetFloat(char* key);
void sAPP_NVS_SaveString(char* key, char* value);
String sAPP_NVS_GetString(char* key);

void sAPP_NVS_GetAllData();


#endif

