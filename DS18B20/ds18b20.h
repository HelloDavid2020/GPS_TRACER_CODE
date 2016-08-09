
//============================================

//      DS18B20.H

//============================================

#ifndef __DS18B20_H
#define __DS18B20_H
#include "includes.h"

#define  SkipROM    0xCC     //跳过ROM
#define  SearchROM  0xF0  //搜索ROM
#define  ReadROM    0x33  //读ROM
#define  MatchROM   0x55  //匹配ROM
#define  AlarmROM   0xEC  //告警ROM

#define  StartConvert    0x44  //开始温度转换，在温度转换期间总线上输出0，转换结束后输出1
#define  ReadScratchpad  0xBE  //读暂存器的9个字节
#define  WriteScratchpad 0x4E  //写暂存器的温度告警TH和TL
#define  CopyScratchpad  0x48  //将暂存器的温度告警复制到EEPROM，在复制期间总线上输出0，复制完后输出1
#define  RecallEEPROM    0xB8    //将EEPROM的温度告警复制到暂存器中，复制期间输出0，复制完成后输出1
#define  ReadPower       0xB4    //读电源的供电方式：0为寄生电源供电；1为外部电源供电


void ds18b20_start(void);
unsigned short ds18b20_read(void);





////IO操作函数											   
//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define DS18B20_DQ_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)
                                          
//读取引脚的电平
#define  DS18B20_DQ_IN()	       GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
   	
void DS18B20_Init(void);//初始化DS18B20
u8 DS18B20_Init_Check(void);//检测DS18B20是否存在
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20
void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);
void Update_DS18B20(void);
unsigned char Check_crc8(unsigned char *p,unsigned char n);





#endif