#ifndef __FM25CL64_H_
#define __FM25CL64_H_
#include "includes.h"

//#define SPI_FRAM_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
//#define SPI_FRAM_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)



//#define FM25CL64_WREN_INST   0x06 	// Set Write Enable Latch
//#define FM25CL64_WRDI_INST   0x04 	// write disable
//#define FM25CL64_RDSR_INST   0x05   // Read Status Register
//#define FM25CL64_WRSR_INST   0x01   // Write Status Register
//#define FM25CL64_WRITE_INST  0x02   // Write Memory Data
//#define FM25CL64_READ_INST   0x03   // Read Memory Data
//#define FM25CL64_STATUS_REG  0x00   //??????? ??
//#define FM25CL64_INIT_INST   0x09   //
#define FM25CL64_RDByte() 	SPI1_WriteByte(0xFF)
#define FM25CL64_WRByte(x)	SPI1_WriteByte(x)


//#define FM25CL64_RDByte() 	SPI_FRAM_SendByte(0xFF)
//#define FM25CL64_WRByte(x)	SPI_FRAM_SendByte(x)

extern uint8_t FRAM_writeBuf[1024];
extern uint8_t FRAM_readBuf[1024];

extern uint32_t PassedCount;
extern uint32_t FailedCount;
extern uint8_t read_fram_id(void);
extern uint8_t SPI1_WriteByte(uint8_t byte);
//extern void SPI2_Init(void);
extern void FM25CL64_Init(void);
extern uint8_t SPI_FRAM_SendByte(uint8_t byte);
extern uint8_t SPI_FRAM_ReadByte(void);

extern void SPI_FRAM_WriteEnable(void);
extern void SPI_FRAM_WaitForWriteEnd(void);
extern uint8_t SPI_FRAM_ByteWrite(uint8_t WriteData, uint16_t WriteAddr);
extern uint8_t SPI_FRAM_ByteRead(uint8_t ReadData, uint16_t ReadAddr);
extern int8_t SPI_FRAM_BufferWrite (uint8_t * buffer,uint32_t address,uint32_t length);
extern int8_t SPI_FRAM_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);






extern uint8_t Read_FM25CL64_nByte(uint16_t addre,uint8_t* buff,uint16_t len);
extern void Write_FM25CL64_nByte(uint16_t addre,uint8_t * buff,uint16_t len);
	
	
extern void SpiFramTest(void);		
	
#endif

