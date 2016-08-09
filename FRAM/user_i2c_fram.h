#ifndef __USER_I2C_FRAM_H
#define __USER_I2C_FRAM_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"


#ifndef OK
#define OK                     1 
#endif

#ifndef ERROR
#define ERROR                  0    
#endif

#ifndef TRUE  
#define TRUE                   1
#endif

#ifndef FALSE
#define FALSE                  0  
#endif

#ifndef NULL
#define NULL                   0ul
#endif 	


#define		FRAM_PAGE_SIZE				32

#define		DEVICE_TYPE_CODE 			0xA0  // Device Type Code
#define		DEVICE_ADDRESS_CODE 	0x00  // Device Address Code 
#define   DEVIDE_SLAVE_ADDR			0xA0  // I2C slave address

#define 	DEVICE_COMMAND_WRITE  0x00
#define		DEVICE_COMMAND_READ   0x01


#define I2C_FLASH_PAGESIZE	32

extern uint16_t EEPROM_ADDRESS;


/* 
	Device Type Code (4 bits)
	The upper 4 bits of the device address word are a device type code that identifies the device type, and are
	fixed at "1010" for the MB85RC256V.

	Device Address Code (3 bits)
	Following the device type code, the 3 bits of the device address code are input in order of A2, A1, and A0.
	The device address code identifies one device from up to eight devices connected to the bus.
	Each MB85RC256V is given a unique 3 bits code on the device address pin (external hardware pin A2, A1,
	and A0). The slave only responds if the received device address code is equal to this unique 3 bits code.
*/


// When the Write Protect pin is the "L" level, the entire memory region can be overwritten.
#define WP_DISABLE	GPIO_ResetBits(GPIOB, GPIO_Pin_5)  

// When the Write Protect pin is the "H" level, the writing operation is enable.
#define WP_ENABLE		GPIO_SetBits(GPIOB, GPIO_Pin_5)

#define SCL_H         GPIOB->BSRR = GPIO_Pin_8
#define SCL_L         GPIOB->BRR  = GPIO_Pin_8 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_9
#define SDA_L         GPIOB->BRR  = GPIO_Pin_9

// #define SCL_READ      GPIOB->IDR  & GPIO_Pin_6
#define SDA_READ      GPIOB->IDR  & GPIO_Pin_9



#define eprom_data_address 7168//1024*7
#define eprom_data_size 1024

extern void User_FramInit(void);
extern uint8_t CheckFramInitState(void);
 // E2PROm中读取
extern uint32_t FlashStartAddress(void);
extern uint32_t FlashCurrentAddress(void);
extern uint32_t FlashEndAddress(void);
extern void FlashSetStartAddress(uint32_t address);
extern void FlashSetCurrentAddress(uint32_t address);
extern void FlashSetEndAddress(uint32_t address);
// ret数据类型uint32 修改为 int32
extern uint32_t FlashDataSize(void);

extern void Simulat_I2C1_Bus_Init(void);
extern void I2C_delay(void);

extern uint8_t I2C_Start(void);
extern void I2C_Stop(void);

extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern uint8_t I2C_WaitAck(void);

extern void I2C_SendByte(uint8_t SendByte);
extern uint8_t I2C_ReceiveByte(void);

extern uint8_t Fram_ByteWrite(uint8_t *pBuffer, uint16_t WriteAddr);
extern void Fram_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
extern void Fram_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);      

extern uint8_t Fram_BufferRead(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToRead);

extern void FRAM_ReadReadDeviceID(uint8_t* pBuffer);
extern void CheckFram(void);

//extern void StoreLogNums(uint32_t logNum);
//extern uint32_t ReadLogNums(void);


extern void LoadDataFromEEPROM(void);
#endif


