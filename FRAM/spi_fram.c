///******************** (C) COPYRIGHT ********************
// * 文件名  ：spi_flash.c

// *          ----------------------------
// *         | PA4-SPI1-NSS  : W25X32-CS  |
// *         | PA5-SPI1-SCK  : W25X32-CLK |
// *         | PA6-SPI1-MISO : W25X32-DO  |
// *         | PA7-SPI1-MOSI : W25X32-DIO |
// *          ----------------------------

//	Manufacturer and Device Identification
//-------------------------------------------------------
//MANUFACTURER ID 					|						(M7-M0)
//--------------------------+----------------------------
//Winbond Serial Flash 			|							EFH
//--------------------------+----------------------------

//														 Device ID		|			Memory Type + Memory Capacity 
//Device ID 								|		(ID7-ID0) 	|							(ID15-ID0)
//--------------------------------------------------------------
//Instruction 							|		ABh, 90h 		|					9Fh
//	W25X10 									|		10h 				|					3011h
//	W25X20 									|		11h 				|					3012h
//	W25X40 									|		12h 				|					3013h
//	W25X80 									|		13h 				|					3014h
//	W25X32 									|		15h 				|					3016h

//Device ID 		(ID7-ID0) 
// The JEDEC assigned Manufacturer ID byte for Winbond (EFh) and two Device ID bytes, 
// Memory Type(ID15-ID8) and Capacity (ID7-ID0) are then shifted out on the falling 
// edge of CLK with most significant bit (MSB) first.
// 
//**********************************************************************************/
#include "spi_fram.h"

unsigned char at25_read_status(void)
{
	unsigned char status =0;
	SPI1_CS_LOW(); 	   						     
	SPI_SendByte(SPI1,AT25_RDSR_CMD);     
	status = SPI_ReadByte(SPI1);         
	SPI1_CS_HIGH();               			 
	return status;
}

//*--------------------------------------------------------------------------------------    
//* Function Name       : at25_wait_ready    
//* Object              : Detect if the eeprom is ready or busy    
//* Input Parameters    : None    
//* Return value        : AT25_READY (0) if ready, else AT25_BUSY (1)    
//*--------------------------------------------------------------------------------------    
unsigned char at25_wait_ready ( void )   
{   
//* Begin    
    unsigned int time_out = 5000 ;       
    unsigned char status_reg = AT25_BUSY ;   
   
    while ( status_reg == AT25_BUSY )   
    {   
        status_reg = ( at25_read_status () & AT25_STATUS_MASK_READY ) ;   
        /* If timeout */   
        if (time_out == 0)   
        return ( AT25_BUSY );   
        time_out-- ;   
    }   
   
    return ( AT25_READY ) ;   
   
//* End    
}

//*-------------------------------------------------------------------------------------- 
//* Function Name       : at25_open 
//* Object              : Open the SPI EEPROM connection and Enable writing 
//* Input Parameters    : 
//* Return value        : TRUE if Status Register Reads OK 
//*-------------------------------------------------------------------------------------- 
unsigned char at25_WriteEnable ( void )   
{   
//* Begin    

// WRITE ENABLE (WREN): The device will power-up in the write disable state when VCC
// is applied. All programming instructions must therefore be preceded by a Write Enable instruction.		 
	SPI1_CS_LOW(); 
	SPI_SendByte(SPI1, AT25_WREN_CMD);
	SPI1_CS_HIGH();  	
	at25_wait_ready () ;   
   
	if ( ( at25_read_status () & AT25_STATUS_MASK_WREN ) != AT25_WREN )   
	  return ( FALSE );   

	return ( TRUE ) ;   
//* End    
}

//*--------------------------------------------------------------------------------------    
//* Function Name       : at25_write_status_reg    
//* Object              : Write a data to the Status Register    
//* Input Parameters    : <status_reg_value> : Value to write to the status register    
//* Return value        : None    
//*--------------------------------------------------------------------------------------    
void at25_write_status_reg ( uint16_t status_reg_value )   
{   
//* Begin    	
	SPI1_CS_LOW();       
	SPI_SendByte(SPI1, AT25_WRSR_CMD );
	SPI_SendByte(SPI1, status_reg_value );
	

	SPI1_CS_HIGH(); 
	at25_wait_ready () ;   
//* End    
}   

//*--------------------------------------------------------------------------------------    
//* Function Name       : at25_write_byte    
//* Object              : Write a byte in memory    
//* Input Parameters    : <address> : Address where to write    
//*                     : <byte>    : Data to write    
//* Return value        : None    
//*--------------------------------------------------------------------------------------    
void at25_write_byte ( uint16_t address, unsigned char byte )   
{   
//* Begin    
	SPI1_CS_LOW(); 

	SPI_SendByte(SPI1, AT25_WRITE_CMD);
	
	/* Send ReadAddr high nibble address byte to read from */
	SPI_SendByte(SPI1, (address& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI_SendByte(SPI1, address & 0x00FF);

	/* Send "Write Enable" instruction */
	SPI_SendByte(SPI1, byte);
 
	SPI1_CS_HIGH(); 
	at25_wait_ready () ;   
   
//* End    
}  

//*--------------------------------------------------------------------------------------    
//* Function Name       : at25_write_page    
//* Object              : Write a 64-byte (maximum) page in memory    
//* Input Parameters    : <address> : First Address to write    
//*                     : <nb_bytes>: Number of bytes to write from <address>    
//*                     : <buffer>  : Pointer to a Buffer of data to write    
//* Return value        : None    
//*--------------------------------------------------------------------------------------    
void at25_write_page ( uint16_t address, uint16_t nb_bytes, uint8_t *buffer )   
{   
//* Begin    
	uint16_t i = 0;
//	uint16_t  write_cmd[3] = { AT25_WRITE_CMD, 0x0, 0x0} ;   

//	//* decompose addr into 2 bytes    
//	write_cmd[1] = ((address & 0x0FF00) >> 8) ;   
//	write_cmd[2] = address & 0x0FF; 

	at25_WriteEnable();
	  
	SPI1_CS_LOW();       

	SPI_SendByte(SPI1, AT25_WRITE_CMD);

	/* Send ReadAddr high nibble address byte to read from */
	SPI_SendByte(SPI1, (address& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI_SendByte(SPI1, address & 0x00FF);


	for( i=0; i<nb_bytes; i++)
	{
		SPI_SendByte(SPI1, * buffer++);
	}


	SPI1_CS_HIGH();
   
	at25_wait_ready () ;   
   
//* End    
}

//*--------------------------------------------------------------------------------------    
//* Function Name       : at25_read_byte    
//* Object              : Read a byte in memory    
//* Input Parameters    : <address> : Address to read    
//*                     : <*read_data> : Pointer to store the Data which has been read    
//*    
//* Return value        : None    
//*--------------------------------------------------------------------------------------    
void at25_read_byte ( uint16_t address, uint8_t  *read_data )   
{   
//* Begin    
   
	uint16_t  read_cmd[4] = { AT25_READ_CMD, 0x0, 0x0, 0x0} ;     

	//* decompose addr into 2 bytes    
	read_cmd[1] = ((address & 0x0FF00) >> 8) ;   
	read_cmd[2] = address & 0x0FF;   

	SPI1_CS_LOW();        

	SPI_SendByte(SPI1, AT25_READ_CMD);

	/* Send ReadAddr high nibble address byte to read from */
	SPI_SendByte(SPI1, (address& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI_SendByte(SPI1, address & 0x00FF);

	*read_data = SPI_ReadByte(SPI1);

	SPI1_CS_HIGH(); 
   
	at25_wait_ready () ;  
//* End    
}   

   
//*--------------------------------------------------------------------------------------    
//* Function Name       : at25_read_page    
//* Object              : Read a 64-byte (maximum) page in memory    
//* Input Parameters    : <address> : First Address to read    
//*                     : <nb_bytes>: Number of bytes to read from <address>    
//*                     : <buffer>  : Pointer to a Buffer of data to read    
//*                     : <nb_bytes> : Number of bytes to read    
//*    
//* Return value        : Pointer to data    
//*--------------------------------------------------------------------------------------    
void at25_read_page ( uint16_t address, uint16_t nb_bytes, uint8_t *read_data )   
{   
//* Begin    
   
	uint16_t  read_cmd[3] = { AT25_READ_CMD, 0x0, 0x0} ;   

	//* decompose addr into 2 bytes    
	read_cmd[1] = ((address & 0x0FF00) >> 8) ;   
	read_cmd[2] = address & 0x0FF;   

	SPI1_CS_LOW();        

	SPI_SendByte(SPI1, AT25_READ_CMD);

	/* Send ReadAddr high nibble address byte to read from */
	SPI_SendByte(SPI1, (address& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI_SendByte(SPI1, address & 0x00FF);


	while(nb_bytes --)
		*read_data++ = SPI_ReadByte(SPI1);	   
	
	SPI1_CS_HIGH(); 

	at25_wait_ready () ;   
   
   
//* End    
}





/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FRAM_WriteEnable(void)
{
	/* Select the FLASH: Chip Select low */
	SPI1_CS_LOW();

	/* Send "Write Enable" instruction */
	SPI_SendByte(SPI1, FM25CL64_WREN_INST);

	/* Deselect the FLASH: Chip Select high */
	SPI1_CS_HIGH();
}


/*******************************************************************************
* Function Name  : SPI_FRAM_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FRAM_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;

	/* Select the FLASH: Chip Select low */
	SPI1_CS_LOW();

	/* Send "Read Status Register" instruction */
	SPI_SendByte(SPI1, FM25CL64_RDSR_INST);

	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		FLASH_Status = SPI_SendByte(SPI1, Dummy_Byte);

	}
	while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

	/* Deselect the FLASH: Chip Select high */
	SPI1_CS_HIGH();
}


/*******************************************************************************
* Function Name  : SPI_FRAM_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/

uint8_t SPI_FRAM_SendByte(uint8_t byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, byte);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}


// 写入单字节
uint8_t SPI_FRAM_ByteWrite(uint8_t WriteData, uint16_t WriteAddr)
{	
	SPI1_CS_LOW();

	/* Send "Write Enable" instruction */
	SPI_SendByte(SPI1, FM25CL64_WREN_INST);
	
	/* Select the FRAM: Chip Select low */
	SPI_SendByte(SPI1, FM25CL64_WRITE_INST);

	/* Send ReadAddr high nibble address byte to read from */
	SPI_SendByte(SPI1, (WriteAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI_SendByte(SPI1, WriteAddr & 0x00FF);

	/* Send "Write Enable" instruction */
	SPI_SendByte(SPI1, WriteData);

	/* Select the FRAM: Chip Select high */
	SPI1_CS_HIGH();

	return OK;
}


// 读取单字节
uint8_t SPI_FRAM_ByteRead(uint8_t ReadData, uint16_t ReadAddr)
{
	/* Select the FRAM: Chip Select low */
	SPI1_CS_LOW();
	SPI_SendByte(SPI1, FM25CL64_READ_INST);

	/* Send ReadAddr high nibble address byte to read from */
	SPI_SendByte(SPI1, (ReadAddr & 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI_SendByte(SPI1,  ReadAddr & 0x00FF);


	ReadData = SPI_SendByte(SPI1, 0x00);

	/* Select the FRAM: Chip Select high */
	SPI1_CS_HIGH();

	return ReadData;
}






