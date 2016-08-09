#include "file.h"
#include  "user_spi.h"

//fgets (buf, size, fp)
//����Ϊ��ʽ���£�
//��1���������л��ļ�����EOF�򷵻ء�
//��2�����ж�ȡ��
//��3��ÿһ�н������Ļ����ַ���\n��Ҳ������ַ���
//��4�����ڴ�СΪsize��buf�����ֻ��ȡ size-1 ���ַ���
//��5���Զ��ذ�buf�����һ���ַ���ͨ���ǻ��з����ĺ���һ���ֽ�������������'\0'����



FRESULT scan_files (char* path);

DIR dirs_data, dirs_blind;
//FATFS fs1;
FRESULT fr;          /* FatFs function common result code */

FRESULT res;
FIL file1,file_blind;		
uint8_t result =0;
FRESULT res_debug;

FILINFO fileinfor;	//�ļ���Ϣ
DIR dirs;  			//Ŀ¼	

char flag_delete_blind_bin = DISABLE;
char flag_show_files =DISABLE;
char flag_format_sd =DISABLE;
char file_name[20] = {0};

//UINT br, bw;         /* File read/write count */			

/* 
 0. ����λ���
 1. spi ��ʼ��
 2. fat ��ʼ��

*/



void sd_file_init(void)
{
	sd_card.sd_fs_ok = OK; // Ĭ��SD������δ��
	sd_card.rw_enable = ENABLE;		// �����д����
	sd_card.state = SD_Detect();		 /*!< Check GPIO to detect SD */


	sd_card.spi_init_flag = sd_spi_init();

	fatfs_int();
	
//	if(	sd_card.fatfs_init_flag == PASSED)
//	{
//		CreatCanDataFile("Data");
//		fr = f_mkdir("Blind");
//		if(fr == FR_OK || fr == FR_EXIST)
//			printf("> �����ļ��� Blind �ɹ�\r\n");
//		else
//			printf("> �����ļ��� Blind ʧ��\r\n");
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
	exfuns_init();		//Ϊfatfs��ر��������ڴ�
		
	printf("mount sd card ...\r\n");
	fr = f_mount(0,fs[0]); 					 	//����SD�� 
	if(fr == FR_OK)
	{
		printf("  -- Register work area for the drive 0  \r\n");
		sd_card.Warn.Bit.err_mount = PASSED;
		
		while(exf_getfree("0",&sd_card.totalSize,&sd_card.freeSize))	//�õ�SD������������ʣ������
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
		printf("  -- Total Size: %dMB\r\n",sd_card.totalSize>>10);	//��ʾSD�������� MB
		printf("  -- Free  Size: %dMB\r\n",sd_card.freeSize>>10);		//��ʾSD��ʣ������ MB
	}
	else
	{
		printf("\r\nFAILED!\r\n");
		sd_card.Warn.Bit.err_mount = FAILED;
		return;
	}	

}

//�����ļ�
 //path:·��
 //����ֵ:ִ�н��
uint8_t scan_sd_files(uint8_t * path)
{
	FRESULT res;
	uint8_t num = 0;
  char *fn;   /* This function is assuming non-Unicode cfg. */ 
	
  res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼			

  if (res == FR_OK) 
	{		
		printf("+ %s\r\n", path);//��ӡ·��	

		while(1)
		{
			res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
			if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
			if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
			fn = fileinfo.fname;
			printf("|---- %s\r\n",  fn);//��ӡ�ļ���
			
			// ����ӡ20���ļ�
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

	printf("\r\n\r\nSD�� �ļ���Ŀ¼\r\n");					
	scan_sd_files("0:");
	printf("\r\n");					
	scan_sd_files("0://BOOT");
	res = scan_sd_files("0://DATA");
		
	if(res==FR_OK)
		printf("\r\nSD�� ����\r\n");
	else
	{
		printf("\r\nSD�� ����\r\n");
		sd_card.err_rw_count++;
	}
		return res;
}

uint8_t sd_format(FATFS *fs)
{
	fr = f_mount(0,fs); //����SD�� 
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
//	sd_card.state = SD_Detect(); // ����SD����λ���

//	sd_rw_handler();	// sd ��д����
//	sd_recovery(9);
//}

//void sd_rw_handler(void)
//{
//	// spi��ʼ���ɹ� + �����㹻��200M��+ ��Ǳպ�
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
//		// ��д����ʧ�ܴ���
//		if(sd_card.err_rw_count > 10)  // ���²��SD���� �ȴ�10������ SD��ʼ��
//		{	
//			sd_card.err_rw_count =0;
//			sd_card.spi_init_flag = FAILED;
//		}
//	}


//}

//void sd_recovery(uint8_t retry_times)
//{
//// 1����һ�γ�ʼ��ʧ��
//// 2����д���̲����Ĵ���
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
//			sd_card.sd_fs_ok =  ERROR;// sd ��
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
//	//ȡCSD��Ϣ������ڼ��������0
//	if(SD_GetCSD(csd)!=0) return 0;    
//	//���ΪSDHC�����������淽ʽ����
//	if((csd[0]&0xC0)==0x40) //V2.00�Ŀ�
//	{
//	csize = csd[9] + ((u16)csd[8] << 8) + 1;
//	Capacity = (u32)csize << 10;//�õ�������   
//	}else//V1.XX�Ŀ�
//	{
//	n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
//	csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
//	Capacity= (u32)csize << (n - 9);//�õ�������   
//	}
//	return Capacity;
//}

