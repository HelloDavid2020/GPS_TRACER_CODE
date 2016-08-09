

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "string.h"
//#include "tick.h"
//#include "watchdog.h"
#include "includes.h"


////#define SPI_FLASH_SIZE      0x2000000	  	 
//#define SPI_FLASH_PAGESIZE    256 
#define SPI_SECTOR_64K_SIZE		0x10000
#define SPI_SECTOR_4K_SIZE	4096

///* SPI Flash supported commands */
//#define FLASH_CMD_WRITE     0x02  /* Write to Memory instruction */
//#define FLASH_CMD_WRSR      0x01  /* Write Status Register instruction */
//#define FLASH_CMD_WREN      0x06  /* Write enable instruction */
//#define FLASH_CMD_READ      0x03  /* Read from Memory instruction */
//#define FLASH_CMD_RDSR      0x05  /* Read Status Register instruction  */
//#define FLASH_CMD_RDID      0x9F  /* Read identification */
//#define FLASH_CMD_4KE       0x20  /* 4k Erase instruction */
//#define FLASH_CMD_64KE      0xD8  /* 64k Erase instruction */
//#define FLASH_CMD_BE        0xC7  /* Bulk Erase instruction */

//#define FLASH_WIP_FLAG      0x01  /* Write In Progress (WIP) flag */

//#define FLASH_DUMMY_BYTE    0xA5	   

#define Num_max_opr			0x400 //允许进行操作的最大的数值

extern unsigned long SPI_FLASH1_SIZE; 
extern unsigned long SPI_FLASH2_SIZE;

typedef enum {FLASH1,FLASH2}ENUM_FLASH;

/* Exported functions --------------------------------------------------------*/
/*----- High layer function -----*/
void SPI_FLASH_Init(void);
unsigned char SPI_FLASH_64KErase(unsigned char num,unsigned long SectorAddr);
unsigned char SPI_FLASH_BulkErase(unsigned char num);
unsigned char SPI_FLASH_4KErase(ENUM_FLASH flash_num,unsigned long SectorAddr);


//unsigned char SPI_FLASH_BufferWrite(unsigned char num,unsigned char* pBuffer,unsigned long WriteAddr,unsigned short NumByteToWrite);
//unsigned char SPI_FLASH_BufferRead(unsigned char num,unsigned char* pBuffer,unsigned long ReadAddr,unsigned short NumByteToRead);
unsigned char SPI_FLASH_BufferRead(ENUM_FLASH flash_num,unsigned char* pBuffer,unsigned long ReadAddr,unsigned short NumByteToRead);
unsigned char SPI_FLASH_BufferWrite(ENUM_FLASH flash_num,unsigned char* pBuffer,unsigned long WriteAddr,unsigned short NumByteToWrite);
unsigned long SPI_FLASH_ReadID(ENUM_FLASH flash_num);
unsigned long SPI_FLASH_CheckCap(ENUM_FLASH flash_num);

u8 SPI_FLASH_SendByte(unsigned char byte) ;
void SPI_FLASH_ChipSelect(ENUM_FLASH flash_num,unsigned char en) ;


#endif /* __SPI_FLASH_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
