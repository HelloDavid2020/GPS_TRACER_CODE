/**
  ******************************************************************************
  * @file    I2C/EEPROM/I2C1.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file provides a set of functions needed to manage the
  *          communication between I2C peripheral and I2C M24CXX EEPROM.
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

/* Includes ------------------------------------------------------------------*/
#include "user_i2c_eeprom.h"


void I2C1_Init(void)
{
  /* I2C configuration */
	I2C_InitTypeDef  	I2C_InitStructure; 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); 		/* I2C Periph clock enable */

	
	//PB5配置为输出;I2C_WP
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	    //2M时钟速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	
	/* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C1_Speed;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C1, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C1, &I2C_InitStructure);

	WP_DISABLE;
	
  /* Select the EEPROM address according to the state of E0, E1, E2 pins */
  EEPROM_ADDRESS = EEPROM_HW_ADDRESS;  
  
}
/**
  * @brief  Writes one byte to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @retval None
  */
void I2C1_ByteWrite(int8u* pBuffer, uint16_t WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
 
  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C1, (int8u)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(I2C1, (int8u)(WriteAddr & 0x00FF));
  
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C1, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read 
  *   from the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EEPROM.
  * @retval None
  */
void I2C1_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{  
    /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
   
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(I2C1, (int8u)((ReadAddr & 0xFF00) >> 8));    
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(I2C1, (int8u)(ReadAddr & 0x00FF));    
  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void I2C1_BufferWrite(int8u* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  int8u NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % I2C_FLASH_PAGESIZE;
  count = I2C_FLASH_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / I2C_FLASH_PAGESIZE;
  NumOfSingle = NumByteToWrite % I2C_FLASH_PAGESIZE;
 
  /* If WriteAddr is I2C_FLASH_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
    if(NumOfPage == 0) 
    {
      I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C1_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_FLASH_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        I2C1_PageWrite(pBuffer, WriteAddr, I2C_FLASH_PAGESIZE); 
        I2C1_WaitEepromStandbyState();
        WriteAddr +=  I2C_FLASH_PAGESIZE;
        pBuffer += I2C_FLASH_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C1_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_FLASH_PAGESIZE aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /* If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Write the data conained in same page */
        I2C1_PageWrite(pBuffer, WriteAddr, count);
        I2C1_WaitEepromStandbyState();      
        
        /* Write the remaining data in the following page */
        I2C1_PageWrite((int8u*)(pBuffer + count), (WriteAddr + count), (NumByteToWrite - count));
        I2C1_WaitEepromStandbyState();        
      }      
      else      
      {
        I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C1_WaitEepromStandbyState();        
      }     
    }
    /* If NumByteToWrite > I2C_FLASH_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_FLASH_PAGESIZE;
      NumOfSingle = NumByteToWrite % I2C_FLASH_PAGESIZE;
      
      if(count != 0)
      {  
        I2C1_PageWrite(pBuffer, WriteAddr, count);
        I2C1_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C1_PageWrite(pBuffer, WriteAddr, I2C_FLASH_PAGESIZE);
        I2C1_WaitEepromStandbyState();
        WriteAddr +=  I2C_FLASH_PAGESIZE;
        pBuffer += I2C_FLASH_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C1_WaitEepromStandbyState();
      }
    }
  }  
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void I2C1_PageWrite(int8u* pBuffer, uint16_t WriteAddr, int8u NumByteToWrite)
{ 
  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C1, (int8u)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(I2C1, (int8u)(WriteAddr & 0x00FF));
  

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C1, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
void I2C1_WaitEepromStandbyState(void)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Read I2C1 SR1 register to clear pending flags */
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
  
  /* STOP condition */    
  I2C_GenerateSTOP(I2C1, ENABLE);  
}


void EepromInit(void)
{
	I2C1_Init();  
	if(!CheckE2PROMInitState())
	{
		EraseEepromData();
		LoadFactoryDefault();
		SaveEepromData();
		LoadEepromData();
	}
	else
	{
		LoadEepromData();
	}
}



uint8_t CheckE2PROMInitState(void)
{
	uint8_t result = 0;
	uint8_t readbuf[10] = {0};
	I2C1_BufferRead(readbuf, 0, 8);


	if(readbuf[6] == '3' && readbuf[7] == 'M')
		result = 1;
	else
		result = 0;
	return result;
}

void EraseEepromData(void)
{
	uint16_t i = 0;
	uint8_t  temp[256] = {0};

	for (i = 0; i < MODBUS_REG4_SIZE; i += 256)
		I2C1_BufferWrite(temp, i, 256);
	for (i = 0; i < MODBUS_REG5_SIZE; i += 256)
		I2C1_BufferWrite(temp, MODBUS_REG4_SIZE + i, 256);
	for (i = 0; i < MODBUS_REG6_SIZE; i += 256)
		I2C1_BufferWrite(temp, MODBUS_REG4_SIZE + MODBUS_REG5_SIZE + i, 256);
}
// ????Save Preferences;Save Parameters;Save config now

void SaveEepromReg4(void)
{
	uint16_t i = 0;
	for (i = 0; i < MODBUS_REG4_SIZE; i += 256)
		I2C1_BufferWrite(g_modbusReg4 + i, i, 256);
}

void SaveEepromReg5(void)
{
	uint16_t i = 0;
	
	for (i = 0; i < MODBUS_REG5_SIZE; i += 256)
		I2C1_BufferWrite(g_modbusReg5 + i, MODBUS_REG4_SIZE + i, 256);
}
void SaveEepromReg6(void)
{
	uint16_t i = 0;
	
	for (i = 0; i < MODBUS_REG6_SIZE; i += 256)
		I2C1_BufferWrite(g_modbusReg6 + i, MODBUS_REG4_SIZE + MODBUS_REG5_SIZE + i, 256);
}

void SaveEepromData(void)
{
	SaveEepromReg4();
	SaveEepromReg5();
	SaveEepromReg6();
}

void LoadEepromData(void)
{
	uint16_t i;
	for (i = 0; i < MODBUS_REG4_SIZE; i += 256)
		I2C1_BufferRead(g_modbusReg4 + i, i, 256);
	
	for (i = 0; i < MODBUS_REG5_SIZE; i += 256)
		I2C1_BufferRead(g_modbusReg5 + i, MODBUS_REG4_SIZE + i, 256);
	
	for (i = 0; i < MODBUS_REG6_SIZE; i += 256)
		I2C1_BufferRead(g_modbusReg6 + i, MODBUS_REG4_SIZE + MODBUS_REG5_SIZE + i, 256);
 
	// 上传数据的地址和数据长度
	memcpy(g_modbusReg5,REGISTER_MODBUS5_BUFFER,80);
}

// 加载默认设置
void LoadFactoryDefault(void)
{
	g_modbusReg4[modbusTerminalBaseStatus] = 6;
	memcpy(g_modbusReg4 + HOMER_3X_DEVIDE_ID, 		"3901213M", 8);
	memcpy(g_modbusReg4 + modbusSMSNumberBack, 		"8613021286541", 13);
	memcpy(g_modbusReg4 + modbusSMSNumberAlarm, 	"8613021286541", 13);
	memcpy(g_modbusReg4 + modbusSMSNumberConcrol, "8613021286541", 13);

	*(int16u*)(g_modbusReg4 + modbusServerPort) 		= 7778;
	*(int16u*)(g_modbusReg4 + modbusServerPortBack) = 7778;   // backup port

	
	// 220.202.153.91
	// 221.223.233.155
	#ifdef TCP_HOME
	g_modbusReg4[modbusServerIP] 			= 220;
	g_modbusReg4[modbusServerIP + 1] 	= 202;
	g_modbusReg4[modbusServerIP + 2] 	= 153;
	g_modbusReg4[modbusServerIP + 3] 	= 91;
	
	g_modbusReg4[modbusServerIPBack] 			= 220;
	g_modbusReg4[modbusServerIPBack + 1] 	= 202;
	g_modbusReg4[modbusServerIPBack + 2] 	= 153;
	g_modbusReg4[modbusServerIPBack + 3] 	= 91;		
	#else
	g_modbusReg4[modbusServerIP] 			= 124;
	g_modbusReg4[modbusServerIP + 1] 	= 202;
	g_modbusReg4[modbusServerIP + 2] 	= 198;
	g_modbusReg4[modbusServerIP + 3] 	= 242;
	
	g_modbusReg4[modbusServerIPBack] 			= 124;
	g_modbusReg4[modbusServerIPBack + 1] 	= 202;
	g_modbusReg4[modbusServerIPBack + 2] 	= 198;
	g_modbusReg4[modbusServerIPBack + 3] 	= 242;

#endif
	*(int16u*)(g_modbusReg4 + modbusTcpHeartInterval) = 30;//120;
	*(int16u*)(g_modbusReg4 + modbusTcpReportIntervalAccOn) = 30;//120;
	*(int16u*)(g_modbusReg4 + modbusTcpReportIntervalAccOff) = 60;//10800;// 3 hour
	*(int16u*)(g_modbusReg4 + modbusSleepDelay) = 600;
	*(int16u*)(g_modbusReg4 + modbusSleepTime) = 300;//
	
	
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_HIGH_ACC_ON) = 0;
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_LOW_ACC_ON) = 35;
	
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_HIGH_ACC_OFF) = 0;
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_LOW_ACC_OFF) = 35;
	
	*(usRegHoldingBuf + DEVEICE_WORK_SECONDS_HIGH) = 0;
	*(usRegHoldingBuf + DEVEICE_WORK_SECONDS_LOW) = 300;
	
	*(usRegHoldingBuf + DEVEICE_SLEEP_SECONDS_HIGH) = 0;
	*(usRegHoldingBuf + DEVEICE_SLEEP_SECONDS_LOW) = 300;
	
	*(usRegHoldingBuf + DEVEICE_SLEEP_ENABLE) = 0; // 允许休眠
	*(usRegHoldingBuf + DEVEICE_RUN_OR_TEST) = 1;	 // 测试模式
	
	
	*(usRegHoldingBuf + ADDR_ADMIN_LOCK_STATE)	 = 1;
	
	*(usRegHoldingBuf + COMM_TIMEOUT_GPRS) = 600;  // Add: 437, 600S
	*(usRegHoldingBuf + COMM_TIMEOUT_UART) = 15; // Add: 438, 15S
	*(usRegHoldingBuf + COMM_TIMEOUT_CAN) = 15; // Add: 439, a5S
	

	g_modbusReg4[modbusCanEnabled] 	= 1;
	g_modbusReg4[modbusCanBaudrate] = 250;
	*(int32u*)(g_modbusReg4 + modbusCanSendID1) = 0x183;
	g_modbusReg4[modbusCanSendHeart1] = 3;
	g_modbusReg4[modbusCanSendData1] 	= 1;
	
	*(int32u*)(g_modbusReg4 + modbusCanSendID2) = 0x283;
	g_modbusReg4[modbusCanSendHeart2] = 1;
	g_modbusReg4[modbusCanSendData2] 	= 2;

	g_modbusReg4[modbusCanNumber] = 2;
	*(int32u*)(g_modbusReg4 + modbusCanID1) = 0x383;
	*(int32u*)(g_modbusReg4 + modbusCanID1 + 4) = 0x483;

	// 上传数据的地址和数据长度
	memcpy(g_modbusReg5,REGISTER_MODBUS5_BUFFER,80);


}






/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
