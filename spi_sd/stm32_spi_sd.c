/**
  ******************************************************************************
  * @file    stm32_eval_spi_sd.c
  * @author  MCD Application Team
  * @version V4.2.0
  * @date    04/16/2010
  * @brief   This file provides a set of functions needed to manage the SPI SD 
  *          Card memory mounted on STM32xx-EVAL board (refer to stm32_eval.h
  *          to know about the boards supporting this memory). 
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32 hardware resources (SPI and 
  *          GPIO) are defined in stm32xx_eval.h file, and the initialization is 
  *          performed in SD_LowLevel_Init() function declared in stm32xx_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          SD_LowLevel_Init() function.
  *            
  *          +-------------------------------------------------------+
  *          |                     Pin assignment                    |
  *          +-------------------------+---------------+-------------+
  *          |  STM32 SPI Pins         |     SD        |    Pin      |
  *          +-------------------------+---------------+-------------+
  *          | SD_SPI_CS_PIN           |   ChipSelect  |    1        |
  *          | SD_SPI_MOSI_PIN / MOSI  |   DataIn      |    2        |
  *          |                         |   GND         |    3 (0 V)  |
  *          |                         |   VDD         |    4 (3.3 V)|
  *          | SD_SPI_SCK_PIN / SCLK   |   Clock       |    5        |
  *          |                         |   GND         |    6 (0 V)  |
  *          | SD_SPI_MISO_PIN / MISO  |   DataOut     |    7        |
  *          +-------------------------+---------------+-------------+
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
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/


#include "stm32f10x.h"
#include "includes.h"
#include "spi_flash.h"

#include "stm32_spi_sd.h"
#include <absacc.h>
#include <stdio.h>
#include "user_sd_card.h"


/** @addtogroup Utilities
  * @{
  */


SD_CardInfo	cardinfo;// __at(0x20003000);
unsigned char block_buff[SD_BLOCK_SIZE] = {0};//SD_BLOCK_SIZE

unsigned char  Card_Type = 0;
 uint32_t Mass_Block_Count[2];		                   //FATFS文件系统变量 
 
void SPI_SD_CARD_HIGH(void)
{
	SPI_FLASH_ChipSelect(SD_CARD,0)	 ;
}
void SPI_SD_CARD_LOW(void)
{
	SPI_FLASH_ChipSelect(SD_CARD,1)	 ;
}

/**
  * @brief  random Writes on the SD
  * @param  WriteBuffer: pointer to the buffer containing the data to be written on 
  *                  the SD.
  * @param  DestAddr: address to write on.
  * @param  WriteSize: write buffer size.
  * @brief	param brief
			- Zerolimit  
			- WriteSizelimit  
			- Maximumlimit 
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_BufferWrite(uint8_t* WriteBuffer, uint32_t DestAddr, uint16_t WriteSize)
{
//	unsigned char block_buff[SD_BLOCK_SIZE] = {0};
	unsigned short temp1 = 0; 		
	unsigned short temp2 = 0;	
	unsigned short i = 0;
	SD_Error ret;
	//0 data
	//put_tick("\r\n写sd卡开始时间：\r\n");
	if(WriteSize == 0)			
	{
		return SD_ILLEGAL_COMMAND;
	}
	//2048 limit
	if(WriteSize > 2048)		
	{
		//DEBUG_OUT("\r\n写sd卡出错2\r\n");
		#if   DEBUG_GSM
		if(sys_common.isComming==0)
		vSerialPutString(DEBUG_GSM_PORT, "\r\n写sd卡出错2\r\n");
		#endif 
		return SD_ILLEGAL_COMMAND;
	}
//	if((cardinfo.SD_csd.DeviceSize << 9)>0x400000)	//大于4g
//	{
//		//Maximum limit
////		if((DestAddr+WriteSize) > 0xffffffff)//((cardinfo.SD_csd.DeviceSize << 9)*1024))	
////		{
////			DEBUG_OUT("\r\n写sd卡出错3\r\n");
////			return SD_ILLEGAL_COMMAND;
////		}
//	}
//	else
//	{
//		if((DestAddr+WriteSize) > ((cardinfo.SD_csd.DeviceSize << 9)*1024))	
//		{
//		//	DEBUG_OUT("\r\n写sd卡出错13\r\n");
//		#if   DEBUG_GSM
//		if(sys_common.isComming==0)
//	vSerialPutString(DEBUG_GSM_PORT, "\r\n写sd卡出错13\r\n");
//		#endif 
//			return SD_ILLEGAL_COMMAND;
//		}
//	}

		if((DestAddr+WriteSize) > ((cardinfo.SD_csd.DeviceSize << 9)*1024))	
		{
			printf("> *** 写sd卡出错,写入数据溢出.\r\n");
			#if   DEBUG_GSM
				if(sys_common.isComming==0)
				vSerialPutString(DEBUG_GSM_PORT, "\r\n写sd卡出错13\r\n");
			#endif 
			return SD_ILLEGAL_COMMAND;
		}
	
	temp1 = DestAddr%SD_BLOCK_SIZE;
	
	if((temp1+WriteSize) < SD_BLOCK_SIZE)		//one block operation 
	{
		//read the only block
		ret = SD_ReadBlock(block_buff, DestAddr-(DestAddr%SD_BLOCK_SIZE), SD_BLOCK_SIZE);	
		if(ret == SD_RESPONSE_NO_ERROR)
		{
			memcpy(block_buff+temp1, WriteBuffer, WriteSize);
			ret = SD_WriteBlock(block_buff, DestAddr-(DestAddr%SD_BLOCK_SIZE), SD_BLOCK_SIZE);	
		}
		//put_tick("\r\n写sd卡结束时间：\r\n");
		return ret;
	}
	else		//multiple block operation
	{
		//first block
		ret = SD_ReadBlock(block_buff, DestAddr-(DestAddr%SD_BLOCK_SIZE), SD_BLOCK_SIZE);
		if(ret == SD_RESPONSE_NO_ERROR)
		{
			memcpy(block_buff+temp1, WriteBuffer, SD_BLOCK_SIZE-temp1);
			ret = SD_WriteBlock(block_buff, DestAddr-(DestAddr%SD_BLOCK_SIZE), SD_BLOCK_SIZE);	
			if(ret != SD_RESPONSE_NO_ERROR)
			{
				//put_tick("\r\n写sd卡结束时间：\r\n");
				//DEBUG_OUT("\r\n写sd卡出错4\r\n");
				#if   DEBUG_GSM
				if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "\r\n写sd卡出错4\r\n");
		#endif
				return ret;
			}
		}
		else
		{
			//put_tick("\r\n写sd卡结束时间：\r\n");
			//DEBUG_OUT("\r\n写sd卡出错5\r\n");
			#if   DEBUG_GSM
			if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "\r\n写sd卡出错5\r\n");
		#endif
			return ret;
		}
		//middle blocks
		temp1 = SD_BLOCK_SIZE-temp1;
		temp2 = WriteSize - temp1;
		i++;
		while(temp2 > SD_BLOCK_SIZE)
		{
			memcpy(block_buff, WriteBuffer+temp1, SD_BLOCK_SIZE);
			ret = SD_WriteBlock(block_buff, (DestAddr+i*SD_BLOCK_SIZE)-((DestAddr+i*SD_BLOCK_SIZE)%SD_BLOCK_SIZE), SD_BLOCK_SIZE);	
			if(ret != SD_RESPONSE_NO_ERROR)
			{
				//put_tick("\r\n写sd卡结束时间：\r\n");
			//	DEBUG_OUT("\r\n写sd卡出错6\r\n");
			#if   DEBUG_GSM
			if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "\r\n写sd卡出错6\r\n");
		#endif
				return ret;
			}
			temp1 += SD_BLOCK_SIZE;
			temp2 -= SD_BLOCK_SIZE;
			i++;
		}
		//the last block
		ret = SD_ReadBlock(block_buff, (DestAddr+i*SD_BLOCK_SIZE)-((DestAddr+i*SD_BLOCK_SIZE)%SD_BLOCK_SIZE), SD_BLOCK_SIZE);
		if(ret == SD_RESPONSE_NO_ERROR)
		{	
			memcpy(block_buff, WriteBuffer+temp1, temp2);
			ret = SD_WriteBlock(block_buff, (DestAddr+i*SD_BLOCK_SIZE)-((DestAddr+i*SD_BLOCK_SIZE)%SD_BLOCK_SIZE), SD_BLOCK_SIZE);	
			//put_tick("\r\n写sd卡结束时间：\r\n");
			return ret;
		}
		else
		{
			//put_tick("\r\n写sd卡结束时间：\r\n");
			//DEBUG_OUT("\r\n写sd卡出错7\r\n");
			#if   DEBUG_GSM
			if(sys_common.isComming==0)
			vSerialPutString(DEBUG_GSM_PORT, "\r\n写sd卡出错7\r\n");
			#endif
			return ret;
		}

	}
}

/**
  * @brief  random Reads on the SD
  * @param  ReadBuffer: pointer to the buffer containing the data to be read on 
  *                  the SD.
  * @param  DestAddr: address to read on.
  * @param  ReadSize: read buffer size.
  * @brief	param brief
			- Zerolimit  
			- WriteSizelimit  
			- Maximumlimit 
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_BufferRead(uint8_t* ReadBuffer, uint32_t DestAddr, uint16_t ReadSize)
{
//	static unsigned char block_buff[SD_BLOCK_SIZE] = {0};
	unsigned short read_num = 0;
	unsigned short temp = 0;
	unsigned short i = 0;
	SD_Error ret;
	//0 data
	//put_tick("\r\n读sd卡开始时间：\r\n");
	feed_dog();
	if(ReadSize == 0)
	{		
		//DEBUG_MCU("读出错位置1\r\n");
		#if   DEBUG_GSM
		if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "读出错位置1\r\n");
		#endif
		return SD_ILLEGAL_COMMAND;
	}
	//2048 limit
	if(ReadSize > 2048)
	{		
		//DEBUG_MCU("读出错位置2\r\n");
		#if   DEBUG_GSM
		if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "读出错位置2\r\n");
		#endif
		return SD_ILLEGAL_COMMAND;
	}
//	if((cardinfo.SD_csd.DeviceSize << 9)>0x400000)	//大于4g
//	{
////	   if((DestAddr+ReadSize) >= 0xfffffffe)//((cardinfo.SD_csd.DeviceSize << 9)*1024))	
////		{
////			DEBUG_MCU("读出错位置3\r\n");
////			return SD_ILLEGAL_COMMAND;
////		}
//	}
//	else
//	{
//		//Maximum limit
//		if((DestAddr+ReadSize) > ((cardinfo.SD_csd.DeviceSize << 9)*1024))	
//		{
//			//DEBUG_MCU("读出错位置3\r\n");
//			#if   DEBUG_GSM
//	vSerialPutString(DEBUG_GSM_PORT, "读出错位置3\r\n");
//		#endif
//			return SD_ILLEGAL_COMMAND;
//		}
//	}
	
		//Maximum limit
		if((DestAddr+ReadSize) > ((cardinfo.SD_csd.DeviceSize << 9)*1024))	
		{
			printf("> *** 读出错位置3 ***\r\n");
			#if   DEBUG_GSM
			vSerialPutString(DEBUG_GSM_PORT, "读出错位置3\r\n");
			#endif
			return SD_ILLEGAL_COMMAND;
		}
	
	temp = DestAddr%SD_BLOCK_SIZE;
	
	if((temp+ReadSize) < SD_BLOCK_SIZE)
	{
		ret = SD_ReadBlock(block_buff, DestAddr-(DestAddr%SD_BLOCK_SIZE), SD_BLOCK_SIZE);
		if(ret == SD_RESPONSE_NO_ERROR)
		{
			memcpy(ReadBuffer, block_buff+temp, ReadSize);
		}
//		DEBUG_MCU("读正确位置4\r\n");
		//put_tick("\r\n读sd卡结束时间：\r\n");
		return ret;
	}
	else		//multiple block operation
	{
		//first block
		ret = SD_ReadBlock(block_buff, DestAddr-(DestAddr%SD_BLOCK_SIZE), SD_BLOCK_SIZE);
		if(ret == SD_RESPONSE_NO_ERROR)
		{
			memcpy(ReadBuffer, block_buff+temp, SD_BLOCK_SIZE-temp);
		}
		else
		{
		//	DEBUG_MCU("读出错位置5\r\n");
			//put_tick("\r\n读sd卡结束时间：\r\n");
			#if   DEBUG_GSM
			if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "读出错位置5\r\n");
		#endif
			return ret;
		}
		//middle blocks
		read_num = SD_BLOCK_SIZE - temp;
		temp = ReadSize - read_num;
		i++;
		while(temp > SD_BLOCK_SIZE)
		{
			ret = SD_ReadBlock(block_buff, (DestAddr+i*SD_BLOCK_SIZE)-((DestAddr+i*SD_BLOCK_SIZE)%SD_BLOCK_SIZE), SD_BLOCK_SIZE);	
			if(ret != SD_RESPONSE_NO_ERROR)
			{
				//DEBUG_MCU("读出错位置6\r\n");
				//put_tick("\r\n读sd卡结束时间：\r\n");
				#if   DEBUG_GSM
				if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "读出错位置6\r\n");
		#endif
				return ret;
			}
			memcpy(ReadBuffer+read_num, block_buff, SD_BLOCK_SIZE);
			read_num += SD_BLOCK_SIZE;
			temp -= SD_BLOCK_SIZE;
			i++;
		}
		//the last block
		ret = SD_ReadBlock(block_buff, (DestAddr+i*SD_BLOCK_SIZE)-((DestAddr+i*SD_BLOCK_SIZE)%SD_BLOCK_SIZE), SD_BLOCK_SIZE);
		if(ret == SD_RESPONSE_NO_ERROR)
		{	
			memcpy(ReadBuffer+read_num, block_buff, temp);
		}
//		DEBUG_MCU("读正确位置7\r\n");
		//put_tick("\r\n读sd卡结束时间：\r\n");
		return ret;

	}
	
	
	
}

/*************************************************************************************************
** 函数名称: void SPI_ClkToMax()    
** 功能描述: 设置SPI的clock到最大值    
** 输　 入: 无         
** 输 　出: 无         
***************************************************************************************************/
void SPI_ClkToMax(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    /* SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SD_SPI, &SPI_InitStructure);
    /* SPI enable */
    SPI_Cmd(SD_SPI, ENABLE);
}
/**
  * @brief  Initializes the SD/SD communication.
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_Init(void)
{
  uint32_t i = 0;
	uint32_t retry=10;
	uint32_t r1=0;

  /*!< SD chip select high */
	
	retry=20;
	do
	{
    /*!< SD chip select high */
		 SPI_SD_CARD_HIGH();
  
    /*!< Send dummy byte 0xFF, 10 times with CS high */
    /*!< Rise CS and MOSI for 80 clocks cycles */		
		for(i=0;i<10;i++)
		{
			/*!< Send dummy byte 0xFF */
		SD_WriteByte(0XFF);//发送最少74个脉冲
		}
			
		SPI_SD_CARD_LOW();

		r1=SD_SPI_SendCmd(0,0,0x95);//进入IDLE状态
		
		
	}while((r1!=0X01) && retry--);  /*!< Wait for In Idle State Response (R1 Format) equal to 0x01 */



  /*!< Send dummy byte 0xFF, 10 times with CS high */
  /*!< Rise CS and MOSI for 80 clocks cycles */
  for (i = 0; i <= 9; i++)
  {
    /*!< Send dummy byte 0xFF */
    SD_WriteByte(SD_DUMMY_BYTE);
  }
  /*------------Put SD in SPI mode--------------*/
  /*!< SD initialized and set to SPI mode properly */
  return (SD_GoIdleState());
}
//SD_Error SD_Init(void)
//{
//	uint8_t r1;      // 存放SD卡的返回值
//	uint16_t retry;  // 用来进行超时计数
//	uint8_t buf[4];  
//	uint16_t i;
//	uint8_t init_flag = 0;
//	
//	retry=20;
//	do
//	{
//    /*!< SD chip select high */
// SPI_SD_CARD_HIGH();
//  
//    /*!< Send dummy byte 0xFF, 10 times with CS high */
//    /*!< Rise CS and MOSI for 80 clocks cycles */		
//		for(i=0;i<10;i++)
//		{
//			/*!< Send dummy byte 0xFF */
//		SD_WriteByte(0XFF);//发送最少74个脉冲
//		}
//			
//		SPI_SD_CARD_LOW();

//		r1=SD_SPI_SendCmd(0,0,0x95);//进入IDLE状态
//		
//		
//	}while((r1!=0X01) && retry--);  /*!< Wait for In Idle State Response (R1 Format) equal to 0x01 */

//}


/**
  * @brief  Returns information about specific card.
  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD 
  *         card information.
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error status = SD_RESPONSE_FAILURE;
  if (Card_Type == SD_HC)
  {
     status = SD_GetCSDRegister(&(cardinfo->SD_csd));
     status = SD_GetCIDRegister(&(cardinfo->SD_cid));
  }
  else
  {
     status = SD_GetCSDRegister(&(cardinfo->SD_csd));
     status = SD_GetCIDRegister(&(cardinfo->SD_cid));
     cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
     cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
     cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
     cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }

  /*!< Returns the reponse */
  return status;
}

/**
  * @brief  Reads a block of data from the SD.
  * @param  pBuffer: pointer to the buffer that receives the data read from the 
  *                  SD.
  * @param  ReadAddr: SD's internal address to read from.
  * @param  BlockSize: the SD card Data block size.
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_ReadBlock(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t BlockSize)
{
  u32 tmp = 0 ;
	uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;

  /*!< SD chip select low */
  SPI_SD_CARD_LOW();

  if (Card_Type == SD_HC)
  {
     ReadAddr >>= 9;
   //	   ReadAddr /= 512 ;
  }
  
  /*!< Send CMD17 (SD_CMD_READ_SINGLE_BLOCK) to read one block */
  SD_SendCmd(SD_CMD_READ_SINGLE_BLOCK, ReadAddr, 0xFF);
  
  /*!< Check if the SD acknowledged the read block command: R1 response (0x00: no errors) */
  tmp  = SD_GetResponse(SD_RESPONSE_NO_ERROR) ;
  if(!tmp)
//   if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    /*!< Now look for the data token to signify the start of the data */
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      /*!< Read the SD block data : read NumByteToRead data */
      for (i = 0; i < BlockSize; i++)
      {
        /*!< Save the received data */
        *pBuffer = SD_ReadByte();
        /*!< Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /*!< Get CRC bytes (not really needed by us, but required by SD) */
      SD_ReadByte();
      SD_ReadByte();
      /*!< Set response value to success */
      rvalue = SD_RESPONSE_NO_ERROR;
    }
  }
  else
  {	
		rvalue =  tmp ;
  }	  
  /*!< SD chip select high */
  SPI_SD_CARD_HIGH();
  
  /*!< Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);
  
  /*!< Returns the reponse */
  return rvalue;
}

/**
  * @brief  Reads multiple block of data from the SD.
  * @param  pBuffer: pointer to the buffer that receives the data read from the 
  *                  SD.
  * @param  ReadAddr: SD's internal address to read from.
  * @param  BlockSize: the SD card Data block size.
  * @param  NumberOfBlocks: number of blocks to be read.
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_ReadMultiBlocks(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  uint32_t i = 0, Offset = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;
  
  /*!< SD chip select low */
  SPI_SD_CARD_LOW();

  /*!< Data transfer */
  while (NumberOfBlocks--)
  {
    if (Card_Type == SD_HC)
    {
       /*!< Send CMD17 (SD_CMD_READ_SINGLE_BLOCK) to read one block */
       SD_SendCmd (SD_CMD_READ_SINGLE_BLOCK, ( (ReadAddr + Offset) >> 9 ), 0xFF);
    }
    else
    {
       /*!< Send CMD17 (SD_CMD_READ_SINGLE_BLOCK) to read one block */
       SD_SendCmd (SD_CMD_READ_SINGLE_BLOCK, ReadAddr + Offset, 0xFF);
    }

    /*!< Check if the SD acknowledged the read block command: R1 response (0x00: no errors) */
    if (SD_GetResponse(SD_RESPONSE_NO_ERROR))
    {
      return  SD_RESPONSE_FAILURE;
    }
    /*!< Now look for the data token to signify the start of the data */
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      /*!< Read the SD block data : read NumByteToRead data */
      for (i = 0; i < BlockSize; i++)
      {
        /*!< Read the pointed data */
        *pBuffer = SD_ReadByte();
        /*!< Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /*!< Set next read address*/
      Offset += 512;
      /*!< get CRC bytes (not really needed by us, but required by SD) */
      SD_ReadByte();
      SD_ReadByte();
      /*!< Set response value to success */
      rvalue = SD_RESPONSE_NO_ERROR;
    }
    else
    {
      /*!< Set response value to failure */
      rvalue = SD_RESPONSE_FAILURE;
    }
  }
  /*!< SD chip select high */
  SPI_SD_CARD_HIGH();
  /*!< Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);
  /*!< Returns the reponse */
  return rvalue;
}

/**
  * @brief  Writes a block on the SD
  * @param  pBuffer: pointer to the buffer containing the data to be written on 
  *                  the SD.
  * @param  WriteAddr: address to write on.
  * @param  BlockSize: the SD card Data block size.
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_WriteBlock(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t BlockSize)
{
  uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;

  /*!< SD chip select low */
  SPI_SD_CARD_LOW();

  if (Card_Type == SD_HC)
  {
     WriteAddr >>= 9;
   	//   WriteAddr /= 512 ;
  }

  /*!< Send CMD24 (SD_CMD_WRITE_SINGLE_BLOCK) to write multiple block */
  SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, WriteAddr, 0xFF);
  
  /*!< Check if the SD acknowledged the write block command: R1 response (0x00: no errors) */
  if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    /*!< Send a dummy byte */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*!< Send the data token to signify the start of the data */
    SD_WriteByte(0xFE);

    /*!< Write the block data to SD : write count data by block */
    for (i = 0; i < BlockSize; i++)
    {
      /*!< Send the pointed byte */
      SD_WriteByte(*pBuffer);
      /*!< Point to the next location where the byte read will be saved */
      pBuffer++;
    }
    /*!< Put CRC bytes (not really needed by us, but required by SD) */
    SD_ReadByte();
    SD_ReadByte();

    /*!< Read data response */
    if (SD_GetDataResponse() == SD_DATA_OK)
    {
      rvalue = SD_RESPONSE_NO_ERROR;
    }
  }
  /*!< SD chip select high */
  SPI_SD_CARD_HIGH();
  /*!< Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);

  /*!< Returns the reponse */
  return rvalue;
}

/**
  * @brief  Writes many blocks on the SD
  * @param  pBuffer: pointer to the buffer containing the data to be written on 
  *                  the SD.
  * @param  WriteAddr: address to write on.
  * @param  BlockSize: the SD card Data block size.
  * @param  NumberOfBlocks: number of blocks to be written.
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_WriteMultiBlocks(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  uint32_t i = 0, Offset = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;

  /*!< SD chip select low */
  SPI_SD_CARD_LOW();
//  WriteAddr /= 512;
  /*!< Data transfer */
  while (NumberOfBlocks--)
  {
    if (Card_Type == SD_HC)
	{
	   /*!< Send CMD24 (SD_CMD_WRITE_SINGLE_BLOCK) to write blocks */
       SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, ( (WriteAddr + Offset)>>9 ), 0xFF);
	}
	else
	{
	   /*!< Send CMD24 (SD_CMD_WRITE_SINGLE_BLOCK) to write blocks */
       SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, WriteAddr + Offset, 0xFF);
	}
    /*!< Check if the SD acknowledged the write block command: R1 response (0x00: no errors) */
    if (SD_GetResponse(SD_RESPONSE_NO_ERROR))
    {
      return SD_RESPONSE_FAILURE;
    }
    /*!< Send dummy byte */
    SD_WriteByte(SD_DUMMY_BYTE);
    /*!< Send the data token to signify the start of the data */
    SD_WriteByte(SD_START_DATA_SINGLE_BLOCK_WRITE);
    /*!< Write the block data to SD : write count data by block */
    for (i = 0; i < BlockSize; i++)
    {
      /*!< Send the pointed byte */
      SD_WriteByte(*pBuffer);
      /*!< Point to the next location where the byte read will be saved */
      pBuffer++;
    }
    /*!< Set next write address */
    Offset += 512;
    /*!< Put CRC bytes (not really needed by us, but required by SD) */
    SD_ReadByte();
    SD_ReadByte();
    /*!< Read data response */
    if (SD_GetDataResponse() == SD_DATA_OK)
    {
      /*!< Set response value to success */
      rvalue = SD_RESPONSE_NO_ERROR;
    }
    else
    {
      /*!< Set response value to failure */
      rvalue = SD_RESPONSE_FAILURE;
    }
  }
  /*!< SD chip select high */
  SPI_SD_CARD_HIGH();
  /*!< Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);
  /*!< Returns the reponse */
  return rvalue;
}
/**
  * @brief  Read the CSD card register.
  *         Reading the contents of the CSD register in SPI mode is a simple 
  *         read-block transaction.
  * @param  SD_csd: pointer on an SCD register structure
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCSDRegister(SD_CSD* SD_csd)
{
  uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;
  uint8_t CSD_Tab[16];
  SD_CardInfo sd_CardInfo;
  /*!< SD chip select low */
  SPI_SD_CARD_LOW();
  /*!< Send CMD9 (CSD register) or CMD10(CSD register) */
  SD_SendCmd(SD_CMD_SEND_CSD, 0, 0xFF);
  /*!< Wait for response in the R1 format (0x00 is no errors) */
  if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      for (i = 0; i < 16; i++)
      {
        /*!< Store CSD register value on CSD_Tab */
        CSD_Tab[i] = SD_ReadByte();
      }
    }
    /*!< Get CRC bytes (not really needed by us, but required by SD) */
    SD_WriteByte(SD_DUMMY_BYTE);
    SD_WriteByte(SD_DUMMY_BYTE);
    /*!< Set response value to success */
    rvalue = SD_RESPONSE_NO_ERROR;
  }
  /*!< SD chip select high */
  SPI_SD_CARD_HIGH();
  /*!< Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);

  /*!< Byte 0 */
  SD_csd->CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
  SD_csd->SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
  SD_csd->Reserved1 = CSD_Tab[0] & 0x03;

  /*!< Byte 1 */
  SD_csd->TAAC = CSD_Tab[1];

  /*!< Byte 2 */
  SD_csd->NSAC = CSD_Tab[2];

  /*!< Byte 3 */
  SD_csd->MaxBusClkFrec = CSD_Tab[3];

  /*!< Byte 4 */
  SD_csd->CardComdClasses = CSD_Tab[4] << 4;

  /*!< Byte 5 */
  SD_csd->CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
  SD_csd->RdBlockLen = CSD_Tab[5] & 0x0F;

  /*!< Byte 6 */
  SD_csd->PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
  SD_csd->WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
  SD_csd->RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
  SD_csd->DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
  SD_csd->Reserved2 = 0; /*!< Reserved */

  if (Card_Type == SD_HC)
  {	
	  /*!< Byte 7 */
	  SD_csd->DeviceSize = (CSD_Tab[7] & 0x3F) << 16;	
	  /*!< Byte 8 */
	  SD_csd->DeviceSize |= CSD_Tab[8] << 8;	
	  /*!< Byte 9 */
	  SD_csd->DeviceSize |= CSD_Tab[9];
	  /*!< Byte 10 */
	  sd_CardInfo.CardCapacity = (SD_csd->DeviceSize + 1) * 512 * 1024;
	  Mass_Block_Count[0] = (SD_csd->DeviceSize + 1) * 1024;
  }
  else
  {
	  SD_csd->DeviceSize = (CSD_Tab[6] & 0x03) << 10;
	
	  /*!< Byte 7 */
	  SD_csd->DeviceSize |= (CSD_Tab[7]) << 2;
	
	  /*!< Byte 8 */
	  SD_csd->DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;
	
	  SD_csd->MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
	  SD_csd->MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);
	
	  /*!< Byte 9 */
	  SD_csd->MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
	  SD_csd->MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
	  SD_csd->DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;
	  /*!< Byte 10 */
	  SD_csd->DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
  }
    
  SD_csd->EraseGrSize = (CSD_Tab[10] & 0x40) >> 6;
  SD_csd->EraseGrMul = (CSD_Tab[10] & 0x3F) << 1;

  /*!< Byte 11 */
  SD_csd->EraseGrMul |= (CSD_Tab[11] & 0x80) >> 7;
  SD_csd->WrProtectGrSize = (CSD_Tab[11] & 0x7F);

  /*!< Byte 12 */
  SD_csd->WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
  SD_csd->ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
  SD_csd->WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
  SD_csd->MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;

  /*!< Byte 13 */
  SD_csd->MaxWrBlockLen |= (CSD_Tab[13] & 0xC0) >> 6;
  SD_csd->WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
  SD_csd->Reserved3 = 0;
  SD_csd->ContentProtectAppli = (CSD_Tab[13] & 0x01);

  /*!< Byte 14 */
  SD_csd->FileFormatGrouop = (CSD_Tab[14] & 0x80) >> 7;
  SD_csd->CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
  SD_csd->PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
  SD_csd->TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
  SD_csd->FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
  SD_csd->ECC = (CSD_Tab[14] & 0x03);

  /*!< Byte 15 */
  SD_csd->CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
  SD_csd->Reserved4 = 1;

  /*!< Return the reponse */
  return rvalue;
}


/**
  * @brief  Read the CID card register.
  *         Reading the contents of the CID register in SPI mode is a simple 
  *         read-block transaction.
  * @param  SD_cid: pointer on an CID register structure
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCIDRegister(SD_CID* SD_cid)
{
  uint32_t i = 0;
  SD_Error rvalue = SD_RESPONSE_FAILURE;
  uint8_t CID_Tab[16];
  
  /*!< SD chip select low */
  SPI_SD_CARD_LOW();
  
  /*!< Send CMD10 (CID register) */
  SD_SendCmd(SD_CMD_SEND_CID, 0, 0xFF);
  
  /*!< Wait for response in the R1 format (0x00 is no errors) */
  if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
  {
    if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
    {
      /*!< Store CID register value on CID_Tab */
      for (i = 0; i < 16; i++)
      {
        CID_Tab[i] = SD_ReadByte();
      }
    }
    /*!< Get CRC bytes (not really needed by us, but required by SD) */
    SD_WriteByte(SD_DUMMY_BYTE);
    SD_WriteByte(SD_DUMMY_BYTE);
    /*!< Set response value to success */
    rvalue = SD_RESPONSE_NO_ERROR;
  }
  /*!< SD chip select high */
  SPI_SD_CARD_HIGH();
  /*!< Send dummy byte: 8 Clock pulses of delay */
  SD_WriteByte(SD_DUMMY_BYTE);

  /*!< Byte 0 */
  SD_cid->ManufacturerID = CID_Tab[0];

  /*!< Byte 1 */
  SD_cid->OEM_AppliID = CID_Tab[1] << 8;

  /*!< Byte 2 */
  SD_cid->OEM_AppliID |= CID_Tab[2];

  /*!< Byte 3 */
  SD_cid->ProdName1 = CID_Tab[3] << 24;

  /*!< Byte 4 */
  SD_cid->ProdName1 |= CID_Tab[4] << 16;

  /*!< Byte 5 */
  SD_cid->ProdName1 |= CID_Tab[5] << 8;

  /*!< Byte 6 */
  SD_cid->ProdName1 |= CID_Tab[6];

  /*!< Byte 7 */
  SD_cid->ProdName2 = CID_Tab[7];

  /*!< Byte 8 */
  SD_cid->ProdRev = CID_Tab[8];

  /*!< Byte 9 */
  SD_cid->ProdSN = CID_Tab[9] << 24;

  /*!< Byte 10 */
  SD_cid->ProdSN |= CID_Tab[10] << 16;

  /*!< Byte 11 */
  SD_cid->ProdSN |= CID_Tab[11] << 8;

  /*!< Byte 12 */
  SD_cid->ProdSN |= CID_Tab[12];

  /*!< Byte 13 */
  SD_cid->Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
  SD_cid->ManufactDate = (CID_Tab[13] & 0x0F) << 8;

  /*!< Byte 14 */
  SD_cid->ManufactDate |= CID_Tab[14];

  /*!< Byte 15 */
  SD_cid->CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
  SD_cid->Reserved2 = 1;

  /*!< Return the reponse */
  return rvalue;
}

/**
  * @brief  Send 5 bytes command to the SD card.
  * @param  Cmd: The user expected command to send to SD card.
  * @param  Arg: The command argument.
  * @param  Crc: The CRC.
  * @retval None
  */
void SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
  uint32_t i = 0x00;
  
  uint8_t Frame[6];
  
  Frame[0] = (Cmd | 0x40); /*!< Construct byte 1 */
  
  Frame[1] = (uint8_t)(Arg >> 24); /*!< Construct byte 2 */
  
  Frame[2] = (uint8_t)(Arg >> 16); /*!< Construct byte 3 */
  
  Frame[3] = (uint8_t)(Arg >> 8); /*!< Construct byte 4 */
  
  Frame[4] = (uint8_t)(Arg); /*!< Construct byte 5 */
  
  Frame[5] = (Crc); /*!< Construct CRC: byte 6 */
  
  for (i = 0; i < 6; i++)
  {
    SD_WriteByte(Frame[i]); /*!< Send the Cmd bytes */
  }
}


//uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
//{
//		uint8_t r1;	
//		uint32_t Retry=0; 
////	SD_DisSelect();//取消上次片选
////	if(SD_Select())return 0XFF;//片选失效 
//	//发送
//    SD_WriteByte(cmd | 0x40);//分别写入命令
//    SD_WriteByte(arg >> 24);
//    SD_WriteByte(arg >> 16);
//    SD_WriteByte(arg >> 8);
//    SD_WriteByte(arg);	  
//    SD_WriteByte(crc); 
//	if(cmd==12)
//		SD_WriteByte(0xff);//Skip a stuff byte when stop reading
//    //等待响应，或超时退出
//	Retry=0XFF;
//	do
//	{
//		r1=SD_WriteByte(0xFF);
//	}while((r1&0X80) && Retry--);	 
//	//返回状态值
//    return r1;
//}	



/**
  * @brief  Get SD card data response.
  * @param  None
  * @retval The SD status: Read data response xxx0<status>1
  *         - status 010: Data accecpted
  *         - status 101: Data rejected due to a crc error
  *         - status 110: Data rejected due to a Write error.
  *         - status 111: Data rejected due to other error.
  */
uint8_t SD_GetDataResponse(void)
{
  uint32_t i = 0;
  uint8_t response, rvalue;

  while (i <= 64)
  {
    /*!< Read resonse */
    response = SD_ReadByte();
    /*!< Mask unused bits */
    response &= 0x1F;
    switch (response)
    {
      case SD_DATA_OK:
      {
        rvalue = SD_DATA_OK;
        break;
      }
      case SD_DATA_CRC_ERROR:
        return SD_DATA_CRC_ERROR;
      case SD_DATA_WRITE_ERROR:
        return SD_DATA_WRITE_ERROR;
      default:
      {
        rvalue = SD_DATA_OTHER_ERROR;
        break;
      }
    }
    /*!< Exit loop in case of data ok */
    if (rvalue == SD_DATA_OK)
      break;
    /*!< Increment loop counter */
    i++;
  }

  /*!< Wait null data */
  while (SD_ReadByte() == 0);

  /*!< Return response */
  return response;
}

/**
  * @brief  Returns the SD response.
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetResponse(uint8_t Response)
{
  uint32_t Count = 0xFFF;

  /*!< Check if response is got or a timeout is happen */
  while ((SD_ReadByte() != Response) && Count)
  {
    Count--;
  }
  if (Count == 0)
  {
    /*!< After time out */
    return SD_RESPONSE_FAILURE;
  }
  else
  {
    /*!< Right response got */
    return SD_RESPONSE_NO_ERROR;
  }
}

/**
  * @brief  Returns the SD status.
  * @param  None
  * @retval The SD status.
  */
uint16_t SD_GetStatus(void)
{
  uint16_t Status = 0;

  /*!< SD chip select low */
  SPI_SD_CARD_LOW();

  /*!< Send CMD13 (SD_SEND_STATUS) to get SD status */
  SD_SendCmd(SD_CMD_SEND_STATUS, 0, 0xFF);

  Status = SD_ReadByte();
  Status |= (uint16_t)(SD_ReadByte() << 8);

  /*!< SD chip select high */
  SPI_SD_CARD_HIGH();

  /*!< Send dummy byte 0xFF */
  SD_WriteByte(SD_DUMMY_BYTE);

  return Status;
}
u8 SD_SPI_SendCmd(u8 Cmd, u32 Arg, u8 Crc)
{
	u8 res = 0xFF; 
	u16 Retry = 0x00;
	/* Send dummy byte 0xFF */
	SD_WriteByte(0xff);
	/* send byte1 */
	SD_WriteByte(Cmd | 0x40);
	/* send byte2 */
	SD_WriteByte((u8)(Arg >> 24));
	/* send byte3 */   
	SD_WriteByte((u8)(Arg >> 16));
	/* send byte4 */
	SD_WriteByte((u8)(Arg >> 8));
	/* send byte5 */
	SD_WriteByte((u8)(Arg));
	/* send CRC: byte6 */
	SD_WriteByte(Crc);   
	//lest read a stuff byte CMD12 数据手册有说明
	if(Cmd == 12) SD_ReadByte();
	/* Wait CMD response try max 8000 */
	while (Retry < 8000)
	{
	    res = SD_ReadByte();
	    /* 当回应为 0x00 0x01 退出 */
	    if(res <= 0x01)   break;
	    Retry++;
	}
	return res;
}
/**
  * @brief  Put SD in Idle state.
  * @param  None
  * @retval The SD Response: 
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GoIdleState(void)
{
   u32 i = 0x00;
   u8 Resp[4];
   u8 res = 0; 
   u16 retry = 0;
	uint8_t buf[4];  
	
   SD_Error rvalue1 = SD_RESPONSE_FAILURE;

   /*!< SD chip select low */
	SPI_SD_CARD_LOW();
  
   /*!< Send CMD0 (SD_CMD_GO_IDLE_STATE) to put SD in SPI mode */
   SD_SendCmd(SD_CMD_GO_IDLE_STATE, 0, 0x95);
  
   /*!< Wait for In Idle State Response (R1 Format) equal to 0x01 */
   if (SD_GetResponse(SD_IN_IDLE_STATE))
   {
     /*!< No Idle State Response: return response failue */
     return SD_RESPONSE_FAILURE;
   }
   printf("\r\n Put SD into Idle State sucessed! \r\n");
  

//	 /*----------Activates the card initialization process-----------*/
//   do
//   {    
//     /*!< Send Dummy byte 0xFF */
//     SD_WriteByte(SD_DUMMY_BYTE);
//    
//     /*!< Send CMD8 */
//     SD_SendCmd(8, 0x1AA, 0x87);

//     /*!< Wait for no error Response (R1 Format) equal to 0x00 */
//   }
//   while (SD_GetResponse(0x01));
//   printf("\r\n Activates the card CMD8! \r\n");
   printf("> 获取SD卡类型.\r\n");
	 //     /*!< Send CMD8 */
		res = SD_SPI_SendCmd(8,0x1AA,0x87); //SD V2.0
		
	 if(res==1)//SD V2.0
		{
			for(i=0;i<4;i++)
				buf[i]=SD_WriteByte(0XFF);	//Get trailing return value of R7 resp //     /*!< Send Dummy byte 0xFF */

			
			if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
			{
				printf("> SD卡支持2.7-3.6V.\r\n");
				retry=100;
				do
				{
					SD_SPI_SendCmd(CMD55,0,0X01);	//发送CMD55
					res=SD_SPI_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
				}while(res&&retry--);
				
				if(retry&&SD_SPI_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
				{
					for(i=0;i<4;i++)
						buf[i]=SD_WriteByte(0XFF);//得到OCR值
					
					if(buf[0]&0x40) 
					{
						Card_Type = SD_HC; 
						sd_card.type =SD_HC;    //检查CCS
						printf("> SD card type: V2HC.\r\n");
						
					}
					else 
					{
						Card_Type = SD_20; 
						sd_card.type =SD_20;   
						printf("> SD card type: V2.\r\n");
						
					}
					
					rvalue1 = SD_RESPONSE_NO_ERROR;
					printf("> Initializes Sucessed! \r\n");
					
				}
			}
		}
		else//SD V1.x/ MMC	V3
		{
			SD_SPI_SendCmd(CMD55,0,0X01);		//发送CMD55
			res=SD_SPI_SendCmd(CMD41,0,0X01);	//发送CMD41
			if(res<=1)
			{		
				sd_card.type =SD_TYPE_V1;
				retry=0XFF;
				do //等待退出IDLE模式
				{
					SD_SPI_SendCmd(CMD55,0,0X01);	//发送CMD55
					res=SD_SPI_SendCmd(CMD41,0,0X01);//发送CMD41
				}while(res&&retry--);
			}else
			{
				sd_card.type =SD_TYPE_MMC;//MMC V3
				printf("> SD card type: MMC\r\n");
				
				retry=0XFF;
				do //等待退出IDLE模式
				{											    
					res=SD_SPI_SendCmd(CMD1,0,0X01);//发送CMD1
				}while(res&&retry--);  
			}
			if(retry==0||SD_SPI_SendCmd(CMD16,512,0X01)!=0) 
			{
				sd_card.type=SD_TYPE_ERR;//错误的卡
				printf("> *** Unkown sd card! ***\r\n ");
			}
		}		
	 
	


    /*!< SD chip select high */
    SPI_SD_CARD_HIGH();
  
    /*!< Send dummy byte 0xFF */
    SD_WriteByte(SD_DUMMY_BYTE);

    SPI_ClkToMax();


//    return SD_RESPONSE_NO_ERROR;
	return rvalue1;
}


//uint8_t SD_WriteByte(uint8_t Data)
//{
//  //!< Wait until the transmit buffer is empty 
//  while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) == RESET)
//  {
//  }
//  
//  //!< Send the byte 
//  SPI_I2S_SendData(SD_SPI, Data);
//  
//  //!< Wait to receive a byte
//  while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
//  {
//  }
//  
//  //!< Return the byte read from the SPI bus  
//  return SPI_I2S_ReceiveData(SD_SPI);
//}

uint8_t SD_WriteByte(uint8_t Data)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SD_SPI, Data); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
	retry++;
	if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SD_SPI); //返回通过SPIx最近接收的数据					    
}




uint8_t SD_ReadByte(void)
{
  uint8_t Data = 0;
  
  //!< Wait until the transmit buffer is empty 
  while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
  }
  //!< Send the byte 
  SPI_I2S_SendData(SD_SPI, SD_DUMMY_BYTE);

  //!< Wait until a data is received 
  while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
  }
  //!< Get the received data 
  Data = SPI_I2S_ReceiveData(SD_SPI);

  //< Return the shifted data 
  return Data;
}


void EraseSDcard(unsigned char flag,unsigned long addr)
{
	unsigned char i=0;
	unsigned char buff[512]={0};
	
	memset(buff,0xff,512);
	if(4==flag)
	{
		for(i=0;i<8;i++)
		{
			if(0==SD_BufferWrite(buff,addr+i*512,512))
			{
				
			}
			else
			{
				//DEBUG_MCU("擦除sd卡数据出错一次\r\n");
				#if   DEBUG_GSM
				if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "擦除sd卡数据出错一次\r\n");
		#endif
			}
			feed_dog();
		}
	}
	else if(64==flag)
	{
		for(i=0;i<128;i++)
		{
			if(0==SD_BufferWrite(buff,addr+i*512,512))
			{
				
			}
			else
			{
			//	DEBUG_MCU("擦除sd卡数据出错一次\r\n");
			#if   DEBUG_GSM
			if(sys_common.isComming==0)
	vSerialPutString(DEBUG_GSM_PORT, "擦除sd卡数据出错一次\r\n");
			#endif
			}
			feed_dog();
		}
	}
}















/*

#define SD_CS_LOW_()     GPIO_ResetBits(GPIOA,GPIO_Pin_4) ;
#define SD_CS_HIGH_()    GPIO_SetBits(GPIOA,GPIO_Pin_4) ;

extern 	 u8  is_init ;

uint8_t  SD_WriteByte(uint8_t byte)
{
	unsigned char i;
	unsigned char j;	
	for(i=0;i<8;i++)
	{		 	   	
		GPIO_ResetBits(GPIOA,GPIO_Pin_5) ; 		
		if( (byte << i) & 0x80 )
			GPIO_SetBits(GPIOA,GPIO_Pin_7);	
		else 
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		if( is_init )	
			for(j = 0 ; j < 0x1f ; j++) ;
		GPIO_SetBits(GPIOA,GPIO_Pin_5) ; 	
		if( is_init )
			for(j = 0 ; j < 0x1f ; j++) ;
	}
	return  1;
}

uint8_t SD_ReadByte(void)
{
	unsigned char byte,i,j;
	byte = 0;
	for(i=0;i<8;i++)
	{			
		GPIO_ResetBits(GPIOA,GPIO_Pin_5) ; 	 	
		byte = (byte<<1);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 1 )	byte |= 0x01;
		if( is_init )
			for(j = 0 ; j < 0x1f ; j++) ;
		GPIO_SetBits(GPIOA,GPIO_Pin_5) ; 	
		if( is_init )
			for(j = 0 ; j < 0x1f ; j++) ;	
	} 	 
	return byte;
}

	  */

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
