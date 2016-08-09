#ifndef __USER_SD_CARD_H
#define __USER_SD_CARD_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdarg.h>
#include <stdlib.h>
	 
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "main.h"
#include "init.h"
#include "serial.h"
#include "FM25CL64.h"
#include "spi_flash.h"
#include "allcommon.h"
#include "led.h"
#include "outer.h"
#include "gps.h"
#include "gsm.h"
#include "handset.h"
#include "gsm_lib.h"
#include "captureIO.h"
#include "nmeap.h"
#include "gps_lib.h"
#include "outset.h"
#include "rtc.h"
#include "camera.h"
#include "watchdog.h"
//#include "jjq.h"
#include "tiaoping.h"
//#include "vtdr.h"
//#include "Pulse_count.h"
#include "adc.h"
//#include "can.h"
#include "luyin.h"
#include "lamp.h"

#include "vtdr.h"
#include "outside_lib.h"
#include "jjq.h"
#include "mzk_center_lib.h"
#include "handset_lib.h"
#include "update.h"
#include "pjqTask.h"
#include "touch.h"
#include "jjq_lib.h"
#include "workoff.h"
#include "MZK_acommon.h"
#include "son_telphone.h"
#include "mzk_on.h"
#include "luyin_command.h"
#include "m66_tts.h"
#include "stm32_spi_sd.h"
#include "luyin_flash.h"
#include "call_back.h"
#include "export_photo.h"
#include "allStore.h"

#include "user_uart.h"
#include "stm32_spi_sd.h"

#define DATA_LEN  (2000)


extern bool sd_card_detect(void);
extern bool acc_status;



extern uint8_t sd_write_buf[DATA_LEN];
extern uint8_t sd_read_buf[DATA_LEN];


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
	volatile bool rw_enable; // read write enable
	volatile bool state; // ��λ���

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

extern void SPI_SD_CARD_HIGH(void);
extern void SPI_SD_CARD_LOW(void);

extern SD_CARD_INFOR sd_card;
extern SD_Error sd_card_init(void);
extern uint8_t SD_GetCID(uint8_t *cid_data);                     //��SD��CID
extern uint8_t SD_GetCSD(uint8_t *csd_data);                     //��SD��CSD
extern uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc);


extern void sd_test(void);

#ifdef __cplusplus
}
#endif	 	 
	 
#endif	 


