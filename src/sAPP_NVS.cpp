#include "sAPP_NVS.h"

Preferences nvs;



void sAPP_NVS_Init(){
    nvs.begin("wws_reci_space", false);
}

void sAPP_NVS_SaveInt(char* key, int value){
    nvs.putInt(key, value);
}

int sAPP_NVS_GetInt(char* key){
    return nvs.getInt(key);
}

void sAPP_NVS_SaveUInt(char* key, uint value){
    nvs.putUInt(key, value);
}

uint sAPP_NVS_GetUInt(char* key){
    return nvs.getUInt(key);
}

void sAPP_NVS_SaveFloat(char* key, float value){
    nvs.putFloat(key, value);
}

float sAPP_NVS_GetFloat(char* key){
    return nvs.getFloat(key);
}

void sAPP_NVS_SaveString(char* key, char* value){
    nvs.putString(key, value);
}

String sAPP_NVS_GetString(char* key){
    return nvs.getString(key);
}

void sAPP_NVS_GetAllData(){
    nvs.begin("wws_reci", false);

    //OLED亮度
    if(nvs.getInt(SAPP_NVS_KEY_OLED_BL_PERCENT,-1) == -1){
        //Serial.printf("key:oled_bl 不存在\n");
        nvs.putInt(SAPP_NVS_KEY_OLED_BL_PERCENT,g_oled_bl_percent);
    }else{
        //Serial.printf("key:oled_bl 存在:%d\n",nvs.getInt(SAPP_NVS_KEY_OLED_BL_PERCENT));
        g_oled_bl_percent = nvs.getInt(SAPP_NVS_KEY_OLED_BL_PERCENT);
    }

    //WS2812亮度
    if(nvs.getInt(SAPP_NVS_KEY_WS2812_BL_PERCENT,-1) == -1){
        nvs.putInt(SAPP_NVS_KEY_WS2812_BL_PERCENT,g_ws2812_bl_percent);
    }else{
        g_ws2812_bl_percent = nvs.getInt(SAPP_NVS_KEY_WS2812_BL_PERCENT);
    }

    //OLED熄屏时间
    if(nvs.getInt(SAPP_NVS_KEY_OLED_SLEEP_TIME,-1) == -1){
        nvs.putInt(SAPP_NVS_KEY_OLED_SLEEP_TIME,g_screen_time_s);
    }else{
        g_screen_time_s = nvs.getInt(SAPP_NVS_KEY_OLED_SLEEP_TIME);
    }
    //防止0值崩溃
    if(g_screen_time_s == 0){g_screen_time_s = 5;}

    //BUZZER音量
    if(nvs.getInt(SAPP_NVS_KEY_BUZZER_VOLUME,-1) == -1){
        nvs.putInt(SAPP_NVS_KEY_BUZZER_VOLUME,g_buzzer_volume);
    }else{
        g_buzzer_volume = nvs.getInt(SAPP_NVS_KEY_BUZZER_VOLUME);
    }


    nvs.end();
}



