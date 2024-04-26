#ifndef __SAPP_FONT_H__
#define __SAPP_FONT_H__
//控制是否启用这个模块
#define __SAPP_FONT_EN__
#ifdef  __SAPP_FONT_EN__

#include <Arduino.h>


typedef struct{
    uint8_t CharByte0;
    uint8_t CharByte1;
    uint8_t CharByte2;
    uint8_t CharByte3;
    uint8_t CharByte4;
}sCGRAM_Char_t;

/* 
*  sAPP_Font.h
*  这是一个字库
*  部分由粟禛恺完成  
*
*  v1.0:第一版
*  By Sightseer. 2023.12.18于HNIP-9607Lab    
*  
*/


#endif
#endif
