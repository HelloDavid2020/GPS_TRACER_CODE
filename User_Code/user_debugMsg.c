#include "user_debugMsg.h"
uint32_t debugTimeCount = 0;

void ShowVehicleMsg(void)
{
	printf(" --- Vehicle Information ---\r\n");
	printf("> CoolTemp :  %3d C\r\n",Vehicle.CoolantTemperature);
	printf("> FuelLevel: <%3d%% \r\n",(Vehicle.FuelLevel*10));
	printf("> OilTempe :  %3d C\r\n",Vehicle.OilTemperature);
	printf("> OilPressure: %1.1f Mpa\r\n",(Vehicle.OilPressure*0.1));
	printf("> EngineSpeed: %5d RPM\r\n",Vehicle.EngineSpeed);
	printf("> BatteryVol : %2.2f V\r\n",(Vehicle.BatteryVoltage*0.01));
//	printf("> Password:%d \r\n",Vehicle.Password);
	printf("> WorkTime   : %.1f Hours\r\n",(Vehicle.WorkTime*0.1));
	
	printf("     Warn Status    \r\n");

	printf("> ErrAirFilter: %d \r\n",Vehicle.ControllerWarn.Bit.ErrAirFilter);
	printf("> ErrOilFilter: %d \r\n",Vehicle.ControllerWarn.Bit.ErrOilFilter);
	printf("> OilTempHigh : %d \r\n",Vehicle.ControllerWarn.Bit.OilTemperatureHigh);
	printf("> CoolTempHigh: %d \r\n",Vehicle.ControllerWarn.Bit.CoolantTemperatureHigh);
	printf("> OilPressLow : %d \r\n",Vehicle.ControllerWarn.Bit.OilPressureLow);
	printf("> BuzzerBeep  : %d \r\n",Vehicle.ControllerWarn.Bit.Beep);
	printf("> VoltageLow  : %d \r\n",Vehicle.ControllerWarn.Bit.VoltageLow);
	printf("> ErrOilWater : %d \r\n",Vehicle.ControllerWarn.Bit.ErrOilWaterSeparator);

//	printf("> WarnVaule:%d \r\n",Vehicle.ControllerWarn.Vaule);
	printf(" ----------------------------\r\n");
}

void ShowStatus(void)
{
	/*
			uint32_t 	Openshell	:1; // 开壳
		uint32_t 	GpsAnt 		:1; // GPS 失去天线
		uint32_t 	ErrComm		:1; // 串口通讯中断
		uint32_t 	ErrRemote	:1; // 与平台通讯中断
		uint32_t 	ErrSIMCard:1; // SIM 异常
		uint32_t 	NoPower		:1; // 外部电源切断
		uint32_t 	LowPower	:1; // 外部电源切断
	*/
	printf(" --- Status ---\r\n");
	
	printf("> AccState :  %1d\r\n", GpsDevice.AccState);
	printf("> MotoState :  %1d\r\n",GpsDevice.MotoState);
	printf("> WorkTime :  %d\r\n", VehicleWorkTimeOfOneDay);
	
	printf("> GprsInitStatus :  %1d\r\n", Gprs.InitStatus);
	printf("> TcpInitStatus :  %1d\r\n", Gprs.TcpInitStatus);
	printf("> SigalStrength:  %2d\r\n", Gprs.SigalStrength);
	
	printf("> Openshell :  %1d\r\n", DeveiceAlarm.Bit.Openshell);
	printf("> GpsAnt:  %1d\r\n", DeveiceAlarm.Bit.GpsAnt);
	printf("> ErrComm :  %1d \r\n", DeveiceAlarm.Bit.ErrComm);
	printf("> ErrRemote: %1d\r\n",DeveiceAlarm.Bit.ErrRemote);
	printf("> ErrSIMCard: %1d\r\n",DeveiceAlarm.Bit.ErrSIMCard);
	printf("> NoPower : %1d\r\n",DeveiceAlarm.Bit.NoPower);
	printf("> LowPower   : %1d\r\n",DeveiceAlarm.Bit.LowPower);
	
	printf("> AdminUnLock   : %d\r\n",AdminCommand.UnLockEnable);
	printf("> AdminLockVal  : %d\r\n",uAdminLock.Vaule);
	
	printf("> RelayA   : %1d\r\n",GpsDevice.RelayAState);
	printf("> RelayB   : %1d\r\n",GpsDevice.RelayBState);
	printf("> RelayC   : %1d\r\n",GpsDevice.RelayCState);
	printf("> RelayD   : %1d\r\n",GpsDevice.RelayDState);
	
	
	
	printf(" ----------------------------\r\n");
}
void ShowModbus5(void)
{
	uint8_t i= 0;
	printf("\r\n");

	for(i = 0; i< 255; i++)
	printf("0x%02x, ",g_modbusReg5[i]);
	printf("\r\n");
}
