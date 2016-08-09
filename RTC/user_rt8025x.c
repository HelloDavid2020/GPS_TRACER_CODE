#include "user_rt8025x.h"


DATE_TIME TimeSet;

DATE_TIME ExRtcTime; // 外部RTC
SLEEP_TIME SleepTime;

volatile uint8_t count_feed_rt8025 = 0;
volatile uint8_t Flag_Feed_RT8025 = DISABLE;

uint8_t t_Rx8025T[7]={0x0};


//   延时总时间=cnt*16+24us
void delay_40us(void)
{
	u8 i;
		for(i=0;i<80;i++);
}



/********************************************************************************************************
**函数信息 ：uint8_t I2C1_ByteWrite(uint8_t data,uint8_t Addr)
**功能描述 ：写寄存器
**输入参数 ：data 数据；addr 寄存器地址
**输出参数 ：无
**调用提示 ：
*******************/
void I2C2_Configuration(void)
{


}


/********************************************************************************************************
**函数信息 ：uint8_t RTC_ByteWrite(uint8_t data,uint8_t Addr)
**功能描述 ：写寄存器
**输入参数 ：data 数据；addr 寄存器地址
**输出参数 ：无
**调用提示 ：
*******************/
void RTC_ByteWrite(uint8_t data, uint8_t Addr)
{
 uint8_t re;
  /* While the bus is busy */
//  while(!SDA_READ); 
//	while(!SCL_READ);
	
  /* Send START condition */
	if( I2C_Start() == FALSE) return;
  
  /* Send  address for write */
	I2C_SendByte(RTC_ADDRESS | 0x00); // 0xA0
	I2C_WaitAck();

  /* Send the start address to read from */
  I2C_SendByte(Addr<<4);    
	I2C_WaitAck();
  
  I2C_SendByte(data);    
	I2C_WaitAck();

	I2C_Stop();
}
/********************************************************************************************************
**函数信息 ：uint8_t RTC_ByteRead(uint8_t Addr)
**功能描述 ：读寄存器
**输入参数 ：addr 寄存器地址
**输出参数 ：无
**调用提示 ：
*******************/
uint8_t RTC_ByteRead(uint8_t Addr)
{  

  uint8_t re;
   /* While the bus is busy */
  
	
  /* Send START condition */
	if( I2C_Start() == FALSE) return 0;
  
  /* Send  address for read */
	I2C_SendByte(RTC_ADDRESS); // 0x64
	I2C_WaitAck();

  /* Send the start address to read from */
  I2C_SendByte(Addr<<4);    
	I2C_WaitAck();

	
  /* Send STRAT condition a second time */  
	if( I2C_Start() == FALSE) return 0;
	
	I2C_SendByte(RTC_ADDRESS | COMMAND_READ); // 0x65
	I2C_WaitAck();
	
  re = I2C_ReceiveByte();
	I2C_NoAck();
	I2C_Stop();
	delay(5);
	return re;
}



uint8_t ReadRx8025T(uint8_t addr, uint8_t *dest, uint8_t n)
{
	uint8_t i = 0;

	for (i = 0; i < MAX_TRY_CNT; i++)
	{
		I2C_Start();

		I2C_SendByte(WRITE_8025T_ADDR);

		I2C_Stop();
		I2C_Stop();
		DelayMS(1);
	}

	if(i >= MAX_TRY_CNT)
	{
		return(FALSE);
	}

	I2C_SendByte(addr);	
	I2C_Start();

	I2C_SendByte(READ_8025T_ADDR);

	while(n--)
 	{
		*dest++ = I2C_ReceiveByte();
		if (n != 0)
		{
			I2C_Ack();
		}
	}

	I2C_NoAck();
	I2C_Stop();
	return(TRUE);
}

uint8_t WriteRx8025T(uint8_t addr, uint8_t *src, uint8_t n)
{
	uint8_t i = 0;

 	for (i = 0; i < MAX_TRY_CNT; i++)
 	{
 		I2C_Start();

		I2C_SendByte(WRITE_8025T_ADDR);
		I2C_Stop();
		I2C_Stop();
		delay_us(100);
	}

	if (i >= MAX_TRY_CNT)
	{
		return(FALSE);
	}

	I2C_SendByte(addr);
	
 	while (n--)
 	{
 		I2C_SendByte(*src++);
 	}

 	I2C_Stop();											//???
	return(TRUE);
}

void ShowTime(void)
{
	GetRx8025Time();
		
		printf("\r\n> %02d-%02d-%02d %02d:%02d:%02d (Week: %02x) RT8025\r\n",
																ExRtcTime.year,
																ExRtcTime.mon,
																ExRtcTime.day,
																ExRtcTime.hour,
																ExRtcTime.min,
																ExRtcTime.sec,
																ExRtcTime.week);
	
	
}

void FeedWDG(void)
{
		if(Flag_Feed_RT8025 == ENABLE) 
		{
			Flag_Feed_RT8025 = DISABLE;
			BSP_RTC_Timer_Alarm (0, 3); // 复位时间顺延3分钟
		}
}

uint16_t HEX2DEC(uint16_t bcd)
{
	uint16_t out;
	out=((bcd>>4)* 10)+(bcd & 0xf);
	
	return out;
}

uint16_t DEC2HEX(uint16_t dec)
{
	uint16_t out;
	out=((dec/10)<<4)+(dec%10);
	return out;
}

/********************************************************************************************************
**函数信息 ：GetRx8025Time(void)
**功能描述 ：读时间
**输入参数 ：无
**输出参数 ：无
**调用提示 ：
*******************/
void GetRx8025Time(void)
{  
  uint8_t i;
  uint8_t buf[7],*ptr;
 
	buf[0]	= RTC_ByteRead(0x00)&0x7F; // seconds
	buf[1]	= RTC_ByteRead(0x01)&0x7F;    
	buf[2]	= RTC_ByteRead(0x02)&0x3F;
	buf[3]	= RTC_ByteRead(0x03)&0x0F;	
	buf[4]	= RTC_ByteRead(0x04)&0x3F; // days
	buf[5]	= RTC_ByteRead(0x05)&0x3F; // months 
	buf[6]	= RTC_ByteRead(0x06)&0xFF; // years 00-99
	
	I2C_Release();
	ExRtcTime.sec = HEX2DEC(buf[0]);     
	ExRtcTime.min = HEX2DEC(buf[1]);    
	ExRtcTime.hour= HEX2DEC(buf[2]);             
	ExRtcTime.week= HEX2DEC(buf[3]);    
	ExRtcTime.day	= HEX2DEC(buf[4]);            
	ExRtcTime.mon = HEX2DEC(buf[5]);       
	ExRtcTime.year= HEX2DEC(buf[6]); 
//	printf("\r\n> RX8025T time : %02d-%02d-%02d %02d:%02d:%02d (Week: %02d)\r\n",
//																ExRtcTime.year,
//																ExRtcTime.mon,
//																ExRtcTime.day,
//																ExRtcTime.hour,
//																ExRtcTime.min,
//																ExRtcTime.sec,
//																ExRtcTime.week	
//				);
	
	
}


void SetRx8025Time(DATE_TIME time)
{
	uint8_t i;
	uint8_t datebuf[7] = {0};
	uint8_t *ptr=datebuf;

	datebuf[0] = time.sec;  // second
	datebuf[1] = time.min;	// minute
	datebuf[2] = time.hour ;  // hour
	datebuf[3] = time.week;
	datebuf[4] = time.day;
	datebuf[5] = time.mon;
	datebuf[6] = time.year;

	for(i = 0;i < 7; i--)
	{
		RTC_ByteWrite(*ptr, i);
		ptr++;
	I2C_delay();
	I2C_delay();	I2C_delay();
	I2C_delay();
	I2C_delay();
	I2C_delay();
	I2C_delay();


	}
	

}	

/********************************************************************************************************
**函数信息 ：void Timer_BCD2Int(int8* ptr)
**功能描述 ：变换时间参数为整数
**输入参数 ：ptr 时间地址
**输出参数 ：无
**调用提示 ：
*******************/
void Timer_BCD2Int(uint8_t* ptr)
{  
   ExRtcTime.sec=(((*ptr) & 0x70)>>4)*10+((*ptr) & 0xf);     
   ptr++;
   ExRtcTime.min=(((*ptr) & 0x70)>>4)*10+((*ptr) & 0xf);   
   ptr++;
   ExRtcTime.hour=(((*ptr) & 0x30)>>4)*10+((*ptr) & 0xf);          
   ptr++;
   ExRtcTime.week=(*ptr) & 0x3;          
   ptr++;
   ExRtcTime.day=(((*ptr) & 0x30)>>4)*10+((*ptr) & 0xf);          
   ptr++;
   ExRtcTime.mon=(((*ptr) & 0x10)>>4)*10+((*ptr) & 0xf);          
   ptr++;
   ExRtcTime.year=(((*ptr) & 0xf0)>>4)*10+((*ptr) & 0xf);          

}

// Alarm DAY
void BSP_RTC_Timer_Alarm(uint8_t delayHour, uint8_t delayMinute)
{
// 	int i =0;
	uint8_t hour = 0;
	uint8_t min	 = 0;
	uint8_t sec		= 0;
	uint8_t deadTime = 10; // 死区时间

	uint8_t ret = 0;
	
	sec	 = RTC_ByteRead(0x00);
	min	 = RTC_ByteRead(0x01);
	hour = RTC_ByteRead(0x02);
	
	
	min		= HEX2DEC( min);
	hour	= HEX2DEC(hour);
	sec 	= HEX2DEC( sec);	

	// 增加时间的死区处理
	if((60-sec) <= deadTime) min++; // 因为不匹配秒，防止在时间设置时的死区	
	
	hour	+= delayHour;
	min 	+= delayMinute;
	
	// min  <0-59>
	if (min >= 60) 
	{
		min -= 60;
		hour++;
	}
	
	// hour <0-23>
	if(hour >= 24)
	{
		hour -= 24;
	}
	
	hour	= DEC2HEX(hour);
	min	 	= DEC2HEX( min);
	sec		= DEC2HEX(0x00);	
	
//	#ifdef DEBUG
	printf("> ▇ Reset time: %02x:%02x:%02x ▇\r\n", hour, min,sec);
//	#endif
	RTC_ByteWrite(0x20 , 0xe );
	
	RTC_ByteWrite(hour, 	0x0c );
	RTC_ByteWrite(min,		0x0b );
	
	ret = RTC_ByteRead(0x0E);
	BITSET(ret,6);
	RTC_ByteWrite(ret, 0xe );
	
	#ifdef DEBUG
	ret = RTC_ByteRead(0x0E);
	printf("> regE: 0x%02x\r\n", ret);
	#endif
}

// Alarm WEEK ,INTB
// 星期匹配报警
void BSP_RTC_Timer_WakeUp(uint8_t delayHour, uint8_t delayMinute)
{
	uint8_t week	= 0;
	uint8_t hour	= 0;
	uint8_t min		= 0;
	uint8_t sec		= 0;
	uint8_t deadTime = 10; // 死区时间
	
	uint8_t regE 	= 0; 	// control_1 status
	uint8_t ret = 0;
	
	sec		= RTC_ByteRead(0x00);
	min		= RTC_ByteRead(0x01);
	hour	= RTC_ByteRead(0x02);
	week	= RTC_ByteRead(0x03);
	
	week	= HEX2DEC(week);
	min		= HEX2DEC( min);
	hour	= HEX2DEC(hour);
	sec 	= HEX2DEC( sec);
	
	printf("> Current time: %02d:%02d:%02d (Week %1d)\r\n", hour, min,sec,week);
	
	// 增加时间的死区处理
	if((60-sec) <= deadTime) min++; // 因为不匹配秒，防止在时间设置时的死区

	hour 	+= delayHour; 	// 设定唤醒时间 hour
	min 	+= delayMinute; // 设定唤醒时间 Min

	
	// min  <0-59>
	if (min >= 60) 
	{
		min -= 60;
		hour++;
	}
	
	// hour <0-23>
	if(hour >= 24)
	{
		hour -= 24;
		week++;
	}
	
	// week <1-7>
	if(week > 7)
	{
		week -= 7;
	}
	
	week	= DEC2HEX(week);
	hour	= DEC2HEX(hour);
	min	 	= DEC2HEX( min);
	sec		= DEC2HEX(0x00);

	printf("> Wakeup  time: %02x:%02x:%02x (Week %1x)\r\n", hour, min, sec, week);

	
	
	BITSET(regE,_24_HOUR_SYSTEM);
	RTC_ByteWrite(regE, 	0xe );

	RTC_ByteWrite(hour, 	0x09 );
	RTC_ByteWrite( min,	0x08 );
	RTC_ByteWrite(0x7f, 	0x0a ); // week day:0111 1111

	BITSET(regE,WALE);
	RTC_ByteWrite(regE,	0x0E );

	ret = RTC_ByteRead(0x0E);
	printf("> regE: 0x%02x\r\n",ret);
}





//
void RTC_RX8025_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	RTC_ByteWrite(0x20, 0xe); // 24小时制
	RTC_ByteWrite(0x00, 0xf);	// close Alarm 
//printf("> Current time: %02d:%02d:%02d (Week %1d)\r\n", hour, min,sec,week);
	
	TimeSet.year=16;
	TimeSet.mon=4;
	TimeSet.day=18;
	TimeSet.hour=0;
	TimeSet.min=1;
	TimeSet.sec=0;
	
	SetRx8025Time(TimeSet);	
	
	ShowTime();

}

