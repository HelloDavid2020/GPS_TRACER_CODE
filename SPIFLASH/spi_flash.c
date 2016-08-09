/******************** (C) COPYRIGHT ********************
 * ÎÄ¼þÃû  £ºspi_flash.c

 *          ----------------------------
 *         | PA4-SPI1-NSS  : W25X32-CS  |
 *         | PA5-SPI1-SCK  : W25X32-CLK |
 *         | PA6-SPI1-MISO : W25X32-DO  |
 *         | PA7-SPI1-MOSI : W25X32-DIO |
 *          ----------------------------

	Manufacturer and Device Identification
-------------------------------------------------------
MANUFACTURER ID 					|						(M7-M0)
--------------------------+----------------------------
Winbond Serial Flash 			|							EFH
--------------------------+----------------------------

														 Device ID		|			Memory Type + Memory Capacity 
Device ID 								|		(ID7-ID0) 	|							(ID15-ID0)
--------------------------------------------------------------
Instruction 							|		ABh, 90h 		|					9Fh
	W25X10 									|		10h 				|					3011h
	W25X20 									|		11h 				|					3012h
	W25X40 									|		12h 				|					3013h
	W25X80 									|		13h 				|					3014h
	W25X32 									|		15h 				|					3016h

Device ID 		(ID7-ID0) 
 The JEDEC assigned Manufacturer ID byte for Winbond (EFh) and two Device ID bytes, 
 Memory Type(ID15-ID8) and Capacity (ID7-ID0) are then shifted out on the falling 
 edge of CLK with most significant bit (MSB) first.
 
**********************************************************************************/
#include "spi_flash.h"


FLASH_DEVICE FlashDevice;
uint16_t FlashlogNums = 0;
uint8_t flashbuf[SECTOR_SIZE] = {0}; //Ò»¸öÉÈÇøµÄ»º´æ
uint8_t SPI_FLASH_BUF[SECTOR_SIZE] = {0};

void Modbus_FlashInit(void)
{
//	WriteTotalLogNums(0);
	SPI_FLASH_ChipErase(); 
}

/*
 * º¯ÊýÃû£ºBuffercmp
 * ÃèÊö  £º±È½ÏÁ½¸ö»º³åÇøÖÐµÄÊý¾ÝÊÇ·ñÏàµÈ
 * ÊäÈë  £º-pBuffer1     src»º³åÇøÖ¸Õë
 *         -pBuffer2     dst»º³åÇøÖ¸Õë
 *         -BufferLength »º³åÇø³¤¶È
 * Êä³ö  £ºÎÞ
 * ·µ»Ø  £º-PASSED pBuffer1 µÈÓÚ   pBuffer2
 *         -FAILED pBuffer1 ²»Í¬ÓÚ pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}


int8_t User_FlashInit(void)
{
	int8_t result = 0;
//	SPI1_Init();
//	SPI_FLASH_ChipErase();
	SPI2_Init();
	BlindZoneData.TotalLogNums =0;
  /* Get SPI Flash Device ID */
  FlashDevice.DeviceId = SPI_FLASH_ReadDeviceID();
	FlashDevice.JedecId  = SPI_FLASH_Read_JEDEC_ID();

  /* Check the SPI Flash ID */
  if (FlashDevice.JedecId == JEDEC_ID_W25X32)  /* #define  sFLASH_ID  0xEF3015 */
  {
// 		uSystem.Bit.flash = 1;
		#ifdef DEBUG
		printf("> SPI Flash Init.\r\n");
		printf("> MemoryType: W25X40.\r\n");
		printf("> Device ID:0x%06X.\r\n", FlashDevice.DeviceId);
		printf("> JedecId:  0x%X.\r\n", FlashDevice.JedecId);
		printf("> ---------------------\r\n");
		#endif
		result =  1;
	}
	else
	{
// 		uSystem.Bit.flash = 0;
		#ifdef DEBUG
		printf("> SPI Flash Init.\r\n");
		printf("> MemoryType is unknown.\r\n");
		printf("> Error!---------------\r\n");
		#endif
		result =  0;
	}
	
	return result;
//   SPI_Flash_PowerDown();  
	
}


/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
int8_t SPI_FLASH_SectorErase(uint32_t SectorNum)
{
	uint32_t SectorAddr = 0;
	
	/*  ¼ì²éÈë¿Ú²ÎÊý */
	if ((SectorNum > SECTOR_NUMS))	
		return ERROR;	
   	
	
	SectorAddr = SectorNum * SECTOR_SIZE;
	
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();	
	
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();
  /* Send Sector Erase instruction */
  SPI_SendByte(SPI2,W25X_SectorErase);
  /* Send SectorAddr high nibble address byte */
  SPI_SendByte(SPI2,(SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI_SendByte(SPI2,(SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI_SendByte(SPI2,SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
	return OK;
	
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ChipErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_ChipErase(void)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
	
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
	
  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();
  /* Send Bulk Erase instruction  */
  SPI_SendByte(SPI2,W25X_ChipErase);
  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
	
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();

  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();
  /* Send "Write to Memory " instruction */
  SPI_SendByte(SPI2,W25X_PageProgram);
  /* Send WriteAddr high nibble address byte to write to */
  SPI_SendByte(SPI2,(WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_SendByte(SPI2,(WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_SendByte(SPI2,WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     printf("\n\r Err: SPI_FLASH_PageWrite too large!");
  }

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    SPI_SendByte(SPI2,*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}


/************************************************************************
** º¯ÊýÃû³Æ:int8_t FlashBufferWrite (uint32_t address,uint32_t buffer,uint32_t length)
** º¯Êý¹¦ÄÜ:SPI Serial FlashµÄÐ´º¯Êý
** Èë¿Ú²ÎÊý:uint32_t address£ºÄ¿±êµØÖ·,·¶Î§ 0x0 - MAX_ADDR£¨MAX_ADDR = 0x1FFFFF£©				
**					uint32_t buffer: Ó¦ÓÃ²ã»º´æÇøÖ¸Õë
**      		uint32_t length: ÒªÐ´µÄÊý¾Ý×Ö½ÚÊý
** ³ö¿Ú²ÎÊý:²Ù×÷³É¹¦Ôò·µ»Ø0,Ê§°ÜÔò·µ»Ø-1		
************************************************************************/
// Õû¸öÉÈÇøµÄÐ´Èë
int8_t FlashBufferWrite (uint8_t * buffer,uint32_t address,uint32_t length)
{
	uint32_t SectorNum 	= 0;
	uint32_t SectorAddr = 0;
	uint32_t WriteAddr 	= 0;  // ½«ÒªÐ´ÈëµÄÊý¾ÝÔÚÉÈÇøÖÐµØÖ·
	uint32_t i = 0;
	uint8_t  * ptr = NULL;
//	uint8_t  tempbuf[SECTOR_SIZE] = {0x00}; // 4K Bytes

	if((address > ADDR_MAX)||(length > SECTOR_SIZE))
		return -1;

   SectorNum  = address / SECTOR_SIZE;   	// ÉÈÇø´óÐ¡ÊÇ0xfff£¬4kbyte
   SectorAddr = SectorNum * SECTOR_SIZE; 	// µ±Ç°ÉÈÇøÆðÊ¼µØÖ·
   WriteAddr   = address%SECTOR_SIZE;			// ½«ÒªÐ´ÈëµÄÊý¾ÝÔÚÉÈÇøÖÐµØÖ·

	 SPI_FLASH_BufferRead(flashbuf, SectorAddr, SECTOR_SIZE); // ¶ÁÈ¡Ô­À´ÉÈÇøµÄ4KÊý¾Ý 
   memcpy(flashbuf + WriteAddr, buffer, length);  // Ïò»º´æÖÐÐ´ÈëÐÂµÄÊý¾Ý
	
   ptr = flashbuf;
	 
   if( SPI_FLASH_SectorErase(SectorNum)== OK )
   {
	  for(i=0;i<16;i++)
	  {
			SPI_FLASH_PageWrite(ptr, SectorAddr, 256);
			ptr += 256;
			SectorAddr += 256;
		} 
		return 0;
   }
   return -1;

}


/*******************************************************************************
* Function Name  : FlashBufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
//NumByteToRead:Òª¶ÁÈ¡µÄ×Ö½ÚÊý(×î´ó65535)
int8_t FlashBufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{

	if ((ReadAddr+NumByteToRead > ADDR_MAX)||(NumByteToRead == 0))	
		return ERROR;	 //	¼ì²éÈë¿Ú²ÎÊý
	
	
	/* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();
	
  /* Send "Read from Memory " instruction */
  SPI_SendByte(SPI2,W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_SendByte(SPI2,(ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_SendByte(SPI2,(ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_SendByte(SPI2,ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_SendByte(SPI2,Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();
	
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();	
	
	return OK;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
//NumByteToRead:Òª¶ÁÈ¡µÄ×Ö½ÚÊý(×î´ó65535)
int8_t SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{

	if ((ReadAddr+NumByteToRead > ADDR_MAX)||(NumByteToRead == 0))	
		return ERROR;	 //	¼ì²éÈë¿Ú²ÎÊý
	
	
	/* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();
	
  /* Send "Read from Memory " instruction */
  SPI_SendByte(SPI2,W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_SendByte(SPI2,(ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_SendByte(SPI2,(ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_SendByte(SPI2,ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_SendByte(SPI2,Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();
	
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();	
	
	return OK;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "RDID " instruction */
  SPI_SendByte(SPI2,W25X_JedecDeviceID);

  /* Read a byte from the FLASH */
  Temp0 = SPI_SendByte(SPI2,Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp1 = SPI_SendByte(SPI2,Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp2 = SPI_SendByte(SPI2,Dummy_Byte);

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	
  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();
	
  return Temp;
}



/*
The instruction is initiated by driving the /CS pin low and shifting the instruction code “9Fh”. The
JEDEC assigned Manufacturer ID byte for Winbond (EFh) and two Device ID bytes, Memory Type
(ID15-ID8) and Capacity (ID7-ID0) are then shifted out on the falling edge of CLK with most significant
bit (MSB) first as shown in figure 19. For memory type and capacity values refer to Manufacturer and
Device Identification table.
*/

uint32_t SPI_FLASH_Read_JEDEC_ID(void)
{
  uint32_t Temp = 0;
	uint32_t ManufacturerID = 0;
	uint32_t MemoryType = 0;
	uint32_t MemoryCapacity = 0;

  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "RDID " instruction */
  SPI_SendByte(SPI2,W25X_JedecDeviceID);

  /* Read a byte from the FLASH */
  ManufacturerID = SPI_SendByte(SPI2,Dummy_Byte);

  /* Read a byte from the FLASH */
  MemoryType = SPI_SendByte(SPI2,Dummy_Byte);

  /* Read a byte from the FLASH */
  MemoryCapacity = SPI_SendByte(SPI2,Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();

  Temp = (ManufacturerID << 16) | (MemoryType << 8) | MemoryCapacity;

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "RDID " instruction */
  SPI_SendByte(SPI2,W25X_DeviceID);
  SPI_SendByte(SPI2,Dummy_Byte);
  SPI_SendByte(SPI2,Dummy_Byte);
  SPI_SendByte(SPI2,Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_SendByte(SPI2,Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_SendByte(SPI2,W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_SendByte(SPI2,(ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_SendByte(SPI2,(ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_SendByte(SPI2,ReadAddr & 0xFF);
}


/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "Write Enable" instruction */
  SPI_SendByte(SPI2,W25X_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_SendByte(SPI2,W25X_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_SendByte(SPI2,Dummy_Byte);

  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();
}


//½øÈëµôµçÄ£Ê½
void SPI_Flash_PowerDown(void)   
{ 
  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_SendByte(SPI2,W25X_PowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();
}   

//»½ÐÑ
void SPI_Flash_WAKEUP(void)   
{
  /* Select the FLASH: Chip Select low */
  SPI2_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_SendByte(SPI2,W25X_ReleasePowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI2_CS_HIGH();                   //µÈ´ýTRES1
}   

