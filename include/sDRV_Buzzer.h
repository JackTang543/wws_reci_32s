#ifndef __SAPP_BUZZER_H__
#define __SAPP_BUZZER_H__

#include <Arduino.h>



#define LED_PIN         21      //定义使用的GPIO引脚
#define LEDC_CHANNEL    0       //选择PWM通道0
#define LEDC_FREQUENCY  1000    //默认PWM频率为1000Hz
#define LEDC_RESOLUTION 8       //分辨率为8位



typedef enum{
    BUZZER_MODE_ADJ_VOLUME = 0,
    BUZZER_MODE_PULSE_CYCLE,
    BUZZER_MODE_PULSE_SINGLE,
    BUZZER_MODE_CYCLE,
}buzzer_mode_t;

typedef enum{
    BUZZER_STATUS_OFF = 0,
    BUZZER_STATUS_ON = 1
}buzzer_status;

typedef struct{
    buzzer_mode_t mode;
    int8_t volume;
    uint16_t time_ms;
    uint32_t prev_chrg_ts;
    uint32_t on_ts;
    uint32_t off_ts;
    buzzer_status status;
    uint8_t single_pulse_trig;
}buzzer_t;

void sDRV_BUZZER_Init();
void sDRV_BUZZER_SetDuty(int8_t duty);
void sDRV_BUZZER_SetFreq(uint16_t freq);

void sDRV_BUZZER_SetMode(buzzer_mode_t buzzer_mode);
void sDRV_BUZZER_SetVolume(uint8_t volume_percent);
void sDRV_BUZZER_SetCycleTime_ms(uint16_t time);
void sDRV_BUZZER_SetPulseTime_ms(uint32_t on_time, uint32_t period_time);
void sDRV_BUZZER_StartSinglePulse();

void sDRV_BUZZER_Handler();




// void sDRV_BUZZER_SetMode(buzzer_mode_t buzzer_mode);
// void sDRV_BUZZER_SetVolume(uint8_t volume_percent);
// void sDRV_BUZZER_StartSinglePulse();
// void sDRV_BUZZER_SetPulseTime_ms(uint32_t on_time, uint32_t off_time);
// void sDRV_BUZZER_SetBlinkTime_ms(uint16_t time);
// void sDRV_BUZZER_Handler();





#endif
