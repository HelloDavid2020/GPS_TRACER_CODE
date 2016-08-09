#include "user_dac.h"

float DacVoltage = 1;


void DAC_VOLTAGE_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef            DAC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* Configure DAC channe1 output pin */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	DAC_DeInit();

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;   
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095; 
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	//

	DAC_Cmd(DAC_Channel_1, ENABLE);
	
	DAC_SetChannel1Data(DAC_Align_12b_R,0x000);
	DAC_DualSoftwareTriggerCmd(ENABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}


void SetDacVoltageOutput(float OutmV)
{
	uint16_t RegValue = 0; // 设定DAC的寄存器值
	
	RegValue = (OutmV/3.3) * 4095;
	DAC_SetChannel1Data(DAC_Align_12b_R,RegValue);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}


