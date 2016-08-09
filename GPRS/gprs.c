#include "includes.h"

uint8_t  ATRecvBuf[512]={0};
uint16_t ATRecv_Num = 0;

uint8_t  TCPSendBuf[256] = {0};
uint16_t TCPSend_Num = 0;
uint8_t  TCPReceiveBuf[256] = {0};
uint16_t TCPReceive_Num = 0;
uint8_t  TCPHeartBuf[TCP_LINK_BUFFER_SIZE] = {0};
uint16_t TCPHeart_Num = 0;

uint16_t heartTimeOut;// = GSM_HEART_SEND_TIMEOUT;
uint16_t tcpTimeOut = GSM_TCP_RECONNECT_TIMEOUT;
uint16_t smsTimeOut = GSM_SMS_RECONNECT_TIMEOUT;
uint32_t tcpDataTimeOut;// = GSM_DATA_SEND_TIMEOUT;
//uint32_t smsDataTimeOut = GSM_SMS_SEND_TIMEOUT;
uint16_t clearModbusTimeOut = MODBUS_CLEAR_TIMEOUT;
uint8_t g_gsmStatus = GSM_STATUS_UNKNOWN;
uint16_t g_reconnectTime = 0;
uint8_t LinkMsg[10]= {0x47, 0x08, 0x54, 0x43, 0x50, 0x49, 0x50, 0x0D, 0x0A, 0x00};
uint8_t gsm_time_Count = 0;	 // 获取GSM信号、LBS 计数



uint8_t Flag_TCP_Send_Data = ENABLE;
uint8_t Flag_SMS_Send_Data = DISABLE;
uint8_t Flag_TCP_Send_Heartbeat = ENABLE;



uint8_t serverIP[16] ={0};
uint8_t serverPort[6]={0};
uint8_t serverConnect = 0;

uint8_t Flag_Gprs_Init = NONE;



SMS_DATA SMS;

void DisablePowerKey(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	
  	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;	// TERM_ON
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	//带上拉输入
	  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

// 使能开机键
void EnablePowerKey(void)
{
	// POWER ON
	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; // TERM_ON
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);
}



void MG323B_PowerON(void)
{
	//给GSM模块的电源控制脚一个下降沿启动模块	
	#ifdef DEBUG
	printf("> GSM power on. Wait for 5 seconds. \r\n");
	printf("> ...... \r\n");
	#endif
	MG323B_RESET_DISABLE;  // disable reset
	MG323B_POWER_OFF;
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
	
	MG323B_POWER_ON;       // power on
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
	
	MG323B_TERM_ON_HIGH;   // 置高
	
	DelayMS(500);
	
	MG323B_TERM_ON_LOW;
	DelayMS(1000);
	
	MG323B_TERM_ON_HIGH;
}


/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void MG323B_Configuration(void)								      ▇
▇ 函数功能：GSM模块工作模式配置																▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
*/
uint8_t MG323B_Configuration(void)
{
	// 确认串口正常，波特率 =115200
	if(AT_CommandRun("AT\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	
	// 关闭回显
	if(AT_CommandRun("ATE0\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	

// 	// 查询当前网络： CHINA MOBILE
// 	if(AT_CommandRun("AT+COPS?\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;

// 	// 查询当地网络信号强度，最大31，最小0
// 	if(AT_CommandRun("AT+CSQ\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;
	
	// 查询当前网络注册状态
// 	if(AT_CommandRun("AT+CREG?\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;	
	
	
	/*
		Manufacture: HUAWEI
		Model: MG323-B
		Revision: 11.210.09.06.00
		Number: 863679010068150
		+GCAP: +CGSM
	*/		
// 	if(AT_CommandRun("ATI\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;	
	
	
// 短信设置	
// 	// 查询存储器状态
// 	if(AT_CommandRun("AT+CPMS?\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;		
	
	// txt
// 	if(AT_CommandRun("AT+CMGF=1\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;	
// 	
// 	// 删除SIM卡上的所有短信
// 	if(AT_CommandRun("AT+CMGD=1,4\r\n", "OK", 10,1000)==AT_ERROR)
// 		return AT_ERROR;	

	return AT_OK;
}

uint8_t MG323B_Init(void)
{	
	// 确认串口正常，波特率 =115200
	
	if(AT_CommandRun("AT\r\n", "OK", 3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// 关闭回显
	if(AT_CommandRun("ATE0\r\n", "OK", 3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// SIM卡在位检测
	if(AT_CommandRun("AT%TSIM\r\n", "TSIM 1",3,1000)==AT_ERROR)
		return AT_ERROR;		

	// SIM卡在位检测PIN1 码状态查询
	if(AT_CommandRun("at+cpin?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// 查询当前网络： CHINA MOBILE
	if(AT_CommandRun("AT+COPS?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;

	// 查询当地网络信号强度，最大31，最小0
	if(AT_CommandRun("AT+CSQ\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;

	


	// 查询当前网络注册状态
// 	if(AT_CommandRun("AT+CREG?\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;	
	
	
	/*
		Manufacture: HUAWEI
		Model: MG323-B
		Revision: 11.210.09.06.00
		Number: 863679010068150
		+GCAP: +CGSM
	*/		
// 	if(AT_CommandRun("ATI\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;	
	
	
// 短信设置	
// 	// 查询存储器状态
// 	if(AT_CommandRun("AT+CPMS?\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;		
	
	// txt
// 	if(AT_CommandRun("AT+CMGF=1\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;	
// 	
 	// 删除SIM卡上的所有短信
 	if(AT_CommandRun("AT+CMGD=1,4\r\n", "OK", 3,1000)==AT_ERROR)
 		return AT_ERROR;	

	return AT_OK;
}

void MG323B_Close(void)
{
	MG323B_POWER_OFF;
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
// 	GPIO_ResetBits(GPIOE, GPIO_Pin_15);
// 	GPIO_SetBits(GPIOE, GPIO_Pin_13);
			
	#ifdef DEBUG
	printf("> MG323B has been  closed!\r\n");
	#endif
}



void ResetGsm(void)
{
	if(MG323B_Init()) 
	{
		#ifdef DEBUG
		printf("> Initialization of MG323B is complete.\r\n");
		#endif
		GSM_TCP_Init();
	}
	else
	{
		#ifdef DEBUG
		printf("> Initialization of MG323B is failed!\r\n");
		#endif
		Gprs.LinkStatus = 	LINK_STATUS_ERROR;
	}
}


void GSM_TCP_Init(void)
{
	Gprs.TcpInitStatus = TcpIpInit();
	if(Gprs.TcpInitStatus == AT_OK) 
	{
		LinkToTcpServer();	  // 连接到服务器	
	}		
	else
	{
		#ifdef DEBUG
		printf("> Can not connect to the GPRS-TCP net.\r\n"); // 未连接到GPRS-TCP
		#endif
		Gprs.LinkStatus =	LINK_STATUS_SMS;
	}
}




// 链接到ＴＣＰ服务器
uint8_t LinkToTcpServer(void)
{
	uint8_t result = 0;
	if (serverConnect < 2) 
		GetServerIP(serverIP, 16, serverPort, 6);//  the primary server
	else 
		GetServerIPBack(serverIP, 16, serverPort, 6);// the standby server
	
	serverConnect++;

	serverConnect = serverConnect > 4 ? 0 : serverConnect;
	
	#ifdef DEBUG
	printf("> Server IP:  %s \r\n", serverIP);
	printf("> Server Port:%s \r\n", serverPort);
	#endif

	
	if(GPRS_TCPIP_Connect(serverIP, serverPort)) // max try times : 5
	{
		result = LINK_STATUS_TCP;
		serverConnect = 0;

		Update_tcpip_Data(); // 更新数据
		GPRS_Data_Upload_Handler();				// 立即上传数据
		DelayMS(100);	// 10ms

		GPRS_TCPIP_Send(LinkMsg, sizeof(LinkMsg));
		DelayMS(100);	// 10ms

	}
	else 
	{
		result = 	LINK_STATUS_SMS;
	}
	return result;
}



//  TcpPortMonitor
void TcpPortMonitor(void)
{
//	if(Gprs.LinkStatus == LINK_STATUS_TCP)
//	{
//		if(Check_ip_port() != 0)
//		{	
//			Gprs.LinkStatus = GSM_STATUS_NEWLINK;		
//		}
//	}
	if(Gprs.LinkStatus == GSM_STATUS_NEWLINK)
	{
		if(TcpIpInit() == AT_OK) 
		{
			if(GPRS_TCPIP_Connect(serverIP, serverPort)) // max try times : 5
			{
				#ifdef DEBUG
				printf("> LINK_STATUS_TCP.\r\n");
				#endif
				Gprs.LinkStatus 	= LINK_STATUS_TCP;
				serverConnect = 0;
				// 立即上传数据
				GPRS_TCPIP_Send(LinkMsg, sizeof(LinkMsg)) ; // 发送心跳包
				Update_tcpip_Data(); // 更新数据
				GPRS_Data_Upload_Handler();		
				DelayMS(10);	// 10ms
		// 		
				#ifdef DEBUG
				printf("> TCPIP. Heartbeat.\r\n"); // 已连接到服务器
				#endif
			}
			else 
			{
				Gprs.LinkStatus = 	LINK_STATUS_SMS;
				
				#ifdef DEBUG
				printf("> Failed to connect server.\r\n");  // 连接不到服务器
				#endif
			}
		}		
		else
		{
			#ifdef DEBUG
			printf("> Can not connect to the GPRS-TCP net.\r\n"); // 未连接到GPRS-TCP
			#endif
			Gprs.LinkStatus =	LINK_STATUS_SMS;
		}
		tcpTimeOut = GSM_TCP_RECONNECT_TIMEOUT; // 1min timeout
	}

}


uint8_t GPRS_Data_Upload_Handler(void)
{
	uint8_t  tempTCPSendBuf[256] = {0};
	uint8_t  tempTCPSend_Num = 0;
	uint16_t i = 0;	
	uint8_t result = 0;

	if (TCPSend_Num != 0) 
	{
		memcpy(tempTCPSendBuf,TCPSendBuf,TCPSend_Num);
		
		tempTCPSendBuf[TCPSend_Num] = 0;
		tempTCPSend_Num = TCPSend_Num;
		TCPSend_Num = 0;
		
		if(GPRS_TCPIP_Send(tempTCPSendBuf,tempTCPSend_Num))
		{
			g_needReportAlarm = 0;
			result = 1;
		}
		else
		{
			Gprs.LinkStatus = LINK_STATUS_SMS;
			result = 0;
		}
	}

	if(!GPRS_SMS_Recv()) 
	{		
		#ifdef DEBUG
		printf("> SMS receive error.\r\n");
		#endif
	}	
	return result;
	
}


void SMS_Handler(void)
{
	uint8_t tempFlashSendBuf[256];
	uint8_t tempFlashSend_Num;
	uint8_t i;
	char  tempbuf[160] = "xxxxxxxxx Hello world! Small moves..Small movesSmall. movesSmall moves...Small movesSmall..Small movesSmall..Small movesSmall..Small    140.................160";	
	char  tempNum[14];
	char  yecx[] = "YECX";
	char  ChinaMobile[]="10086";

		// 获取手机号
		sprintf(SMS.TxMsg,"%3.4f E,%2.5f N\r\nAdmin:%d,Lock:%d\r\nA:%1d,B:%1d,C:%1d,D:%1d\r\nAnt:%d,Open:%1d,Com:%1d\r\nIp:%d.%d.%d.%d\r\nPort:%d\r\n",
		TwoPointLocation.fNowLong,TwoPointLocation.fNowLat,
		AdminCommand.UnLockEnable, uAdminLock.Vaule,
		1^IO1_STATUS, 1^IO2_STATUS, 1^IO3_STATUS, 1^IO4_STATUS,
		DeveiceAlarm.Bit.GpsAnt, DeveiceAlarm.Bit.Openshell, DeveiceAlarm.Bit.ErrComm,
		g_modbusReg4[modbusServerIP],g_modbusReg4[modbusServerIP+1],g_modbusReg4[modbusServerIP+2],g_modbusReg4[modbusServerIP+3],
		*(int16u*)(g_modbusReg4 + modbusServerPort)	);
		
		SMS.Length = strlen(SMS.TxMsg);
		
		memcpy(tempNum, g_modbusReg4 + modbusSMSNumberAlarm, 13);
		tempNum[13] = 0;
		

//		if (SendSMSChar(tempNum, SMS.TxMsg) == AT_OK) 
//		{
//			printf("> Send SMS!\r\n");
//		}
//		else 
//		{			
//			Gprs.LinkStatus = LINK_STATUS_ERROR;
//			printf("> Save blind data!\r\n");
//		}
		SendSMSChar(ChinaMobile, yecx);
//  CXYE  10086		
	os_dly_wait(200);
	if(!GPRS_SMS_Recv()) 
	{
		printf("sms receive error\r\n");
		Gprs.LinkStatus = LINK_STATUS_ERROR;
	}

}


/*********************************************************************************************************
**函数信息 ： TcpIpInit                    
**功能描述 ： GSM/GPRS模块TCPIP配置
**输入参数 ：       
**输出参数 ：【AT_OK】初始化成功，【AT_ERROR】初始化失败
********************************************************************************************************/
uint8_t TcpIpInit(void)
{
	// 网络运营商注册查询
	if(AT_CommandRun("AT+COPS?\r\n", "OK", 3,1000)==AT_ERROR)
		return AT_ERROR; 
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	// 查询GPRS连接状态
	if(AT_CommandRun("AT+CGATT?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS

	// 注册移动的CMNET网关
	if(AT_CommandRun("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS

	// GPRS网络是否可用？
	// +CGREG:x,5或CGREG:x,1 都表示可以使用GPRS网络
	if(AT_CommandRun("AT+CGREG?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	// ETCPIP 前加个2秒延时
	os_dly_wait(200);  // 10MS *50 = 500 mS

	// GPRS初始化，等待返回IP
	if(AT_CommandRun("AT%ETCPIP\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(100);  // 10MS *50 = 500 mS

	// 查询GPRS初始化是否成功
	if(AT_CommandRun("AT%ETCPIP?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;	
	os_dly_wait(100);  // 10MS *50 = 500 mS

	return AT_OK;
}
/*********************************************************************************************************
**函数信息 ： AT_CommandRun                    // WAN.CG // 2012.07.04
**功能描述 ： GSM/GPRS模块AT命令发送、接收和处理
**输入参数 ：【command】命令  【respond】正确的返回值 【times】如果失败，重复次数 【timeout】接收超时      
**输出参数 ：【AT_OK】成功，【AT_ERROR】失败
**调用提示 ：
********************************************************************************************************/
uint8_t AT_CommandRun(char *command,char *respond,uint8_t times,uint16_t timeout)
{
	while(times--)
	{
		if(!AT_CommandSend(UART3SendBuf, command))		//发送AT命令
			continue;

		if(!AT_CommandRecv(UART3RecvBuf, timeout))	    //接收AT命令回应
			continue;

			//如果是异常回复，需要进行处理，比如短信等?
		if(AT_CommandComp(ATRecvBuf,respond))		    //AT回应处理
			return AT_OK;	
	}
	return AT_ERROR;						            //AT回应出错
}


/*********************************************************************************************************
**函数信息 ： AT_CommandSend                    // WAN.CG // 2012.07.04
**功能描述 ： GSM/GPRS模块AT命令发送
**输入参数 ：【sendbuf】命令发送缓冲区  【COMMAND】命令 【times】如果失败，命令发送重复次数      
**输出参数 ：【AT_OK】成功，【AT_ERROR】失败
**调用提示 ：
********************************************************************************************************/
uint8_t AT_CommandSend(void *sendbuf, char *Command)
{
   	if(*Command == '\0')
		return AT_ERROR;					
	QueueWriteStr(sendbuf, Command);		//发送AT命令
	return AT_OK; 
}

/*********************************************************************************************************
**函数信息 ： AT_CommandRecv                    // WAN.CG // 2012.07.05
**功能描述 ： GSM/GPRS模块AT命令回应数据接收
**输入参数 ：【recvbuf】接收缓冲区   【timeout】接收超时
**输出参数 ：【AT_OK】成功，【AT_ERROR】失败
**调用提示 ： 需要用到定时器 2 和全局变量 Timer2_count
********************************************************************************************************/
uint8_t AT_CommandRecv(void *recvbuf,uint16_t timeout)
{
	uint16_t	i,Queue_Num;	
	uint16_t	tempnum = 0;
	Timer2_count = 0; 			              		//接收超时开始计时
	while(!QueueNData((void*)recvbuf))	      //没有数据，等待
	{ 
		if(Timer2_count > timeout)
			return AT_ERROR;				  //超时，出错
	}

	while(1)
	{
		Queue_Num = QueueNData((void*)recvbuf);
		Timer2_count = 0; 		              //开始计时
		while((Queue_Num == QueueNData((void*)recvbuf) ))	 //等待数据接收完成
		{ 					
			if(Timer2_count > 5)
			{
				memset(ATRecvBuf,0,sizeof(ATRecvBuf));				
				
				ATRecv_Num = Queue_Num;
				for(i=0;i<Queue_Num;i++)
				{
					QueueRead(&ATRecvBuf[i], (void*)recvbuf);       //转移数据
				}
				return AT_OK;
			}
// 			os_dly_wait(1);
		}
// 		os_dly_wait(1);			 
	}
}
/*********************************************************************************************************
**函数信息 ： AT_CommandComp                    // WAN.CG // 2012.07.04
**功能描述 ： 比较收到的命令是否与源字符串相同
**输入参数 ：【Command】目标字符串  【str】源字符串
**输出参数 ：【AT_OK】成功，【AT_ERROR】失败
**调用提示 ：
********************************************************************************************************/
uint8_t AT_CommandComp(void *command, char *str)
{
	// extern char *strstr(char *str1, char *str2);
	// 重字符串str1中查找字符串str2
	if(strstr(command,str) == NULL)
		return AT_ERROR;
	else
		return AT_OK; 
}
/*********************************************************************************************************
**函数信息 ： SendSMS                    // WAN.CG // 2012.07.05
**功能描述 ： GSM/GPRS模块短信发送函数
**输入参数 ：【pSMSCenter】短消息中心号码  【pSMSRecipient】接收号码 【pSMSContent】短信内容      
**输出参数 ：
**调用提示 ：
********************************************************************************************************/
uint8_t SendSMS140(char* pSMSCenter, char* pSMSRecipient, char* pSMSContent)
{
	uint8_t  byteH = 0, byteL = 0;
  	uint16_t i = 0, j = 0, k = 0;
  	uint16_t nSMSCenterLen= 0, nSMSPduLen = 0, nPduBufferIndex = 0;
  	char   tmpBuffer[128];
	char   pduBuffer[384];
	char   chTmp;
	 
//   	for(i = 0; i < sizeof(pduBuffer); i++)	  //清空缓冲区
//     	pduBuffer[i] = 0x00;
	memset(pduBuffer,0,sizeof(pduBuffer));				

	

/********************************************************************************************************	
**   下面是PDU组包过程
   * |-----------------------------------------------------------------|
   * |SCA  |PDUType  |MR  |DA   |PID  |DCS  |VP     |UDL  |UD    |END  |
   * |-----------------------------------------------------------------|
   * |1-12 |1        |1   |2-12 |1    |1    |0,1,7  |1    |0-140 |1    | 
   * |-----------------------------------------------------------------|
   * |     |01       |00  |     |00   |08   |AA     |     |      |0x1A |
   * |-----------------------------------------------------------------|
*********************************************************************************************************	
**短信中心号码处理
********************************************************************************************************/	  
//将短信息中心号码去掉+号，看看长度是否为偶数，如果不是，最后添加F 	
	for(k = 0; pSMSCenter[k] != '\0'; k++)		 
    	tmpBuffer[k] = pSMSCenter[k];
  	if(k%2 != 0) 
    	tmpBuffer[k++] = 'F';
			   
//算出 addr 长度，结果除2，格式化成2位的16进制字符串，16 / 2 = 8 => "08"
	i = k/2+1;
  	byteH = 0x0F&(i>>4);
  	if(byteH <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteH+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteH-0x0A+'A';

  	byteL = 0x0F&i;
  	if(byteL <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteL+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteL-0x0A+'A';

//将短信息中心号码前面加上字符91，91是国际化的意思
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//将奇数位和偶数位交换
	for(i = 0, j = k/2; i < j; i++)
  	{
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2+1];
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2];
  	}
  	nSMSCenterLen = nPduBufferIndex/2;

//! PDU类型
  	pduBuffer[nPduBufferIndex++] = '1';
  	pduBuffer[nPduBufferIndex++] = '1';	
  //! For MR
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '0';

/*********************************************************************************************************	
**手机号码处理
********************************************************************************************************/
  	for(k = 0; pSMSRecipient[k] != 0; k++)
    	tmpBuffer[k] = pSMSRecipient[k];

//算出 addr 长度，结果除2，格式化成2位的16进制字符串
  	byteH = 0x0F&(k>>4);
  	if(byteH <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteH+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteH-0x0A+'A';
  	byteL = 0x0F&k;

  	if(byteL <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteL+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteL-0x0A+'A';

//号码前面加上字符91，91是国际化的意思
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//将手机号码去掉+号，看看长度是否为偶数，如果不是，最后添加F
  	if(k%2 != 0)
    	tmpBuffer[k++] = 'F';

//将奇数位和偶数位交换
  	for(i = 0, j = k/2; i < j; i++)
  	{
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2+1];
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2];
  	}

//! For PID
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '0';
 //! For DCS
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '4';
 //! For VP
  	pduBuffer[nPduBufferIndex++] = 'A';
  	pduBuffer[nPduBufferIndex++] = 'A';

/*********************************************************************************************************	
**短信息部分处理
********************************************************************************************************/
  i = nPduBufferIndex;
  pduBuffer[nPduBufferIndex++] = '0';
  pduBuffer[nPduBufferIndex++] = '0'; 

  k = 0;
  while(pSMSContent[k++] != '\0');
  
  //j = Encode_GBK2_UCS2(pSMSContent, k, pduBuffer+nPduBufferIndex,sizeof(pduBuffer)-nPduBufferIndex);
  j = k - 1;
  for(k = 0; k < j; k++) {
  	chTmp = (pSMSContent[k] >> 4) & 0xf;
	if(chTmp <= 0x09)
    	pduBuffer[i+2+k+k] = chTmp-0x00+'0';
	else
		pduBuffer[i+2+k+k] = chTmp-0x0A+'A';
	chTmp = pSMSContent[k] & 0xf;	
    if(chTmp <= 0x09)
    	pduBuffer[i+2+k+k+1] = chTmp-0x00+'0';
	else
		pduBuffer[i+2+k+k+1] = chTmp-0x0A+'A';
  }
  j = j+j;
  
  nPduBufferIndex += j;

  k = j/2;
  byteH = 0x0F&(k>>4);
  if(byteH <= 0x09)
    pduBuffer[i] = byteH+'0';
  else
    pduBuffer[i] = byteH-0x0A+'A';

  byteL = 0x0F&k;
  if(byteL <= 0x09)
    pduBuffer[i+1] = byteL+'0';
  else
    pduBuffer[i+1] = byteL-0x0A+'A';

  //! PDU串的长度,后面AT+CMGS要用到此长度
  nSMSPduLen = nPduBufferIndex/2-nSMSCenterLen;

  //! For END
  pduBuffer[nPduBufferIndex++] = 0x1A;
  pduBuffer[nPduBufferIndex++] = 0x00;

/*********************************************************************************************************	
**下面是发送过程
********************************************************************************************************/
	if(AT_CommandRun("AT\r\n", "OK", 10, 1000)==AT_ERROR)
		return AT_ERROR;
	if(AT_CommandRun("ATE0\r\n", "OK", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	if(AT_CommandRun("AT+CMGF=0\r\n", "OK", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	
	sprintf(tmpBuffer, "AT+CMGS=%d\r\n", nSMSPduLen);
	if(AT_CommandRun(tmpBuffer, ">", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	
	printf("设置号码成功...\r\n");
	
	if(!AT_CommandSend(UART3SendBuf, pduBuffer))		//发送短信内容
		return AT_ERROR;
// 	DebugLog(pduBuffer);	
	printf("\r\n");
	printf("信息已发送，等待SIM900回应...\r\n");

	for(k = 0; k < 200; k++)
	{
		if(!AT_CommandRecv(UART3RecvBuf, 1000))		    //接收AT命令回应
		{
			if(AT_CommandComp(ATRecvBuf,"OK"))		    //AT回应处理
				return AT_OK;
			if(AT_CommandComp(ATRecvBuf,"ERROR"))		//AT回应处理
				return AT_ERROR;
		}
		os_dly_wait(1);
	}
	return AT_ERROR;
}
uint8_t SendSMS70(char* pSMSCenter, char* pSMSRecipient, char* pSMSContent)
{
	uint8_t  byteH = 0, byteL = 0;
  	uint16_t i = 0, j = 0, k = 0;
  	uint16_t nSMSCenterLen= 0, nSMSPduLen = 0, nPduBufferIndex = 0;
  	char   tmpBuffer[128];
	char   pduBuffer[384];
	 
  	for(i = 0; i < sizeof(pduBuffer); i++)	  //清空缓冲区
    	pduBuffer[i] = 0x00;

/********************************************************************************************************	
**   下面是PDU组包过程
   * |-----------------------------------------------------------------|
   * |SCA  |PDUType  |MR  |DA   |PID  |DCS  |VP     |UDL  |UD    |END  |
   * |-----------------------------------------------------------------|
   * |1-12 |1        |1   |2-12 |1    |1    |0,1,7  |1    |0-140 |1    | 
   * |-----------------------------------------------------------------|
   * |     |01       |00  |     |00   |08   |AA     |     |      |0x1A |
   * |-----------------------------------------------------------------|
*********************************************************************************************************	
**短信中心号码处理
********************************************************************************************************/	  
//将短信息中心号码去掉+号，看看长度是否为偶数，如果不是，最后添加F 	
	for(k = 0; pSMSCenter[k] != '\0'; k++)		 
    	tmpBuffer[k] = pSMSCenter[k];
  	if(k%2 != 0) 
    	tmpBuffer[k++] = 'F';
			   
//算出 addr 长度，结果除2，格式化成2位的16进制字符串，16 / 2 = 8 => "08"
	i = k/2+1;
  	byteH = 0x0F&(i>>4);
  	if(byteH <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteH+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteH-0x0A+'A';

  	byteL = 0x0F&i;
  	if(byteL <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteL+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteL-0x0A+'A';

//将短信息中心号码前面加上字符91，91是国际化的意思
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//将奇数位和偶数位交换
	for(i = 0, j = k/2; i < j; i++)
  	{
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2+1];
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2];
  	}
  	nSMSCenterLen = nPduBufferIndex/2;

//! PDU类型
  	pduBuffer[nPduBufferIndex++] = '1';
  	pduBuffer[nPduBufferIndex++] = '1';	
  //! For MR
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '0';

/*********************************************************************************************************	
**手机号码处理
********************************************************************************************************/
  	for(k = 0; pSMSRecipient[k] != 0; k++)
    	tmpBuffer[k] = pSMSRecipient[k];

//算出 addr 长度，结果除2，格式化成2位的16进制字符串
  	byteH = 0x0F&(k>>4);
  	if(byteH <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteH+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteH-0x0A+'A';
  	byteL = 0x0F&k;

  	if(byteL <= 0x09)
    	pduBuffer[nPduBufferIndex++] = byteL+'0';
  	else
    	pduBuffer[nPduBufferIndex++] = byteL-0x0A+'A';

//号码前面加上字符91，91是国际化的意思
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//将手机号码去掉+号，看看长度是否为偶数，如果不是，最后添加F
  	if(k%2 != 0)
    	tmpBuffer[k++] = 'F';

//将奇数位和偶数位交换
  	for(i = 0, j = k/2; i < j; i++)
  	{
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2+1];
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2];
  	}

//! For PID
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '0';
 //! For DCS
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '8';
 //! For VP
  	pduBuffer[nPduBufferIndex++] = 'A';
  	pduBuffer[nPduBufferIndex++] = 'A';

/*********************************************************************************************************	
**短信息部分处理
********************************************************************************************************/
  i = nPduBufferIndex;
  pduBuffer[nPduBufferIndex++] = '0';
  pduBuffer[nPduBufferIndex++] = '0'; 

  k = 0;
  while(pSMSContent[k++] != '\0');
  j = Encode_GBK2_UCS2(pSMSContent, k, pduBuffer+nPduBufferIndex,sizeof(pduBuffer)-nPduBufferIndex);
  nPduBufferIndex += j;

  k = j/2;
  byteH = 0x0F&(k>>4);
  if(byteH <= 0x09)
    pduBuffer[i] = byteH+'0';
  else
    pduBuffer[i] = byteH-0x0A+'A';

  byteL = 0x0F&k;
  if(byteL <= 0x09)
    pduBuffer[i+1] = byteL+'0';
  else
    pduBuffer[i+1] = byteL-0x0A+'A';

  //! PDU串的长度,后面AT+CMGS要用到此长度
  nSMSPduLen = nPduBufferIndex/2-nSMSCenterLen;

  //! For END
  pduBuffer[nPduBufferIndex++] = 0x1A;
  pduBuffer[nPduBufferIndex++] = 0x00;

/*********************************************************************************************************	
**下面是发送过程
********************************************************************************************************/
	if(AT_CommandRun("AT\r\n", "OK", 10, 1000)==AT_ERROR)
		return AT_ERROR;
	if(AT_CommandRun("ATE0\r\n", "OK", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	if(AT_CommandRun("AT+CMGF=0\r\n", "OK", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	
	sprintf(tmpBuffer, "AT+CMGS=%d\r\n", nSMSPduLen);
	if(AT_CommandRun(tmpBuffer, ">", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	
	printf("设置号码成功...\r\n");
	
	if(!AT_CommandSend(UART3SendBuf, pduBuffer))		//发送短信内容
		return AT_ERROR;
// 	DebugLog(pduBuffer);	
	printf("\r\n");
	printf("信息已发送，等待回应...\r\n");
//	return AT_OK;
	
	for(k = 0; k < 200; k++)
	{
		if(!AT_CommandRecv(UART3RecvBuf, 1000))		    //接收AT命令回应
		{
			if(AT_CommandComp(ATRecvBuf,"OK"))		    //AT回应处理
				return AT_OK;
			if(AT_CommandComp(ATRecvBuf,"ERROR"))		//AT回应处理
				return AT_ERROR;
		}
		os_dly_wait(1);
	}
	return AT_ERROR;
}
/********************************************************************************************************	
**text方式发送短信
********************************************************************************************************/
uint8_t SendSMSChar(char* pSMSRecipient, char* pSMSContent)
{
	char   tmpBuffer[128];
	uint16_t k = 0;
	char temp[2];
	temp[0] = 0x1a;
	temp[1] = '\0';
	

/*********************************************************************************************************	
**下面是发送过程
********************************************************************************************************/
	if(AT_CommandRun("AT\r\n", "OK", 10, 1000)==AT_ERROR)
		return AT_ERROR;
	if(AT_CommandRun("ATE0\r\n", "OK", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	if(AT_CommandRun("AT+CMGF=1\r\n", "OK", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	if(AT_CommandRun("AT+CSCS=\"GSM\"\r\n", "OK", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	
	sprintf(tmpBuffer, "AT+CMGS=\"%s\"\r\n", pSMSRecipient);
	if(AT_CommandRun(tmpBuffer, ">", 1, 1000)==AT_ERROR)
		return AT_ERROR;
	
	#ifdef DEBUG
	printf("设置号码成功...\r\n");
	#endif
	
	if(!AT_CommandSend(UART3SendBuf, pSMSContent))		//发送短信内容
		return AT_ERROR;

	if (!AT_CommandSend(UART3SendBuf, temp))
		return AT_ERROR;
	
	#ifdef DEBUG
	printf("信息已发送，等待SIM900回应...\r\n");
	#endif
	
	for(k = 0; k < 200; k++)
	{
		if(!AT_CommandRecv(UART3RecvBuf, 1000))		    //接收AT命令回应
		{
			if(AT_CommandComp(ATRecvBuf,"OK"))		    //AT回应处理
				return AT_OK;
			if(AT_CommandComp(ATRecvBuf,"ERROR"))		//AT回应处理
				return AT_ERROR;
		}
//		os_dly_wait(1);
	}
	return AT_ERROR;
}
/*********************************************************************************************************
**函数信息 ： GPRS_TCPIP_Connect                    // WAN.CG // 2012.07.05
**功能描述 ： 连接服务器
**输入参数 ：【IP_ADDR】IP地址  【IP_PORT】端口号     
**输出参数 ： 【AT_OK】成功，【AT_ERROR】失败
********************************************************************************************************/
uint8_t GPRS_TCPIP_Connect(uint8_t * IP_ADDR,uint8_t * IP_PORT)
{
	uint8_t times = 5;
	uint8_t tempbuf[50];
	
	// 设置接收服务器的协议类型 (TCP 、IP、端口号)
	// 
	sprintf((void *)tempbuf, "AT%%IPOPEN=\"TCP\",\"%s\",%s\r\n", IP_ADDR, IP_PORT);
	g_reconnectTime++;
	
	while(times--)
	{
		if(!AT_CommandSend(UART3SendBuf, (void *)tempbuf))
			return AT_ERROR; 	
		os_dly_wait(100);  // 10MS *100 = 1000 mS
		
		if(AT_CommandRecv(UART3RecvBuf, 1000)==AT_ERROR)	    //接收AT命令回应
			continue;
		
		if(AT_CommandComp(ATRecvBuf,"CONNECT")==AT_OK)		//connect
			return AT_OK;
		
		//already connect
		if(AT_CommandComp(ATRecvBuf,"ERROR: 9")==AT_OK)		
			return AT_OK;
		
		if(AT_CommandComp(ATRecvBuf,"ERROR: 20")==AT_OK) 
		{	//需要etcpip
			if (TcpIpInit())
				continue;
			else
				return AT_ERROR;
		}
	}
	return AT_ERROR; 
}
/*********************************************************************************************************
**函数信息 ： GPRS_TCPIP_DisConnect                    // WAN.CG // 2012.07.05
**功能描述 ： 断开与服务器的连接
**输入参数 ：    
**输出参数 ： 【AT_OK】成功，【AT_ERROR】失败
********************************************************************************************************/
uint8_t GPRS_TCPIP_DisConnect(void)
{
   //AT+CIPCLOSE:断开连接
	if(AT_CommandRun("AT%IPCLOSE\r", "%IPCLOSE: 1", 10,1000)==AT_ERROR)
		return AT_ERROR; 

	return AT_OK;  
}
/*********************************************************************************************************
**函数信息 ： GPRS_TCPIP_DisConnect                    // WAN.CG // 2012.07.05
**功能描述 ： 关闭网诺功能
**输入参数 ：    
**输出参数 ： 【AT_OK】成功，【AT_ERROR】失败
********************************************************************************************************/
uint8_t GPRS_TCPIP_NETShut(void)
{
	//if(AT_CommandRun("AT+CIPSHUT\r\n", "SHUT OK", 1,1000)==AT_ERROR)
	//	return AT_ERROR; 

	return AT_OK;  
}
/*********************************************************************************************************
**函数信息 ： GPRS_TCPIP_Send                    // WAN.CG // 2012.07.05
**功能描述 ： 发送数据
**输入参数 ：【Buffer】发送的数据，【BufferLen】数据长度   
**输出参数 ：【AT_OK】成功/数据长度  【AT_ERROR】失败
********************************************************************************************************/
uint8_t GPRS_TCPIP_Send(uint8_t * Buffer,uint16_t BufferLen)
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

	DelayMS(10);	// 10ms
	while (AT_CommandRecv(UART3RecvBuf, 2000)) 
	{	    //接收AT命令回应	
		// %IPSEND:1,8
		if(AT_CommandComp(ATRecvBuf,"%IPSEND:1"))	//AT回应处理
			return AT_OK;
	}
	return AT_ERROR;
}

uint8_t GPRS_TCPIP_Deal(void)
{
	if(AT_CommandComp(ATRecvBuf,"%IPDATA:"))	
	{
		Check_QuitStandby();
		return GPRS_TCPIP_GetData();
	}
	
	
	if(AT_CommandComp(ATRecvBuf,"%IPCLOSE: 1"))		    //断开连接
	{
		return AT_ERROR;
	}

	if(AT_CommandComp(ATRecvBuf,"CALL")) 
	{
		return AT_OK;
	}
	
	return AT_OK;
}
/*********************************************************************************************************
**函数信息 ： GPRS_TCPIP_Recv                    
**输入参数 ：【Buffer】接收的数据   
**输出参数 ：【AT_OK】成功/数据长度  【AT_ERROR】失败
********************************************************************************************************/
uint8_t GPRS_TCPIP_Recv(void)
{
	// 若无数据接收 则返回
	if(!AT_CommandRecv(UART3RecvBuf, 1000))	       //超时没有接收到数据
		return AT_OK;

	return GPRS_TCPIP_Deal();
}

char* findData(char* src, int srcSize, char* target)
{
	char* tmp;
	int size = 0;
	if (strlen(src) == srcSize)
		return strstr(src, target);
	else {
		while (size < srcSize) {
			tmp = strstr((char*)(src + size), target);
			if (tmp != NULL)
				return tmp;
			else {
				size += strlen(src + size);
				size++;
			}
		}
		return NULL;
	}
}
char* findDataFromEnd(char* src, int srcSize, char target)
{
// 	int currentPosition;
	if (strlen(src) == srcSize)
		return strrchr(src, target);
	while(srcSize != 0) {
		srcSize--;
		if (*(char*)(src + srcSize) == target)
			return (char*)(src + srcSize);
	}
	return NULL;
}
void data2Data(char * des, char * src, int size)
{
	while (size != 0) {
		*des = *src;
		des++;
		src++;
		size--;
	}
}


uint8_t GPRS_TCPIP_GetData(void)
{
	char * AddrFirstData = NULL;
	char * AddrLastData = NULL;
	uint8_t i =0 ;

	uint32_t dataLength = 0;

//	ENTER_CRITICAL_SECTION();
	
	// 数据过滤,取出数据长度
	sscanf((char *)ATRecvBuf, "\r\n%%IPDATA:%d,%*s", &dataLength);

	AddrFirstData = findData((char*)ATRecvBuf, ATRecv_Num, "\"");
	AddrFirstData++;
		
	AddrLastData = findDataFromEnd((char*)ATRecvBuf, ATRecv_Num, '"');
	
	// ascii 码数据长度是 hex的2倍
	dataLength = dataLength << 1;
	
	if (dataLength != (AddrLastData - AddrFirstData))
	{
		#ifdef DEBUG
		printf("> TCP data length is wrong!\r\n");
		#endif
		return AT_ERROR;
	}
	
	ascii2data((char*)TCPReceiveBuf, AddrFirstData, dataLength);
	TCPReceive_Num = dataLength >> 1;
	
	#ifdef DEBUG
	printf("> TCP Data Length: %3d\r\n",TCPReceive_Num);
	printf("> TCPReceiveBuf  :");
	for(i=0;i<TCPReceive_Num;i++)
	{
		printf(" 0x%02X",*(TCPReceiveBuf+i));
	}
	printf("\r\n");
	#endif
	
//	EXIT_CRITICAL_SECTION();
	return AT_OK;
}


uint8_t GPRS_SMS_Recv(void)
{
	if (!AT_CommandRun("at+cmgf=1\r\n", "OK", 3, 1000))
			return AT_ERROR;
	if (!AT_CommandSend(UART3SendBuf, "at+cmgl=\"REC UNREAD\"\r\n"))
			return AT_ERROR;
	if (!AT_CommandRecv(UART3RecvBuf, 5000))	    //接收AT命令回应
			return AT_ERROR;
	return GPRS_SMS_Read();
}
uint8_t GPRS_SMS_Read(void)
{
	char readOrNot[8] = {0};//REC UNREAD or REC READ
	char smsNumber[16] = {0};//13
	char smsTime[32] = {0};//20
	char smsContent[180] = {0};//160
	char* pATRecvBuf;
	char smsTemp[32] = {0};
	char smsID[2] = {0};
	uint16_t smsSize;
// 	#if OS_CRITICAL_METHOD == 3         
// 		OS_CPU_SR  cpu_sr;
// 	#endif

	if (ATRecv_Num > 3)
		if (strstr((char *)ATRecvBuf, "\r\nOK") == (char *)ATRecvBuf)
			return AT_OK;

	if (AT_CommandComp(ATRecvBuf, "+CMGL: ")) 
	{
		pATRecvBuf = (char *)ATRecvBuf;
		sscanf(pATRecvBuf, "%[^,]", smsTemp);
		sscanf(smsTemp, "\r\n+CMGL: %d", (int *)smsID);
		pATRecvBuf += strlen(smsTemp);
		pATRecvBuf += 1;
		sscanf(pATRecvBuf, "%[^,]", smsTemp);
		sscanf(smsTemp, "\"REC %[^\"]", readOrNot);
		pATRecvBuf += strlen(smsTemp);
		pATRecvBuf += 1;
		sscanf(pATRecvBuf, "%[^,]", smsTemp);
		sscanf(smsTemp, "\"%[^\"]", smsNumber);
		pATRecvBuf += strlen(smsTemp);
		pATRecvBuf += 1;
		sscanf(pATRecvBuf, "%[^,]", smsTemp);
		pATRecvBuf += strlen(smsTemp);
		pATRecvBuf += 1;
		sscanf(pATRecvBuf, "%[^\r]", smsTemp);
		
		sscanf(smsTemp, "\"%[^\"]", smsTime);
		pATRecvBuf += strlen(smsTemp);
		pATRecvBuf += 2;
		
		smsSize = ATRecv_Num - ((uint8_t *)pATRecvBuf - ATRecvBuf);
		data2Data(smsContent, pATRecvBuf, smsSize);
		pATRecvBuf = smsContent + smsSize;
		pATRecvBuf -= 2;
		*pATRecvBuf = 0;
		
// 		DebugLog(smsID);
// 		DebugLog(readOrNot);
// 		DebugLog(smsNumber);
// 		DebugLog(smsTime);
// 		DebugLog(smsContent);

		if (strstr(smsNumber, (char *)g_modbusReg4 + modbusSMSNumberAlarm)) {
			//字符转ascII交modbus处理
			smsSize = smsSize >> 1;
// 			OS_ENTER_CRITICAL();
			TCPReceive_Num = smsSize;
			smsSize = smsSize << 1;
			ascii2data((char*)TCPReceiveBuf, smsContent, smsSize);
// 			OS_EXIT_CRITICAL();
		}
		if (!AT_CommandRecv(UART3RecvBuf, 50000))	    //接收AT命令回应
			return AT_ERROR;
		if (AT_CommandComp(ATRecvBuf, "OK"))
			return AT_OK;
	}
	else
		return GPRS_TCPIP_Deal();
	
}






// Location Based Service
void LocationBasedService(void)
{
	uint8_t lbs[4] = {0};
	uint8_t i = 6;
		
		if(!AT_CommandSend(UART3SendBuf, "at+creg=2;+creg?\r\n"))
			return;
		

		if (AT_CommandRecv(UART3RecvBuf, 2000)) 
		{
			if(AT_CommandComp(ATRecvBuf,"+CREG:")) 
			{
				while(ATRecvBuf[i] != '"') 
				{
					if (i > 14)
						return;
					i++;
				}
				if (ATRecvBuf[i + 12] == '"') 
				{
					ascii2data( (char *)lbs, (char *)ATRecvBuf + i + 1, 4);
					ascii2data( (char *)lbs + 2, (char *)ATRecvBuf + i + 8, 4);
				}
				memcpy(g_modbusReg4 + modbusLBSMessage, lbs, 4);
			}
		}
}


// signal strength
uint16_t GetSigalStrength(void)
{
	uint8_t 	i = 5;
	uint8_t 	buf[2] = {0};
	uint16_t SignalStrength =0; // 信号强度
/*

AT+CSQ
+CSQ: 20,99

OK
at+creg=2;+creg?

+CREG: 2,1,"11B0","E1F5"
*/	


	AT_CommandSend(UART3SendBuf, "AT+CSQ\r\n");
	os_dly_wait(100);
	
	if (AT_CommandRecv(UART3RecvBuf, 1000)) 
	{
		if(AT_CommandComp(ATRecvBuf,"+CSQ:")) 
		{
			while(ATRecvBuf[i] != ',') 
			{
				if (i > 30) return 0; // 避免死在此处
				i++;
			}
			if (ATRecvBuf[i] == ',') 
			{
				buf[0] = ATRecvBuf[i-1] - '0';
				buf[1] = ATRecvBuf[i-2] - '0';
				
				
				buf[0] = ((buf[1] << 4) + buf[0]);
				buf[1] = 0x00;
				
				buf[0] = HEX2DEC(buf[0]);
				SignalStrength = buf[0];
			}
		}
		
	}
	return SignalStrength ;
}

uint8_t ModbusInputTcp(uint8_t *pIn, uint32_t inSize)
{	
	uint8_t result = 0;
	if(clearModbusTimeOut == 0) 
	{
		memset(g_modbusReceiveBufTcp,0,MODBUS_BUFFER_SIZE);
		memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
		
		g_modbusReceiveBufNumberTcp = 0;
		g_modbusSendBufNumberTcp = 0;
		modbusAddressTcp = 0;
		modbusLongthTcp = 0;
	}

	clearModbusTimeOut = MODBUS_CLEAR_TIMEOUT;
	
	result = ModbusInput(pIn, 
											inSize, 
											g_modbusReceiveBufTcp, 
											&g_modbusSendBufTcp[0], 
											&g_modbusReceiveBufNumberTcp, 
											&g_modbusSendBufNumberTcp, 
											0, 
											&modbusLongthTcp, &modbusAddressTcp
											);
	return result;
}


void User_GsmPortInit(void)
{
	User_ComInit(3,115200u);
}




uint8_t GprsInit(void)
{		
		// 确认串口正常，波特率 =115200
	if(AT_CommandRun("AT\r", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	// open 回显
	if(AT_CommandRun("ATE1\r", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	Gprs.SIMCardStatus = SIMCardCheck();
	Gprs.TcpInitStatus = TcpIpInit();
	
	if(Gprs.TcpInitStatus == AT_OK) 
	{
		Gprs.InitStatus = LinkToTcpServer();	  // 连接到服务器	
	}		
	
	return Gprs.InitStatus;
}


uint8_t Check_TCPIP(void)
{
	if(AT_CommandRun("AT+COPS?\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR; 

	if(AT_CommandRun("AT+CGATT?\r\n", "OK",5,1000)==AT_ERROR)
		return AT_ERROR;
	
	
	if(AT_CommandRun("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK",2,1000)==AT_ERROR)
		return AT_ERROR;
	
	if(AT_CommandRun("AT+CGREG?\r\n", "OK",5,1000)==AT_ERROR)
		return AT_ERROR;
	
	if(AT_CommandRun("at%etcpip\r\n", "OK",5,1000)==AT_ERROR)
		return AT_ERROR;
	
	if(AT_CommandRun("AT%ETCPIP? \r\n", "OK",5,1000)==AT_ERROR)
		return AT_ERROR;
		
	return AT_OK;

}

void	GSM_Power_Saver_Handler(void)
{
// 修改成其他的无法进入 需要深入测试
	AT_CommandRun("AT+CFUN=0\r\n", "OK",1,1000);
	os_dly_wait(100);

}

