#ifndef __SAPP_TASK_H__
#define __SAPP_TASK_H__


#include <Arduino.h>

#include "sAPP_Func.h"




void sAPP_Task_2D4GHzReciedDataH(void* pvPara);
void sAPP_Task_UpdateScreen(void* pvPara);
void sAPP_Task_BtnHandler(void* pvPara);
void sAPP_Task_BuzzerHandler(void* pvPara);
void sAPP_Task_ReadADC(void* pvPara);
void sAPP_Task_WS2812Handler(void* pvPara);
void sAPP_Task_UARTHandler(void* pvPara);
void sAPP_Task_TIM_SyncTimeByNTP(TimerHandle_t xTimer);
void sAPP_Task_TIM_ScreenSleep(TimerHandle_t xTimer);



#endif
