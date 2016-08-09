#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "includes.h"
/*
	The W25X32A array is organized into 16,384 programmable pages of 256-bytes each. 
	Up to 256 bytes can be programmed at a time using the Page Program instruction. 
	Pages can be erased in groups of 16 (sector erase), groups of 256 (block erase) 
	or the entire chip (chip erase). 
	The W25X32A has 1,024 erasable sectors and 64 erasable blocks. 
	The small 4KB sectors allow for greater flexibility in applications that require 
	data and parameter storage.
*/

// 4Kbytes为一个Sector
// 16个扇区为1个Block
// W25X32
// 容量为4M字节,共有64个Block,1024个Sectors
#ifndef OK
#define OK                     1 
#endif

#ifndef ERROR
#define ERROR                  0    
#endif

/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0x00

#ifndef UNINITIALIZED
#define UNINITIALIZED 0
#endif

#ifndef INITIALIZED
#define INITIALIZED 	1
#endif




// Manufacturer ID
#define WINBOND_SERIAL_FLASH   0xEF 

// device id
#define DEVICE_ID_W25X10					0x10
#define DEVICE_ID_W25X20					0x11
#define DEVICE_ID_W25X40					0x12
#define DEVICE_ID_W25X80					0x13
#define DEVICE_ID_W25X16					0x14
#define DEVICE_ID_W25X32					0x15

// Memory Type: 30
// Memory Capacity:
#define MEMORY_INFOR_W25X10			0x3011
#define MEMORY_INFOR_W25X20			0x3012
#define MEMORY_INFOR_W25X40			0x3013
#define MEMORY_INFOR_W25X80			0x3014

#define JEDEC_ID_W25X10				0xEF3011
#define JEDEC_ID_W25X20				0xEF3012
#define JEDEC_ID_W25X40				0xEF3013
#define JEDEC_ID_W25X80				0xEF3014

#define JEDEC_ID_W25X32	      0xEF3016

#define ADDRESS_PAGE_2047     0x07FF00
#define ADDRESS_PAGE0			    0x000000

#ifndef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE 256
#endif


#define ADDR_SECTOR_1023		0x3FF000



///* 获取缓冲区的长度 */
//#define TxBufferSize1   (countof(TxBuffer1) - 1)
//#define RxBufferSize1   (countof(TxBuffer1) - 1)
//#define countof(a)      (sizeof(a) / sizeof(*(a)))
//#define BufferSize 			(countof(Tx_Buffer)-1)


#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress





#define SPI_FLASH_SPI                           SPI1
#define SPI_FLASH_SPI_CLK                       RCC_APB2Periph_SPI1
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_CS_PIN                        GPIO_Pin_4                  /* PC.04 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOA                       /* GPIOC */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA

#define ADRESS_FALSH_PAGE0  0x00000000



#define ADDR_MAX		0x3FFFFF	// 定义芯片内部最大地址 
#define	SECTOR_NUMS   1024		// 扇区数目
#define SECTOR_SIZE		4096    // 扇区大小 4K

typedef enum ERTYPE{Sec1,Sec8,Sec16,Chip} ErType;  
typedef enum IDTYPE{Manu_ID,Dev_ID,Jedec_ID} idtype;

typedef enum 
{ 
	FAILED = 0, 
	PASSED = !FAILED
} TestStatus;

typedef struct
{
	uint32_t 	JedecId;
	uint8_t 	ManufacturerId;
	uint8_t 	DeviceId;
	uint8_t 	MemoryType;
	uint8_t 	MemoryCapacity;
	uint16_t 	MemoryInfor;
} FLASH_DEVICE;

extern FLASH_DEVICE FlashDevice;



extern uint16_t FlashlogNums;

extern uint8_t flashbuf[SECTOR_SIZE];
extern uint8_t SPI_FLASH_BUF[SECTOR_SIZE];

/*

The W25X10/20/40/80 array is organized into 512/1024/2048/4096 programmable pages of 256-bytes
each. Up to 256 bytes can be programmed at a time using the Page Program instruction. Pages can
be erased in groups of 16 (sector erase), groups of 256 (block erase) or the entire chip (chip erase).
The W25X10/20/40/80 has 32/64/128/256 erasable sectors and 2/4/8/16 erasable blocks respectively.
The small 4KB sectors allow for greater flexibility in applications that require data and parameter
storage.

*/


extern TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
extern void Modbus_FlashInit(void);





extern uint32_t SPI_FLASH_ReadID(void);
extern uint32_t SPI_FLASH_Read_JEDEC_ID(void);
extern uint32_t SPI_FLASH_ReadDeviceID(void);

extern void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
extern void SPI_Flash_PowerDown(void);
extern void SPI_Flash_WAKEUP(void);


extern uint8_t 	SPI_FLASH_ReadByte(void);
extern uint8_t 	SPI_FLASH_SendByte(uint8_t byte);
extern uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
extern void SPI_FLASH_WriteEnable(void);
extern void SPI_FLASH_WaitForWriteEnd(void);

extern void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
extern void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
extern int8_t FlashBufferWrite (uint8_t * buffer,uint32_t address,uint32_t length);
extern int8_t FlashBufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
extern int8_t SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

extern int8_t SPI_FLASH_SectorErase(uint32_t SectorNum);
extern void SPI_FLASH_ChipErase(void);


//extern uint32_t FlashWriteData(char * des, uint32_t size);
//extern uint32_t FlashReadData(char * des, uint32_t size);
//extern uint32_t FlashCleanData(uint32_t size);
//extern uint32_t FlashDataSize(void);
//extern uint32_t FlashSpaceSize(void);
//extern void FlashInit(void);
//extern void FlashInfo(void);
//extern uint32_t FlashWriteDateSMS(char * des, uint32_t size);



// // E2PROm中读取
//extern uint32_t FlashStartAddress(void);
//extern uint32_t FlashCurrentAddress(void);
//extern uint32_t FlashEndAddress(void);
//extern void FlashSetStartAddress(uint32_t address);
//extern void FlashSetCurrentAddress(uint32_t address);
//extern void FlashSetEndAddress(uint32_t address);
//extern uint32_t FlashDataSize(void);

extern int8_t User_FlashInit(void);


#endif /* __SPI_FLASH_H */

