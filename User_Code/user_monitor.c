#include "user_monitor.h"

uint32_t Countdown_Count = 0; // 倒计时
// uint8_t Flag_System_Standby = DISABLE;

uint16_t AdminLock		= DISABLE;
uint16_t AdminEnable	= DISABLE;
uint8_t relay[4];

TCP_ADMIN_COMMAND AdminCommand;

ADMIN_LOCK uAdminLock;

uint8_t  Flag_Enter_Standby = DISABLE; // 系统进入 standby 
uint8_t  Flag_Start_Standby = DISABLE; // 系统进入 standby 

uint8_t  Count_Start_Standby; // 系统进入 standby 
uint8_t  Count_Enter_Standby; // 系统进入 standby


uint32_t CommTimeOut = 0; // 通讯超时

LOG_DATE LogDate;


uint16_t DeviceLcokLevel = 0;

REMOTE_LOCK RemoteLock;





uint8_t WorkSleepModeSwitchHandler(void)
{
	if(Check_Acc_Status() == ACC_ON)
	{
		GpsDevice.WorkSleepMode = WORK_SLEEP_MODE_0;
		GpsDevice.AccOffTimeCount = 0;
		*(usRegHoldingBuf + 248) = 0; // 不休眠
	}
//		GpsDevice.RunOrTestMode = *(usRegHoldingBuf + DEVEICE_RUN_OR_TEST);

	
	if(GpsDevice.WorkSleepMode == WORK_SLEEP_MODE_0)
		if(GpsDevice.AccOffTimeCount >= 1800)
				GpsDevice.WorkSleepMode = WORK_SLEEP_MODE_1;


	if(DeveiceAlarm.Bit.LowPower == 1)
				GpsDevice.WorkSleepMode = WORK_SLEEP_MODE_2;
		

	if(DeveiceAlarm.Bit.NoPower == 1)
				GpsDevice.WorkSleepMode = WORK_SLEEP_MODE_3;
	
	return GpsDevice.WorkSleepMode ;


}


// runtime 系统运行时间处理函数
void RuntimeSwtichHandler(uint8_t workmode)
{
	switch(workmode)
	{
		case WORK_SLEEP_MODE_0:
		{
			GpsDevice.UploadTimeInterval = 60; // 10 mins
		}
		break;
		
		case WORK_SLEEP_MODE_1:
		{
			GpsDevice.WorkTime 	= 360; // 6 mins
			GpsDevice.SleepTime = 3600; // 1 hour
			GpsDevice.UploadTimeInterval = 600; // 10 mins
			
		}
		break;
		
		case WORK_SLEEP_MODE_2:
		{
			GpsDevice.WorkTime 	= 360; 		// 6 mins
			GpsDevice.SleepTime = 3600*5; // 5 hours
			GpsDevice.UploadTimeInterval = 600; // 10 mins
			
		}			
		break;
		
		case WORK_SLEEP_MODE_3:
		{
			GpsDevice.WorkTime 	= 360; 	  // 6 mins
			GpsDevice.SleepTime = 3600*8; // 8 hours
			GpsDevice.UploadTimeInterval = 600; // 10 mins
		}			
		break;

		case WORK_SLEEP_MODE_4:
		{
			GpsDevice.WorkTime 	= 360; 	  // 6 mins
			GpsDevice.SleepTime = 3600*8; // 8 hours
			GpsDevice.UploadTimeInterval = 60; // 1 min
		}	
		break;		
		
		default :
		{
			GpsDevice.WorkTime 	= 360; // 6 mins
			GpsDevice.SleepTime = 360; // 1 hour
			GpsDevice.UploadTimeInterval = 120; // 10 mins
			
		}
		break;
	}
}



// 如果收到TCP数据则 退出2分钟睡眠倒计时
void Check_QuitStandby(void)
{
	Count_Enter_Standby = 0;
	Flag_Enter_Standby = DISABLE;
}

void EnterStandby(uint16_t sleep_hour, uint16_t sleep_min)
{
	Gprs.FlagTxTcpData = DISABLE;
	Flag_SMS_Send_Data = DISABLE;
	
	//▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
	if(sleep_hour == 0)
	sleep_min = sleep_min < 1 ? 1 : sleep_min;// 限定最小休眠时间： 1 min
//▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
	
	sleep_hour = sleep_hour > 24 ? 24 : sleep_hour;
	
	#ifdef RESET_TEST
	BSP_RTC_Timer_Alarm (sleep_hour,  sleep_min); 
	#else
	{
		BSP_RTC_Timer_Alarm (sleep_hour,  (sleep_min + 3)); 
		DelayMS(200);
		BSP_RTC_Timer_WakeUp(sleep_hour, 	sleep_min);
		DelayMS(200);
	}
	#endif

	uSystem.Bit.sleep = DEVICE_SLEEP; 
	*(int32u*)(g_modbusReg4 + MODBUS_SystemStatus) = uSystem.StatusValue; 
	#ifdef DEBUG
	printf("> MG323B enter power saving.\r\n");
	#endif
	
	// 断开TCP连接
	GPRS_TCPIP_DisConnect();

	GSM_Power_Saver_Handler();
	PWR_EnterSTANDBYMode();

}


uint8_t Check_Acc_Status(void)
{
	uint8_t result = 0;
	// when ACC is ON
	if (g_modbusReg4[modbusACC] == 1)
	{
		result = ACC_ON;	
		Countdown_Count = 0;
		uSystem.Bit.acc = ACC_ON;
		uSystem.Bit.sleep = DEVICE_RUN;
	}
	
	// ACC OFF
	else
	{
		result = ACC_OFF;	
	
		uSystem.Bit.acc = ACC_OFF;
		uSystem.Bit.sleep = DEVICE_SLEEP;
	}
	return result;
}

void System_Standby_Handler(void)
{
	// When ACC is off.
	if(Check_Acc_Status() == ACC_OFF)
	{
		// 0 不休眠
		if(!*(usRegHoldingBuf + 248))
			Countdown_Count = 0;
					
		// 测试模式
		if(GpsDevice.RunOrTestMode == MODE_TEST)
		{
			DeviceWorkTime  = *(usRegHoldingBuf +DEVEICE_WORK_SECONDS_HIGH);
			DeviceWorkTime  = (DeviceWorkTime << 16 ) + *(usRegHoldingBuf +DEVEICE_WORK_SECONDS_LOW);

			DeviceSleepTime = *(usRegHoldingBuf + DEVEICE_SLEEP_SECONDS_HIGH);
			DeviceSleepTime = (DeviceSleepTime << 16) + *(usRegHoldingBuf + DEVEICE_SLEEP_SECONDS_LOW);
		}
		// 正常工作模式
		else
		{
			DeviceWorkTime  = GpsDevice.WorkTime;
			DeviceSleepTime	=	GpsDevice.SleepTime;
		}
		
		SleepTime.hour = DeviceSleepTime/3600;		
		SleepTime.min  = (DeviceSleepTime%3600)/60;
		SleepTime.sec  =  (DeviceSleepTime%3600)%60;

		
		*(usRegHoldingBuf + 270) = DeviceWorkTime;
		*(usRegHoldingBuf + 271) = DeviceSleepTime;
		*(usRegHoldingBuf + 272) = Countdown_Count;
		
		#ifdef DEBUG
		printf("> Countdown_Count: %d , Count_Enter_Standby: %d \r\n",Countdown_Count,Count_Enter_Standby);
		printf("> Flag_Enter_Standby: %d\r\n",Flag_Enter_Standby);
		#endif	
	
		if(Flag_Enter_Standby == ENABLE) 
		{
			EnterStandby(SleepTime.hour,SleepTime.sec);
		}

		}
}


// 通信中断处理函数
uint8_t CommunicationBreakdownHandler(uint32_t timeCount)
{ 
	uint32_t timeOut = *(usRegHoldingBuf + COMM_TIMEOUT_UART);
	uint8_t result = 0;
	if(timeCount >=  timeOut)
	{
		timeCount = timeOut;
		result = 1; // 超时
	}
	else
	{
		result = 0;
	}
	
	return result;
}




// 开壳报警 GPS天线报警
void SystemStatusDetect(void)
{
	if (ShellOpenDetect() || GpsAntDetect() ) 
	{
		g_needReportAlarm = 1;		
	}
	DeveiceAlarm.Bit.Openshell = ShellOpenDetect();
	
	
	DeveiceAlarm.Bit.GpsAnt = GpsAntDetect();

	g_modbusReg4[modbusShell] 	= DeveiceAlarm.Bit.Openshell;
	g_modbusReg4[modbusAntenna] = DeveiceAlarm.Bit.GpsAnt;
	
	GpsDevice.ShellState 	= ShellOpenDetect();
	GpsDevice.GpsAntState = GpsAntDetect();
	
	GpsDevice.AccState  = ACC_STATUS;
	GpsDevice.MotoState = MOTO_STATUS;
	

	GpsDevice.RelayAState = 1^IO1_STATUS;
	GpsDevice.RelayBState = 1^IO2_STATUS;
	GpsDevice.RelayCState = 1^IO3_STATUS;
	GpsDevice.RelayDState = 1^IO4_STATUS;
		
	g_modbusReg4[RELAY_A_STATUS] = GpsDevice.RelayAState;
	g_modbusReg4[RELAY_B_STATUS] = GpsDevice.RelayBState;
	g_modbusReg4[RELAY_C_STATUS] = GpsDevice.RelayCState;
	g_modbusReg4[RELAY_D_STATUS] = GpsDevice.RelayDState;
	
	
	DeviceLcokLevel = *(usRegHoldingBuf + ADDR_LOCK_LEVEL);
	SetLockLevel(DeviceLcokLevel);
	GpsDevice.LockLevel = GetLockLevel();
	
// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
// AdminEnable   	
	AdminEnable = *(usRegHoldingBuf + ADDR_ADMIN_RIGHTS_ENABLE);
	SetAdminRights(AdminEnable);
	AdminEnable = GetAdminRights();
	
// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
// 永久解锁	
	AdminCommand.UnLockEnable = *(usRegHoldingBuf + ADDR_ADMIN_UNLOCK_ENABLE);
	SetAdminUnlockCommamd(AdminCommand.UnLockEnable);
	AdminCommand.UnLockEnable = GetAdminUnlockCommamd();

// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
//	uAdminLock.Vaule = *(usRegHoldingBuf + ADDR_ADMIN_LOCK_STATE);
//	SetAdminLockState(uAdminLock.Vaule);
//	uAdminLock.Vaule = GetAdminLockState();

	uAdminLock.Bit.LockA =  g_modbusReg4[842] & 0x01;
	uAdminLock.Bit.LockB =  (g_modbusReg4[842] & 0x02) >> 1 ;
	uAdminLock.Bit.LockC =  (g_modbusReg4[842] & 0x04) >> 2 ;
	uAdminLock.Bit.LockD =  (g_modbusReg4[842] & 0x08 )>> 3 ;
	
	if((uAdminLock.Vaule&0x0f) != 0)
		GpsDevice.RemoteLockState = 1;
	else
	GpsDevice.RemoteLockState = 0;
	
//	uAdminLock.Bit.LockA = uAdminLock.Vaule&0x01;
//	uAdminLock.Bit.LockB = (uAdminLock.Vaule >> 1)&0x01;	
//	uAdminLock.Bit.LockC = (uAdminLock.Vaule >> 2)&0x01;	
//	uAdminLock.Bit.LockD = (uAdminLock.Vaule >> 3)&0x01;

	GpsDevice.RunOrTestMode = *(usRegHoldingBuf + DEVEICE_RUN_OR_TEST);
//	GpsDevice.RunOrTestMode = 1;

	SetRunOrTestMode(GpsDevice.RunOrTestMode);
	GpsDevice.RunOrTestMode = GetRunOrTestMode();
	
		
	if(DeveiceAlarm.AlarmValue&0x07)
		LED_RED_ON;
	else
		LED_RED_OFF;
}


void LockAll(void)
{
	BSP_GPIO_IO1_Set(GPS_LOCK);
	BSP_GPIO_IO2_Set(GPS_LOCK);
	BSP_GPIO_IO3_Set(GPS_LOCK);
	BSP_GPIO_IO4_Set(GPS_LOCK);
}

void UnlockAll(void)
{
	BSP_GPIO_IO1_Set(GPS_UNLOCK);
	BSP_GPIO_IO2_Set(GPS_UNLOCK);
	BSP_GPIO_IO3_Set(GPS_UNLOCK);
	BSP_GPIO_IO4_Set(GPS_UNLOCK);
}

void LockHandler(void)
{
	if(AdminCommand.UnLockEnable != 0)
	{
		UnlockAll(); // 全部解锁
	}
	else
	{
		// 出现报警值
		if((DeveiceAlarm.AlarmValue & 0x07) != NO_ALARM )
		{
			LockAll();
		}
		else
		{
			if(uAdminLock.Bit.LockA)
				BSP_GPIO_IO1_Set(GPS_LOCK);
			else
				BSP_GPIO_IO1_Set(GPS_UNLOCK);
			
			if(uAdminLock.Bit.LockB)
				BSP_GPIO_IO2_Set(GPS_LOCK);
			else
				BSP_GPIO_IO2_Set(GPS_UNLOCK);	
			
			if(uAdminLock.Bit.LockC)
				BSP_GPIO_IO3_Set(GPS_LOCK);
			else
				BSP_GPIO_IO3_Set(GPS_UNLOCK);	

			if(uAdminLock.Bit.LockD)
				BSP_GPIO_IO4_Set(GPS_LOCK);
			else
				BSP_GPIO_IO4_Set(GPS_UNLOCK);				
			
		}			



	}

}
// return 1 发生变化
// return 0 未发生变化
uint8_t CheckAlarmValue(void)
{
	uint8_t  result = 0;
	DeveiceAlarmLastValue = DeveiceAlarm.AlarmValue;
	
	if(DeveiceAlarmLastValue != DeveiceAlarmPrevValue)
	{
		if(DeveiceAlarmLastValue > DeveiceAlarmPrevValue) // 报警增加
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	
	DeveiceAlarmPrevValue = DeveiceAlarmLastValue; // 
	return result ;
}


void ReportAlarmEvent(void)
{
	if(Gprs.InitFlag)
	{

		if(DeveiceAlarm.AlarmValue != NO_ALARM )
		{
			if( CheckAlarmValue() )
			{
				if(Gprs.LinkStatus == LINK_STATUS_TCP)
				{	
					Update_tcpip_Data();
					TcpDataUpload();
					os_dly_wait(10);  // 10MS *10=1000 mS
				}
			}
				
		}
		else
		{
			DeveiceAlarmPrevValue = 0;
			DeveiceAlarmLastValue = 0;	
		}
	}
}


uint16_t GetDayOfYear(uint8_t year_8u, uint8_t month, uint8_t day)
{
	uint16_t sum_days = 0;
	uint16_t year = year_8u + 2000;

	switch (month - 1)
    {
        case 11:sum_days += 30;
        case 10:sum_days += 31;
        case 9: sum_days += 30;
        case 8: sum_days += 31;
        case 7: sum_days += 31;
        case 6: sum_days += 30;
        case 5: sum_days += 31;
        case 4: sum_days += 30;
        case 3: sum_days += 31;
        case 2: sum_days += 28;
        case 1: sum_days += 31; break;
        default:break;
    }

		// 判断是否为闰年
		if((year%4==0&&year%100!=0)||(year%400==0))       
			if (month > 2)
					sum_days++;

    sum_days += day;
			
// ----------------------------------------------
		// 以2013为时基
		if(year >= 2013)	
			sum_days = sum_days + (year -2013)*365;
			
    return sum_days;
}




void UpdateLogDate(void)
{
	uint8_t datebuf[12] = {0};
	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_YEAR) = ExRtcTime.year;
	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_MON)  = ExRtcTime.mon;
	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_DAY)  = ExRtcTime.day;
	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_HOUR) = ExRtcTime.hour;
	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_MIN)  = ExRtcTime.min;
	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_SEC)  = ExRtcTime.sec;

	memcpy(datebuf,g_modbusReg4+880,12);

	// data length (6*2=)12 bytes
	Fram_BufferWrite(datebuf, ADDR_REMOTE_COMM_LOG_YEAR*2, 12);

	os_dly_wait(1); // delay 5ms

}

void RemoteCommBreakdownInterval(LOG_DATE * tempLogDate)
{
// 获取远程通信日志的日期
	tempLogDate->LogYear = *(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_YEAR);
	tempLogDate->LogMon  = *(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_MON);
	tempLogDate->LogDay  = *(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_DAY);
	tempLogDate->DayOfLogYear = GetDayOfYear(tempLogDate->LogYear, tempLogDate->LogMon, tempLogDate->LogDay);
	
	tempLogDate->CurentYear = ExRtcTime.year;
	tempLogDate->CurentMon 	= ExRtcTime.mon;
	tempLogDate->CurentDay 	= ExRtcTime.day;
	tempLogDate->DayOfCurentYear = GetDayOfYear(tempLogDate->CurentYear, tempLogDate->CurentMon, tempLogDate->CurentDay);
	
	if(tempLogDate->DayOfCurentYear >= tempLogDate->DayOfLogYear)
		tempLogDate->TimeOutDays = tempLogDate->DayOfCurentYear - tempLogDate->DayOfLogYear ;
	else
		tempLogDate->TimeOutDays = 0xFF; // 错误表示
	
	*(usRegHoldingBuf + ADDR_REMOTE_COMM_TIMEOUT_DAYS) = tempLogDate->TimeOutDays;

	tempLogDate->TimeOutSetDays = *(usRegHoldingBuf + ADDR_REMOTE_COMM_TIMEOUT_SETDAYS);
	tempLogDate->TimeOutSetDays	= tempLogDate->TimeOutSetDays < 1 ? 1 : tempLogDate->TimeOutSetDays;

}

// 远程通信中断处理函数
void RemoteCommBreakdownHandler(void)
{
// RTC_timestamp
	
	if(LogDate.UpdateLogDateEnable == ENABLE)
	{
		LogDate.UpdateLogDateEnable = DISABLE;
		UpdateLogDate();
	}
	
	RemoteCommBreakdownInterval(&LogDate);
	
	if(LogDate.TimeOutDays < 51)
		GpsDevice.RemoteCommunicationState = 0;
	else
	{
			if(LogDate.TimeOutDays >= 51)
				GpsDevice.RemoteCommunicationState = 1;
				
			if(LogDate.TimeOutDays >= 58)
				GpsDevice.RemoteCommunicationState = 2;
			if(LogDate.TimeOutDays >= 60)
				GpsDevice.RemoteCommunicationState = 3;			

	}
	
	
	
	
	
	
	if(LogDate.TimeOutDays >  LogDate.TimeOutSetDays)
		DeveiceAlarm.Bit.ErrRemote = 1;
	else
		DeveiceAlarm.Bit.ErrRemote = 0;

}

uint16_t GetLockLevel(void)
{
	uint16_t locklevel = 0;
	uint8_t buf[2] = {0};

	Fram_BufferRead(buf, ADDR_LOCK_LEVEL*2, 2);	
	locklevel = buf[1];
	locklevel = (locklevel << 8) + buf[0];

	*(usRegHoldingBuf + ADDR_LOCK_LEVEL) = locklevel; // 更新ram
	
	return locklevel;

}

void SetLockLevel(uint16_t locklevel)
{
	uint8_t buf[2] = {0};
	buf[0] = (locklevel)    &0x00FF;
	buf[1] = (locklevel>> 8)&0x00FF;
	Fram_BufferWrite(buf ,ADDR_LOCK_LEVEL*2, 2);	
	os_dly_wait(1);  // 5MS *1 = 5 mS

}

// 获取管理员权限
uint16_t GetAdminRights(void)
{
	uint16_t adminrights = 0;
	uint8_t buf[2] = {0};

	Fram_BufferRead(buf, ADDR_ADMIN_RIGHTS_ENABLE*2, 2);	
	adminrights = buf[1];
	adminrights = (adminrights << 8) + buf[0];

	*(usRegHoldingBuf + ADDR_ADMIN_RIGHTS_ENABLE) = adminrights; // 更新ram
	
	return adminrights;

}

void SetAdminRights(uint16_t adminrights)
{
	uint8_t buf[2] = {0};
	buf[0] = (adminrights)    &0x00FF;
	buf[1] = (adminrights>> 8)&0x00FF;
	Fram_BufferWrite(buf ,ADDR_ADMIN_RIGHTS_ENABLE*2, 2);	
	os_dly_wait(1);  // 5MS *1 = 5 mS

}

// 获取永久解锁值
uint16_t GetAdminUnlockCommamd(void)
{
	uint16_t unlockstate = 0;
	uint8_t buf[2] = {0};

	Fram_BufferRead(buf, ADDR_ADMIN_UNLOCK_ENABLE*2, 2);	
	unlockstate = buf[1];
	unlockstate = (unlockstate << 8) + buf[0];

	*(usRegHoldingBuf + ADDR_ADMIN_UNLOCK_ENABLE) = unlockstate; // 更新ram
	
	return unlockstate;

}
// 获取永久解锁值
void SetAdminUnlockCommamd(uint16_t unlockstate)
{
	uint8_t buf[2] = {0};
	buf[0] = (unlockstate)    &0x00FF;
	buf[1] = (unlockstate>> 8)&0x00FF;
	Fram_BufferWrite(buf ,ADDR_ADMIN_RIGHTS_ENABLE*2, 2);	
	os_dly_wait(1);  // 5MS *1 = 5 mS

}









// 获取管理员权限
uint16_t GetAdminLockState(void)
{
	uint16_t admin_lock_state = 0;
	uint8_t buf[2] = {0};

	Fram_BufferRead(buf, ADDR_ADMIN_LOCK_STATE*2, 2);	
	admin_lock_state = buf[1];
	admin_lock_state = (admin_lock_state << 8) + buf[0];

	*(usRegHoldingBuf + ADDR_ADMIN_LOCK_STATE) = admin_lock_state; // 更新ram
	
	return admin_lock_state;

}


void SetAdminLockState(uint16_t admin_lock_state)
{
	uint8_t buf[2] = {0};
	buf[0] = (admin_lock_state)    &0x00FF;
	buf[1] = (admin_lock_state>> 8)&0x00FF;
	Fram_BufferWrite(buf ,ADDR_ADMIN_LOCK_STATE*2, 2);	
	os_dly_wait(1);  // 5MS *1 = 5 mS

}

//	DeviceLcokLevel = *(usRegHoldingBuf + ADDR_LOCK_LEVEL);
//	SetLockLevel(DeviceLcokLevel);
//	GpsDevice.LockLevel = GetLockLevel();



uint16_t GetRunOrTestMode(void)
{
	uint16_t mode = 0;
	uint8_t buf[2] = {0};

	Fram_BufferRead(buf, DEVEICE_RUN_OR_TEST*2, 2);	
	mode = buf[1];
	mode = (mode << 8) + buf[0];

	*(usRegHoldingBuf + DEVEICE_RUN_OR_TEST) = mode; // 更新ram
	
	return mode;

}


void SetRunOrTestMode(uint16_t mode)
{
	uint8_t buf[2] = {0};
	buf[0] = (mode)    &0x00FF;
	buf[1] = (mode>> 8)&0x00FF;
	Fram_BufferWrite(buf ,DEVEICE_RUN_OR_TEST*2, 2);	
	os_dly_wait(1);  // 5MS *1 = 5 mS

}


uint16_t GetCoolHigh(void)
{
	uint16_t CoolHigh = 0;
	uint8_t buf[2] = {0};

	Fram_BufferRead(buf, 2110*2, 2);	
	CoolHigh = buf[1];
	CoolHigh = (CoolHigh << 8) + buf[0];

	*(usRegHoldingBuf + 2110) = CoolHigh; // 更新ram
	
	return CoolHigh;

}

uint16_t GetOilLow(void)
{
	uint16_t OilLow = 0;
	uint8_t buf[2] = {0};

	Fram_BufferRead(buf, 2111*2, 2);	
	OilLow = buf[1];
	OilLow = (OilLow << 8) + buf[0];

	*(usRegHoldingBuf + 2111) = OilLow; // 更新ram
	
	return OilLow;

}






