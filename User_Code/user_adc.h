#ifndef __USER_ADC_H
#define __USER_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"
	 
	 
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
	 
#define ADC_FILTER_BUF_SIZE 100
	 
extern uint16_t AdcPower_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcExVin_N16_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcExVin_N17_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcExIin_N18_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcExIin_N19_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcDAOUTA_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcDAOUTB_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcDAOUTC_Buf[ADC_FILTER_BUF_SIZE];
extern uint16_t AdcDAOUTD_Buf[ADC_FILTER_BUF_SIZE];
extern int adc ; 
	 
extern uint32_t Count_AdcFilter;
extern uint32_t Sum_AdcVinput;
extern uint32_t Sum_AdcVbat;
extern uint32_t Sum_AdcExt1;
extern uint32_t Sum_AdcExt2;
extern uint32_t Sum_AdcGpsAnt;	 
extern uint32_t Sum_AdcLight;
extern uint32_t Sum_AdcInterTemp;
	 
	 
extern uint32_t Avr_AdcVinput;
extern uint32_t Avr_AdcVbat;	 
extern uint32_t Avr_AdcExt1; // 外部输入电压1
extern uint32_t Avr_AdcExt2; // 外部输入电压2
extern uint32_t Avr_AdcGpsAnt; // 外部输入电压2
extern uint32_t Avr_AdcLight;
extern uint32_t Avr_AdcInterTemp;

extern uint32_t Sum_AdcPower;
extern uint32_t Sum_AdcExVin_N16;
extern uint32_t Sum_AdcExVin_N17;
extern uint32_t Sum_AdcExIin_N18;
extern uint32_t Sum_AdcExIin_N19;
extern uint32_t Sum_AdcDAOUTA;
extern uint32_t Sum_AdcDAOUTB;
extern uint32_t Sum_AdcDAOUTC;
extern uint32_t Sum_AdcDAOUTD;

extern uint32_t Avr_AdcPower;
extern uint32_t Avr_AdcExVin_N16;
extern uint32_t Avr_AdcExVin_N17;
extern uint32_t Avr_AdcExIin_N18;
extern uint32_t Avr_AdcExIin_N19;
extern uint32_t Avr_AdcDAOUTA;
extern uint32_t Avr_AdcDAOUTB;
extern uint32_t Avr_AdcDAOUTC;
extern uint32_t Avr_AdcDAOUTD;
	 
	 
	 
extern uint32_t Count_LowVinput; // 
extern uint32_t Count_LowVbat; // 
extern uint32_t Count_RecoverVinput; //  电池电压恢复计数
extern uint32_t Count_RecoverVbat; //  电池电压恢复计数	 


typedef struct 
{
  uint16_t Power_v;
  uint16_t Bat_v;
  uint16_t ad1;
  uint16_t ad2;
	uint16_t GpsAnt; 
	uint16_t Light; // 开壳
	uint16_t temp_sensor; // 芯片内部温度
} ADC_STRUCT;
extern ADC_STRUCT Adcresult;


typedef struct 
{
  float Power;
	
  float ExVin_N16;
  float ExVin_N17;
  float ExIin_N18;
	float ExIin_N19; 
	
	float DAOUTA;
	float DAOUTB;	
	float DAOUTC;
	float DAOUTD;	
	
	float temp_sensor; // 芯片内部温度
} ADC_CONVERSION;
extern ADC_CONVERSION ADC1Conversion;



// PA5 -- ADC_POWER  	// 输入电压检测
// PA6 -- ADC_R2     	// 第3路电阻检测	
// PA7 -- ADC_R3     	// 第4路电阻检测	

// PC0 -- ADC_R1  // 第2路电阻检测
// PC1 -- ADC_R0  // 第1路电阻检测
// PC2 -- ADC_V1 			// No1.外部电压检测
// PC3 -- ADC_V2			// No2.外部电压检测
// PC4 -- ADC_I1			// No1.电流信号量检测
// PC5 -- ADC_I2			// No2.电流信号量检测
	
// ---用来保存ADC的转换结果
//typedef struct 
//{
//  uint16_t Channel_5;  // 输入电压检测
//  uint16_t Channel_6;  // DAOUTC 电阻检测	5mA
//  uint16_t Channel_7;  // DAOUTD 电阻检测	5mA
//	
//  uint16_t Channel_10; // DAOUTB 电阻检测	10mA
//	uint16_t Channel_11; // DAOUTA 电阻检测	10mA 
//	uint16_t Channel_12; // INN16 外部电压检测
//	uint16_t Channel_13; // INN17 外部电压检测
//	uint16_t Channel_14; // INN18 电流信号量检测
//	uint16_t Channel_15; // INN19 电流信号量检测
//} ADC_CONVERSION;
//extern ADC_CONVERSION ADC1Conversion;


typedef struct 
{
  uint16_t Channel_8;  // 输入电压检测
  uint16_t Channel_9;  // DAOUTC 电阻检测	5mA
  uint16_t Channel_15;  // DAOUTD 电阻检测	5mA

} ADC_RESULT;
extern ADC_RESULT Adc1Result;



typedef struct 
{
	float Vinput;
	float Vbat;
	float Vext1;
	float Vext2;
	float Temperature;

	uint32_t power;
	uint32_t battery;
	uint32_t ad1;
	uint32_t ad2;

} ADC_SENSOR;

extern ADC_SENSOR AdcSensor;

extern BOOL FlagAdcFilter;
extern uint32_t filter_count;

extern uint16_t BatLowVoltageValue;
extern uint16_t BatRecoverVoltageValue;

extern uint16_t LiBatLowVoltageValue;
extern uint16_t LiBatRecoverVoltageValue;


extern void ADC_Configuration(void);	 
extern void ReadAdResult(void);	 
extern void AdcFilter(void);	 
extern void Power_Management_Handler(void);
extern void Vinput_Handler(void);
extern void Vbat_Handler(void);

extern void GetVoltageConf(void);

extern uint8_t GpsAntDetect(void);
extern uint8_t ShellOpenDetect(void);

extern void AdcFilterHandler(void);
extern void InsertSort(unsigned short a[],int count);

#ifdef __cplusplus
}
#endif 

#endif

