#ifndef __SBSP_SPI_H__
#define __SBSP_SPI_H__

#include <Arduino.h>
#include <SPI.h>

//SPI1 -> OLED
#define SPI1_SCL_PIN   23
#define SPI1_MOSI_PIN  22
//dummy
#define SPI1_MISO_PIN  27
#define SPI1_CS_PIN    13
#define SPI1_FREQ      10000000

//SPI2 -> Si24R1
#define SPI2_SCL_PIN   5
#define SPI2_MISO_PIN  16
#define SPI2_MOSI_PIN  17
#define SPI2_CS_PIN    18
#define SPI2_FREQ      10000000


int8_t sBSP_SPI1M_Init();
int8_t sBSP_SPI2M_Init();

void sBSP_SPI1M_SetCS(uint8_t lv);
void sBSP_SPI2M_SetCS(uint8_t lv);

void sBSP_SPI1M_SendByte(uint8_t byte);

void sBSP_SPI2M_SendByte(uint8_t byte);
uint8_t sBSP_SPI2M_ReadByte();
void sBSP_SPI2M_SendBytes(uint8_t *pData,uint16_t length);
void sBSP_SPI2M_ReadBytes(uint8_t *pData,uint16_t length);


#endif

