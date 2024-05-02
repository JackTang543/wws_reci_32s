#include "sAPP_WS2812.h"


Freenove_ESP32_WS2812 ws2812 = Freenove_ESP32_WS2812(WS2812_COUNT, WS2812_PIN, WS2812_CH, TYPE_GRB);

ws2812_t ws2812_conf;


//伽马校正
static float gamma_correction(float percent_brightness) {
    //将输入的亮度百分比转换为0到1之间的范围
    float normalized_brightness = percent_brightness / 100.0;
    //应用伽马校正公式
    float corrected_brightness = pow(normalized_brightness, GAMMA_VAL);
    //将校正后的亮度映射回PWM占空比范围
    float pwm_duty_cycle = corrected_brightness * 100.0;
    return pwm_duty_cycle;
}


void sAPP_WS2812_Init(){
    ws2812.begin();
    ws2812.setBrightness(0);
}

void sAPP_WS2812_SetBrightness(uint8_t brightness_percent){
    ws2812.setBrightness((uint8_t)gamma_correction((float)brightness_percent));
}

void sAPP_WS2812_SetMode(ws2812_mode_t ws2812_mode){
    ws2812_conf.status = WS2812_STATUS_OFF;
    ws2812_conf.mode = ws2812_mode;
}

//设置闪烁时间
void sAPP_WS2812_SetCycleTime_ms(uint16_t time){
    ws2812_conf.time_ms = time;
}

void sAPP_WS2812_SetPulseTime_ms(uint32_t on_time, uint32_t period_time){
    //参数检查
    if((on_time == 0) || (period_time < on_time)) return;
    ws2812_conf.on_ts = on_time;
    //一个循环的总时长-亮起时间就是熄灭时间
    ws2812_conf.off_ts = period_time - on_time;
}

//启动一次脉冲,只有单脉冲模式生效
void sAPP_WS2812_StartSinglePulse(){
    if(ws2812_conf.mode == WS2812_MODE_PULSE_SINGLE){
        //首先设置不输出
        ws2812_conf.status = WS2812_STATUS_OFF;
        //清空单次脉冲触发标志位
        ws2812_conf.single_pulse_trig = 0;
    }
}


void sAPP_WS2812_SetRGB(uint8_t r, uint8_t g, uint8_t b){
    ws2812_conf.r = r;
    ws2812_conf.g = g;
    ws2812_conf.b = b;
}


void sAPP_WS2812_Handler(){
    //循环脉冲模式
    if(ws2812_conf.mode == WS2812_MODE_PULSE_CYCLE){
        //配置错误处理
        if(ws2812_conf.on_ts == 0 || ws2812_conf.on_ts == 0) return;
        //和闪烁模式大体一样原理,只是多了根据led的状态选择比较哪个时间
        if ((millis() - ws2812_conf.prev_chrg_ts) >= (ws2812_conf.status ? ws2812_conf.on_ts : ws2812_conf.off_ts)) {
            ws2812_conf.prev_chrg_ts = millis();
            ws2812_conf.status = (ws2812_status)!ws2812_conf.status;
            if(ws2812_conf.status){
                ws2812.setAllLedsColorData(ws2812_conf.r, ws2812_conf.g, ws2812_conf.b);
            }else{
                ws2812.setAllLedsColor(0, 0, 0);
            }
        }

    }
    //单次脉冲模式
    else if(ws2812_conf.mode == WS2812_MODE_PULSE_SINGLE){
        //这个标志位使用来保证进入两次单脉冲模式的,进入两次才是一个完整脉冲
        if(ws2812_conf.single_pulse_trig >= 2) return;
        //配置错误处理
        if(ws2812_conf.on_ts == 0 || ws2812_conf.on_ts == 0) return;
        //同上,只是只会触发2次
        if ((millis() - ws2812_conf.prev_chrg_ts) >= (ws2812_conf.status ? ws2812_conf.on_ts : ws2812_conf.off_ts)) {
            ws2812_conf.prev_chrg_ts = millis();
            ws2812_conf.status = (ws2812_status)!ws2812_conf.status;
            if(ws2812_conf.status){
                ws2812.setAllLedsColorData(ws2812_conf.r, ws2812_conf.g, ws2812_conf.b);
            }else{
                ws2812.setAllLedsColor(0, 0, 0);
            }
            //标志位++
            ws2812_conf.single_pulse_trig++;
            
        }
    }
    ws2812.show();
}


