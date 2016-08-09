#ifndef __USER_MONITOR_H
#define __USER_MONITOR_H

#include "includes.h"
#ifdef __cplusplus
 extern "C" {
#endif 
	 
#define NO_ALARM 	0

#define LEVEL1 1
#define LEVEL2 2
	 
#define GPS_UNLOCK	0
#define GPS_LOCK	  1
	 
#define WORK_SLEEP_MODE_0 	 0 	// 车辆工作时，设备正常工作 10分钟上传一次
#define WORK_SLEEP_MODE_1 	 1	// 车辆停机半小时后，GPS休眠，每1小时唤醒1次，工作6分钟 
#define WORK_SLEEP_MODE_2 	 2	// 当欠压时，设备休眠，5小时唤醒一次，工作6分钟。
#define WORK_SLEEP_MODE_3 	 3  // 车辆断电后 8小时唤醒1次
#define WORK_SLEEP_MODE_4 	 4  // 系统自定义
	 
#define RELAY_A_LOCK		GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define RELAY_B_LOCK		GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define RELAY_C_LOCK		GPIO_ResetBits(GPIOC, GPIO_Pin_7)

#define RELAY_A_UNLOCK	GPIO_SetBits(GPIOC, GPIO_Pin_14)
#define RELAY_B_UNLOCK	GPIO_SetBits(GPIOC, GPIO_Pin_12)
#define RELAY_C_UNLOCK	GPIO_SetBits(GPIOC, GPIO_Pin_7)
	 
#define MODE_TEST	(!0)
#define MODE_WORK 0
	 
	 
extern int32u Countdown_Count; // 倒计时
// uint8_t Flag_System_Standby;
extern uint16_t AdminLock;
extern uint16_t AdminUnlockEnable;


typedef struct
{
	uint16_t UnLockEnable;  // 
	uint16_t LockValue;	
	
}
TCP_ADMIN_COMMAND; // tcp 接受到的TCP命令
extern TCP_ADMIN_COMMAND AdminCommand;

extern uint8_t  Flag_Enter_Standby; // 系统进入 standby 
extern uint8_t  Flag_Start_Standby; // 系统进入 standby 

extern uint8_t  Count_Start_Standby; // 系统进入 standby 
extern uint8_t  Count_Enter_Standby; // 系统进入 standby

extern uint32_t CommTimeOut;

extern int32u Countdown_Count;

extern uint16_t DeviceLcokLevel;

extern uint8_t relay[4];

typedef struct 
{	
	uint8_t  UpdateLogDateEnable;  // 刷新使能 
	
	uint8_t  LogYear;
	uint8_t  LogMon;
	uint8_t  LogDay;
	uint16_t DayOfLogYear; // 
	
	uint8_t  CurentYear;
	uint8_t  CurentMon;
	uint8_t  CurentDay;	
	uint16_t DayOfCurentYear; // 
	uint16_t TimeOutDays; //
	uint16_t TimeOutSetDays; //设置超时天数
	

}LOG_DATE;
extern LOG_DATE LogDate;


typedef union 
{
	struct
	{
		uint8_t LockA :1;
		uint8_t LockB :1;
		uint8_t LockC :1;
		uint8_t LockD :1;
		uint8_t rsd	 :4;
	} Bit;		
	uint8_t Vaule;
}ADMIN_LOCK;

//typedef struct 
//{
//	BOOL LockA;
//	BOOL LockB;
//	BOOL LockC;
//	BOOL LockD;
//	uint16_t Vaule;
//}ADMIN_LOCK;



extern ADMIN_LOCK uAdminLock;

typedef struct 
{	

		uint16_t bit0 :1;
		uint16_t bit1 :1;
		uint16_t bit2 :1;
		uint16_t bit3 :1;
		uint16_t bit4 :1;
		uint16_t bit5 :1;
		uint16_t bit6 :1;
		uint16_t bit7 :1;
		uint16_t bit8	:1;
		uint16_t bit9  :1;
		uint16_t bit10 :1;
		uint16_t bit11 :1;
		uint16_t bit12	:1;
		uint16_t bit13 :1;
		uint16_t bit14 :1;
		uint16_t bit15 :1;
		uint16_t Value;
}REMOTE_LOCK;

extern REMOTE_LOCK RemoteLock;

extern uint8_t WorkSleepModeSwitchHandler(void);

// runtime 系统运行时间处理函数
extern void RuntimeSwtichHandler(uint8_t workmode);


extern void Check_QuitStandby(void);
extern void EnterStandby(uint16_t sleep_hour, uint16_t sleep_min);
extern uint8_t Check_Acc_Status(void);

extern void System_Standby_Handler(void);
extern void SystemStatusDetect(void);

extern uint8_t CommunicationBreakdownHandler(uint32_t timeCount);

extern void ReportAlarmEvent(void);
extern uint8_t CheckAlarmValue(void);
extern void RemoteCommBreakdownHandler(void);
extern void UpdateLogDate(void);
extern void RemoteCommBreakdownInterval(LOG_DATE * tempLogDate);
extern uint16_t GetDayOfYear(uint8_t year_8u, uint8_t month, uint8_t day); 





extern void LockAll(void);
extern void UnlockAll(void);
extern void LockHandler(void);

extern uint16_t GetLockLevel(void);
extern void SetLockLevel(uint16_t locklevel);


extern uint16_t GetAdminRights(void);
extern void SetAdminRights(uint16_t adminrights);
// 获取管理员权限
extern uint16_t GetAdminLockState(void);
extern void SetAdminLockState(uint16_t admin_lock_state);


extern void SetAdminUnlockCommamd(uint16_t unlockstate);
extern uint16_t GetAdminUnlockCommamd(void);

extern uint16_t GetRunOrTestMode(void);

extern void SetRunOrTestMode(uint16_t mode);

extern uint16_t GetCoolHigh(void);
extern uint16_t GetOilLow(void);

#ifdef __cplusplus
}
#endif 

#endif

