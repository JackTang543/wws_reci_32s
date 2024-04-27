#include "main.h"


QueueHandle_t si24r1_data_queue;


const char* ssid = "9607a";
const char* password = "960796079607";


const char* host = "192.168.1.116";
const uint16_t port = 8090;

#define LED_PIN 21     // 定义使用的GPIO引脚
#define LEDC_CHANNEL 0  // 选择PWM通道0
#define LEDC_FREQUENCY 100  // PWM频率为5000Hz
#define LEDC_RESOLUTION 8    // 分辨率为8位

void task1(void *pvParameters) {
  while (1) {
    Serial.println("Task 1 is running...");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void task2(void *pvParameters) {
  while (1) {
    Serial.println("Task 2 is running...");
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

// void setup() {
//   Serial.begin(115200);

//   xTaskCreate(task1, "Task1", 1024, NULL, 1, NULL);
//   xTaskCreate(task2, "Task2", 1024, NULL, 1, NULL);
// }

// void loop() {
//   // 什么都不做
// }


void setup() {
    Serial.begin(115200);
    Serial.printf("Hello,ESP32\n");

    sAPP_Func_InitOLED();
    // sAPP_Func_InitBtns();
    // sAPP_Func_InitWS2812();
    sAPP_Func_Init2D4GHz();
    // sAPP_Func_ADCInit();

    // sAPP_Func_SetWS2812Brightness(5);
    // sAPP_Func_SetWS2812();


   si24r1_data_queue = xQueueCreate(1,sizeof(Si24R1_Data_t*));


    if(si24r1_data_queue == NULL){
        while(1){
            Serial.printf("QUEUE ERROR\n");
        }
    }


    //2.4GHz接收到数据处理任务
    xTaskCreate(sAPP_Task_2D4GHzReciedDataH,"2D4GHzISRDataH",2048,NULL,1,&sAPP_TaskH_2D4GHzISRDataH);
    //刷新屏幕任务
    xTaskCreate(sAPP_Task_UpdateScreen,"UpdateScreen",2048,NULL,1,NULL);

    

 // 初始化PWM
//   ledcSetup(LEDC_CHANNEL, LEDC_FREQUENCY, LEDC_RESOLUTION);
   // 将PWM信号绑定到GPIO
//   ledcAttachPin(LED_PIN, LEDC_CHANNEL);
//   ledcWrite(LEDC_CHANNEL, 50);


    //! 千万不要调用vTaskStartScheduler();
}


void loop() {
    // sG2D_SetAllGRAM(0);
    
    // sGBD_Handler();
    
    //sAPP_Func_DataHandler();


    // sG2D_UpdateScreen();

    //delay(100);
}
