/************************ Copyright (c) UP-TECH ********************************
	* History  
  * @modify  Zhao Dawei
  * @version V1.0.1
  * @date    06/26/2013
  * 
  *
  * This file is designed and created by xxx x.  Any modification to this     
  * file may violate the integrity of the product and lead to instability. 
  */ 

#include "includes.h"
#include "LCD.h"
#include "config.h"
#include "G_086_PC.h"
#include "test.h"
#define STACK_SIZE  512

static U32 StackMonitor[1024];	
static U32 StackKey [1024];	
static U32 StackCmd[1024];

uint8_t DownloadInforCount = 0;
uint32_t cnt_test=0;

double gsensor[6];

/* assigned task id of task: */
OS_TID task1;
OS_TID task2;
OS_TID task3;

__task void Monitor(void);
__task void KEYHandler(void);
__task void CMDHandler(void);
__task void task_init(void);


__task void task_init (void) 
{
	task1 = os_tsk_create_user( Monitor,		1,	&StackMonitor,		sizeof(StackMonitor));
	task2	= os_tsk_create_user( KEYHandler,	2,	&StackKey,		    sizeof(StackKey));
	task3	= os_tsk_create_user( CMDHandler,	3,	&StackCmd,		    sizeof(StackCmd));
	os_tsk_delete_self ();
}


/*----------------------------------------------------------------------------
 *  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) 
{
	int i1=0;
	int j1=0;
	
	uint8_t write_buf[128]={0};
	uint8_t read_buf[128]={0};
	uint8_t i=0;
	uint32_t err_count=0;
	uint32_t ok_count=0;
	int tmp=0;	

	SystemClockInit();						// 内部8M时钟初始化 主频36M

	BspInit();									// 板级初始化

	User_NVIC_Configuration();	// 中断向量表初始化， bootloader与APP的中断向量 配置
	RTC_RX8025_Init();

	os_sys_init(task_init);                    /* Initialize RTX and start init    */
}



//-----------------------------------------
//		Task1: system monitor 
//  线程监控任务
//  查看软件狗状态，喂硬件狗
//-----------------------------------------
__task void Monitor(void)
{


	while (1)
	{		
		SoftWdgHandler();
		User_Feed_IWDG(); // 硬件喂狗
		GPIO_Toggle(GPIOA,LED_RED);     // 任务运行指示
		GPIO_Toggle(GPIOC,LED_GREEN);     // 任务运行指示
		GPIO_Toggle(GPIOC,LED_BLUE);     // 任务运行指示
//delay_us(100);
		os_dly_wait(50);  // 10MS * 10=100 mS
	}
}


//-----------------------------------------
//		Task2: KEYHandle
//  线程监控任务
//  查看软件狗状态，喂硬件狗
//-----------------------------------------
__task void KEYHandler(void)
{

	while (1)
	{		
		User_Feed_IWDG(); // 硬件喂狗
		Get_Car_Gesture();

		
		os_dly_wait(50);  // 10MS * 10=100 mS
	}
}



//-------------------------------------------------------
//			task 2: CMD 处理流程
//-------------------------------------------------------


TCOLOR  col_ret[2]={0};
uint8_t x_dat=6,y_dat=6;
__task void CMDHandler(void)
{
	uint8_t write_buf[128]={0};
	uint8_t read_buf[128]={0};
	uint8_t i=0;
	uint32_t err_count=0;
	uint32_t ok_count=0;
	unsigned short tmp=0;
	
	os_dly_wait(300);  // // 10MS * 10 = 100 mS

	Serial_PutString("AT\r\n");
	os_dly_wait(50);  // // 10MS * 10 = 100 mS
	Serial_PutString("ATE1\r\n");
	os_dly_wait(50);  // // 10MS * 10 = 100 mS
	Serial_PutString("AT+CPIN?\r\n");
	os_dly_wait(100);  // // 10MS * 10 = 100 mS
	ds18b20_start();
	while (1)
	{				

	Serial_PutString("AT+CSQ\r\n");
	ShowTime();
	printf("ACC:%5.2f %5.2f %5.2f  GYRO: %5.2f %5.2f %5.2f\r\n\r\n",Acc_Buffer[0],Acc_Buffer[1],Acc_Buffer[2],
																															Gyro_Buffer[0],Gyro_Buffer[1],Gyro_Buffer[2]);
		
		printf("SLOAR: %d, CHARGE: %d, BAT: %d (ADC Value)\r\n",Adc1Result.Channel_8, Adc1Result.Channel_9, Adc1Result.Channel_15);

		for(i=0;i<128;i++)
		write_buf[i]=rand();
		
		at25_write_page(0x00,128,write_buf);
		
		at25_read_page(0x00,128,read_buf);
	if(memcmp(read_buf,write_buf,128) == 0)
	{
		printf("> FRAM Write successed : %d\r\n" ,++ok_count);
	}
	else
	{
		printf("> FRAM Write failed: %d\r\n",++err_count);
	}		
		
	tmp=ds18b20_read();
		printf("> Tmp: %.1f (%d)\r\n",(tmp*0.0039),tmp);
	if(flag_usart3_rx==1)
	{
		flag_usart3_rx=0;
		printf("%s",UART3RecvBuf);
		printf("\r\n");
	}
	printf("---------------------------------------------------------------------------\r\n\r\n");

	os_dly_wait(100);  // // 10MS * 10 = 100 mS

		
	}
		
}
