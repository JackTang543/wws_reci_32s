#include "sAPP_Task.h"
#include "main.h"

Si24R1_Data_t si24r1_data;
data_packet_t data_packet_p1;

TaskHandle_t sAPP_TaskH_2D4GHzISRDataH = NULL;


//2.4Ghz接收到数据处理任务
void sAPP_Task_2D4GHzReciedDataH(void* pvPara){
    Si24R1_Data_t* pData = NULL;
    for(;;){
        //如果队列有数据
        if(xQueueReceive(si24r1_data_queue, &pData, portMAX_DELAY) == pdPASS) {
            //把数据复制过来
            memcpy(&si24r1_data,pData,sizeof(Si24R1_Data_t));
            //然后把数据变成特定的格式
            memcpy(&data_packet_p1,&si24r1_data,sizeof(data_packet_t));
            //dataPacketToJsonString(&packet);

            Serial.printf("AHT10 HUMI: %.1f %%RH,AHT10 TEMP: %.1f degC\n",data_packet_p1.aht10_humi,data_packet_p1.aht10_temp);
            Serial.printf("BMP280 PRESS: %.3f HPa,BMP280 TEMP: %.2f degC\n",data_packet_p1.bmp280_pres / 100,data_packet_p1.bmp280_temp);
            Serial.printf("LIGHT:%.2f %%,Vbat:%.2f mV\n",data_packet_p1.temt_mv,data_packet_p1.vbat);
        }

    }
}


//更新屏幕任务
void sAPP_Task_UpdateScreen(void* pvParameters){
    for(;;){
        sG2D_UpdateScreen();
        sG2D_SetAllGRAM(0);
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}

void sAPP_Task_BtnHandler(void* pvParameters){
    
}

