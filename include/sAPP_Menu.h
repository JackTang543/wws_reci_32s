#ifndef __SAPP_MENU_H__
#define __SAPP_MENU_H__

#include <Arduino.h>
#include "main.h"

void sAPP_Menu_Init();


void ShowWiFiInfoMenu(MenuShow_t *ptShowInfo);
void WiFiInfoPage_Enter();


void sAPP_Menu_Handler();


#endif


