#include "sAPP_ADC.h"

#define WINDOW_SIZE 10  // 滑动窗口大小



void sAPP_ADC_Init(){
    // 配置ADC
    analogReadResolution(12); // 设置ADC分辨率为12位
    analogSetAttenuation(ADC_11db); // 设置衰减，以适应大约0到3.3V的输入
}

float sAPP_ADC_GetVoltMV(){
    static float window[WINDOW_SIZE];  //存储窗口内的值
    static uint8_t windowIndex = 0;        //当前窗口内索引
    static float sum = 0;              //窗口内值的总和
    static uint8_t count = 0;              //当前窗口内的元素数
    float input = (float)analogReadMilliVolts(ADC_VOLT_PIN) * 2.0f;
    //减去即将被替换的元素
    sum -= window[windowIndex];
    //替换元素
    window[windowIndex] = input;
    //添加新的元素
    sum += input;
    //更新索引，实现循环滑动
    windowIndex++;
    if(windowIndex >= WINDOW_SIZE){windowIndex = 0;}
    //在窗口未满时增加计数器
    if(count < WINDOW_SIZE){count++;}
    //返回当前窗口的平均值
    return sum / count;
}

float sAPP_ADC_GetCurrMA(){
    static float window[WINDOW_SIZE];  //存储窗口内的值
    static uint8_t windowIndex = 0;        //当前窗口内索引
    static float sum = 0;              //窗口内值的总和
    static uint8_t count = 0;              //当前窗口内的元素数
    float input = (float)analogReadMilliVolts(ADC_CURR_PIN) - 1100.0f;
    //减去即将被替换的元素
    sum -= window[windowIndex];
    //替换元素
    window[windowIndex] = input;
    //添加新的元素
    sum += input;
    //更新索引，实现循环滑动
    windowIndex++;
    if(windowIndex >= WINDOW_SIZE){windowIndex = 0;}
    //在窗口未满时增加计数器
    if(count < WINDOW_SIZE){count++;}
    //返回当前窗口的平均值
    return sum / count;
}


// 假设电池满电时电压为4.2V，空电时电压为3.0V
static float voltageToSOC(float voltage) {
    return (voltage - 3.0) / (4.2 - 3.0);
}


static float kalmanUpdate(float voltage, float &x, float &P, float Q, float R) {
    // 预测步骤（这里没有动态模型更新，所以忽略）
    // x = x; // 状态估计不变
    P = P + Q; // 增加预测误差的方差

    // 测量更新
    float z = voltageToSOC(voltage); // 从电压转换到估计的SoC
    float y = z - x; // 测量残差
    float K = P / (P + R); // 计算卡尔曼增益
    x = x + K * y; // 更新状态估计
    P = (1 - K) * P; // 更新估计误差方差

    return x; // 返回更新后的状态估计
}

uint8_t sAPP_ADC_GetBatPercent(){
    float voltage = bat_mv / 1000.0f; // 假设这是从ADC读取的电压值
    float estimatedSoC;
    float P = 1.0; // 初始估计误差方差
    float Q = 0.002; // 过程噪声方差
    float R = 0.02; // 测量噪声方差
    estimatedSoC = kalmanUpdate(sAPP_ADC_GetVoltMV() / 1000.0f, estimatedSoC, P, Q, R);
    if(estimatedSoC < 0){
        estimatedSoC = 0;
    }
    return estimatedSoC * 100;
}



