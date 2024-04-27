#ifndef __SAPP_TASK_H__
#define __SAPP_TASK_H__


#include <Arduino.h>

#include "sAPP_Func.h"


extern TaskHandle_t sAPP_TaskH_2D4GHzISRDataH;



void sAPP_Task_2D4GHzReciedDataH(void* pvPara);
void sAPP_Task_UpdateScreen(void* pvPara);
void sAPP_Task_BtnHandler(void* pvPara);
void sAPP_Task_BuzzerHandler(void* pvPara);
void sAPP_Task_ReadADC(void* pvPara);



#endif
