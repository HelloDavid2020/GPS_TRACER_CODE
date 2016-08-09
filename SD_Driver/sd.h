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


// SD�����Ͷ���  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD��ָ���  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8       //����8 ��SEND_IF_COND
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define CMD41   41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
#define	SPI3_CS  PBout(6) //SD��Ƭѡ����					    	  
				    	  
#define	SD_CS_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define	SD_CS_LOW()   GPIO_ResetBits(GPIOB, GPIO_Pin_6)
extern uint8_t  SD_Type;//SD��������
//���������� 

//typedef enum 
//{
//	NOT_PRESENT=0,
//	INSERTTED,
//} SD_CARD_STATE;

union SD_WARN_STATE
{
	uint8_t value; // ����ֵ
	
	struct 
	{
		uint8_t 	not_present:1;    //  �޿�����
		uint8_t 	err_init	 :1;    //  ��ʼ��ʧ��
		uint8_t 	err_mount  :1;		//  ����SD��ʧ��
		uint8_t 	err_format	 :1;  //  ��ʽ��ʧ��		
		uint8_t 	low_disk_space:1; //  ���̿ռ䲻��
		uint8_t 	err_read_write:1; //  �ļ���дʧ��
		
		uint8_t  rsd : 3;
	}Bit;	

};	


typedef struct
{
	volatile char rw_enable; // read write enable
	
	SD_PRESENT_STATE state; 
	uint8_t fatfs_init_flag; // sd&fatfs ��ʼ����־ 
	uint8_t spi_init_flag;
	
  union SD_WARN_STATE Warn;
	uint8_t type;    
	uint32_t totalSize;   
	uint32_t freeSize;
uint8_t err_count;
uint8_t err_total_count; // ��ʼ��ʧ��
	uint8_t err_rw_count;   // SD ��д����
	uint8_t err_init_count; // ��ʼ��ʧ�ܴ������
	uint8_t err_init_total_count; // ��ʼ��ʧ��
	
	uint8_t retry_times; // ��ʼ�����Դ���
	uint8_t sd_fs_ok; // sd �� �޷��޸�
//	corrupt SD card

}SD_CARD_INFOR;

extern SD_CARD_INFOR sd_card;

extern SD_PRESENT_STATE SD_Detect(void);

extern uint8_t SD_SPI_ReadWriteByte(uint8_t data);
extern void SD_SPI_SpeedLow(void);
extern void SD_SPI_SpeedHigh(void);
extern uint8_t SD_WaitReady(void);							//�ȴ�SD��׼��
extern uint8_t SD_GetResponse(uint8_t Response);					//�����Ӧ
extern uint8_t sd_spi_init(void);							//��ʼ��
extern uint8_t SD_ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//����
extern uint8_t SD_WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//д��
extern uint32_t SD_GetSectorCount(void);   					//��������
extern uint8_t SD_GetCID(uint8_t *cid_data);                     //��SD��CID
extern uint8_t SD_GetCSD(uint8_t *csd_data);                     //��SD��CSD

#endif
