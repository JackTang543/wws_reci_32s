#ifndef __SAPP_BTNS_H__
#define __SAPP_BTNS_H__

#include <Arduino.h>

#include "sGenBtnDrv2.h"
#include "main.h"

#define BTN_UP_PIN     36
#define BTN_DOWN_PIN   33
#define BTN_ENTER_PIN  39
#define BTN_BACK_PIN   32

#define BTN_UP_ID      0
#define BTN_DOWN_ID    1
#define BTN_ENTER_ID   2
#define BTN_BACK_ID    3

typedef struct{
    uint8_t   btn_id;
    ev_flag_t btn_ev;
}sAPP_Btns_Event_t;

void sAPP_Btns_EventCb(uint8_t btn_id,ev_flag_t btn_ev);
void sAPP_Btns_Init();



#endif
