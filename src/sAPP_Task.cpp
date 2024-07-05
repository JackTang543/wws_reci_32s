#include "sAPP_Task.h"

#include "sAPP_Menu.h"
#include "main.h"






/**
 * @brief 2.4Ghz接收到数据的处理任务
 */
void sAPP_Task_2D4GHzReciedDataH(void* pvPara){
    Si24R1_Data_t si24r1_data;
    for(;;){
        //如果队列有数据
        if(xQueueReceive(g_si24r1_data_queue, &si24r1_data, portMAX_DELAY) == pdPASS) {
            //让灯闪一下
            sAPP_WS2812_StartSinglePulse();
            //然后把数据变成特定的格式,直接给值为了不复制我自己添加的字段
            memcpy(&g_data_packet_p1,&si24r1_data,26);
            g_data_packet_p1.rssi = si24r1_data.rssi;
            g_data_packet_p1.ppp = si24r1_data.ppp; 
            //获取时间戳
            time_t now = time(NULL);
            g_data_packet_p1.timestamp = now;
            //把数据放入临时队列
            xQueueSend(g_recied_data_temp_queue,&g_data_packet_p1,0);
            //Serial.printf("vbat_mv:%.2f\n",g_data_packet_p1.vbat);

            // Serial.printf("AHT10 HUMI: %.1f %%RH,AHT10 TEMP: %.1f degC\n",data_packet_p1.aht10_humi,data_packet_p1.aht10_temp);
            // Serial.printf("BMP280 PRESS: %.3f HPa,BMP280 TEMP: %.2f degC\n",data_packet_p1.bmp280_pres / 100,data_packet_p1.bmp280_temp);
            // Serial.printf("LIGHT:%.2f %%,Vbat:%.2f mV\n",data_packet_p1.temt_mv,data_packet_p1.vbat);
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


//更新屏幕任务
void sAPP_Task_UpdateScreen(void* pvPara){
    for(;;){
        //调用菜单处理函数
        sAPP_Menu_Handler();
        //更新屏幕
        sG2D_UpdateScreen();
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}

//按键状态机处理
void sAPP_Task_BtnHandler(void* pvPara){
    for(;;){
        sGBD_Handler();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

//蜂鸣器处理
void sAPP_Task_BuzzerHandler(void* pvPara){
    for(;;){
        sDRV_BUZZER_Handler();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

//读取电池电流和电压值
void sAPP_Task_ReadADC(void* pvPara){
    for(;;){
        bat_mv = sAPP_ADC_GetVoltMV();
        bat_ma = sAPP_ADC_GetCurrMA();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

//WS2812处理任务
void sAPP_Task_WS2812Handler(void* pvPara){
    for(;;){
        sAPP_WS2812_Handler();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

//串口指令处理任务
void sAPP_Task_UARTHandler(void* pvPara){
    for(;;){
        sAPP_UART_Handler();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void sAPP_Task_TIM_SyncTimeByNTP(TimerHandle_t xTimer){
    //通过NTP服务器同步时间
    if(WiFi.isConnected()){
        //GMT+8 中国北京
        configTime(3600 * 8,0,"ntp.ntsc.ac.cn");
    }
}

void sAPP_Task_TIM_ScreenSleep(TimerHandle_t xTimer){
    sG2D_SetOLEDShow(0);
}

