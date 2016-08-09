#include "user_system.h"

union SYSTEM uSystem;

union DEVEICE_ALARM  DeveiceAlarm;
uint16_t DeveiceAlarmLastValue = 0; // 当前状态报警值
uint16_t DeveiceAlarmPrevValue = 0; // 前一状态报警值

uint32_t VehicleWorkTimeOfOneDay = 0;
uint32_t ReadVehicleWorkTimeOfOneDay = 0;

GPS_DEVICE GpsDevice;




uint32_t VehicleWorkTime = 0;

void User_SystemParameters_Init(void)
{
	reportHeartTimeOut 	= 	(g_modbusReg4[modbusTcpHeartInterval + 1] << 8) + g_modbusReg4[modbusTcpHeartInterval];
	reportAccOffTimeOut = 	(g_modbusReg4[modbusTcpReportIntervalAccOff + 1] << 8) + g_modbusReg4[modbusTcpReportIntervalAccOff];
	reportAccOnTimeOut 	= 	(g_modbusReg4[modbusTcpReportIntervalAccOn + 1] << 8) + g_modbusReg4[modbusTcpReportIntervalAccOn];
	DeviceWorkTime 			= 	(g_modbusReg4[modbusSleepDelay + 1] << 8) + g_modbusReg4[modbusSleepDelay];
	DeviceSleepTime			= 	(g_modbusReg4[modbusSleepTime + 1] << 8) + g_modbusReg4[modbusSleepTime]; 
	lockNoSignalTimeOut = 	(g_modbusReg4[modbusLockTimeNoSignal + 1] << 8)+ g_modbusReg4[modbusLockTimeNoSignal];

	
	
	alarmNoPLCTimeOut 	= 	(g_modbusReg4[modbusAlarmTimeNoPLC + 1] << 8) + g_modbusReg4[modbusAlarmTimeNoPLC];
	Gprs.UploadTimeCount = 60;	// 开机上传
	Gprs.HeartBeatTimeCount = 30;		// 开机上传
	
	uSystem.Bit.acc = 0;
	uSystem.Bit.gps = 0;
	uSystem.Bit.latitude		= 0;
	uSystem.Bit.longtitude	= 0;
	uSystem.Bit.lock 	= 0;
	uSystem.Bit.sleep 	= 0;
	
	g_modbusReg4[modbusGSMRSSI] = 0;
	g_modbusReg4[modbusGpsNoSV] = 0;
	g_modbusReg4[modbusGpsSpeed] = 0;
	g_modbusReg4[modbusGpsdirection] 	= 0;
	g_modbusReg4[modbusGpsHeight]			= 0;
	g_modbusReg4[modbusGpsLatitude]		= 0;
	g_modbusReg4[modbusGpsLongitude]	= 0;	
	
	
	// 保存初始值
	
	DeveiceAlarmPrevValue = 0;
	DeveiceAlarmLastValue = DeveiceAlarm.AlarmValue;
	
	
	GpsDevice.WorkSleepMode = WORK_SLEEP_MODE_4; // 正常工作模式
	GetServerIP(old_serverIP, 16, old_serverPort, 6); // 保存IP，Port
	
	AccDetect.PrevVal = 0;
	AccDetect.ChangFlag = 0;

	MotoDetect.PrevVal = 0;
	MotoDetect.ChangFlag = 0;
	
	
	BeatCanMsgInit();
	
}








