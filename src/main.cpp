#include "main.h"





//Si24R1的数据消息队列,用于sAPP_2D4GHz到任务sAPP_Task_2D4GHzReciedDataH之间的通信
QueueHandle_t g_si24r1_data_queue;

//sGBD2->sAPP_Menu的按键事件队列
QueueHandle_t g_sgbd2_ev_mq;
//sAPP_Menu->sGBD2的item选中的二值信号量
SemaphoreHandle_t g_sapp_menu_item_selected_sem;
//sAPP_Menu->sGBD2的item退出选中的二值信号量
SemaphoreHandle_t g_sapp_menu_item_unselect_sem;

//2.4G接收到并解析好的数据
data_packet_t g_data_packet_p1;

float bat_mv;
float bat_ma;
uint8_t g_oled_bl_percent = 30;
uint8_t g_ws2812_bl_percent = 10;

Si24R1_Data_t si24r1_data_p1;
Si24R1_Conf_t g_si24r1_conf;


uint32_t page = 1;


char* WiFi_SSID = (char*)"9607a";
char* WiFi_PWD  = (char*)"960796079607";

const char* host = "192.168.1.116";
const uint16_t port = 8090;




TimerHandle_t  SyncTimer;


void setup() {
    Serial.begin(115200);
    Serial.printf("Hello,ESP32\n");

    sAPP_Func_InitOLED();
    sG2D_DrawScreenByImg(sAPP_Font_Img_StartPage);
    sG2D_UpdateScreen();

    // WiFi.begin(WiFi_SSID, WiFi_PWD);
    // while (WiFi.status()!= WL_CONNECTED) {
    //     delay(500);
    // }

    sG2D_SetAllGRAM(0);

    sAPP_Btns_Init();
    sAPP_WS2812_Init();
    sAPP_Func_Init2D4GHz();
    sAPP_ADC_Init();
    sDRV_BUZZER_Init();
    sAPP_Func_RTC_Init();

    sAPP_Menu_Init();


    sAPP_NVS_GetAllData();


    sDRV_BUZZER_SetMode(BUZZER_MODE_PULSE_SINGLE);
    sDRV_BUZZER_SetPulseTime_ms(50,100);
    sDRV_BUZZER_SetVolume(50);

    sAPP_WS2812_SetMode(WS2812_MODE_PULSE_SINGLE);
    sAPP_WS2812_SetBrightness(g_ws2812_bl_percent);
    sAPP_WS2812_SetPulseTime_ms(50,100);
    sAPP_WS2812_SetRGB(255,255,0);

    

    

    sDRV_GenOLED_SetBrightness(g_oled_bl_percent);

    // sDRV_BUZZER_SetFreq(1000);
    // sDRV_BUZZER_SetDuty(50);

    // sDRV_BUZZER_SetMode(BUZZER_MODE_PULSE_SINGLE);
    // sDRV_BUZZER_SetFreq(1000);
    // sDRV_BUZZER_SetVolume(50);
    // sDRV_BUZZER_SetDuty(0);

    //WiFi.begin(WiFi_SSID,WiFi_PWD);


    // sAPP_Func_SetWS2812Brightness(5);
    // sAPP_Func_SetWS2812();


    //消息队列只有一个元素
    g_si24r1_data_queue = xQueueCreate(1,sizeof(Si24R1_Data_t));

    g_sgbd2_ev_mq = xQueueCreate(1,sizeof(sAPP_Btns_Event_t));
    g_sapp_menu_item_selected_sem = xSemaphoreCreateBinary();
    g_sapp_menu_item_unselect_sem = xSemaphoreCreateBinary();

    if(g_si24r1_data_queue == NULL){
        while(1){
            Serial.printf("QUEUE ERROR\n");
        }
    }
    
    

    //*******************************创建任务**************************************
    //2.4GHz接收到数据处理任务,栈分配2KBytes,优先级为5
    xTaskCreate(sAPP_Task_2D4GHzReciedDataH ,"2D4GHzISRDataH"  ,2048,NULL,5,NULL);
    //刷新屏幕任务
    xTaskCreate(sAPP_Task_UpdateScreen      ,"UpdateScreen"    ,8192,NULL,4,NULL);
    //按键状态机处理任务
    xTaskCreate(sAPP_Task_BtnHandler        ,"BtnHandler"      ,2048,NULL,3,NULL);
    //BUZZER处理任务
    xTaskCreate(sAPP_Task_BuzzerHandler     ,"BuzzerHandler"   ,1024,NULL,2,NULL);
    //ADC采样读取电池电压电流任务
    xTaskCreate(sAPP_Task_ReadADC           ,"ReadADC"         ,4096,NULL,2,NULL);
    //WS2812处理任务
    xTaskCreate(sAPP_Task_WS2812Handler     ,"WS2812Handler"   ,2048,NULL,2,NULL);
    //串口数据处理任务
    xTaskCreate(sAPP_Task_UARTHandler       ,"UARTHandler"     ,4096,NULL,3,NULL);


    //同步时间定时器 24H一次
    SyncTimer = xTimerCreate("SyncTimer", pdMS_TO_TICKS(3600 * 24 * 1000), pdTRUE, (void *)0, sAPP_Task_TIM_SyncTimeByNTP);
    xTimerStart(SyncTimer, 0);  


    

    //! 千万不要调用vTaskStartScheduler();
}


void loop() {

}
