#ifndef __USER_SPI_H
#define __USER_SPI_H

#include "includes.h"

#define SPI1_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI1_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define SPI2_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI2_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define FRAM_CS_LOW()       SPI1_CS_LOW() 
#define FRAM_CS_HIGH()      SPI1_CS_HIGH()
 

extern void SPI1_Init(void);
extern void SPI2_Init(void);
extern void SPI3_Init(void);





extern uint8_t SPI_SendByte(SPI_TypeDef* SPIx, uint8_t byte);
extern uint8_t SPI_ReadByte(SPI_TypeDef* SPIx);

#endif

