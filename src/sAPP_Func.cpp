#include "sAPP_Func.h"



String dataPacketToJsonString(const data_packet_t* packet) {
    cJSON *root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "aht10_humi", packet->aht10_humi);
    cJSON_AddNumberToObject(root, "aht10_temp", packet->aht10_temp);
    cJSON_AddNumberToObject(root, "bmp280_pres", packet->bmp280_pres);
    cJSON_AddNumberToObject(root, "bmp280_temp", packet->bmp280_temp);
    cJSON_AddNumberToObject(root, "temt_mv", packet->temt_mv);
    cJSON_AddNumberToObject(root, "vbat", packet->vbat);
    cJSON_AddNumberToObject(root, "status", packet->status);

    char* rawJson = cJSON_Print(root);
    String jsonString = String(rawJson); // 将 char* 转换为 String 对象
    free(rawJson); // 释放 cJSON_Print 分配的内存
    cJSON_Delete(root); // 清理 cJSON 对象

    return jsonString;
}


void sAPP_Func_RTC_Get12HTime(char* str){
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    if(timeinfo.tm_sec % 2 == 0){
        if(timeinfo.tm_hour > 12){
            sprintf(str,"PM %02d:%02d",timeinfo.tm_hour - 12,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour == 12){
            sprintf(str,"PM %02d:%02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour < 12){
            sprintf(str,"AM %02d:%02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
    }else{
        if(timeinfo.tm_hour > 12){
            sprintf(str,"PM %02d %02d",timeinfo.tm_hour - 12,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour == 12){
            sprintf(str,"PM %02d %02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour < 12){
            sprintf(str,"AM %02d %02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
    }
}


void sAPP_Func_RTC_Init(){
    struct tm timeinfo = {};
    timeinfo.tm_year = 2024 - 1900; // 年份 - 1900
    timeinfo.tm_mon = 4 - 1;        // 月份，从0开始
    timeinfo.tm_mday = 29;          // 日
    timeinfo.tm_hour = 19;          // 时
    timeinfo.tm_min = 30;           // 分
    timeinfo.tm_sec = 0;            // 秒
    //做一个时间戳
    time_t t = mktime(&timeinfo);
    timeval now = { .tv_sec = t };
    //用时间戳设置时间
    settimeofday(&now, NULL);
}



