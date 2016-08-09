/* main.c file
编写者：lisn3188
网址：www.chiplab7.net
作者E-mail：lisn3188@163.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2013-05-25
测试： 本程序已在第七实验室的mini INS/GPS上完成测试
功能：
姿态解算 AHRS
将传感器的输出值进行姿态解算。得到目标载体的俯仰角和横滚角 和航向角
------------------------------------
 */

#include "IMU.h"
//#include "delay.h"
unsigned long getTickCount(void);
void delayMS(int i)
{
	extern unsigned long SysTick_GetTicks(void);
	int t0=SysTick_GetTicks();
	while(SysTick_GetTicks()<t0+i);
}
volatile double exInt, eyInt, ezInt;  // 误差积分
volatile double q0, q1, q2, q3; // 全局四元数
volatile double integralFBhand,handdiff;
volatile uint32_t lastUpdate, now; // 采样周期计数 单位 us
struct Imu imu;

// Fast inverse square-root
/**************************实现函数********************************************
*函数原型:	   double invSqrt(double x)
*功　　能:	   快速计算 1/Sqrt(x) 	
输入参数： 要计算的值
输出参数： 结果
*******************************************************************************/
double invSqrt(double x) {
	return 1/sqrt(x);
	double halfx = 0.5f * x;
	double y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(double*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
double SensorValue[6];
void LSM330DLC_getMotion6(double v[6]);
/**************************实现函数********************************************
*函数原型:	   void IMU_init(void)
*功　　能:	  初始化IMU相关	
			  初始化各个传感器
			  初始化四元数
			  将积分清零
			  更新系统时间
输入参数：无
输出参数：没有
*******************************************************************************/
unsigned long getTickCount(void)
{
	return SysTick_GetTicks();
}
void IMU_init(void)
{	 
//	MPU6050_initialize();
//	HMC5983L_SetUp();
//	BMP180_init();
//	delay_ms(50);
//	MPU6050_initialize();
//	HMC5983L_SetUp();
//	BMP180_init();
	// initialize quaternion

  	q0 = 1.0f;  //初始化四元数
  	q1 = 0.0f;
  	q2 = 0.0f;
  	q3 = 0.0f;
  	exInt = 0.0;
  	eyInt = 0.0;
  	ezInt = 0.0;
  	lastUpdate = getTickCount();//更新时间
  	now = getTickCount();

	/* initialises neutrals */
  	RATES_ASSIGN(imu.gyro_neutral,  IMU_GYRO_P_NEUTRAL,  IMU_GYRO_Q_NEUTRAL,  IMU_GYRO_R_NEUTRAL);

  	VECT3_ASSIGN(imu.accel_neutral, IMU_ACCEL_X_NEUTRAL, IMU_ACCEL_Y_NEUTRAL, IMU_ACCEL_Z_NEUTRAL);
	INT_VECT3_ZERO(imu.mag_neutral);
	/*
    Compute quaternion and rotation matrix
    for conversions between body and imu frame
  */
  	struct Int32Eulers body_to_imu_eulers =
    { ANGLE_BFP_OF_REAL(IMU_BODY_TO_IMU_PHI),
      ANGLE_BFP_OF_REAL(IMU_BODY_TO_IMU_THETA),
      ANGLE_BFP_OF_REAL(IMU_BODY_TO_IMU_PSI) };
  INT32_QUAT_OF_EULERS(imu.body_to_imu_quat, body_to_imu_eulers);
  INT32_QUAT_NORMALIZE(imu.body_to_imu_quat);
  INT32_RMAT_OF_EULERS(imu.body_to_imu_rmat, body_to_imu_eulers);

	double values[6];
	int i,j;
	for(j=0;j<6;j++)
			SensorValue[j]=0;;			
  for(i=0;i<10;i++)		
	{			
		delayMS(10);
		LSM330DLC_getMotion6(values);
		for(j=0;j<6;j++)
			SensorValue[j]+=values[j];
	}
	for(i=0;i<6;i++)
		SensorValue[i]/=10;
	delayMS(10);
}

float qq[4];
void IMU_ReInit(void)
{	 
//	MPU6050_initialize();
//	HMC5983L_SetUp();
//	BMP180_init();
//	delay_ms(50);
//	MPU6050_initialize();
//	HMC5983L_SetUp();
//	BMP180_init();
	// initialize quaternion
	/*
  	q0 = 1.0f;  //初始化四元数
  	q1 = 0.0f;
  	q2 = 0.0f;
  	q3 = 0.0f;
	*/
	q0=qq[0];
	q1=qq[1];
	q2=qq[2];
	q3=qq[3];
  	exInt = 0.0;
  	eyInt = 0.0;
  	ezInt = 0.0;
  	lastUpdate = getTickCount();//更新时间
  	now = getTickCount();

	/* initialises neutrals */
  	RATES_ASSIGN(imu.gyro_neutral,  IMU_GYRO_P_NEUTRAL,  IMU_GYRO_Q_NEUTRAL,  IMU_GYRO_R_NEUTRAL);

  	VECT3_ASSIGN(imu.accel_neutral, IMU_ACCEL_X_NEUTRAL, IMU_ACCEL_Y_NEUTRAL, IMU_ACCEL_Z_NEUTRAL);
	INT_VECT3_ZERO(imu.mag_neutral);
	/*
    Compute quaternion and rotation matrix
    for conversions between body and imu frame
  */
  	struct Int32Eulers body_to_imu_eulers =
    { ANGLE_BFP_OF_REAL(IMU_BODY_TO_IMU_PHI),
      ANGLE_BFP_OF_REAL(IMU_BODY_TO_IMU_THETA),
      ANGLE_BFP_OF_REAL(IMU_BODY_TO_IMU_PSI) };
  INT32_QUAT_OF_EULERS(imu.body_to_imu_quat, body_to_imu_eulers);
  INT32_QUAT_NORMALIZE(imu.body_to_imu_quat);
  INT32_RMAT_OF_EULERS(imu.body_to_imu_rmat, body_to_imu_eulers);

}


/**************************实现函数********************************************
*函数原型:	   void IMU_getValues(double * values)
*功　　能:	 读取加速度 陀螺仪 磁力计 的当前值  
输入参数： 将结果存放的数组首地址
输出参数：没有
*******************************************************************************/

void IMU_getValues(double * values) {  
	static long lastTime;
	int16_t accgyroval[6];
	int i;
	//读取加速度和陀螺仪的当前ADC
	
	//10ms 读一次
//测试占时 
	if(lastTime!=0)
	{
		extern int ncc;
		extern long totalFree;
		if(getTickCount()<lastTime+10)
		totalFree+=lastTime+10-getTickCount();
		ncc++;
	}
	lastTime=getTickCount();
	
    LSM330DLC_getMotion6(values);
	
	//做移动平均
	for(i=0;i<6;i++)//这个移动平均不好, 
	{
		//values[i]=values[i]/5+SensorValue[i]*4/5;
		values[i]=values[i]/3+SensorValue[i]*2/3;
		SensorValue[i]=values[i];
	}
}


/**************************实现函数********************************************
*函数原型:	   void IMU_AHRSupdate
*功　　能:	 更新AHRS 更新四元数 
输入参数： 当前的测量值。
输出参数：没有
*******************************************************************************/
#define Kp 1.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.53f   // integral gain governs rate of convergence of gyroscope biases
void IMU_AHRSupdate(double gx, double gy, double gz, double ax, double ay, double az){//, double mx, double my, double mz) {
  double norm;
 double hx, hy, hz, bx, bz;
  double vx, vy, vz, wx, wy, wz;
  double ex, ey, ez,halfT;
  double tempq0,tempq1,tempq2,tempq3;

  // 先把这些用得到的值算好
  double q0q0 = q0*q0;
  double q0q1 = q0*q1;
  double q0q2 = q0*q2;
  double q0q3 = q0*q3;
  double q1q1 = q1*q1;
  double q1q2 = q1*q2;
  double q1q3 = q1*q3;
  double q2q2 = q2*q2;   
  double q2q3 = q2*q3;
  double q3q3 = q3*q3;          
  
  now = getTickCount();  //读取时间
  if(now<lastUpdate){ //定时器溢出过了。
  halfT =  ((double)(now + (0xffffffff- lastUpdate)) / 2000.0f);
  }
  else	{
  halfT =  ((double)(now - lastUpdate) / 2000.0f);
  }
  lastUpdate = now;	//更新时间

  norm = invSqrt(ax*ax + ay*ay + az*az);       
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;
  //把加计的三维向量转成单位向量。
/*
  norm = invSqrt(mx*mx + my*my + mz*mz);          
  mx = mx * norm;
  my = my * norm;
  mz = mz * norm;
*/
  /*
  这是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量。
  */
  // compute reference direction of flux
	/*
  hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
  hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
  hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);         
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz;     
  */
  // estimated direction of gravity and flux (v and w)
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
/*
	wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
  wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
  wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
  */
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy);// + (my*wz - mz*wy);
  ey = (az*vx - ax*vz);// + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) ;//+ (mx*wy - my*wx);

  /*
  axyz是机体坐标参照系上，加速度计测出来的重力向量，也就是实际测出来的重力向量。
axyz是测量得到的重力向量，vxyz是陀螺积分后的姿态来推算出的重力向量，它们都是机体坐标参照系上的重力向量。
那它们之间的误差向量，就是陀螺积分后的姿态和加计测出来的姿态之间的误差。
向量间的误差，可以用向量叉积（也叫向量外积、叉乘）来表示，exyz就是两个重力向量的叉积。
这个叉积向量仍旧是位于机体坐标系上的，而陀螺积分误差也是在机体坐标系，而且叉积的大小与陀螺积分误差成正比，正好拿来纠正陀螺。（你可以自己拿东西想象一下）由于陀螺是对机体直接积分，所以对陀螺的纠正量会直接体现在对机体坐标系的纠正。
  */
//if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
  exInt = exInt + ex * Ki * halfT;
  eyInt = eyInt + ey * Ki * halfT;	
  ezInt = ezInt + ez * Ki * halfT;

  // 用叉积误差来做PI修正陀螺零偏
  gx = gx + Kp*ex + exInt;
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;

  }

  // 四元数微分方程
  tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
  
  // 四元数规范化
  norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
  q0 = tempq0 * norm;
  q1 = tempq1 * norm;
  q2 = tempq2 * norm;
  q3 = tempq3 * norm;
	
	extern float tt[3][3];
			tt[0][0]=q0*q0+q1*q1-q2*q2-q3*q3;
			tt[1][0]=2*(q1*q2-q0*q3);
			tt[2][0]=2*(q1*q3+q0*q2);
			tt[0][1]=2*(q1*q2+q0*q3);
			tt[1][1]=q0*q0-q1*q1+q2*q2-q3*q3;
			tt[2][1]=2*(q3*q2-q0*q1);
			tt[0][2]=2*(q1*q3-q0*q2);
			tt[1][2]=2*(q2*q3+q0*q1);
			tt[2][2]=q0*q0-q1*q1-q2*q2+q3*q3;   
 	
}

/**************************实现函数********************************************
*函数原型:	   void IMU_getQ(double * q)
*功　　能:	 更新四元数 返回当前的四元数组值
输入参数： 将要存放四元数的数组首地址
输出参数：没有
*******************************************************************************/
double mygetqval[9];	//用于存放传感器转换结果的数组

void IMU_getQ(double * q) {

  IMU_getValues(mygetqval);	 
  //将陀螺仪的测量值转成弧度每秒
  //加速度和磁力计保持 ADC值　不需要转换
	IMU_AHRSupdate(mygetqval[3]* M_PI/180, mygetqval[4]* M_PI/180, mygetqval[5]* M_PI/180 ,mygetqval[0], mygetqval[1], mygetqval[2] );
//IMU_AHRSupdate(mygetqval[3] * M_PI/180, mygetqval[4] * M_PI/180, mygetqval[5] * M_PI/180,
  // mygetqval[0], mygetqval[1], mygetqval[2], mygetqval[6], mygetqval[7], mygetqval[8]);

  q[0] = q0; //返回当前值
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;
}

#include "stdio.h"
#include "serialPortEx.h"
/**************************实现函数********************************************
*函数原型:	   void IMU_getYawPitchRoll(double * angles)
*功　　能:	 更新四元数 返回当前解算后的姿态数据
输入参数： 将要存放姿态角的数组首地址
输出参数：没有
*******************************************************************************/
void IMU_getYawPitchRoll(double * angles
	) {
  double q[4]; //　四元数
  volatile double gx=0.0, gy=0.0, gz=0.0; //估计重力方向
  IMU_getQ(q); //更新全局四元数

  angles[0] = -atan2(2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2]*q[2] - 2 * q[3] * q[3] + 1)* 180/M_PI; // yaw
  angles[1] = -asin(-2 * q[1] * q[3] + 2 * q[0] * q[2])* 180/M_PI; // pitch
  angles[2] =	atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1)* 180/M_PI; // roll
/*		
float t=angles[1]/2*M_PI/180;
float p=angles[2]/2*M_PI/180;
		
double a[3];
qq[0]=cos(p)*cos(t);
qq[1]=sin(p)*cos(t);
qq[2]=-cos(p)*sin(t);
qq[3]=sin(p)*sin(t);

  a[1] = -asin(-2 * qq[1] * qq[3] + 2 * qq[0] * qq[2])* 180/M_PI; // pitch
  a[2] =	atan2(2 * qq[2] * qq[3] + 2 * qq[0] * qq[1], -2 * qq[1] * qq[1] - 2 * qq[2] * qq[2] + 1)* 180/M_PI; // roll

char line[128];
sprintf(line,"a: %5.2f %5.2f %5.2f   aa: %5.2f %5.2f\r\n",angles[0],angles[1],angles[2],a[1],a[2]);
SerialPort_SendStringEx(COM1,line);
*/
}


/*
void Get_IMU_Sesor(void){
	int16_t accgyroval[9];
	//读取加速度和陀螺仪的当前ADC
    MPU6050_getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], &accgyroval[3], &accgyroval[4], &accgyroval[5]);
    HMC5983_mgetValuesint(&accgyroval[6]);	//读取磁力计的ADC值	
	
	RATES_ASSIGN(imu.gyro_unscaled, accgyroval[3], accgyroval[4], accgyroval[5]);
  	VECT3_ASSIGN(imu.accel_unscaled, accgyroval[0], accgyroval[1], accgyroval[2]);
  	VECT3_ASSIGN(imu.mag_unscaled, accgyroval[6], accgyroval[7], accgyroval[8]);
	ImuScaleAccel(imu);
	ImuScaleGyro(imu);
	ImuScaleMag(imu);

}
*/
//------------------End of File----------------------------
