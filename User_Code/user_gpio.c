#include "user_gpio.h"


PORT_INPUT   ExInput;
PORT_OUTPUT  ExOutput;


void exit_wake_up(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;    
	EXTI_InitTypeDef EXTI_InitStructure;   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE); 
 
	/* EXTI line gpio config(PA0) */       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
   
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);    
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;   
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;   
	EXTI_Init(&EXTI_InitStructure);	
}

//  toggle gpio
void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)));
}

void BSP_GPIO_IO1_Set(uint8_t enabled)
{
	if (enabled)		
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		
	else
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
}


void BSP_GPIO_IO2_Set(uint8_t enabled)
{
	if (enabled)
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	else
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
}


void BSP_GPIO_IO3_Set(uint8_t enabled)
{
	if (enabled)
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
}

void BSP_GPIO_IO4_Set(uint8_t enabled)
{
	if (enabled)
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
}


/********************************************************************************************************
**函数信息 ：void BSP_GPIO_Init(void)                         // WAN.CG // 2010.12.8 
**功能描述 ：GPIO初始化函数，初始化实验用到的所有GPIO口
**输入参数 ：无
**输出参数 ：无
**调用提示 ：RCC_APB2PeriphClockCmd（）；GPIO_Init（）
********************************************************************************************************/
void User_GpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //开启GPIOD端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  

	
	// CHRG_STATUS -PA1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1|GPIO_Pin_2);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//开C口时钟，复用时钟。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //设为输出　注意：2MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	//GPIO_SetBits(GPIOC, GPIO_Pin_13);



}

// LED系统运行指示
void LED_SystemRun(void)
{
	uint8_t num = 3;
// 	LED_RED_ON;

	while(num--)
	{
		GPIO_Toggle(GPIOC,LED_GREEN);
		GPIO_Toggle(GPIOA,LED_RED);
		GPIO_Toggle(GPIOC,LED_BLUE);
		DelayMS(100);
	}
	LED_RED_OFF;
	LED_GREEN_OFF;
	LED_BLUE_OFF;
	
}

void DetectPortInput(void)
{
	ExInput.Bit.In0_0 = I0_0_STATUS;
	ExInput.Bit.In0_1 = I0_1_STATUS;
	ExInput.Bit.In0_2 = I0_2_STATUS;
	
	
	ExInput.Bit.In0_3 = 1^I0_3_STATUS;
	ExInput.Bit.In0_4 = 1^I0_4_STATUS;
	ExInput.Bit.In0_5 = 1^I0_5_STATUS;
	ExInput.Bit.In0_6 = 1^I0_6_STATUS;
	ExInput.Bit.In0_7 = 1^I0_7_STATUS;
	ExInput.Bit.In0_8 = 1^I0_8_STATUS;
	
	ExInput.Bit.In1_0 = I1_0_STATUS;
	ExOutput.Value=0;

}

// Q0.0 -- PA3 @PWM TIM5_CH1
// Q0.1 -- PA2 @PWM TIM5_CH2  
// Q0.2 -- PA1 @PWM TIM5_CH3
// Q0.3 -- PA0 @PWM TIM5_CH4
// Q0.4 -- PB0
// Q0.5 -- PB1
// Q0.6 -- PC13
// Q0.7 -- PD0

void OutputControl(PORT_OUTPUT Output)
{

			if(Output.Bit.Q0) 
				GPIO_ResetBits(LED_PORT, BEEP);
			else 
				GPIO_SetBits(LED_PORT, BEEP);

			if(ExInput.Bit.In1_0==0)		
				GPIO_ResetBits(LED_PORT, LED_BLUE);  // open LED
			else 
				GPIO_SetBits(LED_PORT, LED_BLUE);    // close LED
		
}


