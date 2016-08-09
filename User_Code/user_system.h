#ifndef __USER_SYSTEM_H
#define __USER_SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"
	 
//#define REG_HOLDING_START     0x0000
//#define REG_HOLDING_NREGS     1024*10	 
	 
	 
#define 	TCP_CONNECT			1
#define 	TCP_NO_CONNECT  0	 
	 
#define 	WORK_SLEEP_MODE_0 	 0 	// 车辆工作时，设备正常工作 10分钟上传一次
#define 	WORK_SLEEP_MODE_1 	 1	// 车辆停机半小时后，GPS休眠，每1小时唤醒1次，工作6分钟 
#define 	WORK_SLEEP_MODE_2 	 2	// 当欠压时，设备休眠，5小时唤醒一次，工作6分钟。
#define 	WORK_SLEEP_MODE_3 	 3  // 车辆断电后 8小时唤醒1次
	 
#define		ACC_ON						1 // acc 开
#define 	ACC_OFF						0	// acc 关 
	 
#define		LOCK							1 // 控制器锁车
#define 	LOCK_NONE					0	// 控制器未锁车

#define 	DEVICE_RUN				0 // 设备运行
#define		DEVICE_SLEEP			1 // device sleep
	 
union SYSTEM
{
	uint32_t StatusValue;
	struct 
	{
		uint32_t 	gps					:1;
		uint32_t	latitude		:1;	
		uint32_t  longtitude	:1;
		uint32_t 	acc					:1;
		uint32_t 	lock				:1;
		uint32_t 	sleep				:1;
		uint32_t  rsv					:26; // reserved
	}Bit;	

};	 


union DEVEICE_ALARM 
{
	uint32_t AlarmValue; // 报警值
	
	struct 
	{
		uint32_t 	Openshell	:1; // 开壳
		uint32_t 	GpsAnt 		:1; // GPS 失去天线
		uint32_t 	NoPower		:1; // 外部电源切断

		uint32_t 	ErrRemote	:1; // 与平台通讯中断
		
		uint32_t 	ErrComm		:1; // 串口通讯中断
		uint32_t 	ErrSIMCard:1; // SIM 异常
		
		uint32_t 	LowPower	:1; // 外部电源切断
		
		uint32_t	ErrAirFilter						:1;  	// 空滤堵塞
		uint32_t	ErrOilFilter						:1;	 	// 油滤堵塞
		uint32_t	OilTemperatureHigh			:1;	 	// 油温高指示
		uint32_t 	CoolantTemperatureHigh	:1;	 	// 水温高指示
		uint32_t 	OilPressureLow					:1;		// 油压低指示
		uint32_t 	Beep										:1;		// beep 蜂鸣
		uint32_t 	VoltageLow							:1;		// 电压低指示
		uint32_t 	ErrOilWaterSeparator 		:1; 	// 油分报警
		uint32_t  BeyondTheMark           :1;   // 越界报警
		
		uint32_t  user_CoolantTemperatureHigh :1;
		uint32_t  user_OilPressureLow :1;
		
		uint32_t  rsd : 14;
		
		
	}Bit;	

};	
extern union DEVEICE_ALARM  DeveiceAlarm;

extern uint16_t DeveiceAlarmLastValue; // 当前状态报警值
extern uint16_t DeveiceAlarmPrevValue; // 前一状态报警值

// 车辆一天的工作时间
extern uint32_t VehicleWorkTimeOfOneDay;
extern uint32_t ReadVehicleWorkTimeOfOneDay;

typedef struct  
{
	uint8_t		AccState	  ;
	uint8_t		MotoState	  ;
	
	uint8_t		ShellState	  ;
	uint8_t		GpsAntState	  ;
	
	uint8_t   CanLockState;
	uint8_t		LockState	  ;
	uint8_t		UnlockState ;
	uint8_t		RelayAState ;
	uint8_t		RelayBState ;
	uint8_t		RelayCState ;
	uint8_t 	RelayDState	;
	uint8_t 	PasswordRequired ;
	
	uint8_t 	tempData ;  // 0/1 
	uint8_t 	PasswordState  ;
	uint8_t 	RemoteLockState;
	uint8_t 	RemoteCommunicationState;
	uint8_t 	SIMCardState;   

	uint8_t 	OutputPowerState;	// 外部电源状态	
	
	uint8_t		WorkSleepMode;

	uint8_t		RunOrTestMode;

	uint16_t  LockLevel;
	
	uint32_t  WorkTime;  // 
	uint32_t  SleepTime;
	uint32_t  UploadTimeInterval; // 数据上传时间间隔
	uint32_t  UploadTimes; 				// 数据上传数目
	
	uint32_t  AccOffTimeCount;  // acc关闭计数
	
}GPS_DEVICE;

extern GPS_DEVICE GpsDevice;
 
extern union SYSTEM uSystem;
// extern union MG323B_STATUS MG323B;

extern uint16_t GprsSigalStrength;
extern uint16_t GprsStatus; 





extern void User_SystemParameters_Init(void);
extern uint32_t VehicleWorkTime;

#ifdef __cplusplus
}
#endif 

#endif

