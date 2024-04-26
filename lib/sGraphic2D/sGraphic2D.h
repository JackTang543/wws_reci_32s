#ifndef __S_GRAPHIC_2D_H__
#define __S_GRAPHIC_2D_H__


#include "Arduino.h"

#include "sDRV_GenOLED.h"

#include "math.h"
#include "stdlib.h"
#include "sAPP_Font.h"

void sG2D_SetDot(uint8_t x, uint8_t y, uint8_t dot_en);
void sG2D_SetByte(uint8_t x, uint8_t y, uint8_t data);
void sG2D_WriteNumber(uint8_t x,uint8_t y,uint32_t num);
void sG2D_WriteString(uint8_t x,uint8_t y,char* str);
void sG2D_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t dot_en);
void sG2D_DrawVLine(uint8_t x, uint8_t y0, uint8_t y1, uint8_t dot_en);
void sG2D_DrawHLine(uint8_t x0, uint8_t x1,uint8_t y, uint8_t dot_en);
void sG2D_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot_en);
void sG2D_DrawRectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t is_fill);
void sG2D_SetAllGRAM(uint8_t px_en);
void sG2D_UpdateScreen();



#endif
