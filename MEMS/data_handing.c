#include "data_handing.h"
#include "LSM330DLC.h"


float angle_buffer1 = 0,angle_buffer2 = 0,angle_buffer3 = 0; //������ʵƫת���õ��ı���
float Course_Angle = 360;                                    //��������Ƕ�

double lon = 6666.66666,lat = 66666.66666;                         //�ߵ���γ��
double lon1 = 0,lat1 = 0;                                          //���Ǿ�γ��
double lat2 = 0,lon2 =0,lon3=0,lat3=0,lon11=0,lon22=0,lon33=0,lat11=0,lat22=0,lat33=0;
uint32_t lon88=0,lat88=0;

uint8_t j = 0,p = 0,m = 0;
float Acc_X,Acc_Y,Acc_Z;

/************************************************************************************
**�������ƣ�void Get_Car_Gesture(void)
**�������ܣ����������̬
**��ڲ�����
**���ڲ�����   
**˵���� 	 
**           
*/
void Get_Car_Gesture(void)
{
	while(ACC_DataUpdate() != SET);
	ACC_DataRead();
		while(GYRO_DataUpdate() != SET);  //��ȡ�Ǽ��ٶ�
		GYRO_DataRead();

}
