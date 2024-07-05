#include "main.h"

/**
 * WWS Reci
 * 无线气象站 软件By SIGHTSEER.
 * 
 * v1.0 TIME 2024.05.04
 * 初版完成
 * 
 * v1.1 TIME 2024.06.22
 * 比完赛之后进行优化版本
 * 
 * 
 */


//Si24R1的数据消息队列,用于sAPP_2D4GHz到任务sAPP_Task_2D4GHzReciedDataH之间的通信
QueueHandle_t g_si24r1_data_queue;
//2.4GHz接收到数据的队列,有20个元素(640字节) 用于临时保存,给上位机读取
QueueHandle_t g_recied_data_temp_queue;
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
uint8_t g_buzzer_volume;
uint8_t g_screen_time_s = 20;

Si24R1_Data_t si24r1_data_p1;
Si24R1_Conf_t g_si24r1_conf;

char* WiFi_SSID = "9607a";
char* WiFi_PWD  = "960796079607";

const char* host = "192.168.1.116";
const uint16_t port = 8090;

TimerHandle_t  SyncTimer;
TimerHandle_t  g_screen_sleep_timer;


void setup() {
    //初始化串口
    Serial.begin(115200);
    Serial.printf("\n\nHello,ESP32\n");

    //初始化OLED显示屏
    sDRV_GenOLED_Init();
    //在显存里画一个启动页面图片
    sG2D_DrawScreenByImg(sAPP_Font_Img_StartPage);
    //上传显存
    sG2D_UpdateScreen();
    //清空显存
    sG2D_SetAllGRAM(0);

    //初始化按键驱动
    sAPP_Btns_Init();
    //初始化WS2812驱动
    sAPP_WS2812_Init();
    //初始化2.4GHz驱动
    sAPP_Func_Init2D4GHz();
    //初始化读取电池电压电流的ADC
    sAPP_ADC_Init();
    //初始化蜂鸣器
    sDRV_BUZZER_Init();
    //初始化RTC
    sAPP_Func_RTC_Init();
    //初始化多级菜单
    sAPP_Menu_Init();

    //从NVS里获取所有数据,上载存在EEPROM里的数据
    sAPP_NVS_GetAllData();

    //设置蜂鸣器参数
    sDRV_BUZZER_SetMode(BUZZER_MODE_PULSE_SINGLE);
    sDRV_BUZZER_SetPulseTime_ms(50,100);
    sDRV_BUZZER_SetVolume(50);

    //设置WS2812参数
    sAPP_WS2812_SetMode(WS2812_MODE_PULSE_SINGLE);
    sAPP_WS2812_SetBrightness(g_ws2812_bl_percent);
    sAPP_WS2812_SetPulseTime_ms(50,100);
    sAPP_WS2812_SetRGB(255,255,0);

    //设置音量,这里音量其实设置的是占空比,基本没多大用,搞着好玩
    //sDRV_BUZZER_SetVolume(g_buzzer_volume);
    sDRV_GenOLED_SetBrightness(g_oled_bl_percent);

    //消息队列只有一个元素,用于2.4Ghz和菜单的数据传递
    g_si24r1_data_queue = xQueueCreate(1,sizeof(Si24R1_Data_t));
    g_recied_data_temp_queue = xQueueCreate(20,sizeof(data_packet_t));
    g_sgbd2_ev_mq = xQueueCreate(1,sizeof(sAPP_Btns_Event_t));
    g_sapp_menu_item_selected_sem = xSemaphoreCreateBinary();
    g_sapp_menu_item_unselect_sem = xSemaphoreCreateBinary();
    
    

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

    //屏幕熄屏定时器,不自动重载
    g_screen_sleep_timer = xTimerCreate("ScreenSleepTimer", pdMS_TO_TICKS(g_screen_time_s * 1000), pdFALSE, (void *)0, sAPP_Task_TIM_ScreenSleep);
    if(g_screen_time_s < 200){
        xTimerStart(g_screen_sleep_timer, 0); 
    } 

    //! 千万不要调用vTaskStartScheduler();
}


void loop() {

}
