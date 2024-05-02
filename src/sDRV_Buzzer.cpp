#include "sDRV_Buzzer.h"
 

buzzer_t buzzer;

void sDRV_BUZZER_Init(){
    ledcSetup(LEDC_CHANNEL,LEDC_FREQUENCY,LEDC_RESOLUTION);
    //将PWM信号绑定到GPIO
    ledcAttachPin(LED_PIN,LEDC_CHANNEL);
    ledcWrite(LEDC_CHANNEL,0);
}

void sDRV_BUZZER_SetDuty(int8_t duty){
    ledcWrite(LEDC_CHANNEL,duty);
}

void sDRV_BUZZER_SetFreq(uint16_t freq){
    ledcWriteTone(LEDC_CHANNEL,freq);
}

void sDRV_BUZZER_SetMode(buzzer_mode_t buzzer_mode){
    buzzer.status = BUZZER_STATUS_OFF;
    buzzer.mode = buzzer_mode;
}

void sDRV_BUZZER_SetVolume(uint8_t volume_percent){
    //限幅
    if(volume_percent > 90){
        volume_percent = 90;
    }
    buzzer.volume = volume_percent;
}

//设置闪烁时间
void sDRV_BUZZER_SetCycleTime_ms(uint16_t time){
    buzzer.time_ms = time;
}


void sDRV_BUZZER_SetPulseTime_ms(uint32_t on_time, uint32_t period_time){
    //参数检查
    if((on_time == 0) || (period_time < on_time)) return;
    buzzer.on_ts = on_time;
    //一个循环的总时长-亮起时间就是熄灭时间
    buzzer.off_ts = period_time - on_time;
}

//启动一次脉冲,只有单脉冲模式生效
void sDRV_BUZZER_StartSinglePulse(){
    if(buzzer.mode == BUZZER_MODE_PULSE_SINGLE){
        //首先设置不输出
        ledcWrite(LEDC_CHANNEL,0);
        buzzer.status = BUZZER_STATUS_OFF;
        //清空单次脉冲触发标志位
        buzzer.single_pulse_trig = 0;
    }
}



void sDRV_BUZZER_Handler(){
    //循环脉冲模式
    if(buzzer.mode == BUZZER_MODE_PULSE_CYCLE){
        //配置错误处理
        if(buzzer.on_ts == 0 || buzzer.on_ts == 0) return;
        //和闪烁模式大体一样原理,只是多了根据led的状态选择比较哪个时间
        if ((millis() - buzzer.prev_chrg_ts) >= (buzzer.status ? buzzer.on_ts : buzzer.off_ts)) {
            buzzer.prev_chrg_ts = millis();
            buzzer.status = (buzzer_status)!buzzer.status;
            if(buzzer.status){
                sDRV_BUZZER_SetDuty(buzzer.volume);
            }else{
                sDRV_BUZZER_SetDuty(0);
            }
        }

    }
    //单次脉冲模式
    else if(buzzer.mode == BUZZER_MODE_PULSE_SINGLE){
        //这个标志位使用来保证进入两次单脉冲模式的,进入两次才是一个完整脉冲
        if(buzzer.single_pulse_trig >= 2) return;
        //配置错误处理
        if(buzzer.on_ts == 0 || buzzer.on_ts == 0) return;
        //同上,只是只会触发2次
        if ((millis() - buzzer.prev_chrg_ts) >= (buzzer.status ? buzzer.on_ts : buzzer.off_ts)) {
            buzzer.prev_chrg_ts = millis();
            buzzer.status = (buzzer_status)!buzzer.status;
            if(buzzer.status){
                sDRV_BUZZER_SetDuty(buzzer.volume);
            }else{
                sDRV_BUZZER_SetDuty(0);
            }
            //标志位++
            buzzer.single_pulse_trig++;
        }
    }
}


