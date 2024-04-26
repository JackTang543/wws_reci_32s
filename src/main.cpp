#include <Arduino.h>


#include "sAPP_Func.h"

#include "sGraphic2D.h"

#include "sAPP_Task.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>

#include "sBSP_SPI.h"



const char* ssid = "9607a";
const char* password = "960796079607";


const char* host = "192.168.1.116";
const uint16_t port = 8090;

#define LED_PIN 21     // 定义使用的GPIO引脚
#define LEDC_CHANNEL 0  // 选择PWM通道0
#define LEDC_FREQUENCY 100  // PWM频率为5000Hz
#define LEDC_RESOLUTION 8    // 分辨率为8位

#include <list>

using namespace std;

list<int> mylist;

void setup() {
    Serial.begin(115200);
    Serial.printf("Hello,ESP32\n");

    sAPP_Func_InitOLED();
    sAPP_Func_InitBtns();
    sAPP_Func_InitWS2812();
    sAPP_Func_Init2D4GHz();
    sAPP_Func_ADCInit();



    sAPP_Func_SetWS2812Brightness(5);

    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi...");
    // }
    // Serial.println("Connected to WiFi\n");

    // Serial.printf("My IP:%s\n",WiFi.localIP().toString());
    
    
    sAPP_Func_SetWS2812();

    mylist.push_back(1);
    mylist.push_back(5);
    mylist.push_back(10);

    for(int val : mylist){
        Serial.println(val);
    }


 // 初始化PWM
//   ledcSetup(LEDC_CHANNEL, LEDC_FREQUENCY, LEDC_RESOLUTION);
   // 将PWM信号绑定到GPIO
//   ledcAttachPin(LED_PIN, LEDC_CHANNEL);
//   ledcWrite(LEDC_CHANNEL, 50);
    
}


void loop() {
    sG2D_SetAllGRAM(0);
    
    // // sGBD_Handler();
    
    sAPP_Func_DataHandler();


    sG2D_UpdateScreen();

    delay(100);
}
