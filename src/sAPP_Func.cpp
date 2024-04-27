#include "sAPP_Func.h"
#include "sAPP_Task.h"



data_packet_t reci_data_temp;


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

Si24R1_Data_t si24r1_data_temp;
Si24R1_Data_t* si24r1_data_temp_p;

//IRQ:2.4Ghz接收到数据回调,在这里发送一个消息队列给数据处理任务
void IRAM_ATTR sAPP_Func_ISR_2D4ReciedData(Si24R1_Data_t* data){
    //Serial.printf("recied data:\"%s\",len:%d,ppp:%d,rssi:%d\n",data->msg,data->len,data->ppp,data->rssi);
    //先缓存一下数据
    memcpy(&si24r1_data_temp,data,sizeof(Si24R1_Data_t));
    //更新一下指针
    si24r1_data_temp_p = &si24r1_data_temp;
    //用于检查是否要唤醒更高优先级的任务
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //把数据指针推入队列,这里的数据指针其实是2级指针
    xQueueSendFromISR(si24r1_data_queue, &si24r1_data_temp_p, &xHigherPriorityTaskWoken);
    //检查是否需要立即切换任务
    if(xHigherPriorityTaskWoken == pdTRUE){
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}


#include "sGraphic2D.h"


void sAPP_Func_DataHandler(){

    sG2D_WriteString(0,0,"TRANS LINKED");
    sG2D_DrawHLine(0,127,8,1);
    static char buffer[32]; // 足够大的缓冲区来存储格式化的字符串

    // // 显示温度
    // snprintf(buffer, sizeof(buffer), "TEMP:  %.1f", packet.aht10_temp);
    // sG2D_WriteString(10, 15, buffer);
    // sG2D_WriteString(100,15,"degC");

    // // 显示湿度
    // snprintf(buffer, sizeof(buffer), "HUMI:  %.1f", packet.aht10_humi);
    // sG2D_WriteString(10, 25, buffer);
    // sG2D_WriteString(100,25,"%RH");

    // // 显示气压（转换为百帕斯卡）
    // snprintf(buffer, sizeof(buffer), "PRES:  %.2f", packet.bmp280_pres);
    // sG2D_WriteString(10, 35, buffer);
    // sG2D_WriteString(100,35,"HPa");

    // // 显示光照
    // snprintf(buffer, sizeof(buffer), "LIGHT: %.2f", packet.temt_mv); // 假设光照单位为Lux，且整数足够
    // sG2D_WriteString(10, 45, buffer);
    // sG2D_WriteString(100,45,"%");

    // snprintf(buffer, sizeof(buffer), "%4.0fmV", sAPP_Func_GetVoltMV());
    // sG2D_WriteString(10, 55, buffer);

    // snprintf(buffer, sizeof(buffer), "%4.0fmA", sAPP_Func_GetCurrMA());
    // sG2D_WriteString(70, 55, buffer);

        

        // if (!client.connect(host, port)) {
        //     Serial.println("Connection failed.");
        //     return;
        // }   
        
        // Serial.println("Connected to server");
    
        // // 向服务器发送消息
        // client.print(dataPacketToJsonString(&packet)); // 发送 JSON 数据

        // // 读取服务器响应
        // while(client.available()){
        //     String line = client.readStringUntil('\r');
        //     Serial.print(line);
        // }

        // client.stop();

        // udp.beginPacket(host, port);
        // udp.print(dataPacketToJsonString(&packet)); // 发送 JSON 数据
        // udp.endPacket(); // 完成发送

}

bool get_lv(uint8_t btn_id){
    if(btn_id == BTN_UP_ID){
        return !!digitalRead(BTN_UP_PIN);
    }
    else if(btn_id == BTN_DOWN_ID){
        return !!digitalRead(BTN_DOWN_PIN);
    }
    else if(btn_id == BTN_ENTER_ID){
        return !!digitalRead(BTN_ENTER_PIN);
    }
    else if(btn_id == BTN_BACK_ID){
        return !!digitalRead(BTN_BACK_PIN);
    }
    return false;
}

void trig(uint8_t btn_id,ev_flag_t btn_ev){
    if(btn_id == BTN_UP_ID){
        if(btn_ev == ev_rlsd){
            page >= 5?page = 1:page++;
            sDRV_BUZZER_SetPulseTime_ms(300,100);
        }
        else if(btn_ev == ev_lp){

        }
    }
    else if(btn_id == BTN_DOWN_ID){
        if(btn_ev == ev_rlsd){
            page <= 1?page = 5:page--;
            sDRV_BUZZER_SetPulseTime_ms(300,100);
        }
        else if(btn_ev == ev_lp){

        }
    }
    else if(btn_id == BTN_ENTER_ID){
        if(btn_ev == ev_rlsd){
            y_pos+=12;
        }
        else if(btn_ev == ev_lp){

        }
    }
    else if(btn_id == BTN_BACK_ID){
        if(btn_ev == ev_rlsd){
            y_pos-=12;
        }
        else if(btn_ev == ev_lp){

        }
    }
}

void sAPP_Func_InitOLED(){
    sDRV_GenOLED_Init();
}

uint32_t GetTick(){
    return millis();
}

void sAPP_Func_InitBtns(){
    pinMode(BTN_UP_PIN,INPUT);
    pinMode(BTN_DOWN_PIN,INPUT);
    pinMode(BTN_ENTER_PIN,INPUT);
    pinMode(BTN_BACK_PIN,INPUT);

    btn_init_t btn_init;
    btn_init.en = 1;                //使能此按键
    btn_init.lv_rev = lv_non_reverse;   //空闲时的电平反转
    btn_init.dp_mode = dp_disable;  //禁用双击,可提高连续单击速度
    btn_init.lp_loop_pridt = 500;   //设置长按循环触发间隔每500ms触发一次
    btn_init.lp_trig_waitt = 2000;  //设置长按触发时间2000ms
    btn_init.dp_prid_waitt = 200;   //设置最大等待双击时间
    sGBD_SetAllBtnEnable(1);        //设置所有按键使能
    sGBD_SetAllBtnMode(&btn_init);  //装载btn_init的配置参数

    sGBD_Init(get_lv,trig,GetTick);

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

void sAPP_Func_Init2D4GHz(){
    Si24R1_Conf_t si24r1;
    //地址宽度
    si24r1.addr_width = ADDR_WIDTH_5;
    //接收地址P0
    si24r1.addr.rx_addr_p0 = 0x123456789a;
    //接收地址P1
    si24r1.addr.rx_addr_p1 = 0x1111111111;
    //P2
    si24r1.addr.rx_addr_p2 = 0x22;
    //P3
    si24r1.addr.rx_addr_p3 = 0x33;
    //P4
    si24r1.addr.rx_addr_p4 = 0x44;
    //P5
    si24r1.addr.rx_addr_p5 = 0x55;
    //发送地址
    si24r1.addr.tx_addr = 0x123456789a;
    //自动确认使能
    si24r1.aa_en = EN_AA_ENAA_P0 | EN_AA_ENAA_P1 | EN_AA_ENAA_P2 | EN_AA_ENAA_P3 | EN_AA_ENAA_P4 | EN_AA_ENAA_P5;
    //ACK模式
    si24r1.ack_mode = ACK_MODE_DIS;
    //自动重发次数
    si24r1.arc_times = 4;
    //自动重发延迟
    si24r1.ard_us = 1000;
    //CRC使能
    si24r1.crc_en = CRC_EN;
    //CRC长度
    si24r1.crc_len = CRC_LEN_2;
    //动态负载长度
    si24r1.dynpd = DYNPD_DPL_P0 | DYNPD_DPL_P1 | DYNPD_DPL_P2 | DYNPD_DPL_P3 | DYNPD_DPL_P4 | DYNPD_DPL_P5;
    //ACK负载
    si24r1.en_ack_payd = EN_ACK_PAYD_DIS;
    //使能动态负载
    si24r1.en_dpl = EN_DPL_DIS;
    //发射还是接收模式
    si24r1.mode = MODE_RX;
    //射频信道
    si24r1.rf_ch = 100;
    //发射功率
    si24r1.rf_pwr = RF_PWR_4DBM;
    //射频速率
    si24r1.rf_spd = RF_SPD_250K;
    //接收管道长度
    si24r1.rx_pw.rx_pw_p0 = 32;
    si24r1.rx_pw.rx_pw_p1 = 32;
    si24r1.rx_pw.rx_pw_p2 = 32;
    si24r1.rx_pw.rx_pw_p3 = 32;
    si24r1.rx_pw.rx_pw_p4 = 32;
    si24r1.rx_pw.rx_pw_p5 = 32;
    //接收管道使能
    si24r1.rxaddr_en = EN_RXADDR_MSK_ERX_P5 | EN_RXADDR_MSK_ERX_P4 | EN_RXADDR_MSK_ERX_P3 | \
                       EN_RXADDR_MSK_ERX_P2 | EN_RXADDR_MSK_ERX_P0 | EN_RXADDR_MSK_ERX_P0;
    si24r1.irq_msk = 0;

    sDRV_Si24R1_Init(&si24r1,sAPP_Func_ISR_2D4ReciedData);

    sDRV_Si24R1_SetStandby();

    sDRV_Si24R1_StartRX();
    Serial.printf("Si24R1 start rx!\n");
    
}


