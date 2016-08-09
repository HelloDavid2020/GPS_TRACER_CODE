#ifndef __SPI_DMA_H
#define __SPI_DMA_H
#include "LSM330DLC.h"
#include "math.h"
#include "allcommon.h"
#include "status.h"
#include "commonmy.h"
#include <stdio.h>

#include "serialPortEx.h"
#include "user_log.h"



extern uint8_t spi1_tx_buf[6];
extern uint8_t spi1_rx_buf[12];
extern uint8_t spi1_tx_index;
extern uint8_t spi1_rx_index;
extern void get_lms330_data(void);
extern int16_t gZ;
extern float f_gZ ;
extern void SPI1_DMA_Configuration( void );
extern void SPI1_ReceiveSendByte( u16 num );
extern uint8 SPI_SendByte(uint8 Data);
extern unsigned char SPI_Read(unsigned char* data,unsigned char bytesNumber);

#endif


