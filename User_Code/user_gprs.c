 #include "user_gprs.h"
GPRS_DATA Gprs;

BOOL GprsTest = 0;

// Location Based Service
void GetLbs(uint8_t * pLBS)
{
	uint8_t lbs[4] = {0};
	uint8_t i = 6;

	if(!AT_CommandSend(UART3SendBuf, "at+creg=2;+creg?\r\n"))
		return;
	os_dly_wait(100);
	
	if (AT_CommandRecv(UART3RecvBuf, 1000)) 
	{
		if(AT_CommandComp(ATRecvBuf,"+CREG:")) 
		{
			while(ATRecvBuf[i] != '"') 
			{
				if (i > 20) return ; // 避免死在此处
				i++;
			}
			if (ATRecvBuf[i + 12] == '"') 
			{
				ascii2data( (char *)lbs, (char *)ATRecvBuf + i + 1, 4);
				ascii2data( (char *)lbs + 2, (char *)ATRecvBuf + i + 8, 4);
			}
			memcpy(pLBS, lbs, 4);
		}
	}
}

//  
void GetSigalStrengthAndLBS(uint8_t TimeInterval)
{
	// TimeInterval 时间间隔 秒
 		if( gsm_time_Count > TimeInterval)   //  5 s 获取一次
 		{			
			gsm_time_Count = 0;

			Gprs.SigalStrength = GetSigalStrength();			 			// 获取信号强度
			g_modbusReg4[modbusGSMRSSI] = Gprs.SigalStrength;		// 获取信号强度
			
			GetLbs(Gprs.Lbs);
 		}
}

void TurnOnGprs(void)
{
	//给GSM模块的电源控制脚一个下降沿启动模块	
	#ifdef DEBUG
	printf("> GSM power on. Wait for 5 seconds. \r\n");
	printf("> ...... \r\n");
	#endif

	MG323B_POWER_ON;
	DelayMS(500);	
	
	MG323B_TERM_ON_LOW;
	DelayMS(700);
 	MG323B_TERM_ON_HIGH;	
	DelayMS(500);	
}


void TurnOffGprs(void)
{
	MG323B_POWER_OFF;
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);	

}


uint8_t SIMCardCheck(void)
{
	// SIM卡在位检测
	if(AT_CommandRun("AT%TSIM\r\n", "TSIM 1",5,1000)==AT_ERROR)
	{
		DeveiceAlarm.Bit.ErrSIMCard = 1;
		return AT_NO_SIM_CARD;
	}
	os_dly_wait(50);
	
	// SIM卡在位检测PIN1 码状态查询
	if(AT_CommandRun("at+cpin?\r\n", "OK",5,1000)==AT_ERROR)
	{
		DeveiceAlarm.Bit.ErrSIMCard = 1;
		return AT_PIN_LOCK;		
	}
	os_dly_wait(50);
	
	DeveiceAlarm.Bit.ErrSIMCard = 0;
	return AT_OK;
}

//void TcpDataHandler(void)
//{
//	int8_t result = 0;
//	switch(Gprs.LinkStatus)
//	{
//		case LINK_STATUS_TCP :
//			if (Gprs.FlagTxTcpData == ENABLE ) 
//			{		
//				Gprs.UploadTimeCount = Gprs.UploadInterval;  // 重新加载上传时间间隔
//				ReloadGprsData(&Tcp); 											 // 重新加载TCP上传数据
//				result =SendGprsDataPackage(&Tcp);           // 上传GPRS数据
//				
//				// 如果没有发生成功
//				if(result != OK)					// 盲区数据存储
//				{
//					SaveGprsDataToFlash(&Tcp);
//				}
//				
//				Gprs.FlagTxTcpData = DISABLE;
//			}

//		break;
//		
//		case (!LINK_STATUS_TCP):
//				if(BlindZoneData.TxFlag == ENABLE)
//				{
//					ReloadGprsData(&Tcp); 											 // 重新加载TCP上传数据

//					// 读取flash
//					// 若上传成功，则lognum --
//					// 否则进入下一次上传
//					SaveGprsDataToFlash(&Tcp);
//					
//					BlindZoneData.TxCount = 0;
//					BlindZoneData.TxFlag  = DISABLE;
//				}
//		
//		break;
//			
//		// 盲区数据
//		case LINK_STATUS_BLIND:
////			if (Gprs.FlagTxTcpData == ENABLE ) 
////			{
////				Gprs.FlagTxTcpData = DISABLE;
////				Gprs.UploadTimeCount = Gprs.UploadInterval;
////				
////				smsDataTimeOut = GSM_SMS_SEND_TIMEOUT;
////				Update_tcpip_Data();  // 更新数据
////			}
////		
////			TcpDataUpload();		
//		
//			
//		break;
//		
//		default :
//		break;
//	}		
//}


// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
void TcpDataHandler(void)
{
	int8_t result = 0;
	uint16_t flashLogNum = 0;
	
	if(GprsTest == 1)
	{
		Gprs.LinkStatus = LINK_STATUS_BLIND;
	}
	
	if(Gprs.LinkStatus == LINK_STATUS_TCP)
	{
		if (Gprs.FlagTxTcpData == ENABLE ) 
		{
				Gprs.UploadTimeCount = Gprs.UploadInterval;  // 重新加载上传时间间隔
				ReloadGprsData(&Tcp); 											 // 重新加载TCP上传数据
				result =SendGprsDataPackage(&Tcp);           // 上传GPRS数据
				
				// 如果没有发生成功
				if(result != OK)					// 盲区数据存储
				{
					SaveGprsDataToFlash(&Tcp);
				}
				
				Gprs.FlagTxTcpData = DISABLE;
		}
	
		// 上传盲区数据
		if((Gprs.FlagTxTcpData == DISABLE)&&(BlindZoneData.TxFlag == ENABLE))
		{
			
			BlindZoneData.TotalLogNums = ReadTotalLogNums();
			BlindZoneData.LogNum = BlindZoneData.TotalLogNums % MAX_LOG_NUMS;
			
			if(BlindZoneData.LogNum != 0)
			{
				flashLogNum = BlindZoneData.LogNum -1;
				
				FlashBufferRead(Tcp.TxData, (flashLogNum * FLASH_PAGE_SIZE), 256); // 加载盲区数据   
				
				result = SendGprsDataPackage(&Tcp);  // 发送盲区数据
				
				if(result == OK)					
				{
					BlindZoneData.LogNum --;
					BlindZoneData.TotalLogNums --;

					WriteTotalLogNums(BlindZoneData.TotalLogNums);		
				}
			}
			
			BlindZoneData.TxCount = 0;
			BlindZoneData.TxFlag  = DISABLE;
		}

	}
	
	else
	{
		if (Gprs.FlagTxTcpData == ENABLE ) 
		{
				Gprs.UploadTimeCount = Gprs.UploadInterval;  // 重新加载上传时间间隔
				ReloadGprsData(&Tcp); 											 // 重新加载TCP上传数据
				SaveGprsDataToFlash(&Tcp);
				Gprs.FlagTxTcpData = DISABLE;
		}
		
	}
	if(GprsTest == 1)
	{
		Gprs.LinkStatus = LINK_STATUS_TCP;
	}
	
}

void FlashData(TCP_DATA * tcp)
{



}








uint8_t TcpSendData(uint8_t * Buffer,uint16_t BufferLen)
{
	char high;
	char low;
	
	ENTER_CRITICAL_SECTION();
	
	AT_CommandSend(UART3SendBuf, "at%ipsend=\"");
	
	// 需要优化
	while(BufferLen--)	
	{	
		high = *Buffer >> 4;
		low = *Buffer & 0xf;
		if (high > 9)
		high = high + 'a' - 0xa;
		else
		high = high + '0';
		if (low > 9)
		low = low + 'a' - 0xa;
		else
		low = low + '0';
		
		
		QueueWrite(UART3SendBuf, high);		
		QueueWrite(UART3SendBuf, low);		
		Buffer++;
	}
	AT_CommandSend(UART3SendBuf, "\"\r\n");
	
	EXIT_CRITICAL_SECTION();

//	DelayMS(10);	// 10ms
	while (AT_CommandRecv(UART3RecvBuf, 3000)) 
	{	    //接收AT命令回应	
		// %IPSEND:1,8
		if(AT_CommandComp(ATRecvBuf,"%IPSEND:1"))	//AT回应处理
			return AT_OK;
	}
	return AT_ERROR;
}


uint8_t CheckTcpLink(void)
{
	uint8_t result = 0;
	result = SIMCardCheck();
	if(result)
	{
		result = TcpIpInit();
		if(result)
		{
			result = GPRS_TCPIP_Connect(serverIP, serverPort);
			if(result)
			{
				result = TcpSendData(LinkMsg, sizeof(LinkMsg));
				if(result) // 发送心跳包	
				{
					result = LINK_STATUS_TCP;
				}
				else
				{
					result = ERR_TCP_SEND;
				}
			}
			else
			{
				result = ERR_TCP_CONNECT;
			}
		}
		else
		{
			result = ERR_TCP_CONFIG;
		}			

	}
	else
	{
		result = ERR_SIM_CARD;
	}
	
	return result;
}




void TcpKeepAlive(void)
{
		if( (Gprs.FlagTxHeartBeat == ENABLE) && (Gprs.LinkStatus == LINK_STATUS_TCP))
		{
			Gprs.FlagTxHeartBeat = DISABLE;
			Gprs.HeartBeatTimeCount = Gprs.HeartBeatInterval;
			
			if(TcpSendData(LinkMsg, sizeof(LinkMsg)))
			{
				#ifdef DEBUG
				printf("> TCPIP Link.\r\n");
				#endif
			}
			else
			{
				Gprs.LinkStatus = LINK_STATUS_SMS;
			}
		}
}

void TcpDataUpload(void)
{
	uint8_t  tempTCPSendBuf[256] = {0};
	uint8_t  tempTCPSend_Num = 0;
	uint16_t i = 0;	
	
	if (TCPSend_Num != 0) 
	{
		memcpy(tempTCPSendBuf,TCPSendBuf,TCPSend_Num);
		
		tempTCPSendBuf[TCPSend_Num] = 0;
		tempTCPSend_Num = TCPSend_Num;
		TCPSend_Num = 0;
		
		if(TcpSendData(tempTCPSendBuf,tempTCPSend_Num))
		{
			#ifdef DEBUG
			printf("> Data Upload OK!\r\n");
			for(i = 0; i < tempTCPSend_Num; i++)
			{
				printf("0x%02x ",tempTCPSendBuf[i]);
			}
			printf(" \r\n");			
			#endif
			g_needReportAlarm = 0;
			// 保存最后一次发送的时间
			LogDate.UpdateLogDateEnable = ENABLE;

		}
		else
		{
			#ifdef DEBUG
			printf("> Data Upload Fail!\r\n");
			#endif
			Gprs.LinkStatus = LINK_STATUS_SMS;
		}
	}


}



void ModbusTCPPool( void )
{
	uint8_t temp = 0;
	uint8_t result = 0;
	// 若无数据接收 则返回
	if(!AT_CommandRecv(UART3RecvBuf, 1000))	       //超时没有接收到数据
		return ;

	GPRS_TCPIP_Deal();
	
	if (TCPReceive_Num > 0) 
	{
		result = IapRemoteInterface(TCPReceiveBuf);		// 远程升级数据协议
		
		if( result == PROTOCOL_MODBUS)
		{
			ModbusInputTcp(TCPReceiveBuf, TCPReceive_Num);
			if(TCPReceiveBuf[0] == 0x06 || TCPReceiveBuf[0] == 0x10)
			{
				SaveModbus4();
				os_dly_wait(50);
			}
//			if(TCPReceiveBuf[0] == 0x06 && TCPReceiveBuf[1] == 0x01 && TCPReceiveBuf[2] == 0xA5)
//			{
//				RemoteLock.Value = TCPReceiveBuf[4];
//				RemoteLock.bit0 = (TCPReceiveBuf[4] & BIT0);
//				RemoteLock.bit1 = (TCPReceiveBuf[4] & BIT1) >> 1;
//				RemoteLock.bit2 = (TCPReceiveBuf[4] & BIT2) >> 2;
//				RemoteLock.bit3 = (TCPReceiveBuf[4] & BIT3) >> 3;
//			}
		}

		TCPReceive_Num 		= 0;
		TCPReceiveBuf[0] 	= 0;	

	}		
}

// 检查GPRS网络连接
uint8_t CheckTheNetworkConnection(void)
{
	// 网络运营商注册查询
	if(AT_CommandRun("AT+COPS?\r\n", "OK", 3,1000)==AT_ERROR)
		return AT_ERROR; 
	
	// 查询GPRS连接状态
	if(AT_CommandRun("AT+CGATT?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// 注册移动的CMNET网关
	if(AT_CommandRun("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;

	// GPRS网络是否可用？
	// +CGREG:x,5或CGREG:x,1 都表示可以使用GPRS网络
	if(AT_CommandRun("AT+CGREG?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// GPRS初始化，等待返回IP
	if(AT_CommandRun("AT%ETCPIP\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;

	// 查询GPRS初始化是否成功
	if(AT_CommandRun("AT%ETCPIP?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;	
	
	return AT_OK;
}

// 连接到网络服务器
void Connect2TcpServer(void)
{
	//  the TCP server
	GetServerIP(serverIP, 16, serverPort, 6);
	
	if(GPRS_TCPIP_Connect(serverIP, serverPort)) // max try times : 5
	{
		Gprs.LinkStatus	= LINK_STATUS_TCP;
		// 立即上传数据
		TcpSendData(LinkMsg, sizeof(LinkMsg)) ; // 发送心跳包
		os_dly_wait(1); // delay 10ms
		Update_tcpip_Data();
		TcpDataUpload();		
		os_dly_wait(1); // delay 10ms
		
	}
	else 
	{
		Gprs.LinkStatus = UNABLE_TO_CONNECT;
	}
}



// tcpip 错误处理机制
void GprsErrorModeHandler(void)
{
	if(Gprs.LinkStatus != LINK_STATUS_TCP)
	{
		Gprs.RetriesTimes ++;
		if(	Gprs.RetriesTimes < 20)
		{
			// 检查GPRS网络连接 
			Gprs.TcpInitStatus = CheckTheNetworkConnection();
			if(Gprs.TcpInitStatus == AT_OK) 
			{
				Connect2TcpServer();	  // 连接到服务器	
			}		
			else
			{
				Gprs.LinkStatus =	LINK_STATUS_SMS;
			}
			os_dly_wait(20);  // 5MS *20 = 100 mS

		}
		else
		{
			Gprs.FirstSendFlag =0;
			Gprs.RetriesTimes = 0;
//			TurnOffGprs();
//			TurnOnGprs();
			Gprs.InitFlag = 0;
			Gprs.LinkStatus = LINK_STATUS_UNKNOWN;
			
		// 软件复位
			MG323B_RESET_ENABLE;
			os_dly_wait(50);  // 10MS *50 = 500 mS
			MG323B_RESET_DISABLE;			
		}
	}
	else
	{
			Gprs.RetriesTimes = 0;
	}
}


