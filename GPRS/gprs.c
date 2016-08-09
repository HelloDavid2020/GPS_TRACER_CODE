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
uint8_t gsm_time_Count = 0;	 // ��ȡGSM�źš�LBS ����



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
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	//����������
	  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

// ʹ�ܿ�����
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
	//��GSMģ��ĵ�Դ���ƽ�һ���½�������ģ��	
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
	
	MG323B_TERM_ON_HIGH;   // �ø�
	
	DelayMS(500);
	
	MG323B_TERM_ON_LOW;
	DelayMS(1000);
	
	MG323B_TERM_ON_HIGH;
}


/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~ ��������void MG323B_Configuration(void)								      �~
�~ �������ܣ�GSMģ�鹤��ģʽ����																�~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
*/
uint8_t MG323B_Configuration(void)
{
	// ȷ�ϴ��������������� =115200
	if(AT_CommandRun("AT\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	
	// �رջ���
	if(AT_CommandRun("ATE0\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	

// 	// ��ѯ��ǰ���磺 CHINA MOBILE
// 	if(AT_CommandRun("AT+COPS?\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;

// 	// ��ѯ���������ź�ǿ�ȣ����31����С0
// 	if(AT_CommandRun("AT+CSQ\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;
	
	// ��ѯ��ǰ����ע��״̬
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
	
	
// ��������	
// 	// ��ѯ�洢��״̬
// 	if(AT_CommandRun("AT+CPMS?\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;		
	
	// txt
// 	if(AT_CommandRun("AT+CMGF=1\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;	
// 	
// 	// ɾ��SIM���ϵ����ж���
// 	if(AT_CommandRun("AT+CMGD=1,4\r\n", "OK", 10,1000)==AT_ERROR)
// 		return AT_ERROR;	

	return AT_OK;
}

uint8_t MG323B_Init(void)
{	
	// ȷ�ϴ��������������� =115200
	
	if(AT_CommandRun("AT\r\n", "OK", 3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// �رջ���
	if(AT_CommandRun("ATE0\r\n", "OK", 3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// SIM����λ���
	if(AT_CommandRun("AT%TSIM\r\n", "TSIM 1",3,1000)==AT_ERROR)
		return AT_ERROR;		

	// SIM����λ���PIN1 ��״̬��ѯ
	if(AT_CommandRun("at+cpin?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	
	// ��ѯ��ǰ���磺 CHINA MOBILE
	if(AT_CommandRun("AT+COPS?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;

	// ��ѯ���������ź�ǿ�ȣ����31����С0
	if(AT_CommandRun("AT+CSQ\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;

	


	// ��ѯ��ǰ����ע��״̬
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
	
	
// ��������	
// 	// ��ѯ�洢��״̬
// 	if(AT_CommandRun("AT+CPMS?\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;		
	
	// txt
// 	if(AT_CommandRun("AT+CMGF=1\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;	
// 	
 	// ɾ��SIM���ϵ����ж���
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
		LinkToTcpServer();	  // ���ӵ�������	
	}		
	else
	{
		#ifdef DEBUG
		printf("> Can not connect to the GPRS-TCP net.\r\n"); // δ���ӵ�GPRS-TCP
		#endif
		Gprs.LinkStatus =	LINK_STATUS_SMS;
	}
}




// ���ӵ��ԣãз�����
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

		Update_tcpip_Data(); // ��������
		GPRS_Data_Upload_Handler();				// �����ϴ�����
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
				// �����ϴ�����
				GPRS_TCPIP_Send(LinkMsg, sizeof(LinkMsg)) ; // ����������
				Update_tcpip_Data(); // ��������
				GPRS_Data_Upload_Handler();		
				DelayMS(10);	// 10ms
		// 		
				#ifdef DEBUG
				printf("> TCPIP. Heartbeat.\r\n"); // �����ӵ�������
				#endif
			}
			else 
			{
				Gprs.LinkStatus = 	LINK_STATUS_SMS;
				
				#ifdef DEBUG
				printf("> Failed to connect server.\r\n");  // ���Ӳ���������
				#endif
			}
		}		
		else
		{
			#ifdef DEBUG
			printf("> Can not connect to the GPRS-TCP net.\r\n"); // δ���ӵ�GPRS-TCP
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

		// ��ȡ�ֻ���
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
**������Ϣ �� TcpIpInit                    
**�������� �� GSM/GPRSģ��TCPIP����
**������� ��       
**������� ����AT_OK����ʼ���ɹ�����AT_ERROR����ʼ��ʧ��
********************************************************************************************************/
uint8_t TcpIpInit(void)
{
	// ������Ӫ��ע���ѯ
	if(AT_CommandRun("AT+COPS?\r\n", "OK", 3,1000)==AT_ERROR)
		return AT_ERROR; 
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	// ��ѯGPRS����״̬
	if(AT_CommandRun("AT+CGATT?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS

	// ע���ƶ���CMNET����
	if(AT_CommandRun("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS

	// GPRS�����Ƿ���ã�
	// +CGREG:x,5��CGREG:x,1 ����ʾ����ʹ��GPRS����
	if(AT_CommandRun("AT+CGREG?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	// ETCPIP ǰ�Ӹ�2����ʱ
	os_dly_wait(200);  // 10MS *50 = 500 mS

	// GPRS��ʼ�����ȴ�����IP
	if(AT_CommandRun("AT%ETCPIP\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(100);  // 10MS *50 = 500 mS

	// ��ѯGPRS��ʼ���Ƿ�ɹ�
	if(AT_CommandRun("AT%ETCPIP?\r\n", "OK",3,1000)==AT_ERROR)
		return AT_ERROR;	
	os_dly_wait(100);  // 10MS *50 = 500 mS

	return AT_OK;
}
/*********************************************************************************************************
**������Ϣ �� AT_CommandRun                    // WAN.CG // 2012.07.04
**�������� �� GSM/GPRSģ��AT����͡����պʹ���
**������� ����command������  ��respond����ȷ�ķ���ֵ ��times�����ʧ�ܣ��ظ����� ��timeout�����ճ�ʱ      
**������� ����AT_OK���ɹ�����AT_ERROR��ʧ��
**������ʾ ��
********************************************************************************************************/
uint8_t AT_CommandRun(char *command,char *respond,uint8_t times,uint16_t timeout)
{
	while(times--)
	{
		if(!AT_CommandSend(UART3SendBuf, command))		//����AT����
			continue;

		if(!AT_CommandRecv(UART3RecvBuf, timeout))	    //����AT�����Ӧ
			continue;

			//������쳣�ظ�����Ҫ���д���������ŵ�?
		if(AT_CommandComp(ATRecvBuf,respond))		    //AT��Ӧ����
			return AT_OK;	
	}
	return AT_ERROR;						            //AT��Ӧ����
}


/*********************************************************************************************************
**������Ϣ �� AT_CommandSend                    // WAN.CG // 2012.07.04
**�������� �� GSM/GPRSģ��AT�����
**������� ����sendbuf������ͻ�����  ��COMMAND������ ��times�����ʧ�ܣ�������ظ�����      
**������� ����AT_OK���ɹ�����AT_ERROR��ʧ��
**������ʾ ��
********************************************************************************************************/
uint8_t AT_CommandSend(void *sendbuf, char *Command)
{
   	if(*Command == '\0')
		return AT_ERROR;					
	QueueWriteStr(sendbuf, Command);		//����AT����
	return AT_OK; 
}

/*********************************************************************************************************
**������Ϣ �� AT_CommandRecv                    // WAN.CG // 2012.07.05
**�������� �� GSM/GPRSģ��AT�����Ӧ���ݽ���
**������� ����recvbuf�����ջ�����   ��timeout�����ճ�ʱ
**������� ����AT_OK���ɹ�����AT_ERROR��ʧ��
**������ʾ �� ��Ҫ�õ���ʱ�� 2 ��ȫ�ֱ��� Timer2_count
********************************************************************************************************/
uint8_t AT_CommandRecv(void *recvbuf,uint16_t timeout)
{
	uint16_t	i,Queue_Num;	
	uint16_t	tempnum = 0;
	Timer2_count = 0; 			              		//���ճ�ʱ��ʼ��ʱ
	while(!QueueNData((void*)recvbuf))	      //û�����ݣ��ȴ�
	{ 
		if(Timer2_count > timeout)
			return AT_ERROR;				  //��ʱ������
	}

	while(1)
	{
		Queue_Num = QueueNData((void*)recvbuf);
		Timer2_count = 0; 		              //��ʼ��ʱ
		while((Queue_Num == QueueNData((void*)recvbuf) ))	 //�ȴ����ݽ������
		{ 					
			if(Timer2_count > 5)
			{
				memset(ATRecvBuf,0,sizeof(ATRecvBuf));				
				
				ATRecv_Num = Queue_Num;
				for(i=0;i<Queue_Num;i++)
				{
					QueueRead(&ATRecvBuf[i], (void*)recvbuf);       //ת������
				}
				return AT_OK;
			}
// 			os_dly_wait(1);
		}
// 		os_dly_wait(1);			 
	}
}
/*********************************************************************************************************
**������Ϣ �� AT_CommandComp                    // WAN.CG // 2012.07.04
**�������� �� �Ƚ��յ��������Ƿ���Դ�ַ�����ͬ
**������� ����Command��Ŀ���ַ���  ��str��Դ�ַ���
**������� ����AT_OK���ɹ�����AT_ERROR��ʧ��
**������ʾ ��
********************************************************************************************************/
uint8_t AT_CommandComp(void *command, char *str)
{
	// extern char *strstr(char *str1, char *str2);
	// ���ַ���str1�в����ַ���str2
	if(strstr(command,str) == NULL)
		return AT_ERROR;
	else
		return AT_OK; 
}
/*********************************************************************************************************
**������Ϣ �� SendSMS                    // WAN.CG // 2012.07.05
**�������� �� GSM/GPRSģ����ŷ��ͺ���
**������� ����pSMSCenter������Ϣ���ĺ���  ��pSMSRecipient�����պ��� ��pSMSContent����������      
**������� ��
**������ʾ ��
********************************************************************************************************/
uint8_t SendSMS140(char* pSMSCenter, char* pSMSRecipient, char* pSMSContent)
{
	uint8_t  byteH = 0, byteL = 0;
  	uint16_t i = 0, j = 0, k = 0;
  	uint16_t nSMSCenterLen= 0, nSMSPduLen = 0, nPduBufferIndex = 0;
  	char   tmpBuffer[128];
	char   pduBuffer[384];
	char   chTmp;
	 
//   	for(i = 0; i < sizeof(pduBuffer); i++)	  //��ջ�����
//     	pduBuffer[i] = 0x00;
	memset(pduBuffer,0,sizeof(pduBuffer));				

	

/********************************************************************************************************	
**   ������PDU�������
   * |-----------------------------------------------------------------|
   * |SCA  |PDUType  |MR  |DA   |PID  |DCS  |VP     |UDL  |UD    |END  |
   * |-----------------------------------------------------------------|
   * |1-12 |1        |1   |2-12 |1    |1    |0,1,7  |1    |0-140 |1    | 
   * |-----------------------------------------------------------------|
   * |     |01       |00  |     |00   |08   |AA     |     |      |0x1A |
   * |-----------------------------------------------------------------|
*********************************************************************************************************	
**�������ĺ��봦��
********************************************************************************************************/	  
//������Ϣ���ĺ���ȥ��+�ţ����������Ƿ�Ϊż����������ǣ�������F 	
	for(k = 0; pSMSCenter[k] != '\0'; k++)		 
    	tmpBuffer[k] = pSMSCenter[k];
  	if(k%2 != 0) 
    	tmpBuffer[k++] = 'F';
			   
//��� addr ���ȣ������2����ʽ����2λ��16�����ַ�����16 / 2 = 8 => "08"
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

//������Ϣ���ĺ���ǰ������ַ�91��91�ǹ��ʻ�����˼
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//������λ��ż��λ����
	for(i = 0, j = k/2; i < j; i++)
  	{
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2+1];
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2];
  	}
  	nSMSCenterLen = nPduBufferIndex/2;

//! PDU����
  	pduBuffer[nPduBufferIndex++] = '1';
  	pduBuffer[nPduBufferIndex++] = '1';	
  //! For MR
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '0';

/*********************************************************************************************************	
**�ֻ����봦��
********************************************************************************************************/
  	for(k = 0; pSMSRecipient[k] != 0; k++)
    	tmpBuffer[k] = pSMSRecipient[k];

//��� addr ���ȣ������2����ʽ����2λ��16�����ַ���
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

//����ǰ������ַ�91��91�ǹ��ʻ�����˼
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//���ֻ�����ȥ��+�ţ����������Ƿ�Ϊż����������ǣ�������F
  	if(k%2 != 0)
    	tmpBuffer[k++] = 'F';

//������λ��ż��λ����
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
**����Ϣ���ִ���
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

  //! PDU���ĳ���,����AT+CMGSҪ�õ��˳���
  nSMSPduLen = nPduBufferIndex/2-nSMSCenterLen;

  //! For END
  pduBuffer[nPduBufferIndex++] = 0x1A;
  pduBuffer[nPduBufferIndex++] = 0x00;

/*********************************************************************************************************	
**�����Ƿ��͹���
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
	
	printf("���ú���ɹ�...\r\n");
	
	if(!AT_CommandSend(UART3SendBuf, pduBuffer))		//���Ͷ�������
		return AT_ERROR;
// 	DebugLog(pduBuffer);	
	printf("\r\n");
	printf("��Ϣ�ѷ��ͣ��ȴ�SIM900��Ӧ...\r\n");

	for(k = 0; k < 200; k++)
	{
		if(!AT_CommandRecv(UART3RecvBuf, 1000))		    //����AT�����Ӧ
		{
			if(AT_CommandComp(ATRecvBuf,"OK"))		    //AT��Ӧ����
				return AT_OK;
			if(AT_CommandComp(ATRecvBuf,"ERROR"))		//AT��Ӧ����
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
	 
  	for(i = 0; i < sizeof(pduBuffer); i++)	  //��ջ�����
    	pduBuffer[i] = 0x00;

/********************************************************************************************************	
**   ������PDU�������
   * |-----------------------------------------------------------------|
   * |SCA  |PDUType  |MR  |DA   |PID  |DCS  |VP     |UDL  |UD    |END  |
   * |-----------------------------------------------------------------|
   * |1-12 |1        |1   |2-12 |1    |1    |0,1,7  |1    |0-140 |1    | 
   * |-----------------------------------------------------------------|
   * |     |01       |00  |     |00   |08   |AA     |     |      |0x1A |
   * |-----------------------------------------------------------------|
*********************************************************************************************************	
**�������ĺ��봦��
********************************************************************************************************/	  
//������Ϣ���ĺ���ȥ��+�ţ����������Ƿ�Ϊż����������ǣ�������F 	
	for(k = 0; pSMSCenter[k] != '\0'; k++)		 
    	tmpBuffer[k] = pSMSCenter[k];
  	if(k%2 != 0) 
    	tmpBuffer[k++] = 'F';
			   
//��� addr ���ȣ������2����ʽ����2λ��16�����ַ�����16 / 2 = 8 => "08"
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

//������Ϣ���ĺ���ǰ������ַ�91��91�ǹ��ʻ�����˼
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//������λ��ż��λ����
	for(i = 0, j = k/2; i < j; i++)
  	{
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2+1];
    	pduBuffer[nPduBufferIndex++] = tmpBuffer[i*2];
  	}
  	nSMSCenterLen = nPduBufferIndex/2;

//! PDU����
  	pduBuffer[nPduBufferIndex++] = '1';
  	pduBuffer[nPduBufferIndex++] = '1';	
  //! For MR
  	pduBuffer[nPduBufferIndex++] = '0';
  	pduBuffer[nPduBufferIndex++] = '0';

/*********************************************************************************************************	
**�ֻ����봦��
********************************************************************************************************/
  	for(k = 0; pSMSRecipient[k] != 0; k++)
    	tmpBuffer[k] = pSMSRecipient[k];

//��� addr ���ȣ������2����ʽ����2λ��16�����ַ���
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

//����ǰ������ַ�91��91�ǹ��ʻ�����˼
  	pduBuffer[nPduBufferIndex++] = '9';
  	pduBuffer[nPduBufferIndex++] = '1';

//���ֻ�����ȥ��+�ţ����������Ƿ�Ϊż����������ǣ�������F
  	if(k%2 != 0)
    	tmpBuffer[k++] = 'F';

//������λ��ż��λ����
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
**����Ϣ���ִ���
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

  //! PDU���ĳ���,����AT+CMGSҪ�õ��˳���
  nSMSPduLen = nPduBufferIndex/2-nSMSCenterLen;

  //! For END
  pduBuffer[nPduBufferIndex++] = 0x1A;
  pduBuffer[nPduBufferIndex++] = 0x00;

/*********************************************************************************************************	
**�����Ƿ��͹���
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
	
	printf("���ú���ɹ�...\r\n");
	
	if(!AT_CommandSend(UART3SendBuf, pduBuffer))		//���Ͷ�������
		return AT_ERROR;
// 	DebugLog(pduBuffer);	
	printf("\r\n");
	printf("��Ϣ�ѷ��ͣ��ȴ���Ӧ...\r\n");
//	return AT_OK;
	
	for(k = 0; k < 200; k++)
	{
		if(!AT_CommandRecv(UART3RecvBuf, 1000))		    //����AT�����Ӧ
		{
			if(AT_CommandComp(ATRecvBuf,"OK"))		    //AT��Ӧ����
				return AT_OK;
			if(AT_CommandComp(ATRecvBuf,"ERROR"))		//AT��Ӧ����
				return AT_ERROR;
		}
		os_dly_wait(1);
	}
	return AT_ERROR;
}
/********************************************************************************************************	
**text��ʽ���Ͷ���
********************************************************************************************************/
uint8_t SendSMSChar(char* pSMSRecipient, char* pSMSContent)
{
	char   tmpBuffer[128];
	uint16_t k = 0;
	char temp[2];
	temp[0] = 0x1a;
	temp[1] = '\0';
	

/*********************************************************************************************************	
**�����Ƿ��͹���
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
	printf("���ú���ɹ�...\r\n");
	#endif
	
	if(!AT_CommandSend(UART3SendBuf, pSMSContent))		//���Ͷ�������
		return AT_ERROR;

	if (!AT_CommandSend(UART3SendBuf, temp))
		return AT_ERROR;
	
	#ifdef DEBUG
	printf("��Ϣ�ѷ��ͣ��ȴ�SIM900��Ӧ...\r\n");
	#endif
	
	for(k = 0; k < 200; k++)
	{
		if(!AT_CommandRecv(UART3RecvBuf, 1000))		    //����AT�����Ӧ
		{
			if(AT_CommandComp(ATRecvBuf,"OK"))		    //AT��Ӧ����
				return AT_OK;
			if(AT_CommandComp(ATRecvBuf,"ERROR"))		//AT��Ӧ����
				return AT_ERROR;
		}
//		os_dly_wait(1);
	}
	return AT_ERROR;
}
/*********************************************************************************************************
**������Ϣ �� GPRS_TCPIP_Connect                    // WAN.CG // 2012.07.05
**�������� �� ���ӷ�����
**������� ����IP_ADDR��IP��ַ  ��IP_PORT���˿ں�     
**������� �� ��AT_OK���ɹ�����AT_ERROR��ʧ��
********************************************************************************************************/
uint8_t GPRS_TCPIP_Connect(uint8_t * IP_ADDR,uint8_t * IP_PORT)
{
	uint8_t times = 5;
	uint8_t tempbuf[50];
	
	// ���ý��շ�������Э������ (TCP ��IP���˿ں�)
	// 
	sprintf((void *)tempbuf, "AT%%IPOPEN=\"TCP\",\"%s\",%s\r\n", IP_ADDR, IP_PORT);
	g_reconnectTime++;
	
	while(times--)
	{
		if(!AT_CommandSend(UART3SendBuf, (void *)tempbuf))
			return AT_ERROR; 	
		os_dly_wait(100);  // 10MS *100 = 1000 mS
		
		if(AT_CommandRecv(UART3RecvBuf, 1000)==AT_ERROR)	    //����AT�����Ӧ
			continue;
		
		if(AT_CommandComp(ATRecvBuf,"CONNECT")==AT_OK)		//connect
			return AT_OK;
		
		//already connect
		if(AT_CommandComp(ATRecvBuf,"ERROR: 9")==AT_OK)		
			return AT_OK;
		
		if(AT_CommandComp(ATRecvBuf,"ERROR: 20")==AT_OK) 
		{	//��Ҫetcpip
			if (TcpIpInit())
				continue;
			else
				return AT_ERROR;
		}
	}
	return AT_ERROR; 
}
/*********************************************************************************************************
**������Ϣ �� GPRS_TCPIP_DisConnect                    // WAN.CG // 2012.07.05
**�������� �� �Ͽ��������������
**������� ��    
**������� �� ��AT_OK���ɹ�����AT_ERROR��ʧ��
********************************************************************************************************/
uint8_t GPRS_TCPIP_DisConnect(void)
{
   //AT+CIPCLOSE:�Ͽ�����
	if(AT_CommandRun("AT%IPCLOSE\r", "%IPCLOSE: 1", 10,1000)==AT_ERROR)
		return AT_ERROR; 

	return AT_OK;  
}
/*********************************************************************************************************
**������Ϣ �� GPRS_TCPIP_DisConnect                    // WAN.CG // 2012.07.05
**�������� �� �ر���ŵ����
**������� ��    
**������� �� ��AT_OK���ɹ�����AT_ERROR��ʧ��
********************************************************************************************************/
uint8_t GPRS_TCPIP_NETShut(void)
{
	//if(AT_CommandRun("AT+CIPSHUT\r\n", "SHUT OK", 1,1000)==AT_ERROR)
	//	return AT_ERROR; 

	return AT_OK;  
}
/*********************************************************************************************************
**������Ϣ �� GPRS_TCPIP_Send                    // WAN.CG // 2012.07.05
**�������� �� ��������
**������� ����Buffer�����͵����ݣ���BufferLen�����ݳ���   
**������� ����AT_OK���ɹ�/���ݳ���  ��AT_ERROR��ʧ��
********************************************************************************************************/
uint8_t GPRS_TCPIP_Send(uint8_t * Buffer,uint16_t BufferLen)
{
	char high;
	char low;
	
	ENTER_CRITICAL_SECTION();
	
	AT_CommandSend(UART3SendBuf, "at%ipsend=\"");
	
	// ��Ҫ�Ż�
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
	{	    //����AT�����Ӧ	
		// %IPSEND:1,8
		if(AT_CommandComp(ATRecvBuf,"%IPSEND:1"))	//AT��Ӧ����
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
	
	
	if(AT_CommandComp(ATRecvBuf,"%IPCLOSE: 1"))		    //�Ͽ�����
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
**������Ϣ �� GPRS_TCPIP_Recv                    
**������� ����Buffer�����յ�����   
**������� ����AT_OK���ɹ�/���ݳ���  ��AT_ERROR��ʧ��
********************************************************************************************************/
uint8_t GPRS_TCPIP_Recv(void)
{
	// �������ݽ��� �򷵻�
	if(!AT_CommandRecv(UART3RecvBuf, 1000))	       //��ʱû�н��յ�����
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
	
	// ���ݹ���,ȡ�����ݳ���
	sscanf((char *)ATRecvBuf, "\r\n%%IPDATA:%d,%*s", &dataLength);

	AddrFirstData = findData((char*)ATRecvBuf, ATRecv_Num, "\"");
	AddrFirstData++;
		
	AddrLastData = findDataFromEnd((char*)ATRecvBuf, ATRecv_Num, '"');
	
	// ascii �����ݳ����� hex��2��
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
	if (!AT_CommandRecv(UART3RecvBuf, 5000))	    //����AT�����Ӧ
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
			//�ַ�תascII��modbus����
			smsSize = smsSize >> 1;
// 			OS_ENTER_CRITICAL();
			TCPReceive_Num = smsSize;
			smsSize = smsSize << 1;
			ascii2data((char*)TCPReceiveBuf, smsContent, smsSize);
// 			OS_EXIT_CRITICAL();
		}
		if (!AT_CommandRecv(UART3RecvBuf, 50000))	    //����AT�����Ӧ
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
	uint16_t SignalStrength =0; // �ź�ǿ��
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
				if (i > 30) return 0; // �������ڴ˴�
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
		// ȷ�ϴ��������������� =115200
	if(AT_CommandRun("AT\r", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	// open ����
	if(AT_CommandRun("ATE1\r", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(50);  // 10MS *50 = 500 mS
	
	Gprs.SIMCardStatus = SIMCardCheck();
	Gprs.TcpInitStatus = TcpIpInit();
	
	if(Gprs.TcpInitStatus == AT_OK) 
	{
		Gprs.InitStatus = LinkToTcpServer();	  // ���ӵ�������	
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
// �޸ĳ��������޷����� ��Ҫ�������
	AT_CommandRun("AT+CFUN=0\r\n", "OK",1,1000);
	os_dly_wait(100);

}

