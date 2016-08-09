#include "user_pwm.h"

uint16_t pwm_frequency = 200;
uint16_t pwm_duty = 50;

//PWM_INPUT ExPWM_Input;
PWM_OUTPUT PwmOutput;
uint16_t TIM4_ETR_Counts = 0;
uint16_t TIM4_ETR_GetDropCounts = 0;
uint32_t ExPwmInputFrequency  = 0;
//uint16_t Last_TIM4_ETR_GetDropCounts = 0; // 当前计数
//uint16_t Prve_TIM4_ETR_GetDropCounts = 0; // 前一计数

// 
void UserPwmOutInit(uint16_t frequency, uint16_t duty)
{
	
	uint16_t PrescalerValue = 0, Period =0,  Pulse =0; 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	/* PA0 Configuration:TIM5 Channel 1 as alternate function push-pull */
	/* PA1 Configuration:TIM5 Channel 2 as alternate function push-pull */
	/* PA2 Configuration:TIM5 Channel 3 as alternate function push-pull */
	/* PA3 Configuration:TIM5 Channel 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	/* Compute the prescaler value */
	PrescalerValue =  36- 1;  //(36000000 / 1000000)
	/* Time base configuration */
	if(frequency>0)
		Period = (uint16_t)(1000000L/frequency -1);
	else
		Period =1;
	
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	Pulse = (uint16_t)(Period+1)*duty/100;
	TIM_OCInitStructure.TIM_Pulse = Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);

	
	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	Pulse = (uint16_t)(Period+1)*duty/100;
	TIM_OCInitStructure.TIM_Pulse = Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);	
	
	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	Pulse = (uint16_t)(Period+1)*duty/100;
	TIM_OCInitStructure.TIM_Pulse = Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);	
		
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	Pulse = (uint16_t)(Period+1)*duty/100;
	TIM_OCInitStructure.TIM_Pulse = Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);	
	
	
  /* TIM5 counter enable */	
	TIM_ARRPreloadConfig(TIM5, ENABLE);	
	TIM_Cmd(TIM5, ENABLE);	
    
	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM5, ENABLE);
	
}


void User_PWM_Init(uint16_t frequency, uint16_t duty)
{
	uint16_t PrescalerValue = 0, Period =0,  Pulse =0; 
	

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* GPIOA clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	/* GPIOA.1 Configuration:TIM2 Channel 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (36000000 / 1000000) - 1;
	/* Time base configuration */
	if(frequency>0)
		Period = (uint16_t)(1000000L/frequency -1);
	else
		Period =1;
	
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	Pulse = (uint16_t)(Period+1)*duty/100;
	TIM_OCInitStructure.TIM_Pulse = Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM5, ENABLE);	

	TIM_Cmd(TIM5, ENABLE);
}



// PWM 方波的输入捕获
// I0.5 --- PE0/ TIM4_EIR
void User_PWM_Capture_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef        TIM_OCInitStructure;
	TIM_ICInitTypeDef 			 TIM_ICInitStructure;
	
	// TIMER4 CLK = 18M MHz 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/* GPIOE clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // GPIO_Mode_IPU
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

// Prescaler = (18000000 / 1000000) -1 =17
// 1MHz
	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); // Time base configuration


// 外部促发ETR极性不反向 高电平或上升沿有效
	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);

//	/* 中断配置 */
//	// .....
//	//以1Mhz的频率计数 

//	TIM_TimeBaseStructure.TIM_Period = 65535;
//	TIM_TimeBaseStructure.TIM_Prescaler = 37;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

//	//初始化TIM5输入捕获参数
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
//	TIM_ICInit(TIM4, &TIM_ICInitStructure);

////	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
////	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
////	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
////	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
////	TIM_ICInitStructure.TIM_ICFilter = 0x0;           
////	TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);

//	/* Select the TIM4 Input Trigger: TI2FP2 */
//	TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);
//	/* Select the slave Mode: Reset Mode */
//	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
//	/* Enable the Master/Slave Mode */
//	TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable); 
//	/* TIM enable counter */
//	TIM_Cmd(TIM4, ENABLE);                          
//	/* Enable the CC2 Interrupt Request */
//	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);  
//	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
}

// frequency max : 60K HZ
// duty: 0-100;
void PWM_Output(uint16_t frequency, uint16_t duty)
{
	uint16_t Period = 0, Pulse = 0;
	
	frequency = frequency  <  1    ?   0   : frequency;
	frequency = frequency  > 60000 ? 60000 : frequency;
	
	Period = (uint16_t)(1000000L/frequency -1);
  /* Set the Auto reload value */
	TIM5->ARR =  Period;
//	
//	duty = duty <  1  ?  0  : duty;
//	duty = duty > 100 ? 100 : duty;
//	
//	Pulse = (uint16_t)(Period+1)*duty/100;
//  /* Set the Capture Compare Register value */
//  TIM5->CCR2 = Pulse;
}

void User_PWM_Handler(void)
{
	pwm_duty = *(int16u*)(g_modbusReg4 + modbusPWM);
	PWM_Output(pwm_frequency, pwm_duty);	
}

// 外部ＰＷＭ输入 引脚中断初始化
void ExPwmInputGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // GPIO_Mode_IPU
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
	


   
    /* Connect Button EXTI Line to Button GPIO Pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);  

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	
		
		EXTI_GenerateSWInterrupt(EXTI_Line0);

}
