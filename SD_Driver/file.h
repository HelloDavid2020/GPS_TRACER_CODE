#ifndef __FILE_H 
#define __FILE_H 


#include "stm32f10x.h"
#include "stdio.h"
#include "user_timedelay.h"
//#include "usart.h"
//#include "led.h"
#include "sd.h"
#include "sys_io.h"
#include "exfuns.h"
#include "malloc.h"


#define FALSE 0

extern FRESULT scan_files (char* path);

#define  F_PUTS 		1		
#define  F_READ     1  
#define  F_UNLINK   0    
#define  SCAN_FILES 	1 

extern FILINFO fileinfor;	//文件信息
extern DIR dirs;  			//目录	
//extern FATFS fs1;
extern FRESULT res;
extern FIL file1,file_blind;		
extern uint8_t result;

extern DIR dirs_data, dirs_blind;

extern FRESULT fr;          /* FatFs function common result code */

extern char flag_show_files;
extern char flag_format_sd;

extern char flag_delete_blind_bin;
extern char file_name[20];
extern void fatfs_int(void);
extern uint8_t scan_sd_files(uint8_t * path);
extern FRESULT show_sd_files(void);
extern void SaveBootLog(void);
extern void sd_file_init(void);
extern uint8_t sd_format(FATFS *fs);

extern void sd_rw_handler(void);
extern void file_processing_handler(void);
extern void sd_recovery(uint8_t retry_times);

#endif
