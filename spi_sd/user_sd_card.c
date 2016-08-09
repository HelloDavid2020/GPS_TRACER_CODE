#include "user_sd_card.h"
SD_CARD_INFOR sd_card;

bool acc_status = 0;
bool flag_sd_rw_test = 0;

uint8_t sd_write_buf[DATA_LEN]={0x00};
uint8_t sd_read_buf[DATA_LEN]={0x00};


/* 
 0. ����λ���
 1. spi ��ʼ��
*/

SD_Error sd_card_init(void)
{
	uint8_t r1;      // ���SD���ķ���ֵ
	uint16_t retry;  // �������г�ʱ����
	uint8_t buf[4];  
	uint16_t i;
	uint8_t init_flag = 0;	

	sd_card.sd_fs_ok  = 1; // Ĭ��SD������δ��
	sd_card.rw_enable = 1;		// �����д����
	sd_card.state = sd_card_detect();		 /*!< Check GPIO to detect SD */

	printf("> SD����λ���\r\n");
	

	for(i=0;i<DATA_LEN;i++)
	{
		sd_write_buf[i]=rand();
	}
	i=0;
	
	
	
	
	r1=SD_Init() ;

	


		SD_GetCardInfo(&cardinfo); 
		sd_flag=1;
		
		printf("> SD    ����: %d KB\r\n",(cardinfo.SD_csd.DeviceSize << 9));
		printf("> block nums: %d \r\n",cardinfo.SD_csd.DeviceSize);

}

/* 
 * SD��д���ȡ���Գ��� 
 * ACC�źŴ���
*/

void sd_test(void)
{
	static bool rw_enable = 0;
	static uint32_t ok_count = 0;
	static uint32_t err_count = 0;

	uint32_t i =0;
	uint32_t n =0;
	uint32_t block_num =1;
	uint32_t wr_add =1;
	uint32_t rd_add =1;
	uint32_t rw_add =1;
	

	if(acc_status == 1 ) 
	{
		// a+rand()%(b-a);	
			rw_add = 0+rand()%(cardinfo.SD_csd.DeviceSize-0);	// 8G(15053) 32G(61055)

		
			printf("\r\n sector num: < %d > \r\n",rw_add);

			status = SD_BufferWrite(sd_write_buf,rw_add,DATA_LEN);
			printf("-------------sd_write_buf-------------------------------------\r\n");
			
			for(n=0;n<DATA_LEN;n++)
			{
				printf("0x%02X ",sd_write_buf[n]);
			}
			printf("--------------------------------------------------------\r\n");
			
			
			if(status == SD_RESPONSE_NO_ERROR)
			{
				printf("\r\n sd ����д��ɹ�\r\n");
			}
			else
			{
				printf("\r\n sd ����д��ʧ��\r\n");
			}

			
			status = SD_BufferRead(sd_read_buf,rw_add,DATA_LEN);

//			SD_ReadBlock(sd_read_buf, 1,1);
			printf("\r\n-----sd_read_buf----------------------------------------\r\n",i);
			
			for(n=0;n<DATA_LEN;n++)
			{
				printf("0x%02X ",sd_read_buf[n]);
			}
			printf("--------------------------------------------------------\r\n");

			

			if(memcmp(sd_read_buf,sd_write_buf,DATA_LEN) == 0)
			{
				printf("> SD������д���������ȡ����һ�£�д��ɹ�����: %d\r\n" ,++ok_count);
			}
			else
			{
				printf("> SD������д���������ȡ���ݲ�һ�£�д��ʧ��: %d\r\n",++err_count);
			}

	}
	
}

