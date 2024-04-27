#include "sAPP_Menu.h"
#include "sGraphic2D.h"



//todo 基于C++标准库的框架



void sAPP_Menu_ShowHome(){
    // sG2D_WriteString(0,0,"TRANS LINKED");
    // sG2D_DrawHLine(0,127,8,1);
    // static char buffer[32]; // 足够大的缓冲区来存储格式化的字符串

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

}

