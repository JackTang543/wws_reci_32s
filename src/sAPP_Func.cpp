#include "sAPP_Func.h"
#include "sAPP_Task.h"




Freenove_ESP32_WS2812 ws2812 = Freenove_ESP32_WS2812(WS2812_COUNT, WS2812_PIN, WS2812_CH, TYPE_GRB);



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


void sAPP_Func_ADCInit(){
    // 配置ADC
    analogReadResolution(12); // 设置ADC分辨率为12位
    analogSetAttenuation(ADC_11db); // 设置衰减，以适应大约0到3.3V的输入
    
}


float sAPP_Func_GetVoltMV(){
    //return ((float)analogRead(ADC_VOLT_PIN) / 4095.0f) * 3300.0f * 2.0f;
    return (float)analogReadMilliVolts(ADC_VOLT_PIN) * 2.0f;
}

float sAPP_Func_GetCurrMA(){
    //return ((float)analogRead(ADC_CURR_PIN) / 4095.0f) * 3300.0f - 1100.0f;
    return (float)analogReadMilliVolts(ADC_CURR_PIN) - 1100.0f;
}





#include "sGraphic2D.h"




void sAPP_Func_InitOLED(){
    sDRV_GenOLED_Init();
}




void sAPP_Func_InitWS2812(){
    ws2812.begin();
    ws2812.setBrightness(10);
}

void sAPP_Func_SetWS2812Brightness(uint8_t brightness){
    ws2812.setBrightness(brightness);
}

void sAPP_Func_SetWS2812(){
    ws2812.setAllLedsColor(60,60,0);
    ws2812.show();
}



#include "time.h"

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
    time_t t = mktime(&timeinfo);
    timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
}


#define WINDOW_SIZE 10  // 滑动窗口大小

float slidingWindowFilter(float input) {
    static float window[WINDOW_SIZE];  // 存储窗口内的值
    static int windowIndex = 0;        // 当前窗口内索引
    static float sum = 0;              // 窗口内值的总和
    static int count = 0;              // 当前窗口内的元素数

    // 减去即将被替换的元素
    sum -= window[windowIndex];
    
    // 替换元素
    window[windowIndex] = input;
    
    // 添加新的元素
    sum += input;
    
    // 更新索引，实现循环滑动
    windowIndex++;
    if (windowIndex >= WINDOW_SIZE) {
        windowIndex = 0;
    }
    
    // 在窗口未满时增加计数器
    if (count < WINDOW_SIZE) {
        count++;
    }
    
    // 返回当前窗口的平均值
    return sum / count;
}


// 假设电池满电时电压为4.2V，空电时电压为3.0V
float voltageToSOC(float voltage) {
    return (voltage - 3.0) / (4.2 - 3.0);
}


float kalmanUpdate(float voltage, float &x, float &P, float Q, float R) {
    // 预测步骤（这里没有动态模型更新，所以忽略）
    // x = x; // 状态估计不变
    P = P + Q; // 增加预测误差的方差

    // 测量更新
    float z = voltageToSOC(slidingWindowFilter(voltage)); // 从电压转换到估计的SoC
    float y = z - x; // 测量残差
    float K = P / (P + R); // 计算卡尔曼增益
    x = x + K * y; // 更新状态估计
    P = (1 - K) * P; // 更新估计误差方差

    return x; // 返回更新后的状态估计
}

uint8_t sAPP_Func_GetBatPercent(){
    float voltage = bat_mv / 1000.0f; // 假设这是从ADC读取的电压值
    float estimatedSoC;
    float P = 1.0; // 初始估计误差方差
    float Q = 0.002; // 过程噪声方差
    float R = 0.02; // 测量噪声方差
    estimatedSoC = kalmanUpdate(voltage, estimatedSoC, P, Q, R);
    return estimatedSoC * 100;
}
