#ifndef __USER_RT8025X_H
#define __USER_RT8025X_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"
	 
#define SYS_TIME_LEN	0x07		// 系统时间长度
#define MAX_TRY_CNT	  0x40		//8025T最大操作次数

#define COMMAND_READ 		0x1  // 8025T读设备地址
#define COMMAND_WRITE 	0x0	// 8025T写设备地址 
	 
#define READ_8025T_ADDR		0x65  // 8025T读设备地址
#define WRITE_8025T_ADDR	0x64	// 8025T写设备地址 

#define SCL2_H         GPIOB->BSRR = GPIO_Pin_10
#define SCL2_L         GPIOB->BRR  = GPIO_Pin_10 
   
#define SDA2_H         GPIOB->BSRR = GPIO_Pin_11
#define SDA2_L         GPIOB->BRR  = GPIO_Pin_11

// #define SCL_READ      GPIOB->IDR  & GPIO_Pin_6
#define SDA2_READ      GPIOB->IDR  & GPIO_Pin_11
  /* Configure I2C2 pins: SCL2-PB.10 and SDA2-PB.11 */

	 
/* Private define ------------------------------------------------------------*/
#define I2C2_Speed             200000u
#define RTC_ADDRESS            0x64

#define BCD2BIN(val) (((val) & 0x0f) + ((val)>>4)*10)
#define BIN2BCD(val) ((((val)/10)<<4) + (val)%10)

// bit0 - bit7
#define		BITSET(var,bitno)				((var)	|=	(1 << (bitno)))			      // 
#define		BITCLR(var,bitno)				((var)	&=~	(1 << (bitno)))		    // 
#define		testbit_on(data,bitno)	((data 	>> 	bitno) & 0x01)		// 

#define _24_HOUR_SYSTEM 5
#define WALE 7
#define WAFG 1


	 
typedef struct
{
	uint8_t year;    /* years since 00 */
	uint8_t mon;     /* months since January - [0,11] */
	uint8_t day;    /* day of the month - [1,31] */
	uint8_t week;    /* days since Sunday - [0,6] */
	uint8_t hour;    /* hours since midnight - [0,23] */
	uint8_t min;     /* minutes after the hour - [0,59] */
	uint8_t sec;     /* seconds after the minute - [0,59] */
}DATE_TIME;

extern DATE_TIME ExRtcTime;
extern DATE_TIME TimeSet;


typedef struct 
{
	uint32_t hour;
	uint32_t min;
	uint32_t sec;
}SLEEP_TIME;

extern SLEEP_TIME SleepTime;

extern volatile uint8_t count_feed_rt8025;
extern volatile uint8_t Flag_Feed_RT8025;

extern uint8_t t_Rx8025T[7];

extern void delay_40us(void);


extern void I2C2_Configuration(void);
extern void I2C2_ByteWrite(uint8_t data, uint8_t Addr);
//extern uint8_t I2C2_ByteRead(uint8_t Addr);

extern void ShowTime(void);
extern void FeedWDG(void);

extern uint16_t HEX2DEC(uint16_t bcd);
extern uint16_t DEC2HEX(uint16_t dec);
	 
extern void RTC_RX8025_Init(void);
extern void SetRx8025Time(DATE_TIME time);	 

extern void GetRx8025Time(void);
extern void Timer_BCD2Int(uint8_t* ptr);

extern void BSP_RTC_Timer_Alarm(uint8_t delayHour, uint8_t delayMinute);
extern void BSP_RTC_Timer_WakeUp(uint8_t delayHour, uint8_t delayMinute);


extern void RTC_ByteWrite(uint8_t data, uint8_t Addr);
extern uint8_t RTC_ByteRead(uint8_t Addr);



extern uint8_t WriteRx8025T(uint8_t addr, uint8_t *src, uint8_t n);
extern uint8_t ReadRx8025T(uint8_t addr, uint8_t *dest, uint8_t n);

#ifdef __cplusplus
}
#endif 

#endif

