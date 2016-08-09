#include "file.h"
#include  "user_spi.h"

//fgets (buf, size, fp)
//其行为方式如下：
//（1）遇到换行或文件结束EOF则返回。
//（2）按行读取。
//（3）每一行结束处的换行字符‘\n’也算该行字符。
//（4）对于大小为size的buf，最多只读取 size-1 个字符。
//（5）自动地把buf中最后一个字符（通常是换行符）的后面一个字节填充零结束符（'\0'）。



FRESULT scan_files (char* path);

DIR dirs_data, dirs_blind;
//FATFS fs1;
FRESULT fr;          /* FatFs function common result code */

FRESULT res;
FIL file1,file_blind;		
uint8_t result =0;
FRESULT res_debug;

FILINFO fileinfor;	//文件信息
DIR dirs;  			//目录	

char flag_delete_blind_bin = DISABLE;
char flag_show_files =DISABLE;
char flag_format_sd =DISABLE;
char file_name[20] = {0};

//UINT br, bw;         /* File read/write count */			

/* 
 0. 卡在位检测
 1. spi 初始化
 2. fat 初始化

*/



void sd_file_init(void)
{
	sd_card.sd_fs_ok = OK; // 默认SD正常，未损坏
	sd_card.rw_enable = ENABLE;		// 允许读写操作
	sd_card.state = SD_Detect();		 /*!< Check GPIO to detect SD */


	sd_card.spi_init_flag = sd_spi_init();

	fatfs_int();
	
//	if(	sd_card.fatfs_init_flag == PASSED)
//	{
//		CreatCanDataFile("Data");
//		fr = f_mkdir("Blind");
//		if(fr == FR_OK || fr == FR_EXIST)
//			printf("> 创建文件夹 Blind 成功\r\n");
//		else
//			printf("> 创建文件夹 Blind 失败\r\n");
//		
//		show_sd_files();
//	}

}



void fatfs_int(void)
{
	uint8_t timeout =2;
	char buf[120];
	
	sd_card.fatfs_init_flag = FAILED;

	if (sd_card.spi_init_flag == FAILED) return;
	
	printf("\r\n> load fatfs.\r\n\r\n");
	exfuns_init();		//为fatfs相关变量申请内存
		
	printf("mount sd card ...\r\n");
	fr = f_mount(0,fs[0]); 					 	//挂载SD卡 
	if(fr == FR_OK)
	{
		printf("  -- Register work area for the drive 0  \r\n");
		sd_card.Warn.Bit.err_mount = PASSED;
		
		while(exf_getfree("0",&sd_card.totalSize,&sd_card.freeSize))	//得到SD卡的总容量和剩余容量
		{
			delay_ms(1000);
			printf("check sd\r\n");
			delay_ms(1000);
			printf("check sd\r\n");
			delay_ms(1000);
			printf("check sd\r\n");

			timeout--;
			if(timeout == 0) 	break;
		}
		printf("  -- Total Size: %dMB\r\n",sd_card.totalSize>>10);	//显示SD卡总容量 MB
		printf("  -- Free  Size: %dMB\r\n",sd_card.freeSize>>10);		//显示SD卡剩余容量 MB
	}
	else
	{
		printf("\r\nFAILED!\r\n");
		sd_card.Warn.Bit.err_mount = FAILED;
		return;
	}	

}

//遍历文件
 //path:路径
 //返回值:执行结果
uint8_t scan_sd_files(uint8_t * path)
{
	FRESULT res;
	uint8_t num = 0;
  char *fn;   /* This function is assuming non-Unicode cfg. */ 
	
  res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录			

  if (res == FR_OK) 
	{		
		printf("+ %s\r\n", path);//打印路径	

		while(1)
		{
			res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
			if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
			if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
			fn = fileinfo.fname;
			printf("|---- %s\r\n",  fn);//打印文件名
			
			// 最多打印20个文件
			num ++;
			if(num > 20)	
			break;	
			
		} 
   }	  
    return res;	  
}

FRESULT show_sd_files(void)
{
	FRESULT res;

	printf("\r\n\r\nSD卡 文件夹目录\r\n");					
	scan_sd_files("0:");
	printf("\r\n");					
	scan_sd_files("0://BOOT");
	res = scan_sd_files("0://DATA");
		
	if(res==FR_OK)
		printf("\r\nSD卡 正常\r\n");
	else
	{
		printf("\r\nSD卡 错误\r\n");
		sd_card.err_rw_count++;
	}
		return res;
}

uint8_t sd_format(FATFS *fs)
{
	fr = f_mount(0,fs); //挂载SD卡 
	if(fr == FR_OK)
		printf("mount sd card sucessed!\r\n");
	else
		printf("mount sd card failed!...\r\n");

	 printf("formating...\r\n");
	fr =f_mkfs(0, 0, 0); 
	if(fr == FR_OK)
		printf("format sucessed!\r\n");
	else
	{
		printf("format failed!\r\n");
		return FAILED;
	}	
	return PASSED;
}


//void file_processing_handler(void)
//{
//	if(sd_card.rw_enable == DISABLE)
//		return;
//	
//	sd_card.state = SD_Detect(); // 忽略SD卡在位检测

//	sd_rw_handler();	// sd 读写操作
//	sd_recovery(9);
//}

//void sd_rw_handler(void)
//{
//	// spi初始化成功 + 容量足够（200M）+ 外壳闭合
//	if(sd_card.spi_init_flag == PASSED && sd_card.Warn.Bit.low_disk_space == 0 && GpsDevice.ShellState==0 )
//	{			
//		save_can_data();
//		save_blind_data();	
//		read_blind_data();
//		delete_blind_data();
//		if(flag_show_files == ENABLE)
//		{
//			flag_show_files = DISABLE;
//			show_sd_files();
//		}
//		
//		if(flag_format_sd == ENABLE)
//		{
//			flag_format_sd = DISABLE;
//			if(sd_format(fs[0]) == PASSED)
//				sd_file_init();
//			delay_ms(1000);
//		}	
//		
//		// 读写操作失败处理
//		if(sd_card.err_rw_count > 10)  // 重新插拔SD卡后 等待10秒重新 SD初始化
//		{	
//			sd_card.err_rw_count =0;
//			sd_card.spi_init_flag = FAILED;
//		}
//	}


//}

//void sd_recovery(uint8_t retry_times)
//{
//// 1、第一次初始化失败
//// 2、读写过程产生的错误
//	if(sd_card.spi_init_flag == FAILED && sd_card.sd_fs_ok == OK)
//	{
//		sd_card.retry_times = 3;
//		sd_card.err_init_count = 0;
//		while(sd_card.retry_times--)
//		{
//			sd_file_init();
//			if(sd_card.spi_init_flag == PASSED)
//				break;
//			if(sd_card.spi_init_flag == FAILED)
//				sd_card.err_init_count++;
//			
//			sd_card.err_init_total_count++;
//			os_dly_wait(100);
//		}
//		if(sd_card.err_init_count>=3)
//		{
//			sd_card.err_init_count=0;
//			sd_format(fs[0]);
//			delay_ms(1000);		
//			sd_file_init();
//		}
//		
//		if(sd_card.err_init_total_count > retry_times)
//		{
//			sd_card.sd_fs_ok =  ERROR;// sd 损坏
//		}
//			
//	}


//}

//u32 SD_GetSectorCount(void)
//{
//	u8 csd[16];
//	u32 Capacity;  
//	u8 n;
//	u16 csize;      
//	//取CSD信息，如果期间出错，返回0
//	if(SD_GetCSD(csd)!=0) return 0;    
//	//如果为SDHC卡，按照下面方式计算
//	if((csd[0]&0xC0)==0x40) //V2.00的卡
//	{
//	csize = csd[9] + ((u16)csd[8] << 8) + 1;
//	Capacity = (u32)csize << 10;//得到扇区数   
//	}else//V1.XX的卡
//	{
//	n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
//	csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
//	Capacity= (u32)csize << (n - 9);//得到扇区数   
//	}
//	return Capacity;
//}

