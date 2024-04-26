#include "sBSP_SPI.h"

//SPI1
SPIClass* spi1 = new SPIClass(HSPI);
//SPI2
SPIClass* spi2 = new SPIClass(VSPI);

int8_t sBSP_SPI1M_Init(){
    spi1->begin(SPI1_SCL_PIN, SPI1_MISO_PIN, SPI1_MOSI_PIN, SPI1_CS_PIN);
    pinMode(spi1->pinSS(), OUTPUT);
    return 0;
}

void sBSP_SPI1M_SetCS(uint8_t lv){
    digitalWrite(spi1->pinSS(),!!lv);
}

void sBSP_SPI1M_SendByte(uint8_t byte){
    spi1->beginTransaction(SPISettings(SPI1_FREQ,MSBFIRST,SPI_MODE0));
    spi1->transfer(byte);
    spi1->endTransaction();
}



int8_t sBSP_SPI2M_Init(){
    spi2->begin(SPI2_SCL_PIN, SPI2_MISO_PIN, SPI2_MOSI_PIN, SPI2_CS_PIN);
    pinMode(spi2->pinSS(), OUTPUT);
    return 0;
}

void sBSP_SPI2M_SetCS(uint8_t lv){
    digitalWrite(spi2->pinSS(),!!lv);
}

void sBSP_SPI2M_SendByte(uint8_t byte){
    spi2->beginTransaction(SPISettings(SPI2_FREQ,MSBFIRST,SPI_MODE0));
    spi2->transfer(byte);
    spi2->endTransaction();
}

uint8_t sBSP_SPI2M_ReadByte(){
    uint8_t byte = 0;
    spi2->beginTransaction(SPISettings(SPI2_FREQ,MSBFIRST,SPI_MODE0));
    byte = spi2->transfer(0x00);
    spi2->endTransaction();
    return byte;
}

void sBSP_SPI2M_SendBytes(uint8_t *pData,uint16_t length){
    spi2->beginTransaction(SPISettings(SPI2_FREQ,MSBFIRST,SPI_MODE0));
    spi2->transfer(pData,length);
    spi2->endTransaction();
}

void sBSP_SPI2M_ReadBytes(uint8_t *pData,uint16_t length){
    spi2->beginTransaction(SPISettings(SPI2_FREQ,MSBFIRST,SPI_MODE0));
    for(uint16_t i = 0; i < length; i++){
        pData[i] = spi2->transfer(0x00);
    }
    spi2->endTransaction();
} 



