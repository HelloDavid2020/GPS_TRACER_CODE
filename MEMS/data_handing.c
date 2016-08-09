#include "data_handing.h"
#include "LSM330DLC.h"


float angle_buffer1 = 0,angle_buffer2 = 0,angle_buffer3 = 0; //计算真实偏转角用到的变量
float Course_Angle = 360;                                    //汽车航向角度

double lon = 6666.66666,lat = 66666.66666;                         //惯导经纬度
double lon1 = 0,lat1 = 0;                                          //卫星经纬度
double lat2 = 0,lon2 =0,lon3=0,lat3=0,lon11=0,lon22=0,lon33=0,lat11=0,lat22=0,lat33=0;
uint32_t lon88=0,lat88=0;

uint8_t j = 0,p = 0,m = 0;
float Acc_X,Acc_Y,Acc_Z;

/************************************************************************************
**函数名称：void Get_Car_Gesture(void)
**函数功能：获得汽车姿态
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
void Get_Car_Gesture(void)
{
	while(ACC_DataUpdate() != SET);
	ACC_DataRead();
		while(GYRO_DataUpdate() != SET);  //获取角加速度
		GYRO_DataRead();

}
