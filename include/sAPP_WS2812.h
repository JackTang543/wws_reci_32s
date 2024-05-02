#ifndef __SAPP_WS2812_H__
#define __SAPP_WS2812_H__

#include "main.h"

#include <Freenove_WS2812_Lib_for_ESP32.h>


#define WS2812_COUNT   4
#define WS2812_PIN	   26
#define WS2812_CH	   1

#define GAMMA_VAL 1.4f


typedef enum{
    WS2812_MODE_ADJ_VOLUME = 0,
    WS2812_MODE_PULSE_CYCLE,
    WS2812_MODE_PULSE_SINGLE,
    WS2812_MODE_CYCLE,
}ws2812_mode_t;

typedef enum{
    WS2812_STATUS_OFF = 0,
    WS2812_STATUS_ON = 1
}ws2812_status;

typedef struct{
    ws2812_mode_t mode;
    int8_t brightness;
    uint16_t time_ms;
    uint32_t prev_chrg_ts;
    uint32_t on_ts;
    uint32_t off_ts;
    ws2812_status status;
    uint8_t single_pulse_trig;
    uint8_t r;
    uint8_t g;
    uint8_t b;
}ws2812_t;


void sAPP_WS2812_Init();
void sAPP_WS2812_SetBrightness(uint8_t brightness_percent);
void sAPP_WS2812_SetMode(ws2812_mode_t ws2812_mode);
void sAPP_WS2812_SetCycleTime_ms(uint16_t time);
void sAPP_WS2812_SetPulseTime_ms(uint32_t on_time, uint32_t period_time);
void sAPP_WS2812_StartSinglePulse();
void sAPP_WS2812_SetRGB(uint8_t r, uint8_t g, uint8_t b);
void sAPP_WS2812_Handler();





#endif
