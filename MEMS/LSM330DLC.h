/****************************************************************
 *                                                              *
 *    Copyright (c) Linker Chip Corp. All rights reserved.      *
 *    														    *
 *    Created by Anakin											*
 *                                                      		*
 ****************************************************************/

#ifndef __LSM330DLC_H
#define __LSM330DLC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "math.h"
#include "stdint.h"
#define pi 3.14159265
#define zerodriftOffset (7132)

#define AVER_NUMBER 10

/* Defines -------------------------------------------------------------------*/

//#define GYRORATE 	70.0/1000		//+-2000DPS
#define GYRORATE 	(17.5/1000)		//+-500DPS
//#define ACCRATE 	1.0/100

extern float iaxx;
extern float iayy;

extern void IMU_getYawPitchRoll(double * angles	);

	extern float tt[3][3];
	extern int noMove;
	
extern float averGx;
extern float averGy;
extern float averGz;
extern float varGx;
extern float varGy;
extern float varGz;

extern float  averAx;
extern float averAy;
extern float averAz;
extern float  varAx;
extern float varAy;
extern float varAz;
extern float lastAx[AVER_NUMBER];
extern float lastAy[AVER_NUMBER];
extern float lastAz[AVER_NUMBER];
extern int nAx;



extern void SaveAngleState(double pitch,double roll);
extern float lastGx[AVER_NUMBER],lastGy[AVER_NUMBER],lastGz[AVER_NUMBER];
extern int 	nGx;
extern int bStatic;
/* Globals -------------------------------------------------------------------*/
extern float Init_RollAng,Init_PitchAng;                          //�豸��ʼƫת��
extern float volatile angle_x,angle_y,angle_z ;                //������ƫת�Ƕ�
extern float anglex_buffer[200],angley_buffer[200],anglez_buffer[200];    //ƫת�ǶȻ�����
extern float RollAng , PitchAng ,fSinRoll,fSinPitch;          //���ٶȼƵķ����Ǻ͸�����


extern double Acc_Buffer[3] ;                                       //����Ƕ�ֵ�ͼ��ٶ�ֵ
extern double Gyro_Buffer[3] ;
extern float fNormAcc ;                                                 //�������ٶȵĺ�
extern float High_Acc ;
extern unsigned char High_Acc_Num ,High_Acc_Flag ;
//unsigned char num = 0;                                                   //��Ư����
extern float num00,num11,num22;      //������Ư�ı���

extern float tts[3][3];	//�ȶ�����ֵ
extern int bUseStaticAngle;




/* Exported macro ------------------------------------------------------------*/

// address of ADDR_LSM330DLC_G: 11010100b
#define ADDR_LSM330DLC_G    0xD4

// address of ADDR_LSM330DLC_A: 00110000b
#define ADDR_LSM330DLC_A    0x30

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)

#define PI   (float)     3.14159265f
/************************** LSM330DLC �ڲ��Ĵ�����ַ **************************/

//Gyroscope

#define WHO_AM_I_G            0x0F
#define CTRL_REG1_G           0x20
#define CTRL_REG2_G           0x21
#define CTRL_REG3_G           0x22
#define CTRL_REG4_G           0x23
#define CTRL_REG5_G           0x24
#define REFERENCE_G           0x25
#define OUT_TEMP_G            0x26
#define STATUS_REG_G          0x27
#define OUT_X_L_G             0x28
#define OUT_X_H_G             0x29
#define OUT_Y_L_G             0x2A
#define OUT_Y_H_G             0x2B
#define OUT_Z_L_G             0x2C
#define OUT_Z_H_G             0x2D
#define FIFO_CTRL_REG_G       0x2E
#define FIFO_SRC_REG_G        0x2F
#define INT1_CFG_G            0x30
#define INT1_SRC_G            0x31
#define INT1_TSH_XH_G         0x32
#define INT1_TSH_XL_G         0x33
#define INT1_TSH_YH_G         0x34
#define INT1_TSH_YL_G         0x35
#define INT1_TSH_ZH_G         0x36
#define INT1_TSH_ZL_G         0x37
#define INT1_DURATION_G       0x38




//Accelerometer

#define CTRL_REG1_A           0x20
#define CTRL_REG2_A           0x21
#define CTRL_REG3_A           0x22
#define CTRL_REG4_A           0x23
#define CTRL_REG5_A           0x24
#define CTRL_REG6_A           0x25
#define REFERENCE_A           0x26
#define STATUS_REG_A          0x27
#define OUT_X_L_A             0x28
#define OUT_X_H_A             0x29
#define OUT_Y_L_A             0x2A
#define OUT_Y_H_A             0x2B
#define OUT_Z_L_A             0x2C
#define OUT_Z_H_A             0x2D
#define FIFO_CTRL_REG_A       0x2E
#define FIFO_SRC_REG_A        0x2F
#define INT1_CFG_A            0x30
#define INT1_SOURCE_A         0x31
#define INT1_THS_A            0x32
#define INT1_DURATION_A       0x33
#define INT2_CFG_A            0x34
#define INT2_SOURCE_A         0x35
#define INT2_THS_A            0x36
#define INT2_DURATION_A       0x37
#define CLICK_CFG_A           0x38
#define CLICK_SRC_A			  0x39
#define CLICK_THS_A			  0x3A
#define TIME_LIMIT_A		  0x3B
#define TIME_LATENCY_A		  0x3C
#define TIME_WINDOW_A 		  0x3D
#define Act_THS 		      0x3E
#define Act_DUR 		      0x3F

#define Acc_Value 3.5
extern float Init_RollAng,Init_PitchAng;                      //�豸��ʼƫת��
extern float volatile angle_x,angle_y,angle_z;                //������ƫת�Ƕ�
extern float anglex_buffer[200],angley_buffer[200],anglez_buffer[200];    //ƫת�ǶȻ�����
extern float RollAng,PitchAng,fSinRoll,fSinPitch;          //���ٶȼƵķ����Ǻ͸�����

extern double Acc_Buffer[3];                                       //����Ƕ�ֵ�ͼ��ٶ�ֵ
extern double Gyro_Buffer[3];
extern float fNormAcc;  
extern float High_Acc;
extern unsigned char High_Acc_Num,High_Acc_Flag;
//extern unsigned char num;                                                  //��Ư����
extern float volatile num0,num1,num2,num3;    
/* Exported functions ------------------------------------------------------- */
extern double meterTotal;
extern float gpsSpeed;
int isCarStatic(void);
extern 		int DoDr(void);

/************************************************************************************
**�������ƣ�void zero_drift(void)
**�������ܣ���Ư�Ʋ���
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
void zero_drift(void);
/************************************************************************************
**�������ƣ�void LSM330DLC_Init(void)
**�������ܣ�LSM330DLC��ʼ��
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
void LSM330DLC_Init(void);
/************************************************************************************
**�������ƣ�void GYRO_DataRead(void)
**�������ܣ���LSM330DLC��ȡ���ٶ�ֵ
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
void GYRO_DataRead(void);
/************************************************************************************
**�������ƣ�FlagStatus GYRO_DataOverrun(void)
**�������ܣ���ѯLSM330DLC�ļ��ٶ��Ƿ񳬳��޶�
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
FlagStatus GYRO_DataOverrun(void);
/************************************************************************************
**�������ƣ�FlagStatus GYRO_DataUpdate(void)
**�������ܣ���ѯLSM330DLC�ļ��ٶ������Ƿ��Ѿ�����
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
FlagStatus GYRO_DataUpdate(void);


/************************************************************************************
**�������ƣ�void ACC_DataRead(void)
**�������ܣ���LSM330DLC��ȡ���ٶ�ֵ
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
void ACC_DataRead(void);

/************************************************************************************
**�������ƣ�FlagStatus ACC_DataUpdate(void)
**�������ܣ���ѯLSM330DLC�Ľ��ٶ�ֵ�Ƿ��Ѿ�����
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
FlagStatus ACC_DataUpdate(void);
/************************************************************************************
**�������ƣ�FlagStatus ACC_DataOverrun(void)
**�������ܣ���ѯLSM330DLC�Ľ��ٶ��Ƿ񳬳��޶�
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
FlagStatus ACC_DataOverrun(void);


#endif
