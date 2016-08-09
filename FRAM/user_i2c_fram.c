#include "user_i2c_fram.h"


uint16_t EEPROM_ADDRESS;


void User_FramInit(void)
{
	Simulat_I2C1_Bus_Init();  // IO模拟
	if(!CheckFramInitState())
	{
		EraseEEPROM();
		LoadModbusDefault();
		SaveEEPROM();
		LoadDataFromEEPROM();
	}
	else
	{
		LoadDataFromEEPROM();
	}
}

uint8_t CheckFramInitState(void)
{
	uint8_t result = 0;
	uint8_t readbuf[10] = {0};
	Fram_BufferRead(readbuf, 0, 8);

	if(readbuf[6] == '3' && readbuf[7] == 'M')
		result = 1;
	else
		result = 0;
	
	return result;
}





 // E2PROm中读取
int32u FlashStartAddress(void)
{
	unsigned char temp[4];
	int32u *address = (int32u*)temp;
	Fram_BufferRead(temp, eprom_data_address, 4);
	return *address;
}

// I2C？？？
int32u FlashCurrentAddress(void)
{
	unsigned char temp[4];
	int32u *address = (int32u*)temp;
	Fram_BufferRead(temp, eprom_data_address + 4, 4);
	return *address;
}

int32u FlashEndAddress(void)
{
	unsigned char temp[4];
	int32u *address = (int32u*)temp;
	Fram_BufferRead(temp, eprom_data_address + 8, 4);
	return *address;
}

void FlashSetStartAddress(int32u address)
{
	unsigned char *temp = (unsigned char*)&address;
	Fram_BufferWrite(temp, eprom_data_address, 4);
}

void FlashSetCurrentAddress(int32u address)
{
	unsigned char *temp = (unsigned char*)&address;
	Fram_BufferWrite(temp, eprom_data_address + 4, 4);
}

void FlashSetEndAddress(int32u address)
{
	unsigned char *temp = (unsigned char*)&address;
	Fram_BufferWrite(temp, eprom_data_address + 8, 4);
}

// ret数据类型uint32 修改为 int32
int32u FlashDataSize(void)
{
	int32_t ret = FlashCurrentAddress() - FlashStartAddress();
	if (ret < 0)
		return (ret + ADDR_MAX + 1);
	else
		return (ret);
}


// IO I2C BUS
void Simulat_I2C1_Bus_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口

	//PB5配置为输出;I2C_WP
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	    //2M时钟速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LoadDataFromEEPROM(void)
{
	uint16_t i;
// 	uint8_t temp[16];
	for (i = 0; i < MODBUS_REG4_SIZE; i += 256)
		Fram_BufferRead(g_modbusReg4 + i, i, 256);
	for (i = 0; i < MODBUS_REG5_SIZE; i += 256)
		Fram_BufferRead(g_modbusReg5 + i, MODBUS_REG4_SIZE + i, 256);
	for (i = 0; i < MODBUS_REG6_SIZE; i += 256)
		Fram_BufferRead(g_modbusReg6 + i, MODBUS_REG4_SIZE + MODBUS_REG5_SIZE + i, 256);

	memcpy(g_modbusReg5,REGISTER_MODBUS5_BUFFER,80);
}


void I2C_delay(void)
{        
	// 20		- 330K
	// 40	 	- 200K 波形不好
	// 50	 	- 132K
	// 80 	- 100K 波形良好
	// 100	- 80k  波形良好
	
  uint8_t i=80; 
  while(i--) 
	{
		__nop();
	}
}

uint8_t I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_READ)return FALSE;        //SDA
	SDA_L;
	I2C_delay();
	if(SDA_READ) return FALSE;        //SDA
	SDA_L;
	I2C_delay();
	return TRUE;
}

void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

void I2C_Ack(void)
{        
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

void I2C_NoAck(void)
{        
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

uint8_t I2C_WaitAck(void)         
{
	SCL_L;
	I2C_delay();
	SDA_H;                        
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_READ)
	{
		SCL_L;
		return FALSE;
	}
	SCL_L;
	return TRUE;
}

void I2C_SendByte(uint8_t SendByte) 
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
                SCL_H;
        I2C_delay();
    }
    SCL_L;
}

uint8_t I2C_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;                                
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
          SCL_H;
      I2C_delay();        
      if(SDA_READ)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

uint8_t Fram_ByteWrite(uint8_t *pBuffer, uint16_t WriteAddr)
{
	if(!I2C_Start())return FALSE;
	
	I2C_SendByte(0xA0);
	if(!I2C_WaitAck())
	{
		I2C_Stop(); 
		return FALSE;
	}

	I2C_SendByte( (uint8_t)((WriteAddr & 0xFF00) >> 8) );
	I2C_WaitAck();
	
	I2C_SendByte((uint8_t)(WriteAddr & 0x00FF));
	I2C_WaitAck();
	
	I2C_SendByte(*pBuffer);
	I2C_WaitAck();  
	I2C_Stop(); 

	return TRUE;
}


void Fram_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{ 
	I2C_Start();
	
	I2C_SendByte(DEVIDE_SLAVE_ADDR | 0x00); // 0xA0
	I2C_WaitAck();


	I2C_SendByte( (uint8_t)((WriteAddr & 0xFF00) >> 8) );
	I2C_WaitAck();
	
	I2C_SendByte((uint8_t)(WriteAddr & 0x00FF));
	I2C_WaitAck();
	
  /* While there is data to be written */
  while(NumByteToWrite ) 
  {
    /* Send the current byte */
    I2C_SendByte( *pBuffer); 
		
    /* Point to the next byte to be written */
    pBuffer++; 
		NumByteToWrite--;
		I2C_WaitAck();
  }	
		
	I2C_Stop();
	
	User_DelayuS(50); //50*100us =5mS 
	
}


void Fram_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % FRAM_PAGE_SIZE;
  count = FRAM_PAGE_SIZE - Addr;
  NumOfPage =  NumByteToWrite / FRAM_PAGE_SIZE;
  NumOfSingle = NumByteToWrite % FRAM_PAGE_SIZE;
 
  /* If WriteAddr is FRAM_PAGE_SIZE aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < FRAM_PAGE_SIZE */
    if(NumOfPage == 0) 
    {
      Fram_PageWrite(pBuffer, WriteAddr, NumOfSingle);

    }
    /* If NumByteToWrite > FRAM_PAGE_SIZE */
    else  
    {
      while(NumOfPage)
      {
        Fram_PageWrite(pBuffer, WriteAddr, FRAM_PAGE_SIZE); 

        WriteAddr +=  I2C_FLASH_PAGESIZE;
        pBuffer += I2C_FLASH_PAGESIZE;
				NumOfPage--;
      }

      if(NumOfSingle!=0)
      {
        Fram_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  /* If WriteAddr is not FRAM_PAGE_SIZE aligned  */
  else 
  {
    /* If NumByteToWrite < FRAM_PAGE_SIZE */
    if(NumOfPage== 0) 
    {
      /* If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Write the data conained in same page */
        Fram_PageWrite(pBuffer, WriteAddr, count);

       
        /* Write the remaining data in the following page */
        Fram_PageWrite((uint8_t*)(pBuffer + count), (WriteAddr + count), (NumByteToWrite - count));

      }      
      else      
      {
        Fram_PageWrite(pBuffer, WriteAddr, NumOfSingle);

      }     
    }
    /* If NumByteToWrite > FRAM_PAGE_SIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / FRAM_PAGE_SIZE;
      NumOfSingle = NumByteToWrite % FRAM_PAGE_SIZE;
      
      if(count != 0)
      {  
        Fram_PageWrite(pBuffer, WriteAddr, count);

        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage)
      {
        Fram_PageWrite(pBuffer, WriteAddr, FRAM_PAGE_SIZE);

        WriteAddr +=  FRAM_PAGE_SIZE;
        pBuffer += FRAM_PAGE_SIZE;  
				NumOfPage--;
      }
      if(NumOfSingle != 0)
      {
        Fram_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
      }
    }
  } 	
}


     
uint8_t Fram_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{                
	if (!I2C_Start()) return FALSE;

	I2C_SendByte(DEVIDE_SLAVE_ADDR | 0x00); // 0xA0
	I2C_WaitAck();

	I2C_SendByte( (uint8_t)((ReadAddr & 0xFF00) >> 8) );
	I2C_WaitAck();
	I2C_SendByte((uint8_t)(ReadAddr & 0x00FF));
	I2C_WaitAck();
	
	I2C_Start();
	I2C_SendByte(DEVIDE_SLAVE_ADDR | 0x01); // 0xA1
	I2C_WaitAck();
	
	while(NumByteToRead)
	{
		*pBuffer  = I2C_ReceiveByte();
		if(NumByteToRead == 1)I2C_NoAck();
		else I2C_Ack();
		pBuffer ++;
		NumByteToRead--;
		
	}
	I2C_Stop();
	return TRUE;
}

// SPI_FLASH_ReadDeviceID     
void FRAM_ReadReadDeviceID(uint8_t* pBuffer)
{             
// 	uint8_t NumByteToRead = 3;
	
	I2C_Start();
	I2C_SendByte(0xF8); // Reserved slave id
	I2C_WaitAck();

// 	I2C_SendByte(DEVIDE_SLAVE_ADDR | DEVICE_COMMAND_READ); // 0xA1
	
	I2C_SendByte(0xA1 ); // 
	I2C_WaitAck();

	I2C_Start();
	I2C_SendByte(0xF9); // Reserved slave id
	I2C_WaitAck();
	
	*pBuffer  = I2C_ReceiveByte();
	I2C_WaitAck();
	
	*pBuffer++ = I2C_ReceiveByte();
	I2C_WaitAck();
		
	*pBuffer++ = I2C_ReceiveByte();

	I2C_NoAck();
	I2C_Stop();
}

// Manufacture ID = 00AH Product ID = 510H

void CheckFram(void)
{
	uint8_t 	deviceidbuf[3] = {0};
	uint32_t	DeviceID = 0;
	FRAM_ReadReadDeviceID(deviceidbuf);
	DeviceID = ((uint32_t)deviceidbuf[0] << 16) + ((uint32_t)deviceidbuf[1] <<  8) + (uint32_t)deviceidbuf[2];


	#ifdef DEBUG
	printf("deviceidbuf: 0x%2x, 0x%2x, 0x%2x",deviceidbuf[0],deviceidbuf[1],deviceidbuf[2]);
	

	#endif
}


void FRAM_Read_DeviceID(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{  
    /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
   
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, 0xF8, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(I2C1, (uint8_t)((ReadAddr & 0xFF00) >> 8));    
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(I2C1, (uint8_t)(ReadAddr & 0x00FF));    
  

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




