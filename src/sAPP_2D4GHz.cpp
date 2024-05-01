#include "sAPP_2D4GHz.h"

//WWS Reci项目 By Sightseer.
//Si24R1的应用层
//TIME:2024.04.30


/**
 * @brief  Si24R1接收到数据的回调,在这里发送一个消息给数据处理任务,这是由ISR调用的函数
 * 
 * @param  data : sDRV_Si24R1.cpp给我的接收到的数据的指针
 * 
 * @return 无
 */
void IRAM_ATTR twoD4_ReciedDataCb(Si24R1_Data_t* data){
    //Serial.printf("recied data:\"%s\",len:%d,ppp:%d,rssi:%d\n",data->msg,data->len,data->ppp,data->rssi);
    //通道0
    if(data->ppp == 0){
        //用于检查是否要唤醒更高优先级的任务
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        //把数据推入队列
        xQueueSendFromISR(g_si24r1_data_queue,data, &xHigherPriorityTaskWoken);
        //检查是否需要立即切换任务
        if(xHigherPriorityTaskWoken == pdTRUE){
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}


int8_t sAPP_2D4GHz_GetRSSI(){
    return si24r1_data_p1.rssi;
}


/**
 * @brief  初始化Si24R1
 * 
 * @param  无
 * 
 * @return 无
 */
void sAPP_Func_Init2D4GHz(){
    //地址宽度
    g_si24r1_conf.addr_width = ADDR_WIDTH_5;
    //接收地址P0
    g_si24r1_conf.addr.rx_addr_p0 = 0x123456789a;
    //接收地址P1
    g_si24r1_conf.addr.rx_addr_p1 = 0x1111111111;
    //P2
    g_si24r1_conf.addr.rx_addr_p2 = 0x22;
    //P3
    g_si24r1_conf.addr.rx_addr_p3 = 0x33;
    //P4
    g_si24r1_conf.addr.rx_addr_p4 = 0x44;
    //P5
    g_si24r1_conf.addr.rx_addr_p5 = 0x55;
    //发送地址
    g_si24r1_conf.addr.tx_addr = 0x123456789a;
    //自动确认使能
    g_si24r1_conf.aa_en = EN_AA_ENAA_P0 | EN_AA_ENAA_P1 | EN_AA_ENAA_P2 | EN_AA_ENAA_P3 | EN_AA_ENAA_P4 | EN_AA_ENAA_P5;
    //ACK模式
    g_si24r1_conf.ack_mode = ACK_MODE_DIS;
    //自动重发次数
    g_si24r1_conf.arc_times = 4;
    //自动重发延迟
    g_si24r1_conf.ard_us = 1000;
    //CRC使能
    g_si24r1_conf.crc_en = CRC_EN;
    //CRC长度
    g_si24r1_conf.crc_len = CRC_LEN_2;
    //动态负载长度
    g_si24r1_conf.dynpd = DYNPD_DPL_P0 | DYNPD_DPL_P1 | DYNPD_DPL_P2 | DYNPD_DPL_P3 | DYNPD_DPL_P4 | DYNPD_DPL_P5;
    //ACK负载
    g_si24r1_conf.en_ack_payd = EN_ACK_PAYD_DIS;
    //使能动态负载
    g_si24r1_conf.en_dpl = EN_DPL_DIS;
    //发射还是接收模式
    g_si24r1_conf.mode = MODE_RX;
    //射频信道
    g_si24r1_conf.rf_ch = 100;
    //发射功率
    g_si24r1_conf.rf_pwr = RF_PWR_4DBM;
    //射频速率
    g_si24r1_conf.rf_spd = RF_SPD_250K;
    //接收管道长度
    g_si24r1_conf.rx_pw.rx_pw_p0 = 32;
    g_si24r1_conf.rx_pw.rx_pw_p1 = 32;
    g_si24r1_conf.rx_pw.rx_pw_p2 = 32;
    g_si24r1_conf.rx_pw.rx_pw_p3 = 32;
    g_si24r1_conf.rx_pw.rx_pw_p4 = 32;
    g_si24r1_conf.rx_pw.rx_pw_p5 = 32;
    //接收管道使能
    g_si24r1_conf.rxaddr_en = EN_RXADDR_MSK_ERX_P5 | EN_RXADDR_MSK_ERX_P4 | EN_RXADDR_MSK_ERX_P3 | \
                       EN_RXADDR_MSK_ERX_P2 | EN_RXADDR_MSK_ERX_P0 | EN_RXADDR_MSK_ERX_P0;
    g_si24r1_conf.irq_msk = 0;

    //装载参数并初始化
    sDRV_Si24R1_Init(&g_si24r1_conf,twoD4_ReciedDataCb);
    //开始接收数据
    sDRV_Si24R1_SetStandby();
    sDRV_Si24R1_StartRX();
}
