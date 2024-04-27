#include "sAPP_Menu.h"
#include "sGraphic2D.h"
#include "main.h"


//todo 基于C++标准库的框架



void sAPP_Menu_ShowHome(){
    sG2D_WriteString(0,0,"TRANS LINKED");
    sG2D_DrawHLine(0,127,8,1);
    static char buffer[32]; // 足够大的缓冲区来存储格式化的字符串

    // 显示温度
    snprintf(buffer, sizeof(buffer), "TEMP:  %.1f", data_packet_p1.aht10_temp);
    sG2D_WriteString(10, 15, buffer);
    sG2D_WriteString(100,15,"degC");

    // 显示湿度
    snprintf(buffer, sizeof(buffer), "HUMI:  %.1f", data_packet_p1.aht10_humi);
    sG2D_WriteString(10, 25, buffer);
    sG2D_WriteString(100,25,"%RH");

    // 显示气压（转换为百帕斯卡）
    snprintf(buffer, sizeof(buffer), "PRES:  %.2f", data_packet_p1.bmp280_pres);
    sG2D_WriteString(10, 35, buffer);
    sG2D_WriteString(100,35,"HPa");

    // 显示光照
    snprintf(buffer, sizeof(buffer), "LIGHT: %.2f", data_packet_p1.temt_mv); // 假设光照单位为Lux，且整数足够
    sG2D_WriteString(10, 45, buffer);
    sG2D_WriteString(100,45,"%");

    snprintf(buffer, sizeof(buffer), "%4.0fmV", bat_mv);
    sG2D_WriteString(10, 55, buffer);

    snprintf(buffer, sizeof(buffer), "%4.0fmA", bat_ma);
    sG2D_WriteString(50, 55, buffer);

}





void sAPP_Menu_ShowInfo(){

    sG2D_DrawTriangle(1,y_pos + 2,7,y_pos + 5,1,y_pos + 8,1);
    sG2D_WriteString(10,y_pos + 2,"WIFI INFO");
    // sG2D_DrawHLine(0,127,9,1);
    // sG2D_DrawHLine(0,127,10,1);

    sG2D_RevRectArea(0,y_pos + 0,127,y_pos + 10);

    sG2D_WriteString(0,y_pos + 13,"STATE:      CONNECTED");
    sG2D_DrawHLine(0,127,y_pos + 22,1);   

    sG2D_WriteString(0,y_pos + 25,"IP   :  192.168.1.104");
    sG2D_DrawHLine(0,127,y_pos + 34,1);   

    sG2D_WriteString(0,y_pos + 37,"MAC  : 74:4F:98:4A:8B");
    sG2D_DrawHLine(0,127,y_pos + 46,1);   

    sG2D_WriteString(0,y_pos + 49,"RSSI :         -76dBm");
    sG2D_DrawHLine(0,127,y_pos + 58,1);   

    sG2D_WriteString(0,y_pos + 61,"ACCX :         1.753g");
    sG2D_DrawHLine(0,127,y_pos + 70,1);   

    sG2D_WriteString(0,y_pos + 73,"ACCY :         0.142g");
    sG2D_DrawHLine(0,127,y_pos + 82,1);   

    sG2D_WriteString(0,y_pos + 85,"ACCZ :         7.561g");
    sG2D_DrawHLine(0,127,y_pos + 94,1);   

    //sG2D_DrawHLine(0,127,y_pos + 63,1);   
}


void sAPP_Menu_Handler(){
    if(page == 1){
        Serial.printf("page:%d\n",page);
        sAPP_Menu_ShowHome();
    }
    else if(page == 2){
        Serial.printf("page:%d\n",page);
        sAPP_Menu_ShowInfo();
        //sG2D_WriteString(10,30,"THIS IS PAGE 2");
    }
    else if(page == 3){
        Serial.printf("page:%d\n",page);
        sG2D_WriteString(10,10,"THIS IS PAGE 3");
    }
    else if(page == 4){Serial.printf("page:%d\n",page);
        sG2D_WriteString(10,10,"THIS IS PAGE 4");
    }
    else if(page == 5){
        Serial.printf("page:%d\n",page);
        sG2D_WriteString(10,10,"THIS IS PAGE 5");
    }
    
}

