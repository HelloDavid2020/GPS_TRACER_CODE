#ifndef __USER_I2C_H
#define __USER_I2C_H

#include "includes.h"



#define SCL_H         GPIOB->BSRR = GPIO_Pin_8
#define SCL_L         GPIOB->BRR  = GPIO_Pin_8 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_9
#define SDA_L         GPIOB->BRR  = GPIO_Pin_9

#define SCL_READ      GPIOB->IDR  & GPIO_Pin_8
#define SDA_READ      GPIOB->IDR  & GPIO_Pin_9



extern void Simulat_I2C1_Bus_Init(void);
extern void I2C_delay(void);

extern uint8_t I2C_Start(void);
extern void I2C_Stop(void);

extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern uint8_t I2C_WaitAck(void);

extern void I2C_Release(void);

extern void I2C_SendByte(uint8_t SendByte);
extern uint8_t I2C_ReceiveByte(void);






#endif
