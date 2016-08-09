#include "user_acc.h"

STATE_CHANGE AccDetect;
STATE_CHANGE MotoDetect;

RTC_DATE RtcDate;
CREAT_FLAG_OF_A_NEW_DAY CheckNewDay;
uint8_t FlagOfaNewDay[2] = {0};


void cleanAcc(void)
{
	uint8_t i;
	g_modbusReg4[modbusTimeNum] = 0;
	for (i = 0; i < 120; i++)
		g_modbusReg4[modbusTimeData + i] = 0;
	
	
	AutoSaveModbus4();
}
void ReportAcc(void)
{

	uint8_t i;
	uint8_t size;
	
	while(TCPSend_Num > 127)
		os_dly_wait(1);
	
	size = g_modbusReg4[modbusTimeNum] << 2;
	TCPSendBuf[TCPSend_Num]	= 0x47;
	TCPSend_Num++;
	TCPSendBuf[TCPSend_Num] = size + 1;
	TCPSend_Num++;
	TCPSendBuf[TCPSend_Num] = g_modbusReg4[modbusTimeNum];
	TCPSend_Num++;
	for (i = 0; i < size; i++) {
		TCPSendBuf[TCPSend_Num] = g_modbusReg4[modbusTimeNum + i];
		TCPSend_Num++;
	}

	cleanAcc();
}

// Work hours Statistics
void Work_hours_Statistics(void)
{



}


void TaskTimeAcc(void)
{
	uint8_t acc;
	uint8_t moto;
	
	GetRx8025Time();

	if ((g_modbusReg4[modbusTimeNum] > 1) && (g_modbusReg4[modbusTimeData + 1] != ExRtcTime.day))
		ReportAcc();
	
	// 当天日期
	g_modbusReg4[modbusTimeDay] 		= ExRtcTime.year;
	g_modbusReg4[modbusTimeDay + 1] = ExRtcTime.mon;
	g_modbusReg4[modbusTimeDay + 2] = ExRtcTime.day;
	
	acc  = GpsDevice.AccState;
	moto = GpsDevice.MotoState;

	// 所需上传的工作时间 段数
	if (g_modbusReg4[modbusTimeNum] == 30) 
	{
		g_modbusReg4[modbusTimeNum] = 0;
	}
	
	
	if (acc == 0 && g_modbusReg4[modbusACC] == 1) 
	{
		//close
		uint8_t *p = g_modbusReg4 + modbusTimeData + g_modbusReg4[modbusTimeNum] + 1;
		*p = ExRtcTime.mon;
		*p |= (ExRtcTime.year << 4);
		p++;
		*p = ExRtcTime.day;
		p++;
		*p = ExRtcTime.hour;
		p++;
		*p = ExRtcTime.min;
		g_modbusReg4[modbusTimeNum] += 1;
	}
	if (acc == 1 && g_modbusReg4[modbusACC] == 0) 
	{
		//open
		uint8_t *p = g_modbusReg4 + modbusTimeData + g_modbusReg4[modbusTimeNum];
		*p = ExRtcTime.mon;
		*p |= (ExRtcTime.year << 4);
		p++;
		*p = ExRtcTime.day;
		p++;
		*p = ExRtcTime.hour;
		p++;
		*p = ExRtcTime.min;
	}

	g_modbusReg4[modbusMoto] = moto;
	g_modbusReg4[modbusACC] = acc;

	
	
	AutoSaveModbus4();  // 保存统计工时
}

// 保存最后一次发动机运行时间
void SaveLastEngineStartTime(void)
{
	*(usRegHoldingBuf + ADDR_LAST_ENGINE_START_TIME) = ExRtcTime.year;
	*(usRegHoldingBuf + ADDR_LAST_ENGINE_START_TIME + 1) = ExRtcTime.mon;
	*(usRegHoldingBuf + ADDR_LAST_ENGINE_START_TIME + 2) = ExRtcTime.day;
	*(usRegHoldingBuf + ADDR_LAST_ENGINE_START_TIME + 3) = ExRtcTime.hour;
	*(usRegHoldingBuf + ADDR_LAST_ENGINE_START_TIME + 4) = ExRtcTime.min;
	*(usRegHoldingBuf + ADDR_LAST_ENGINE_START_TIME + 5) = ExRtcTime.sec;

	Fram_BufferWrite(g_modbusReg4 + ADDR_LAST_ENGINE_START_TIME*2 ,ADDR_LAST_ENGINE_START_TIME*2, 12);	
	os_dly_wait(1);  // 5MS *1 = 5 mS
}

void SaveFirstEngineStartTime(void)
{
	*(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME) = ExRtcTime.year;
	*(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 1) = ExRtcTime.mon;
	*(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 2) = ExRtcTime.day;
	*(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 3) = ExRtcTime.hour;
	*(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 4) = ExRtcTime.min;
	*(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 5) = ExRtcTime.sec;

	Fram_BufferWrite(g_modbusReg4 + ADDR_FIRST_ENGINE_START_TIME*2 ,ADDR_FIRST_ENGINE_START_TIME*2, 14);	
	os_dly_wait(1);  // 5MS *1 = 5 mS
}


void GetFirstEngineStartTime(RTC_DATE *rtc_date)
{
	rtc_date->year = *(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME);
	rtc_date->mon  = *(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 1);
	rtc_date->day  = *(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 2)	;
	rtc_date->hour = *(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 3);
	rtc_date->min  = *(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 4)	;
	rtc_date->sec  = *(usRegHoldingBuf + ADDR_FIRST_ENGINE_START_TIME + 5)	;

	os_dly_wait(1);  // 5MS *1 = 5 mS
}

void GetFlagOfaNewDay(void)
{
	uint8_t flag = 0;
	uint8_t flagbuf[2] ={0x01,0x00}; // 低位在前
	uint8_t tempbuf[2] ={0x00,0x00};
	CheckNewDay.DayOfCurentYear = GetDayOfYear(ExRtcTime.year, ExRtcTime.mon, ExRtcTime.day);
	
	// 跨0点工作 强制产生新的时间记录
	if(	CheckNewDay.DayOfCurentYear > CheckNewDay.DayOfPrevYear ) // 新的一天
	{
		CheckNewDay.NewFlag = 1;
		VehicleWorkTimeOfOneDay = 0;
		tempbuf[0] = 0;
		tempbuf[1] = 0;
		Fram_BufferWrite(tempbuf ,ADDR_VEHICLE_WORK_TIME_OF_THE_CURRENT_DAY_LOW*2, 2);	

		if(GpsDevice.MotoState)
		{
			SaveFirstEngineStartTime();
			Fram_BufferWrite(flagbuf,3678, 2);	
			os_dly_wait(1);  // 5MS *1 = 5 mS
			CheckNewDay.DayOfPrevYear = CheckNewDay.DayOfCurentYear ; // 更新日期
		}
	} 
	else   // 在同一天内 判断是否需要保存
	{
		Fram_BufferRead(FlagOfaNewDay,3678, 2);
		flag = FlagOfaNewDay[0] | FlagOfaNewDay[1];
		if(! flag ) // flag = 0
		{
			if(GpsDevice.MotoState)
			{
				SaveFirstEngineStartTime();
				Fram_BufferWrite(flagbuf,3678, 2);	
				CheckNewDay.DayOfPrevYear = CheckNewDay.DayOfCurentYear ; // 更新日期

				os_dly_wait(1);  // 5MS *1 = 5 mS
			}
		}
		else
		{
			tempbuf[0] = (VehicleWorkTimeOfOneDay)    &0x00FF;
			tempbuf[1] = (VehicleWorkTimeOfOneDay>> 8)&0x00FF;
			Fram_BufferWrite(tempbuf ,ADDR_VEHICLE_WORK_TIME_OF_THE_CURRENT_DAY_LOW*2, 2);	
			os_dly_wait(1);  // 5MS *1 = 5 mS
			
		}
	}
	
}

uint16_t GetVehicleWorkTimeOfOneDay(void)
{
	uint8_t buf[2] = {0};
	uint16_t time = 0;	
	Fram_BufferRead(buf, ADDR_VEHICLE_WORK_TIME_OF_THE_CURRENT_DAY_LOW*2, 2);

	time = buf[1];
	time = (time << 8) + buf[0];

	*(usRegHoldingBuf + ADDR_VEHICLE_WORK_TIME_OF_THE_CURRENT_DAY_LOW) = time;

	return time;
}

void CheckAccAndMoto(void)
{
	MotoDetect.CurrentVal	= MOTO_STATUS;
	AccDetect.CurrentVal 	= ACC_STATUS;
	
	if(MotoDetect.CurrentVal != MotoDetect.PrevVal)
	{
		MotoDetect.ChangFlag =1;
		if(Gprs.InitFlag)
			if(Gprs.LinkStatus == LINK_STATUS_TCP)
			{
				Update_tcpip_Data();  // 更新数据
				TcpDataUpload();
				os_dly_wait(2);
				MotoDetect.PrevVal = MotoDetect.CurrentVal;
			}

	}
	
	
	
	if(AccDetect.CurrentVal != AccDetect.PrevVal)
	{
		AccDetect.ChangFlag =1;	
		if(Gprs.InitFlag)
			if(Gprs.LinkStatus == LINK_STATUS_TCP)
			{
				Update_tcpip_Data();  // 更新数据
				TcpDataUpload();
				os_dly_wait(2);
				AccDetect.PrevVal = AccDetect.CurrentVal;
			}
	}
	
	




}


