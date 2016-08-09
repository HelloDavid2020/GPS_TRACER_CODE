#ifndef __SPI_EEPROM_H_20120521
#define __SPI_EEPROM_H_20120521

#include "includes.h"     // DSP2803x Headerfile Include File


//-------------------------------------------------------------------------------- 
// Command Definition  
//-------------------------------------------------------------------------------- 
 
#define AT25_WREN_CMD           0x06        /* Set Write Enable Latch */ 
#define AT25_WRDI_CMD           0x04        /* Reset Write Enable Latch */ 
#define AT25_RDSR_CMD           0x05        /* Read Status Register */ 
#define AT25_WRSR_CMD           0x01        /* Write Status Register */ 
#define AT25_READ_CMD           0x03        /* Read Data from Memory Array */ 
#define AT25_WRITE_CMD          0x02        /* Write Data to Memory Array */ 
#define AT25_STATUS         		0x00        /* Status Register */ 
 
//-------------------------------------------------------------------------------- 
// Size Definition 
//-------------------------------------------------------------------------------- 
#define AT25256_SIZE	32768   // 32,768x8 byte 
//#define PAGE_LEN     	64      //  64-byte
//#define PAGE_SIZE    	512     


#define AT25_STATUS_MASK_READY      0x01        /* Status Register ready/busy mask */ 
#define AT25_BUSY                   0x1         /* RDY bit of SR = 0x1 */ 
#define AT25_READY                  0x0         /* RDY bit of SR = 0x0 */ 
 
#define AT25_STATUS_MASK_WREN       0x02        /* Status Register WREN mask */ 
#define AT25_WREN                   0x2         /* WEN of SR = 0x1 */ 



#endif

