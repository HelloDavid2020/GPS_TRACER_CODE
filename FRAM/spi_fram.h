#ifndef __SPI_FRAM_H_
#define __SPI_FRAM_H_
#include "includes.h"


#define FM25CL64_WREN_INST   0x06 	// Set Write Enable Latch
#define FM25CL64_WRDI_INST   0x04 	// write disable
#define FM25CL64_RDSR_INST   0x05   // Read Status Register
#define FM25CL64_WRSR_INST   0x01   // Write Status Register
#define FM25CL64_WRITE_INST  0x02   // Write Memory Data
#define FM25CL64_READ_INST   0x03   // Read Memory Data
#define FM25CL64_STATUS_REG  0x00   //??????? ??
#define FM25CL64_INIT_INST   0x09   //


extern uint8_t SPI_FRAM_SendByte(uint8_t byte);

extern uint8_t SPI_FRAM_ByteRead(uint8_t ReadData, uint16_t ReadAddr);
extern uint8_t SPI_FRAM_ByteWrite(uint8_t WriteData, uint16_t WriteAddr);

extern unsigned char at25_read_status(void);
extern unsigned char at25_wait_ready ( void );

extern unsigned char at25_WriteEnable ( void ); 
extern void at25_write_status_reg ( uint16_t status_reg_value ); 

extern void at25_write_byte ( uint16_t address, unsigned char byte );   
extern void at25_write_page ( uint16_t address, uint16_t nb_bytes, uint8_t *buffer ); 
   
extern void at25_read_byte ( uint16_t address, uint8_t  *read_data ); 
extern void at25_read_page ( uint16_t address, uint16_t nb_bytes, uint8_t *read_data ); 





#endif


