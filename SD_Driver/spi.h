#ifndef __SPI_H
#define __SPI_H 

#include "stm32f10x.h"

extern uint8_t SPI3_ReadWriteByte(uint8_t TxData);//SPI总线读写一个字节										  
extern void SPI3_SetSpeed(uint8_t SpeedSet);
#endif
