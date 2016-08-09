#include "user_timer.h"


uint32_t Timer2_count=0;


void User_Timer2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  		//开启TIM2时钟
	TIM_DeInit(TIM2);						 			  		//复位TIM2，可以设置数据

	
	// 250ms 
	TIM_TimeBaseStructure.TIM_Period=5;                		//计数值，计数等于该数则发生中断
	TIM_TimeBaseStructure.TIM_Prescaler= (36 - 1);     		//分频数,即每36M/36000计数一次
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                 		//清除溢出中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2, ENABLE);                                		//开启定时器
	
}


/* 中断周期为1 s */
void User_Timer3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	TIM_DeInit(TIM3);
	
	// 25ms 
	TIM_TimeBaseStructure.TIM_Period=1;		 								/* 自动重装载寄存器周期的值(计数值) */
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* 时钟预分频数 36M/36 */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);																		/* 开启时钟 */

	#ifdef DEBUG
	printf("> TIMER3 init.\r\n");
	#endif	  
		
}

