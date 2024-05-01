#ifndef __SAPP_MENU_H__
#define __SAPP_MENU_H__

#include <Arduino.h>
#include "main.h"

void sAPP_Menu_Init();


void ShowWiFiInfoMenu(MenuShow_t *ptS);
void Show2D4GHzInfoMenu(MenuShow_t *ptS);
void ShowSettingsMenu(MenuShow_t *ptS);
void ShowSettingsMenuBrightPage(MenuShow_t *ptS);
void ShowSettingsMenuSleepPage(MenuShow_t *ptS);
void ShowSettingsMenuWiFiPage(MenuShow_t *ptS);
void ShowSettingsMenuSyncNTPPage(MenuShow_t *ptS);

void TwoD4GHzInfoPage_Enter();
void WiFiInfoPage_Enter();
void SettingsPage_Enter();
void SettingsPage_Bright_Enter();
void SettingsPage_Sleep_Enter();
void SettingsPage_Volume_Enter();
void SettingsPage_WS2812_Enter();
void SettingsPage_WiFi_Enter();
void SettingsPage_NTP_Enter();

void sAPP_Menu_Handler();


#endif


