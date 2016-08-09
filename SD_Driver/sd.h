#ifndef __SD_H
#define __SD_H
 				
#include "stm32f10x.h"
#include "user_uart.h"


/**
  * @brief  SD detection on its memory slot
  */
//#define SD_PRESENT        ((uint8_t)0x01)
//#define SD_NOT_PRESENT    ((uint8_t)0x00)

typedef enum
{
	SD_PRESENT = 0,	
	SD_NOT_PRESENT,
}SD_PRESENT_STATE;



typedef enum
{
/**
  * @brief  SD reponses and error flags
  */
  SD_RESPONSE_NO_ERROR      = (0x00),
  SD_IN_IDLE_STATE          = (0x01),
  SD_ERASE_RESET            = (0x02),
  SD_ILLEGAL_COMMAND        = (0x04),
  SD_COM_CRC_ERROR          = (0x08),
  SD_ERASE_SEQUENCE_ERROR   = (0x10),
  SD_ADDRESS_ERROR          = (0x20),
  SD_PARAMETER_ERROR        = (0x40),
  SD_RESPONSE_FAILURE       = (0xFF),

/**
  * @brief  Data response error
  */
  SD_DATA_OK                = (0x05),
  SD_DATA_CRC_ERROR         = (0x0B),
  SD_DATA_WRITE_ERROR       = (0x0D),
  SD_DATA_OTHER_ERROR       = (0xFF)
} SD_Error;


// SD卡类型定义  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
#define	SPI3_CS  PBout(6) //SD卡片选引脚					    	  
				    	  
#define	SD_CS_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define	SD_CS_LOW()   GPIO_ResetBits(GPIOB, GPIO_Pin_6)
extern uint8_t  SD_Type;//SD卡的类型
//函数申明区 

//typedef enum 
//{
//	NOT_PRESENT=0,
//	INSERTTED,
//} SD_CARD_STATE;

union SD_WARN_STATE
{
	uint8_t value; // 报警值
	
	struct 
	{
		uint8_t 	not_present:1;    //  无卡报警
		uint8_t 	err_init	 :1;    //  初始化失败
		uint8_t 	err_mount  :1;		//  挂载SD卡失败
		uint8_t 	err_format	 :1;  //  格式化失败		
		uint8_t 	low_disk_space:1; //  磁盘空间不足
		uint8_t 	err_read_write:1; //  文件读写失败
		
		uint8_t  rsd : 3;
	}Bit;	

};	


typedef struct
{
	volatile char rw_enable; // read write enable
	
	SD_PRESENT_STATE state; 
	uint8_t fatfs_init_flag; // sd&fatfs 初始化标志 
	uint8_t spi_init_flag;
	
  union SD_WARN_STATE Warn;
	uint8_t type;    
	uint32_t totalSize;   
	uint32_t freeSize;
uint8_t err_count;
uint8_t err_total_count; // 初始化失败
	uint8_t err_rw_count;   // SD 读写错误
	uint8_t err_init_count; // 初始化失败错误计数
	uint8_t err_init_total_count; // 初始化失败
	
	uint8_t retry_times; // 初始化尝试次数
	uint8_t sd_fs_ok; // sd 损坏 无法修复
//	corrupt SD card

}SD_CARD_INFOR;

extern SD_CARD_INFOR sd_card;

extern SD_PRESENT_STATE SD_Detect(void);

extern uint8_t SD_SPI_ReadWriteByte(uint8_t data);
extern void SD_SPI_SpeedLow(void);
extern void SD_SPI_SpeedHigh(void);
extern uint8_t SD_WaitReady(void);							//等待SD卡准备
extern uint8_t SD_GetResponse(uint8_t Response);					//获得相应
extern uint8_t sd_spi_init(void);							//初始化
extern uint8_t SD_ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//读块
extern uint8_t SD_WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//写块
extern uint32_t SD_GetSectorCount(void);   					//读扇区数
extern uint8_t SD_GetCID(uint8_t *cid_data);                     //读SD卡CID
extern uint8_t SD_GetCSD(uint8_t *csd_data);                     //读SD卡CSD

#endif
