#include "LSM330DLC.h"
#include "includes.h"



extern double SensorValue[6];
uint32_t time_count = 0;

/************************************************************************************
**函数名称：static void LSM330DLC_LowLevel_Init(void)
**函数功能：LSM330DLC硬件初始化
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
static void LSM330DLC_LowLevel_Init(void);
/************************************************************************************
**函数名称：static uint8_t LSM330DLC_SendByte(uint8_t byte)
**函数功能：向LSM330DLC发送一个字节
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
static uint8_t LSM330DLC_SendByte(uint8_t byte);
/************************************************************************************
**函数名称：void LSM330DLC_Write(uint8_t ADDR_LSM330DLC, uint8_t WriteAddr, uint8_t inBuf)
**函数功能：向LSM330DLC写一个字节
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
void LSM330DLC_Write(uint8_t ADDR_LSM330DLC, uint8_t WriteAddr, uint8_t inBuf);

/************************************************************************************
**函数名称：unsigned char LSM330DLC_Read(uint8_t ADDR_LSM330DLC, uint8_t ReadAddr)
**函数功能：从LSM330DLC读一个字节
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
unsigned char LSM330DLC_Read(uint8_t ADDR_LSM330DLC, uint8_t ReadAddr);

/* Defines -------------------------------------------------------------------*/

//#define GYRORATE 	70.0/1000		//+-2000DPS
#define GYRORATE 	(17.5/1000)		//+-500DPS
//#define ACCRATE 	1.0/100




/* Globals -------------------------------------------------------------------*/
float Init_RollAng = 0,Init_PitchAng = 0;                          //设备初始偏转角
float volatile angle_x = 0,angle_y = 0,angle_z = 0;                //陀螺仪偏转角度
float anglex_buffer[200],angley_buffer[200],anglez_buffer[200];    //偏转角度缓冲区
float RollAng = 0.0f, PitchAng = 0.0f,fSinRoll,fSinPitch;          //加速度计的翻滚角和俯仰角


double Acc_Buffer[3] = {0.0f};                                       //储存角度值和加速度值
double Gyro_Buffer[3] = {0.0f};
float fNormAcc = 0;                                                 //汽车加速度的和
float High_Acc = 0;
unsigned char High_Acc_Num = 0,High_Acc_Flag = 0;
//unsigned char num = 0;                                                   //零漂计数
float num00=0,num11=0,num22=0;      //消除零漂的变量

float tts[3][3]={{1,0,0},{0,1,0},{0,0,1}};	//稳定可用值
int bUseStaticAngle=0;

float iaxx=0;
float iayy=0;


/************************************************************************************
**函数名称：void LSM330DLC_Init(void)
**函数功能：LSM330DLC初始化
**入口参数：
**出口参数：   
**说明： 	 
**           
*/

void SaveAngleState(double pitch,double roll)
{
	//只需算出 t11 t12 t13
float q0,q1,q2,q3;

char line[128];
//	sprintf(line,"**************pitch =%5.2f, roll=%5.2f \r\n",pitch*180/pi,roll*180/pi);
float t=pitch/2;
float p=roll/2;
//	SerialPort_SendStringEx(COM1,line);
	q0=cos(p)*cos(t);
	q1=sin(p)*cos(t);
	q2=-cos(p)*sin(t);
	q3=sin(p)*sin(t);	

	tts[0][0]=q0*q0+q1*q1-q2*q2-q3*q3;
	tts[1][0]=2*(q1*q2-q0*q3);
	tts[2][0]=2*(q1*q3+q0*q2);
	tts[0][1]=2*(q1*q2+q0*q3);
	tts[1][1]=q0*q0-q1*q1+q2*q2-q3*q3;
	tts[2][1]=2*(q3*q2-q0*q1);
	tts[0][2]=2*(q1*q3-q0*q2);
	tts[1][2]=2*(q2*q3+q0*q1);
	tts[2][2]=q0*q0-q1*q1-q2*q2+q3*q3;   
	bUseStaticAngle=1;
}
void LSM330DLC_Init(void)
{
  LSM330DLC_LowLevel_Init();
	
	LSM330DLC_Write(ADDR_LSM330DLC_G, CTRL_REG2_G, 0x00);           
  
  LSM330DLC_Write(ADDR_LSM330DLC_G, CTRL_REG3_G, 0x00);          		// Disable Interrupt
  
//  LSM330DLC_Write(ADDR_LSM330DLC_G, CTRL_REG4_G, 0x20);	      		// FS = +-2000dps  BDU Enable  Little Endian
  LSM330DLC_Write(ADDR_LSM330DLC_G, CTRL_REG4_G, 0x10);	      		// FS = +-500dps  BDU Enable  Little Endian
  
  LSM330DLC_Write(ADDR_LSM330DLC_G, CTRL_REG5_G, 0x00);          	
  
  //Gyroscope Enable
//  LSM330DLC_Write(ADDR_LSM330DLC_G, CTRL_REG1_G, 0xCF);          		// ODR: 760Hz	 
	  LSM330DLC_Write(ADDR_LSM330DLC_G, CTRL_REG1_G, 0x6F);          		// ODR: 190Hz	 
  
  
  /*
  Accelerometer Configuration 
  
  */


  LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG2_A, 0x00);				// High pass filter disable


// D7:CLICK interrupt on INT1_A 
// D6:AOI1 interrupt on INT1_A
  LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG3_A, 0xC0);				// Enable Interrupt
  
  LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG4_A, 0x08);				// FS = +-2 g  Little Endian  High resolution disable
  
  LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG5_A, 0x00);
  
  LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG6_A, 0x00);

//		LSM330DLC_Write(ADDR_LSM330DLC_A, INT1_CFG_A, 0x00);
  LSM330DLC_Write(ADDR_LSM330DLC_A, INT1_DURATION_A, 0x0f);			//Set Int1
  
  //Accelerometer Enable
//  LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG1_A, 0x77);          		// 0111 0111b   ODR: 400Hz
 // LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG1_A, 0x6f);          		// 0110    ODR: 200Hz
   LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG1_A, 0x67); 
 //0x40  190hz    , cutoff  0--3  12.5 25 50 70 what mean ?
 // LSM330DLC_Write(ADDR_LSM330DLC_A, CTRL_REG1_A, 0x77);          		// 0110    ODR: 200Hz
  

}





/************************************************************************************
**函数名称：static void LSM330DLC_LowLevel_Init(void)
**函数功能：LSM330DLC硬件初始化
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
static void LSM330DLC_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  /* Enable the SPI periph */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOE, ENABLE);


  /* Configure SPI2 pins: NSS, SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	
  /* SPI2 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  
  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);   


  /* Configure GPIO PIN for Lis Chip select */ // PB6
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	
  /* Configure GPIO PIN for Lis Chip select */   // PB8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
  /* Configure GPIO PIN for Lis Chip select */ // PA4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  /* Configure GPIO PINs to detect Interrupts */ // PA12
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  // PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // PB7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;// PE6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Deselect : Chip Select high */
	 GPIO_SetBits(GPIOC, GPIO_Pin_7); // PB8
	 GPIO_SetBits(GPIOC, GPIO_Pin_6); // PA4
	 

}  

/************************************************************************************
**函数名称：static uint8_t LSM330DLC_SendByte(uint8_t byte)
**函数功能：向LSM330DLC发送一个字节
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
static uint8_t LSM330DLC_SendByte(uint8_t byte)
{
	uint16_t retry = 0;
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry >200)return 0;
	}

  /* Send byte through the SPI3 peripheral */
  SPI_I2S_SendData(SPI2, byte);

  /* Wait to receive a byte */	
	retry =0;
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry >200)return 0;
	}

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/************************************************************************************
**函数名称：void LSM330DLC_Write(uint8_t ADDR_LSM330DLC, uint8_t WriteAddr, uint8_t inBuf)
**函数功能：向LSM330DLC写一个字节
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
void LSM330DLC_Write(uint8_t ADDR_LSM330DLC, uint8_t WriteAddr, uint8_t inBuf)
{
	if(ADDR_LSM330DLC != ADDR_LSM330DLC_A)
	{
		 GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		 GPIO_SetBits(GPIOC, GPIO_Pin_6);  
	}
  else
	{
		 GPIO_SetBits(GPIOC, GPIO_Pin_7);
		 GPIO_ResetBits(GPIOC, GPIO_Pin_6); 		
	}
  /* Send the Address of the indexed register */
  LSM330DLC_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  LSM330DLC_SendByte(inBuf);
  
  /* Set chip select High at the end of the transmission */ 
	 GPIO_SetBits(GPIOC, GPIO_Pin_7);
	 GPIO_SetBits(GPIOC, GPIO_Pin_6); 	
}

/************************************************************************************
**函数名称：unsigned char LSM330DLC_Read(uint8_t ADDR_LSM330DLC, uint8_t ReadAddr)
**函数功能：从LSM330DLC读一个字节
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
unsigned char LSM330DLC_Read(uint8_t ADDR_LSM330DLC, uint8_t ReadAddr)
{  
	unsigned char i = 0;
  /* Set chip select Low at the start of the transmission */
	if(ADDR_LSM330DLC != ADDR_LSM330DLC_A)
	{
		 GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		 GPIO_SetBits(GPIOC, GPIO_Pin_6);  
	}
  else
	{
		 GPIO_SetBits(GPIOC, GPIO_Pin_7);
		 GPIO_ResetBits(GPIOC, GPIO_Pin_6); 		
	}
  ReadAddr |= (uint8_t)READWRITE_CMD;
  /* Send the Address of the indexed register */
  LSM330DLC_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
    /* Send dummy byte (0x00) to generate the SPI clock to L3GD20 (Slave device) */
  i = LSM330DLC_SendByte(0);
  
  /* Set chip select High at the end of the transmission */ 
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_6);

  return i; 	
}  






/* Gyroscope Functions ------------------------------------------------------------*/


/************************************************************************************
**函数名称：void GYRO_DataRead(void)
**函数功能：从LSM330DLC获取加速度值
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
float averGx,averGy,averGz;
float varGx=1,varGy=1,varGz=1;
float lastGx[AVER_NUMBER],lastGy[AVER_NUMBER],lastGz[AVER_NUMBER];
int 	nGx;
void GYRO_DataRead(void)
{
  uint8_t G_X_Buffer[2];
  uint8_t G_Y_Buffer[2];
  uint8_t G_Z_Buffer[2];
	
	// raw data
	int16_t G_X;
	int16_t G_Y;
	int16_t G_Z;

  
  G_X_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_X_L_G);
  G_X_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_X_H_G);
  G_X = (int16_t)( G_X_Buffer[1]<<8 | G_X_Buffer[0] );
  
  G_Y_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Y_L_G);
  G_Y_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Y_H_G);
  G_Y = (int16_t)( G_Y_Buffer[1]<<8 | G_Y_Buffer[0] );
  
  G_Z_Buffer[0] = (int8_t)(LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Z_L_G));
  G_Z_Buffer[1] = (int8_t)(LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Z_H_G));
  G_Z = (int16_t)( G_Z_Buffer[1]<<8 | G_Z_Buffer[0] );
  
		
  /* Convert Unit */
//  Gyro_Buffer[0] = G_X * GYRORATE - num00; 
//  Gyro_Buffer[1] = G_Y * GYRORATE - num11; 
//  Gyro_Buffer[2] = G_Z * GYRORATE - num22; 
	
	
  Gyro_Buffer[0] = G_X * GYRORATE; 
  Gyro_Buffer[1] = G_Y * GYRORATE; 
  Gyro_Buffer[2] = G_Z * GYRORATE; 

#if 1	
	lastGx[nGx%AVER_NUMBER]=Gyro_Buffer[0];
	lastGy[nGx%AVER_NUMBER]=Gyro_Buffer[1];
	lastGz[nGx%AVER_NUMBER]=Gyro_Buffer[2];
	
	if(nGx>AVER_NUMBER)
	{
		int i;
		float sum,aver,var;
		sum=0;
		for(i=0;i<AVER_NUMBER;i++)
			sum+=lastGx[i];
		averGx=aver=sum/AVER_NUMBER;
		var=0;
		for(i=0;i<AVER_NUMBER;i++)
			var+=(lastGx[i]-aver)*(lastGx[i]-aver);
		varGx=var=var/AVER_NUMBER;
		
		sum=0;
		for(i=0;i<AVER_NUMBER;i++)
			sum+=lastGy[i];
		averGy=aver=sum/AVER_NUMBER;
		var=0;
		for(i=0;i<AVER_NUMBER;i++)
			var+=(lastGy[i]-aver)*(lastGy[i]-aver);
		varGy=var=var/AVER_NUMBER;
		
		sum=0;
		for(i=0;i<AVER_NUMBER;i++)
			sum+=lastGz[i];
		averGz=aver=sum/AVER_NUMBER;
		var=0;
		for(i=0;i<AVER_NUMBER;i++)
			var+=(lastGz[i]-aver)*(lastGz[i]-aver);
		varGz=var=var/AVER_NUMBER;
	}
	nGx++;
#else
//对原始采样值做移动平均, 用于动态校0    //移动平均用20个数做
	averGx=averGx*0.95+Gyro_Buffer[0]*0.05;
	averGy=averGy*0.95+Gyro_Buffer[1]*0.05;
	averGz=averGz*0.95+Gyro_Buffer[2]*0.05;	
	float d;
	d=(averGx-Gyro_Buffer[0]);d=d*d;
	varGx=varGx*0.99+d*0.01;			//方差用100个数算
	d=(averGy-Gyro_Buffer[1]);d=d*d;
	varGy=varGy*0.99+d*0.01;			//方差用100个数算
	d=(averGz-Gyro_Buffer[2]);d=d*d;
	varGz=varGz*0.99+d*0.01;			//方差用100个数算
#endif	
	
	
  Gyro_Buffer[0] +=- num00; 
  Gyro_Buffer[1] +=- num11; 
  Gyro_Buffer[2] +=- num22; 		
}

/************************************************************************************
**函数名称：FlagStatus GYRO_DataUpdate(void)
**函数功能：查询LSM330DLC的加速度数据是否已经更新
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
FlagStatus GYRO_DataUpdate(void)
{
  uint8_t status;
  status = ( LSM330DLC_Read(ADDR_LSM330DLC_G, STATUS_REG_G) & (1<<3) );
  if (status)
    return SET;
  else 
    return RESET;
}

/*
int GyroRead(double *gx,double *gy,double *gz)
{
	while(SET!=GYRO_DataUpdate());
  uint8_t G_X_Buffer[2];
  uint8_t G_Y_Buffer[2];
  uint8_t G_Z_Buffer[2];
	
	// raw data
	int16_t G_X;
	int16_t G_Y;
	int16_t G_Z;

  
  G_X_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_X_L_G);
  G_X_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_X_H_G);
  G_X = (int16_t)( G_X_Buffer[1]<<8 | G_X_Buffer[0] );
  
  G_Y_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Y_L_G);
  G_Y_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Y_H_G);
  G_Y = (int16_t)( G_Y_Buffer[1]<<8 | G_Y_Buffer[0] );
  
  G_Z_Buffer[0] = (int8_t)(LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Z_L_G));
  G_Z_Buffer[1] = (int8_t)(LSM330DLC_Read(ADDR_LSM330DLC_G, OUT_Z_H_G));
  G_Z = (int16_t)( G_Z_Buffer[1]<<8 | G_Z_Buffer[0] );
	
  *gx = G_X * GYRORATE; 
  *gy = G_Y * GYRORATE; 
  *gz = G_Z * GYRORATE; 
//对原始采样值做移动平均, 用于动态校0
	averGx=averGx*0.9+*gx*0.1;
	averGy=averGy*0.9+*gy*0.1;
	averGz=averGz*0.9+*gz*0.1;
	
	
  *gx +=- num00; 
  *gy +=- num11; 
  *gz +=- num22; 	
	return 1;
	
}
*/
/************************************************************************************
**函数名称：FlagStatus GYRO_DataOverrun(void)
**函数功能：查询LSM330DLC的加速度是否超出限度
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
FlagStatus GYRO_DataOverrun(void)
{
  uint8_t status;
  status = ( LSM330DLC_Read(ADDR_LSM330DLC_G, STATUS_REG_G) & (1<<7) );
  if (status)
    return SET;
  else 
    return RESET;
}

/* Accelerometer Functions ----------------------------------------------------------*/


/************************************************************************************
**函数名称：void ACC_DataRead(void)
**函数功能：从LSM330DLC获取角速度值
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
/*
void ACC_DataRead(void)
{
  uint8_t A_X_Buffer[2];
  uint8_t A_Y_Buffer[2];
  uint8_t A_Z_Buffer[2];

	int16_t A_X;
	int16_t A_Y;
	int16_t A_Z;
  
  A_X_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_X_L_A);
  A_X_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_X_H_A);
  A_X = (int16_t)( A_X_Buffer[1]<<8 | A_X_Buffer[0] );
  A_X = A_X >> 4;
  
  A_Y_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Y_L_A);
  A_Y_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Y_H_A);
  A_Y = (int16_t)( A_Y_Buffer[1]<<8 | A_Y_Buffer[0] );
  A_Y = A_Y >> 4;
  
  A_Z_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Z_L_A);
  A_Z_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Z_H_A);
  A_Z = (int16_t)( A_Z_Buffer[1]<<8 | A_Z_Buffer[0] );
  A_Z = A_Z >> 4;
  
 //
  Acc_Buffer[0] = A_X * ACCRATE; 
  Acc_Buffer[1] = A_Y * ACCRATE; 
  Acc_Buffer[2] = A_Z * ACCRATE;
  
}
*/



/************************************************************************************
**函数名称：FlagStatus ACC_DataUpdate(void)
**函数功能：查询LSM330DLC的角速度值是否已经更新
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
FlagStatus ACC_DataUpdate(void)
{
  uint8_t status;
  status = ( LSM330DLC_Read(ADDR_LSM330DLC_A, STATUS_REG_A) & (1<<3) );
  if (status)
    return SET;
  else 
    return RESET;
}



/************************************************************************************
**函数名称：FlagStatus ACC_DataOverrun(void)
**函数功能：查询LSM330DLC的角速度是否超出限度
**入口参数：
**出口参数：   
**说明： 	 
**           
*/
FlagStatus ACC_DataOverrun(void)
{
  uint8_t status;
  status = ( LSM330DLC_Read(ADDR_LSM330DLC_A, STATUS_REG_A) & (1<<7) );
  if (status)
    return SET;
  else 
    return RESET;
}

//读加速度传感器
//使用原始数据 ,直接转换为g, 读数单位是g, g直接取9.8就可以, 全国范围误差0.0015  ,可以
int ReadACCRawG(double *ax,double *ay,double *az)
{  
	uint8_t A_X_Buffer[2];
  uint8_t A_Y_Buffer[2];
  uint8_t A_Z_Buffer[2];

	int16_t A_X;
	int16_t A_Y;
	int16_t A_Z;
	while(SET!=ACC_DataUpdate());

  
  A_X_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_X_L_A);
  A_X_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_X_H_A);
  A_X = (int16_t)( A_X_Buffer[1]<<8 | A_X_Buffer[0] );
  A_X = A_X >> 4;
  
  A_Y_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Y_L_A);
  A_Y_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Y_H_A);
  A_Y = (int16_t)( A_Y_Buffer[1]<<8 | A_Y_Buffer[0] );
  A_Y = A_Y >> 4;
  
  A_Z_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Z_L_A);
  A_Z_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Z_H_A);
  A_Z = (int16_t)( A_Z_Buffer[1]<<8 | A_Z_Buffer[0] );
  A_Z = A_Z >> 4;	
	

	
	
	//测量范围  +-2g ,   12位  +-2048
	*ax=A_X/1024.0;
	*ay=A_Y/1024.0;
	*az=A_Z/1024.0;
	return 1;
}
#define g_standard 9.8
struct _accParameter_{
	float ax0;
	float ay0;
	float az0;
	float Sax;
	float Say;
	float Saz;
//	float Kax1,Kax2,Kax3;
//	float Kay1,Kay2,Kay3;
//	float Kaz1,Kaz2,Kaz3;
};
struct _accParameter_ accParameter={
	
//	0.013, -0.005,-0.056,0.994,1.003,0.981		// 2号板子  2015.02.04
//	0.005,-0.043,0.014 , 1.01,1.038,0.994 		//1号板子 	2015.02.04
//	0.003,-0.042,0.010,1.01,1.039,0.995			//1号板子 后测
//	-0.006 , 0.000,  0.029, 0.983 , 0.978,  0.998 
//	a0: -0.009 -0.005  0.019 Sa:  0.983  0.978  0.997 
	-0.010, -0.006,  0.014, 0.982,  0.977,  0.998 
};
float averAx=0,averAy=0,averAz=9.8;
float varAx=1,varAy=1,varAz=1;
float lastAx[AVER_NUMBER],lastAy[AVER_NUMBER],lastAz[AVER_NUMBER];
int nAx;
int ReadACC(double *ax,double *ay,double *az) //去除0漂后的数据, 单位 米/秒方
{
  uint8_t A_X_Buffer[2];
  uint8_t A_Y_Buffer[2];
  uint8_t A_Z_Buffer[2];

	int16_t A_X;
	int16_t A_Y;
	int16_t A_Z;
  
  A_X_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_X_L_A);
  A_X_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_X_H_A);
  A_X = (int16_t)( A_X_Buffer[1]<<8 | A_X_Buffer[0] );
  A_X = A_X >> 4;
  
  A_Y_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Y_L_A);
  A_Y_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Y_H_A);
  A_Y = (int16_t)( A_Y_Buffer[1]<<8 | A_Y_Buffer[0] );
  A_Y = A_Y >> 4;
  
  A_Z_Buffer[0] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Z_L_A);
  A_Z_Buffer[1] = LSM330DLC_Read(ADDR_LSM330DLC_A, OUT_Z_H_A);
  A_Z = (int16_t)( A_Z_Buffer[1]<<8 | A_Z_Buffer[0] );
  A_Z = A_Z >> 4;	
	

	//测量范围  +-2g ,   12位  +-2048
	*ax=A_X/1024.0;
	*ay=A_Y/1024.0;
	*az=A_Z/1024.0;
//用0漂和刻度因子校准
//	*ax=g_standard*(accParameter.ax0+accParameter.Sax* *ax);
//	*ay=g_standard*(accParameter.ay0+accParameter.Say* *ay);
//	*az=g_standard*(accParameter.az0+accParameter.Saz* *az);
//	
	*ax=g_standard*(*ax-accParameter.ax0)/accParameter.Sax;
	*ay=g_standard*(*ay-accParameter.ay0)/accParameter.Say;
	*az=g_standard*(*az-accParameter.az0)/accParameter.Saz;
#if 1	
	lastAx[nGx%AVER_NUMBER]=*ax;
	lastAy[nGx%AVER_NUMBER]=*ay;
	lastAz[nGx%AVER_NUMBER]=*az;	
	if(nAx>AVER_NUMBER)
	{
		int i;
		float sum,aver,var;
		sum=0;
		for(i=0;i<AVER_NUMBER;i++)
			sum+=lastAx[i];
		aver=sum/AVER_NUMBER;
		var=0;
		for(i=0;i<AVER_NUMBER;i++)
			var+=(lastAx[i]-aver)*(lastAx[i]-aver);
		varAx=var=var/AVER_NUMBER;
		
		sum=0;
		for(i=0;i<AVER_NUMBER;i++)
			sum+=lastAy[i];
		aver=sum/AVER_NUMBER;
		var=0;
		for(i=0;i<AVER_NUMBER;i++)
			var+=(lastAy[i]-aver)*(lastAy[i]-aver);
		varAy=var=var/AVER_NUMBER;
		
		sum=0;
		for(i=0;i<AVER_NUMBER;i++)
			sum+=lastAz[i];
		aver=sum/AVER_NUMBER;
		var=0;
		for(i=0;i<AVER_NUMBER;i++)
			var+=(lastAz[i]-aver)*(lastAz[i]-aver);
		varAz=var=var/AVER_NUMBER;
	}
	nAx++;	
#else
	
// 加速度数据的方差不敏感, 没意义	
	//速度移动平均用50个数 0.5秒
	averAx=averAx*0.98+*ax*0.02;
	averAy=averAy*0.98+*ay*0.02;
	averAz=averAz*0.98+*az*0.02;
	
	float d;
	d=(averAx-*ax);d=d*d;
	varAx=varAx*0.99+d*0.01;			//方差用100个数算
	d=(averAy-*ay);d=d*d;
	varAy=varAy*0.99+d*0.01;			//方差用100个数算
	d=(averAz-*az);d=d*d;
	varAz=varAz*0.99+d*0.01;			//方差用100个数算	
#endif	
	
		return 1;
}

void Get_Car_Gesture(void)
{
	if(ACC_DataUpdate()==SET)
	ReadACC(Acc_Buffer+0,Acc_Buffer+1,Acc_Buffer+2);
	if(GYRO_DataUpdate() == SET)  //获取角加速度
	GYRO_DataRead();

}
void LSM330DLC_getMotion6(double v[6])
{
	Get_Car_Gesture();
	v[0]=Acc_Buffer[0];
	v[1]=Acc_Buffer[1];
	v[2]=Acc_Buffer[2];
	v[3]=Gyro_Buffer[0];
	v[4]=Gyro_Buffer[1];
	v[5]=Gyro_Buffer[2];
}



extern double Ax[7],Ay[7],Az[7];	



//int DoDr(void)
//{
//	static uint32_t delta_time = 0; //  时钟差
//	static uint32_t prev_time = 0; //   上一个 System tick
//	static uint32_t last_time = 0; //   当前   System tick

//	static uint16_t delta_filter_count = 0; // 100ms 保存一次结果
//	static uint16_t num_adjust_value  = 0; //  返回的大于 1.2 数据个数
//	float avr_delta = 0;
//	
//	static int gpsToStaticTime;//有gps速度, 至少要这么长时间(调用次数)后才可能静止
//	unsigned long getTickCount(void);
//	static unsigned long lastTime;
//	if(getTickCount()<lastTime+9)
//	return 0;
//	while(getTickCount()<lastTime+10);
//	
//	lastTime=getTickCount();
//	if(gpsSpeed>-100)
//	{
//		speed=gpsSpeed;
//		gpsSpeed=-1000;
//	}
//	char line[256];

//	double angles[3];
//	long t;	
//	double axx,ayy,azz; //机体加速度,(去掉重力影响)
//	double ax,ay,az;


//	void LSM330DLC_getMotion6(double v[6]);
//	double vv[6];

//		extern double SensorValue[6];
//		void IMU_getYawPitchRoll(double * angles	);
//		IMU_getYawPitchRoll(angles);
//		double yaw=angles[0]*pi/180;
//		double pitch=angles[1]*pi/180;
//		double roll=angles[2]*pi/180;
//		
//		
//		axx=tt[0][0]*SensorValue[0]+tt[1][0]*SensorValue[1]+tt[2][0]*SensorValue[2];		
//		ayy=tt[0][1]*SensorValue[0]+tt[1][1]*SensorValue[1]+tt[2][1]*SensorValue[2];
//		
//		float aa;
//		aa=axx*cos(yaw)+ayy*sin(yaw);		
//		azz=tt[0][2]*SensorValue[0]+tt[1][2]*SensorValue[1]+tt[2][2]*SensorValue[2];		

//		float aas;//直接用稳定姿态计算的纵向加速度

//		if(bUseStaticAngle)//开机取得静态角以后才开始用速度数据
//			aas=tts[0][0]*SensorValue[0]+tts[1][0]*SensorValue[1]+tts[2][0]*SensorValue[2];		
//		else
//			aas=0;
//		if(fabs(aas)<0.03)
//			aas=0; 

//		currentAx=aas;

//		static int icall;

//	static unsigned long lastTimes[5]; //当时时间 , 避免时间不准问题
//	static float angleLast50ms[5]; //角度用100毫秒作为度量周期,减少瞬间误差,  所以需要记录过去10次的值
//		icall++;
//		if(icall>300)//3秒钟以后才能使用加速度量, 否则可能有很大的初始误差
//		{
//			gpsToStaticTime--;
//			float dtime=1.0*(lastTime-lastTimes[(icall-1)%5])/1000;

//			avr_delta = abs(varGx);
//			avr_delta = avr_delta + abs(varGy);
//			avr_delta = avr_delta + abs(varGz);
//			
//			if(avr_delta > 6.0) // 即平均值>2.0
//			{
//				flag_car_move = 1;
//				move_count++;
//			}
//			else
//			{
//				flag_car_move = 0;
//			}			
//			
//			
//			last_time = lastTime; // 更新 系统时钟计数

//			delta_time = last_time-prev_time; // 当前系统计数值 - 上一个系统时钟计数
//			
//			if(delta_time >= 1000)
//			{
//				prev_time=last_time;
//				
//				time_count++;
//				sys_run_times++;
//				if(move_count > 0)  // 1秒内有1次大于0，则认为汽车处于非静止
//				{
//					bStatic=0;
//					car_move_times++;
//				}
//				else
//					bStatic=1;

//				move_count = 0;			
//			
//			}
//			
//			
////2. 完全静止	
//		
////---------------------------------------------------------------------------------------------------
//// 急加速 急减速			
//			if(bStatic == 1)
//			{
//				noMove++;
//				if(noMove>300 && gpsToStaticTime<=0)//保证从gps有速度会经过大致最少减速时间才进入静止状态
//				{
//					SaveAngleState(pitch,roll);//保存稳定姿态
//					noMove=0;
////					bStatic=1;
//					speed=0;
//					//调整 角速度0漂 
//					num00=averGx;
//					num11=averGy;
//					num22=averGz;
//					float nums[3];
//					nums[0]=num00;
//					nums[1]=num11;
//					nums[2]=num22;
//					//FM25CLxx_WR((unsigned char *)nums,sizeof(nums),zerodriftOffset+24);
//				}
//			}
//			else
//			{
//				noMove=0;
//			}
//			meterTotal+=speed*dtime;
//			if(currentAx>axMax)
//				axMax=currentAx;
//			if(currentAx<axMin)
//				axMin=currentAx;	
//			
//			double dAngle=(SensorValue[5]-angleLast50ms[(icall-5)%5]);
////			double dAngle=(angles[0]-angleLast50ms[(icall-5)%5]);
//			//跨过 0, 360的判断 			, 100ms不可能超过180度   ,转换值 +-180
//			
//			if(dAngle>180)
//				dAngle=dAngle-360;
//			
//			if(dAngle<-180)
//				dAngle=dAngle+360;
//			
//			int dTime5=lastTime-lastTimes[(icall-5)%5];
//			if(dTime5>20&&dTime5<1000)
//			{
//				dAngle=dAngle*1000/dTime5;
//			
//				if(dAngle>dAngleMax)
//					dAngleMax=dAngle;
//				if(dAngle<dAngleMin)
//					dAngleMin=dAngle;
//			}
//		}
//		angleLast50ms[icall%5]=	angles[0];
//		lastTimes[icall%5]=lastTime;
//		return 1;
//}			

//int lsm330d_data_handler(void)
//{}
//void sharp_turn(void)
//{

//}

/*
int lsm330d_data_handler(void)
{
	static uint16_t delta_filter_count = 0; // 100ms 保存一次结果
	static uint16_t num_adjust_value  = 0; //  返回的大于 1.2 数据个数
	float avr_delta = 0;

	static int gpsToStaticTime;//有gps速度, 至少要这么长时间(调用次数)后才可能静止
	unsigned long getTickCount(void);
	static unsigned long lastTime;

	if(gpsSpeed>-100)
	{
//		if(gpsSpeed>5 //高速度才用gps数据
//			|| fabs(gpsSpeed-speed)>5)  //惯导误差太大了
//			speed=gpsSpeed;
//		if(gpsSpeed>1)	//避免gps实际静止时的噪音速度数据
//		{
//		gpsToStaticTime=gpsSpeed/3 *100;
//		bStatic=0;
//		}
		speed=gpsSpeed;
		gpsSpeed=-1000;
	}
	char line[256];

	double angles[3];
	long t;	
	double axx,ayy,azz; //机体加速度,(去掉重力影响)
	double ax,ay,az;


	void LSM330DLC_getMotion6(double v[6]);
	double vv[6];

				__disable_irq();
		IMU_getYawPitchRoll(angles);
				__enable_irq();

		double yaw=angles[0]*pi/180;
		double pitch=angles[1]*pi/180;
		double roll=angles[2]*pi/180;
		
		
		axx=tt[0][0]*SensorValue[0]+tt[1][0]*SensorValue[1]+tt[2][0]*SensorValue[2];		
		ayy=tt[0][1]*SensorValue[0]+tt[1][1]*SensorValue[1]+tt[2][1]*SensorValue[2];
		iayy =ayy;
		iaxx = axx;
		float aa;
		aa=axx*cos(yaw)+ayy*sin(yaw);		
		azz=tt[0][2]*SensorValue[0]+tt[1][2]*SensorValue[1]+tt[2][2]*SensorValue[2];		

		float aas;//直接用稳定姿态计算的纵向加速度

		if(bUseStaticAngle)//开机取得静态角以后才开始用速度数据
			aas=tts[0][0]*SensorValue[0]+tts[1][0]*SensorValue[1]+tts[2][0]*SensorValue[2];		
		else
			aas=0;
		if(fabs(aas)<0.03)
			aas=0; 

		currentAx=aas;
		static int icall;
		

	static unsigned long lastTimes[5]; //当时时间 , 避免时间不准问题
	static float angleLast50ms[5]; //角度用100毫秒作为度量周期,减少瞬间误差,  所以需要记录过去10次的值
		icall++;

		if(icall>300)//3秒钟以后才能使用加速度量, 否则可能有很大的初始误差
		{
			gpsToStaticTime--;
			float dtime=1.0*(lastTime-lastTimes[(icall-1)%5])/1000;

//2. 完全静止			
			avr_delta = abs(varGx);
			avr_delta = avr_delta + abs(varGy);
			avr_delta = avr_delta + abs(varGz);
			
			if(avr_delta > 6.0)
			{
				flag_car_move = 1;
				move_count++;
			}
			else
			{
				flag_car_move = 0;
			}			
			
			delta_filter_count++;
			if(delta_filter_count > 100)
			{
				sys_run_times++;
				delta_filter_count = 0;
				if(move_count > 0)
					car_move_times++;
				move_count = 0;
			}

			
//			if(varGx<3.0&& varGy<3.0 &&varGz<5.0	 && varAx<0.5&& varAy<0.5 &&varAz<0.5)//角速度移动方差很小, 确定是静止状态, 但非常非常平稳运动的时候可能会错
			if(abs(avr_delta) < 1.2 )
			{
				noMove++;
				if(noMove>300 && gpsToStaticTime<=0)//保证从gps有速度会经过大致最少减速时间才进入静止状态
				{
					SaveAngleState(pitch,roll);//保存稳定姿态
					noMove=0;
					bStatic=1;
					speed=0;
					//调整 角速度0漂 
					num00=averGx;
					num11=averGy;
					num22=averGz;
					float nums[3];
					nums[0]=num00;
					nums[1]=num11;
					nums[2]=num22;
//					FM25CLxx_WR((unsigned char *)nums,sizeof(nums),zerodriftOffset+24);
				}
			}
			else
			{
				noMove=0;
				
// --------------------------------------------------------------------------------------				
				if(bStatic)
					SerialPort_SendStringEx(COM1,"\r\n\r\n**********\r\n\r\n");				
				 bStatic=0;
// --------------------------------------------------------------------------------------				
			}
			
				meterTotal+=speed*dtime;
//加速度用过去若干次的移动平均, 5次

			if(currentAx>axMax)
				axMax=currentAx;
			if(currentAx<axMin)
				axMin=currentAx;	
			
			double dAngle=(angles[0]-angleLast50ms[(icall-5)%5]);
			//跨过 0, 360的判断 			, 100ms不可能超过180度   ,转换值 +-180
			while(dAngle>180)
				dAngle=dAngle-360;
			while(dAngle<-180)
				dAngle=dAngle+360;
			int dTime5=lastTime-lastTimes[(icall-5)%5];
			if(dTime5>20&&dTime5<1000)
			{
				dAngle=dAngle*1000/dTime5;
			
				if(dAngle>dAngleMax)
					dAngleMax=dAngle;
				if(dAngle<dAngleMin)
					dAngleMin=dAngle;
			}
	
		}

		angleLast50ms[icall%5]=	angles[0];
		lastTimes[icall%5]=lastTime;

		return 1;
}	

*/

extern float gpsSpeed;
unsigned  char  GetMeter  (double*  meter);
unsigned char  SetMeter(double  meter);
char  GetSensor(float* sensor);
void  SetSensor(float  v1 , float  v2);
int  GetMaxA(float*  maxAx,float*  minAx);
