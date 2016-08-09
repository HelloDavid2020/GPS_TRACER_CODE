#include "user_timedelay.h"


__IO uint32_t TimingDelay=0;


void delay_us(__IO uint32_t Nus)
{
// SysTick->LOAD=Nus*9;          //时间加载         
// SysTick->CTRL|=0x01;             //开始倒数       
// while(!(SysTick->CTRL&(1<<16))); //等待时间到达    
// SysTick->CTRL=0X00000000;        //关闭计数器   
// SysTick->VAL=0X00000000;         //清空计数器  
//	
//	if(Nus%5!=0)
//		TimingDelay=Nus/5 + 1;
//	else
//		TimingDelay=Nus/5;
	TimingDelay=Nus;
	while(TimingDelay!=0);
	
}

void User_DelayuS(uint32_t delay) 
{
// delay time = 100us * x 

	delay =2*delay;
	while(delay--)
	{
			for(;delay>0;delay--)
			__nop();	
	}
       
}
void DelayMS(uint32_t delayms) 
{
// delay time = 100us * x 

	delayms = delayms > 10000UL ? 10000UL : delayms;
	
	delayms =4000*delayms;
	while(delayms--)
	{
			for(;delayms>0;delayms--)
			__nop();	
	}
       
}



void delay_ms(uint32_t delayms) 
{
// delay time = 100us * x 
	uint32_t i=0;
	while(delayms--)
	{
		i=12000;
		while(i--)
		{
			__nop();	
		}
	}
       
}

void delay(u16 cnt)
{
	u8 i;
	for(;cnt>0;cnt--)
		for(i=0;i<5;i++);
}
