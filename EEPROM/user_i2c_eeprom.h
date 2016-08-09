/**
  ******************************************************************************
  * @file    I2C/EEPROM/i2c_ee.h 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Header for i2c_ee.c module
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __USER_I2C_EEPROM_H
#define __USER_I2C_EEPROM_H

#include "includes.h"


/* Private define ------------------------------------------------------------*/
#define I2C1_Speed             100000u
#define I2C1_SLAVE_ADDRESS7    0xA0
#define I2C_FLASH_PAGESIZE     32
#define EEPROM_HW_ADDRESS      0xA0   /* E0 = E1 = E2 = 0 */ 

#define TEMP_FLASH_DATA_SIZE  1024*4
#define TEMP_FLASH_DATA_ADDR  1024*10

// flash数据临时写入存储区，地址为 10240 buffersize: 4K 
#define ADDR_FLASH_WRITE_DATA_BUFFER    TEMP_FLASH_DATA_ADDR

// flash数据临时读取存储区
#define ADDR_FLASH_READ_DATA_BUFFER     (ADDR_FLASH_WRITE_DATA_BUFFER + TEMP_FLASH_DATA_SIZE)



extern uint16_t EEPROM_ADDRESS;


/* Includes ------------------------------------------------------------------*/
extern void I2C1_Init(void);
extern void I2C1_ByteWrite(int8u* pBuffer, int16u WriteAddr);
extern void I2C1_PageWrite(int8u* pBuffer, int16u WriteAddr, int8u NumByteToWrite);
extern void I2C1_BufferWrite(int8u* pBuffer,int16u WriteAddr,int16u NumByteToWrite);
extern void I2C1_BufferRead(uint8_t* pBuffer, int16u ReadAddr, int16u NumByteToRead);
extern void I2C1_WaitEepromStandbyState(void);
extern void EepromInit(void);
extern uint8_t 	CheckE2PROMInitState(void);
extern void EraseEepromData(void);
extern void SaveEepromReg4(void);
extern void SaveEepromReg5(void);
extern void SaveEepromReg6(void);
extern void SaveEepromData(void);
extern void LoadEepromData(void);
extern void LoadFactoryDefault(void);


#endif /* __I2C_EE_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
