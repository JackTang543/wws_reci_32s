#include "sAPP_Btns.h"


//sAPP_Btns.cpp
//WWS reci应用层 按键
//By Sightseer.
//TIME:2024.05.01 五一快乐

//用于告诉sAPP_Menu发生了什么事件
sAPP_Btns_Event_t btns_event;
//是否处在选中模式,如果处在选中模式,则会发送消息给items
uint8_t is_selected = 0;



/**
 * @brief 按键事件回调,sGBD2检测到按键事件会自动调用
 * 
 * @param btn_id : 触发事件的按键的ID
 * @param btn_ev : 触发的事件
 */
void sAPP_Btns_EventCb(uint8_t btn_id,ev_flag_t btn_ev){
    //非阻塞式读取二值信号量
    if(xSemaphoreTake(g_sapp_menu_item_selected_sem,0) == pdTRUE){
        Serial.println("selected");
        is_selected = 1;
    }
    if(xSemaphoreTake(g_sapp_menu_item_unselect_sem,0) == pdTRUE){
        Serial.println("unselect");
        is_selected = 0;
    }

    //有任何按键事件都会导致熄屏定时器重置,并重新启动OLED使能
    sG2D_SetOLEDShow(1);
    //如果大于200s说明永远不息屏
    if(g_screen_time_s < 200){
        xTimerReset(g_screen_sleep_timer,0);
    }
    

    //缓存事件
    btns_event.btn_id = btn_id;
    btns_event.btn_ev = btn_ev;

    //按键:向上
    if(btn_id == BTN_UP_ID){
        //按下松手
        if(btn_ev == ev_rlsd){
            //让蜂鸣器响一下
            sDRV_BUZZER_StartSinglePulse();

            //如果不处在选中模式,才给cotMenu一个操作
            if(!is_selected){
                cotMenu_SelectPrevious(1);
            }else{
                //处在选中模式,发送消息给items
                xQueueSend(g_sgbd2_ev_mq,&btns_event,0);
            }
        }
        //长按按下
        else if(btn_ev == ev_lp){

        }
        //长按循环触发
        else if(btn_ev == ev_lp_loop){
            if(!is_selected){
                cotMenu_SelectPrevious(1);
            }else{
                xQueueSend(g_sgbd2_ev_mq,&btns_event,0);
            }
        }
    }
    //按键:向下
    else if(btn_id == BTN_DOWN_ID){
        if(btn_ev == ev_rlsd){
            sDRV_BUZZER_StartSinglePulse();
            if(!is_selected){
                cotMenu_SelectNext(1);
            }else{
                xQueueSend(g_sgbd2_ev_mq,&btns_event,0);
            }
            
        }
        else if(btn_ev == ev_lp){

        }
        else if(btn_ev == ev_lp_loop){
            if(!is_selected){
                cotMenu_SelectNext(1);
            }else{
                xQueueSend(g_sgbd2_ev_mq,&btns_event,0);
            }
        }
    }
    //按键:确定
    else if(btn_id == BTN_ENTER_ID){
        if(btn_ev == ev_rlsd){
            
            if(!is_selected){
                if(cotMenu_Enter() == 0){
                    sDRV_BUZZER_StartSinglePulse();
                }else{
                    Serial.printf("cotMenu_Enter error\n");
                }
            }else{
                sDRV_BUZZER_StartSinglePulse();
                xQueueSend(g_sgbd2_ev_mq,&btns_event,0);
            }
        }
        else if(btn_ev == ev_lp){

        }
    }
    //按键:返回
    else if(btn_id == BTN_BACK_ID){
        if(btn_ev == ev_rlsd){
            if(cotMenu_Exit(1) == 0){
                //操作有效才响一下
                sDRV_BUZZER_StartSinglePulse();
            }
            
        }
        else if(btn_ev == ev_lp){

        }
    }
}

/**
 * @brief  获取按键电平给sGBD2
 * 
 * @param  btn_id : sGBD2想要获取的按键ID
 * @return bool   : 按键电平
 */
static bool get_lv(uint8_t btn_id){
    if(btn_id == BTN_UP_ID){
        return !!digitalRead(BTN_UP_PIN);
    }
    else if(btn_id == BTN_DOWN_ID){
        return !!digitalRead(BTN_DOWN_PIN);
    }
    else if(btn_id == BTN_ENTER_ID){
        return !!digitalRead(BTN_ENTER_PIN);
    }
    else if(btn_id == BTN_BACK_ID){
        return !!digitalRead(BTN_BACK_PIN);
    }
    return false;
}

/**
 * @brief 获取系统滴答定时器
 * 
 * @return uint32_t : tick
 */
static uint32_t GetTick(){
    return millis();
}

/**
 * @brief 按键初始化
 * 
 */
void sAPP_Btns_Init(){
    pinMode(BTN_UP_PIN   ,INPUT);
    pinMode(BTN_DOWN_PIN ,INPUT);
    pinMode(BTN_ENTER_PIN,INPUT);
    pinMode(BTN_BACK_PIN ,INPUT);

    btn_init_t btn_init;
    btn_init.en = 1;                 //使能此按键
    btn_init.lv_rev = lv_non_reverse;//空闲时的电平反转
    btn_init.dp_mode = dp_disable;   //禁用双击,可提高连续单击速度
    btn_init.lp_loop_pridt = 200;    //设置长按循环触发间隔每xms触发一次
    btn_init.lp_trig_waitt = 600;    //设置长按触发时间
    btn_init.dp_prid_waitt = 200;    //设置最大等待双击时间
    sGBD_SetAllBtnEnable(1);         //设置所有按键使能
    sGBD_SetAllBtnMode(&btn_init);   //装载btn_init的配置参数

    sGBD_Init(get_lv,sAPP_Btns_EventCb,GetTick);
}

