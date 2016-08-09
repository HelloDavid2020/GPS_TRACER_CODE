
//============================================

//      DS18B20.H

//============================================

#ifndef __DS18B20_H
#define __DS18B20_H
#include "includes.h"

#define  SkipROM    0xCC     //����ROM
#define  SearchROM  0xF0  //����ROM
#define  ReadROM    0x33  //��ROM
#define  MatchROM   0x55  //ƥ��ROM
#define  AlarmROM   0xEC  //�澯ROM

#define  StartConvert    0x44  //��ʼ�¶�ת�������¶�ת���ڼ����������0��ת�����������1
#define  ReadScratchpad  0xBE  //���ݴ�����9���ֽ�
#define  WriteScratchpad 0x4E  //д�ݴ������¶ȸ澯TH��TL
#define  CopyScratchpad  0x48  //���ݴ������¶ȸ澯���Ƶ�EEPROM���ڸ����ڼ����������0������������1
#define  RecallEEPROM    0xB8    //��EEPROM���¶ȸ澯���Ƶ��ݴ����У������ڼ����0��������ɺ����1
#define  ReadPower       0xB4    //����Դ�Ĺ��緽ʽ��0Ϊ������Դ���磻1Ϊ�ⲿ��Դ����


void ds18b20_start(void);
unsigned short ds18b20_read(void);





////IO��������											   
//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DS18B20_DQ_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)
                                          
//��ȡ���ŵĵ�ƽ
#define  DS18B20_DQ_IN()	       GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
   	
void DS18B20_Init(void);//��ʼ��DS18B20
u8 DS18B20_Init_Check(void);//���DS18B20�Ƿ����
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20
void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);
void Update_DS18B20(void);
unsigned char Check_crc8(unsigned char *p,unsigned char n);





#endif