#include "user_adc.h"
int adc = 0;
ADC_SENSOR AdcSensor;

uint32_t filter_count =100;

BOOL FlagAdcFilter = ENABLE; // Adc 采样滤波标志

uint16_t AdcPower_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcExVin_N16_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcExVin_N17_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcExIin_N18_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcExIin_N19_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcDAOUTA_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcDAOUTB_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcDAOUTC_Buf[ADC_FILTER_BUF_SIZE] ={0};
uint16_t AdcDAOUTD_Buf[ADC_FILTER_BUF_SIZE] ={0};

//uint32_t Sum_AdcPower =0;
//uint32_t Sum_AdcExVin_N16	 =0;
//uint32_t Sum_AdcExVin_N17	 =0;
//uint32_t Sum_AdcExIin_N18	 =0;
//uint32_t Sum_AdcExIin_N19 =0;
//uint32_t Sum_AdcDAOUTA  =0;
//uint32_t Sum_AdcDAOUTB  =0;
//uint32_t Sum_AdcDAOUTC  =0;
//uint32_t Sum_AdcDAOUTD  =0;
ADC_CONVERSION ADC1Conversion; // 保存转换后的实际值

ADC_RESULT Adc1Result;  // 保存寄存器结果

uint32_t Count_AdcFilter = 0; // 

uint32_t Count_LowVinput = 0; // 
uint32_t Count_NoVinput = 0; // 

uint32_t Count_LowVbat = 0; // 

uint32_t Count_RecoverVinput 	= 0; //  电池电压恢复计数
uint32_t Count_RecoverVbat 		= 0; //  电池电压恢复计数

uint32_t Sum_AdcVinput =0;
uint32_t Sum_AdcVbat	 =0;
uint32_t Sum_AdcExt1	 =0;
uint32_t Sum_AdcExt2	 =0;
uint32_t Sum_AdcGpsAnt =0;
uint32_t Sum_AdcLight  =0;
uint32_t Sum_AdcInterTemp = 0;


uint32_t Avr_AdcVinput =0;
uint32_t Avr_AdcVbat	 =0;
uint32_t Avr_AdcExt1	 =0; // 外部输入电压1
uint32_t Avr_AdcExt2	 =0; // 外部输入电压2
uint32_t Avr_AdcGpsAnt =0; // 外部输入电压2
uint32_t Avr_AdcLight	 =0;
uint32_t Avr_AdcInterTemp = 0;

//-----------------------------------------------------------------------------

/*
  uint16_t Power;
	
  uint16_t ExVin_N16;
  uint16_t ExVin_N17;
  uint16_t ExIin_N18;
	uint16_t ExIin_N19; 
	
	uint16_t DAOUTA;
	uint16_t DAOUTB;	
	uint16_t DAOUTC;
	uint16_t DAOUTD;

*/
uint32_t Sum_AdcPower =0;
uint32_t Sum_AdcExVin_N16	 =0;
uint32_t Sum_AdcExVin_N17	 =0;
uint32_t Sum_AdcExIin_N18	 =0;
uint32_t Sum_AdcExIin_N19 =0;
uint32_t Sum_AdcDAOUTA  =0;
uint32_t Sum_AdcDAOUTB  =0;
uint32_t Sum_AdcDAOUTC  =0;
uint32_t Sum_AdcDAOUTD  =0;

uint32_t Avr_AdcPower =0;
uint32_t Avr_AdcExVin_N16	 =0;
uint32_t Avr_AdcExVin_N17	 =0;
uint32_t Avr_AdcExIin_N18	 =0;
uint32_t Avr_AdcExIin_N19 =0;
uint32_t Avr_AdcDAOUTA  =0;
uint32_t Avr_AdcDAOUTB  =0;
uint32_t Avr_AdcDAOUTC  =0;
uint32_t Avr_AdcDAOUTD  =0;


//-----------------------------------------------------------------------------

uint8_t Flag_LowVbat   =0;

uint8_t Flag_RecoverVinput =0;
uint8_t Flag_RecoverVbat	 =0;

ADC_STRUCT Adcresult;

uint16_t BatLowVoltageValue = 0;
uint16_t BatRecoverVoltageValue = 0;

uint16_t LiBatLowVoltageValue = 0;
uint16_t LiBatRecoverVoltageValue = 0;

// Temperature=(1.43-Avr_AdcInterTemp*3.3/4096)*1000/4.35 + 25

/********************************************************************************************************
**函数信息 ：ADC_Configuration()
**功能描述 ：ADC初始化函数
**输入参数 ：无
**输出参数 ：无
**调用提示 ：
********************************************************************************************************/
void ADC_Configuration(void)
{	
	GPIO_InitTypeDef 	GPIO_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;
	ADC_InitTypeDef 	ADC_InitStructure;



// PC5 -- ADC_I2			// No2.电流信号量检测
// PB0 -- ADC_R1  // SOLAR
// PB1 -- ADC_R0  // CHARGE

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

//	/* DMA1 channel1 configuration ----------------------------------------------*/
	
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Adc1Result; ;
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3; // 9个数据
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 3;		// 用于转换的通道数

	// 配置ADC时钟 内部基准源 36M/4 = 9M
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration */ 

	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 	1, ADC_SampleTime_239Cycles5); 	// PC0
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 	2, ADC_SampleTime_239Cycles5);	// PC1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15,	3, ADC_SampleTime_239Cycles5);	// PC2


/* Enable ADC1 EOC interupt */
	//ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ReadAdResult(void)
{	
	double  temp = 0;
//	y = 0.0082x - 0.0381
// y = 0.0082 x
	
	AdcFilterHandler();
	// (5.227/650.0)
	ADC1Conversion.Power = (0.0080)*Avr_AdcPower;
	
	ADC1Conversion.ExVin_N16 = (10.1/280.0)*Avr_AdcExVin_N16;
	ADC1Conversion.ExVin_N17 = 0.0025*Avr_AdcExVin_N17+0.08;
	
	
	// y = 0.005 x - 0.0081
	ADC1Conversion.ExIin_N18 = 0.005 * Avr_AdcExIin_N18 - 0.0081;	
	ADC1Conversion.ExIin_N19 = 0.005 * Avr_AdcExIin_N19 - 0.0001;
	

	if(ADC1Conversion.Power < 0.0)      ADC1Conversion.Power = 0.0;
	if(ADC1Conversion.ExVin_N16 < 0.0)  ADC1Conversion.ExVin_N16 = 0.0;
	if(ADC1Conversion.ExVin_N17 < 0.0 ) ADC1Conversion.ExVin_N17 = 0.0;
	if(ADC1Conversion.ExIin_N18 < 0.0)  ADC1Conversion.ExIin_N18 = 0.0;
	if(ADC1Conversion.ExIin_N19 < 0.0)  ADC1Conversion.ExIin_N19 = 0.0;	

}


//  uint16_t Channel_5;  // 输入电压检测
//  uint16_t Channel_6;  // DAOUTC 电阻检测	5mA
//  uint16_t Channel_7;  // DAOUTD 电阻检测	5mA
//  uint16_t Channel_10; // DAOUTB 电阻检测	10mA
//	uint16_t Channel_11; // DAOUTA 电阻检测	10mA 
//	uint16_t Channel_12; // INN16 外部电压检测
//	uint16_t Channel_13; // INN17 外部电压检测
//	uint16_t Channel_14; // INN18 电流信号量检测
//	uint16_t Channel_15; // INN19 电流信号量检测


void AdcFilter(void)
{
	uint32_t i = 0;
	
	if(FlagAdcFilter == ENABLE )
	{
	
		AdcExIin_N19_Buf[Count_AdcFilter] =Adc1Result.Channel_15;
			
		Count_AdcFilter ++;
		if(Count_AdcFilter > (filter_count-1))
		{
			Count_AdcFilter = 0;
			FlagAdcFilter = DISABLE;
		}		
	}
}

void AdcFilterHandler(void)
{
	uint32_t i = 0;
	
	if(FlagAdcFilter == DISABLE)
	{
		// 插值法排序
		InsertSort(AdcPower_Buf, filter_count);
		InsertSort(AdcExVin_N16_Buf, filter_count);
		InsertSort(AdcExVin_N17_Buf, filter_count);
		InsertSort(AdcExIin_N18_Buf, filter_count);
		InsertSort(AdcExIin_N19_Buf, filter_count);


		// 去掉最大10个 去掉最小10个
		// buf[10],...buf[89]
		for(i=10;i< (filter_count-10); i++ )
			Sum_AdcPower += AdcPower_Buf[i];
		
		for(i=10;i< (filter_count-10); i++ )
			Sum_AdcExVin_N16 += AdcExVin_N16_Buf[i];
		
		for(i=10;i< (filter_count-10); i++ )
			Sum_AdcExVin_N17 += AdcExVin_N17_Buf[i];
		
		for(i=10;i< (filter_count-10); i++ )
			Sum_AdcExIin_N18 += AdcExIin_N18_Buf[i];  
		
		for(i=10;i< (filter_count-10); i++ )
			Sum_AdcExIin_N19 += AdcExIin_N19_Buf[i];
	
		Avr_AdcPower			= Sum_AdcPower / (filter_count-20);
		Avr_AdcExVin_N16	= Sum_AdcExVin_N16 / (filter_count-20);
		Avr_AdcExVin_N17 	=	Sum_AdcExVin_N17 / (filter_count-20);
		Avr_AdcExIin_N18 	= Sum_AdcExIin_N18 / (filter_count-20);
		Avr_AdcExIin_N19 	= Sum_AdcExIin_N19 / (filter_count-20);

		Sum_AdcPower  		= 0;
		Sum_AdcExVin_N16	= 0;
		Sum_AdcExVin_N17  = 0;
		Sum_AdcExIin_N18	= 0;
		Sum_AdcExIin_N19 	= 0;
		
		FlagAdcFilter = ENABLE;
	}
}



// 直接插入法排序
void InsertSort(unsigned short a[],int count)  
{  
	int i,j,temp;  
	for(i=1;i<count;i++)     
	{  
		 temp=a[i];  
		 j=i-1;  
		 while(a[j]>temp && j>=0)  
		 {  
			 a[j+1]=a[j];  
				j--;  
		 }  
		 if(j!=(i-1))       
			 a[j+1]=temp;  
	}  
}




uint8_t GpsAntDetect(void)
{
	if(Avr_AdcGpsAnt > 1000) 
		return 1; 
	else 
		return  0;
}


uint8_t ShellOpenDetect(void)
{
	if(Avr_AdcLight > 100) 
		return 1; 
	else 
		return  0;
}
