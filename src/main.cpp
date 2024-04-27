#include "main.h"


QueueHandle_t si24r1_data_queue;
QueueHandle_t btn_ev_data_queue;
float bat_mv;
float bat_ma;

int16_t y_pos;


data_packet_t data_packet_p1;
uint32_t page = 1;


const char* ssid = "9607a";
const char* password = "960796079607";


const char* host = "192.168.1.116";
const uint16_t port = 8090;





void setup() {
    Serial.begin(115200);
    Serial.printf("Hello,ESP32\n");

    sAPP_Func_InitOLED();
    sAPP_Func_InitBtns();
    // sAPP_Func_InitWS2812();
    sAPP_Func_Init2D4GHz();
    sAPP_Func_ADCInit();
    sDRV_BUZZER_Init();


    // sDRV_BUZZER_SetFreq(1000);
    // sDRV_BUZZER_SetDuty(50);

    sDRV_BUZZER_SetMode(BUZZER_MODE_PULSE_SINGLE);
    sDRV_BUZZER_SetFreq(1000);
    sDRV_BUZZER_SetVolume(50);
    sDRV_BUZZER_SetDuty(0);

    
    

    // ledcSetup(LEDC_CHANNEL, LEDC_FREQUENCY, LEDC_RESOLUTION);
    // //将PWM信号绑定到GPIO
    // ledcAttachPin(LED_PIN, LEDC_CHANNEL);
    // ledcWrite(LEDC_CHANNEL, 50);

    //sDRV_BUZZER_SetMode(BUZZER_MODE_ADJ_VOLUME);
    //sDRV_BUZZER_SetVolume(100);

    // sAPP_Func_SetWS2812Brightness(5);
    // sAPP_Func_SetWS2812();


   si24r1_data_queue = xQueueCreate(1,sizeof(Si24R1_Data_t*));


    if(si24r1_data_queue == NULL){
        while(1){
            Serial.printf("QUEUE ERROR\n");
        }
    }


    //2.4GHz接收到数据处理任务
    xTaskCreate(sAPP_Task_2D4GHzReciedDataH,"2D4GHzISRDataH",2048,NULL,3,&sAPP_TaskH_2D4GHzISRDataH);
    //刷新屏幕任务
    xTaskCreate(sAPP_Task_UpdateScreen,"UpdateScreen",2048,NULL,1,NULL);
    //按键状态机处理任务
    xTaskCreate(sAPP_Task_BtnHandler,"BtnHandler",2048,NULL,1,NULL);
    //BUZZER处理任务
    xTaskCreate(sAPP_Task_BuzzerHandler,"BuzzerHandler",1048,NULL,1,NULL);
    //ADC采样读取电池电压电流任务
    xTaskCreate(sAPP_Task_ReadADC,"ReadADC",1024,NULL,1,NULL);
    




    //! 千万不要调用vTaskStartScheduler();
}


void loop() {
    // sG2D_SetAllGRAM(0);
    
    // sGBD_Handler();
    
    //sAPP_Func_DataHandler();


    // sG2D_UpdateScreen();

    //delay(100);
}
