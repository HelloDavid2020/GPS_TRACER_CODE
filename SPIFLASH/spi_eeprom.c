//#include "spi_eeprom.h"


//unsigned char at25_read_status(void)
//{
//	unsigned char status =0;
//	SPI2_CS_LOW(); 	   						     
//	SPI_SendByte(SPI2,AT25_RDSR_CMD);     
//	status = SPI_ReadByte(SPI2);         
//	SPI2_CS_HIGH();               			 
//	return status;
//}
////*--------------------------------------------------------------------------------------    
////* Function Name       : at25_wait_ready    
////* Object              : Detect if the eeprom is ready or busy    
////* Input Parameters    : None    
////* Return value        : AT25_READY (0) if ready, else AT25_BUSY (1)    
////*--------------------------------------------------------------------------------------    
//unsigned char at25_wait_ready ( void )   
//{   
////* Begin    
//    unsigned char time_out = 5000 ;       
//    unsigned char status_reg = AT25_BUSY ;   
//   
//    while ( status_reg == AT25_BUSY )   
//    {   
//        status_reg = ( at25_read_status () & AT25_STATUS_MASK_READY ) ;   
//        /* If timeout */   
//        if (time_out == 0)   
//        return ( AT25_BUSY );   
//        time_out-- ;   
//    }   
//   
//    return ( AT25_READY ) ;   
//   
////* End    
//}

////*-------------------------------------------------------------------------------------- 
////* Function Name       : at25_open 
////* Object              : Open the SPI EEPROM connection and Enable writing 
////* Input Parameters    : 
////* Return value        : TRUE if Status Register Reads OK 
////*-------------------------------------------------------------------------------------- 
//unsigned char at25_WriteEnable ( void )   
//{   
////* Begin    

//// WRITE ENABLE (WREN): The device will power-up in the write disable state when VCC
//// is applied. All programming instructions must therefore be preceded by a Write Enable instruction.		 
//	SPI2_CS_LOW();
//	SPI_SendByte(SPI2,AT25_WREN_CMD);
//	SPI2_CS_HIGH();	
//	at25_wait_ready () ;   
//   
//	if ( ( at25_read_status () & AT25_STATUS_MASK_WREN ) != AT25_WREN )   
//	  return ( FALSE );   

//	return ( TRUE ) ;   
////* End    
//}

////*--------------------------------------------------------------------------------------    
////* Function Name       : at25_write_status_reg    
////* Object              : Write a data to the Status Register    
////* Input Parameters    : <status_reg_value> : Value to write to the status register    
////* Return value        : None    
////*--------------------------------------------------------------------------------------    
//void at25_write_status_reg ( uint16_t status_reg_value )   
//{   
////* Begin    
//	uint16_t   write_status_cmd[2] = { AT25_WRSR_CMD, 0x0 } ;   
//	write_status_cmd[1] = status_reg_value ;   

//	SPI2_CS_LOW();        
//	SendPacket_Spib(write_status_cmd,2);
//	SPI2_CS_HIGH(); 
//	at25_wait_ready () ;   
////* End    
//}   

////*--------------------------------------------------------------------------------------    
////* Function Name       : at25_write_byte    
////* Object              : Write a byte in memory    
////* Input Parameters    : <address> : Address where to write    
////*                     : <byte>    : Data to write    
////* Return value        : None    
////*--------------------------------------------------------------------------------------    
//void at25_write_byte ( uint16_t address, unsigned char byte )   
//{   
////* Begin    

//	uint16_t  write_cmd[4] = { AT25_WRITE_CMD, 0x0, 0x0, 0x0 } ;   

//	//* Decompose <address> into 2 bytes    
//	write_cmd[1] = ((address & 0x0FF00) >> 8) ;   
//	write_cmd[2] = address & 0x0FF;   
//	write_cmd[3] = byte ;   
//	 
//	SPI2_CS_LOW();        
//	SendPacket_Spib(write_cmd,4);
//	SPI2_CS_HIGH(); 
//	      
//	at25_wait_ready () ;   
//   
////* End    
//}  

////*--------------------------------------------------------------------------------------    
////* Function Name       : at25_write_page    
////* Object              : Write a 64-byte (maximum) page in memory    
////* Input Parameters    : <address> : First Address to write    
////*                     : <nb_bytes>: Number of bytes to write from <address>    
////*                     : <buffer>  : Pointer to a Buffer of data to write    
////* Return value        : None    
////*--------------------------------------------------------------------------------------    
//void at25_write_page ( uint16_t address, uint16_t nb_bytes, uint16_t *buffer )   
//{   
////* Begin    
//	uint16_t  write_cmd[3] = { AT25_WRITE_CMD, 0x0, 0x0} ;   

//	//* decompose addr into 2 bytes    
//	write_cmd[1] = ((address & 0x0FF00) >> 8) ;   
//	write_cmd[2] = address & 0x0FF; 

//	at25_WriteEnable();
//	  
//	SPI2_CS_LOW();        
//	SendPacket_Spib(write_cmd,3);
//	SendPacket_Spib((uint16_t *)buffer,nb_bytes);
//	SPI2_CS_HIGH(); 
//   
//	at25_wait_ready () ;   
//   
////* End    
//}

////*--------------------------------------------------------------------------------------    
////* Function Name       : at25_read_byte    
////* Object              : Read a byte in memory    
////* Input Parameters    : <address> : Address to read    
////*                     : <*read_data> : Pointer to store the Data which has been read    
////*    
////* Return value        : None    
////*--------------------------------------------------------------------------------------    
//void at25_read_byte ( uint16_t address, uint16_t  *read_data )   
//{   
////* Begin    
//   
//	uint16_t  read_cmd[4] = { AT25_READ_CMD, 0x0, 0x0, 0x0} ;     

//	//* decompose addr into 2 bytes    
//	read_cmd[1] = ((address & 0x0FF00) >> 8) ;   
//	read_cmd[2] = address & 0x0FF;   

//	SPI2_CS_LOW();        
//	SendPacket_Spib(read_cmd,4);
//	*read_data =ReceiveChar_Spib();                                     //??????                
//	SPI2_CS_HIGH(); 
//   
//	at25_wait_ready () ;  
//     

//   
////* End    
//}   

//   
////*--------------------------------------------------------------------------------------    
////* Function Name       : at25_read_page    
////* Object              : Read a 64-byte (maximum) page in memory    
////* Input Parameters    : <address> : First Address to read    
////*                     : <nb_bytes>: Number of bytes to read from <address>    
////*                     : <buffer>  : Pointer to a Buffer of data to read    
////*                     : <nb_bytes> : Number of bytes to read    
////*    
////* Return value        : Pointer to data    
////*--------------------------------------------------------------------------------------    
//void at25_read_page ( uint16_t address, uint16_t nb_bytes, uint16_t *read_data )   
//{   
////* Begin    
//   
//	uint16_t  read_cmd[3] = { AT25_READ_CMD, 0x0, 0x0} ;   

//	//* decompose addr into 2 bytes    
//	read_cmd[1] = ((address & 0x0FF00) >> 8) ;   
//	read_cmd[2] = address & 0x0FF;   

//	SPI2_CS_LOW();        
//	SendPacket_Spib(read_cmd,3);
//	while(nb_bytes --)
//	*read_data++ =ReceiveChar_Spib();                                     //??????     
//	SPI2_CS_HIGH(); 

//	at25_wait_ready () ;   
//   
//   
////* End    
//}

//void at25_clear_eeprom(void)
//{
//	uint16_t i=0;
//	uint16_t writedata[64];
//	for(i=0;i<64;i++)
//	writedata[i]=0;

//// void at25_write_page ( uint16_t address, uint16_t nb_bytes, uint16_t *buffer )   
//	at25_write_page (0,64,writedata);
//}
