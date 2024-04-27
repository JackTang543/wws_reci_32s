#ifndef __MAIN_H__
#define __MAIN_H__



#include <Arduino.h>

#include "sAPP_Func.h"

#include "sGraphic2D.h"

#include "sAPP_Task.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>

#include "sBSP_SPI.h"

#include "sAPP_Task.h"


//全局变量,2.4G数据消息队列,只有一个元素
extern QueueHandle_t si24r1_data_queue;







#endif
