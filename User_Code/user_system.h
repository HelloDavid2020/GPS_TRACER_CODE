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
	 
#define 	WORK_SLEEP_MODE_0 	 0 	// ��������ʱ���豸�������� 10�����ϴ�һ��
#define 	WORK_SLEEP_MODE_1 	 1	// ����ͣ����Сʱ��GPS���ߣ�ÿ1Сʱ����1�Σ�����6���� 
#define 	WORK_SLEEP_MODE_2 	 2	// ��Ƿѹʱ���豸���ߣ�5Сʱ����һ�Σ�����6���ӡ�
#define 	WORK_SLEEP_MODE_3 	 3  // �����ϵ�� 8Сʱ����1��
	 
#define		ACC_ON						1 // acc ��
#define 	ACC_OFF						0	// acc �� 
	 
#define		LOCK							1 // ����������
#define 	LOCK_NONE					0	// ������δ����

#define 	DEVICE_RUN				0 // �豸����
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
	uint32_t AlarmValue; // ����ֵ
	
	struct 
	{
		uint32_t 	Openshell	:1; // ����
		uint32_t 	GpsAnt 		:1; // GPS ʧȥ����
		uint32_t 	NoPower		:1; // �ⲿ��Դ�ж�

		uint32_t 	ErrRemote	:1; // ��ƽ̨ͨѶ�ж�
		
		uint32_t 	ErrComm		:1; // ����ͨѶ�ж�
		uint32_t 	ErrSIMCard:1; // SIM �쳣
		
		uint32_t 	LowPower	:1; // �ⲿ��Դ�ж�
		
		uint32_t	ErrAirFilter						:1;  	// ���˶���
		uint32_t	ErrOilFilter						:1;	 	// ���˶���
		uint32_t	OilTemperatureHigh			:1;	 	// ���¸�ָʾ
		uint32_t 	CoolantTemperatureHigh	:1;	 	// ˮ�¸�ָʾ
		uint32_t 	OilPressureLow					:1;		// ��ѹ��ָʾ
		uint32_t 	Beep										:1;		// beep ����
		uint32_t 	VoltageLow							:1;		// ��ѹ��ָʾ
		uint32_t 	ErrOilWaterSeparator 		:1; 	// �ͷֱ���
		uint32_t  BeyondTheMark           :1;   // Խ�籨��
		
		uint32_t  user_CoolantTemperatureHigh :1;
		uint32_t  user_OilPressureLow :1;
		
		uint32_t  rsd : 14;
		
		
	}Bit;	

};	
extern union DEVEICE_ALARM  DeveiceAlarm;

extern uint16_t DeveiceAlarmLastValue; // ��ǰ״̬����ֵ
extern uint16_t DeveiceAlarmPrevValue; // ǰһ״̬����ֵ

// ����һ��Ĺ���ʱ��
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

	uint8_t 	OutputPowerState;	// �ⲿ��Դ״̬	
	
	uint8_t		WorkSleepMode;

	uint8_t		RunOrTestMode;

	uint16_t  LockLevel;
	
	uint32_t  WorkTime;  // 
	uint32_t  SleepTime;
	uint32_t  UploadTimeInterval; // �����ϴ�ʱ����
	uint32_t  UploadTimes; 				// �����ϴ���Ŀ
	
	uint32_t  AccOffTimeCount;  // acc�رռ���
	
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

