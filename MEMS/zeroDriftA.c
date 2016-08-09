//zeroDriftA
#include <math.h>
extern int ReadACCRawG(double *ax,double *ay,double *az); //使用原始数据 ,直接转换为g, 读数单位是g, g直接取9.8就可以, 全国范围误差0.001  ,可以
extern void delayMS(int i);

void waitStationary(void)
{
double ax0,ay0,az0;
double ax,ay,az;
int nStat=0;
	ReadACCRawG(&ax0,&ay0,&az0);
	while(1)
	{
		delayMS(10);
		ReadACCRawG(&ax,&ay,&az);
		if(fabs(ax-ax0)<0.02 && fabs(ay-ay0)<0.02 && fabs(az-az0)<0.02)
			nStat++;
		else
			nStat=0;
		ax0=ax;
		ay0=ay;
		az0=az;
		if(nStat>10)
			break;
	}
}
#define CAL_FACTOR 0.2
int GetDir(void) //1--6  ,6个垂直方向   ,0或其他 非垂直方向
{
double ax,ay,az;
	ReadACCRawG(&ax,&ay,&az);
	if(fabs(ax)<CAL_FACTOR && fabs(ay)<CAL_FACTOR)
	{
		if(az>1-CAL_FACTOR && az<1+CAL_FACTOR)
			return 6;
		else 
		if(az<-(1-CAL_FACTOR) && az>-(1+CAL_FACTOR))
			return 2;
		else
			return 100;
	}
	if(fabs(ax)<CAL_FACTOR && fabs(az)<CAL_FACTOR)
	{
		if(ay>1-CAL_FACTOR && ay<1+CAL_FACTOR)
			return 4;
		else 
		if(ay<-(1-CAL_FACTOR) && ay>-(1+CAL_FACTOR))
			return 1;
		else
			return 100;
	}
	if(fabs(ay)<CAL_FACTOR && fabs(az)<CAL_FACTOR)
	{
		if(ax>1-CAL_FACTOR && ax<1+CAL_FACTOR)
			return 3;
		else 
		if(ax<-(1-CAL_FACTOR) && ax>-(1+CAL_FACTOR))
			return 5;
		else
			return 100;
	}
	return 0;
}
struct _accParameter_{
	float ax0;
	float ay0;
	float az0;
	float Sax;
	float Say;
	float Saz;
	float Kax1,Kax2,Kax3;
	float Kay1,Kay2,Kay3;
	float Kaz1,Kaz2,Kaz3;
};
extern struct _accParameter_ accParameter;
	

#include "stdio.h"
extern void SerialPort_SendStringEx(char ,char *line);
void Send_Com1(char *line,int len)
{
	SerialPort_SendStringEx(0,line);
}
double Ax[7],Ay[7],Az[7];	
int DoZeroDrift(void)
{
char line[100];
double sumAx,sumAy,sumAz;
double ax,ay,az;
int directDone;//位表示
int dir;

int i;
extern double Acc_Buffer[3];                                       //储存角度值和加速度值
extern double Gyro_Buffer[3] ;
	
	directDone=0;
	
//强制先读取传感器数据, 查看是否有坏传感器
	sprintf(line,"DoZeroDrift SensorValue:\r\n");
	Send_Com1(line,strlen(line));	
	Get_Car_Gesture();	
	sprintf(line,"Acc:%5.2f %5.2f %5.2f  Gyro: %5.2f %5.2f %5.2f\r\n",Acc_Buffer[0],Acc_Buffer[1],Acc_Buffer[2],
	Gyro_Buffer[0],Gyro_Buffer[1],Gyro_Buffer[2]);
	Send_Com1(line,strlen(line));	
	
	
	while(1)
	{
retest:		
		delayMS(10);
		{

		sprintf(line,"DoZeroDrift direcdone= %02x\r\n",directDone);
		Send_Com1(line,strlen(line));
		}
		if(directDone==0x7E)
			break;
			//取到一个方向的静态值并记录到Ax Ay Az
		waitStationary();
		dir=GetDir();
		if(dir<1 || dir>6 //不是6个稳态方向之一
			|| directDone&(1<<dir))//方向已经测试过, 换方向
			continue;
		//静态的新方向 , 需要3秒钟连续如此
		for(i=0;i<300;i++)
		{
			if(dir!=GetDir())
				goto retest;
			delayMS(10);
		}
		//读取若干数据并平均 , 但期间数据不能波动太大 
		sumAx=sumAy=sumAz=0;
		for(i=0;i<50;i++)
		{
			ReadACCRawG(&ax,&ay,&az);
			if(i && (fabs(ax-sumAx/i)>0.04 || fabs(ay-sumAy/i)>0.04 || fabs(az-sumAz/i)>0.04))
				goto retest;
			sumAx+=ax;
			sumAy+=ay;
			sumAz+=az;
			delayMS(10);
		}
		Ax[dir]=sumAx/50.0;
		Ay[dir]=sumAy/50.0;
		Az[dir]=sumAz/50.0;
		directDone|=(1<<dir);
	}
//lgq 我自己的算法 ,  
	// 零漂是4个水平放置位 (重力分量两两抵消) 的均值  ,这个和文献算法一样  ?  6个方向的均值更好?
		accParameter.ax0=(Ax[1]+Ax[4]+Ax[2]+Ax[6]+Ax[3]+Ax[5] )/6;
		accParameter.ay0=(Ay[3]+Ay[5]+Ay[2]+Ay[6]+Ay[1]+Ay[4])/6;
		accParameter.az0=(Az[1]+Az[4]+Az[3]+Az[5]+Az[2]+Az[6])/6;

	float d1,d2,d3,d;
	
	// 轴在3个垂直方向(6次放置两两平均 ) 分量的向量和 ,就是测量值 , 但实际值是g  ,所以这个值就是刻度因子	
	//这样算法不太依赖安装角度, 只要测量用的方体是三面垂直的就可以
	d1=(Ax[1]-Ax[4])/2 ;
	d2=(Ax[2]-Ax[6])/2 ;
	d3=(Ax[3]-Ax[5])/2 ;
	accParameter.Sax=sqrt(d1*d1+d2*d2+d3*d3);
	
	d1=(Ay[1]-Ay[4])/2 ;
	d2=(Ay[2]-Ay[6])/2 ;
	d3=(Ay[3]-Ay[5])/2 ;
	accParameter.Say=sqrt(d1*d1+d2*d2+d3*d3);

	d1=(Az[1]-Az[4])/2 ;
	d2=(Az[2]-Az[6])/2 ;
	d3=(Az[3]-Az[5])/2 ;
	accParameter.Saz=sqrt(d1*d1+d2*d2+d3*d3);
	return 1;
	
//直接根据文献 <<MEMS 加速度计的六位置测试法>> 计算参数     测控技术  2009年28卷 7期
	accParameter.ax0=(Ax[1]+Ax[2]+Ax[4]+Ax[6])/4;
	accParameter.Sax=(Ax[3]-Ax[5])/2;
	accParameter.Kax1=(Ax[4]-Ax[1])/2;
	accParameter.Kax2=(Ax[6]-Ax[2])/2;
	accParameter.Kax3=(-Ax[1]-Ax[2]+2*Ax[3]-Ax[4]+2*Ax[5]-Ax[6])/4;
	
	
	accParameter.ay0=(Ay[2]+Ay[3]+Ay[5]+Ay[6])/4;
	accParameter.Say=(Ay[4]-Ay[1])/2;
	accParameter.Kay1=(Ay[3]-Ay[5])/2;
	accParameter.Kay2=(Ay[6]-Ay[2])/2;
	accParameter.Kay3=(2*Ay[1]-Ay[2]-Ay[3]+2*Ay[4]-Ay[5]-Ay[6])/4;
	
	accParameter.az0=(Az[1]+Az[3]+Az[4]+Az[5])/4;
	accParameter.Saz=(Az[6]-Az[2])/2;
	accParameter.Kaz1=(Az[3]-Az[5])/2;
	accParameter.Kaz2=(Az[4]-Az[1])/2;
	accParameter.Kaz3=(-Az[1]+2*Az[2]-Az[3]-Az[4]-Az[5]+2*Az[6])/4;
	
	return 1;
}
	
	
	
	
	

			
		
	
	
	
	