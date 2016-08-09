#include "MG323B.h"

// unsigned char g_initPort[6] = TCPIP_SERVER_PORT;
// unsigned char g_initIP[16] = TCPIP_SERVER_ADDR;

// uint8_t serverIP[16] ={0};
// uint8_t serverPort[6]={0};
// uint8_t serverConnect = 0;


// void DisablePowerKey(void)
// {
// 	  GPIO_InitTypeDef GPIO_InitStructure;
// 	
//   	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;	// TERM_ON
// 	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	//带上拉输入
// 	  GPIO_Init(GPIOE, &GPIO_InitStructure);
// }

// // 使能开机键
// void EnablePowerKey(void)
// {
// 	// POWER ON
// 	  GPIO_InitTypeDef GPIO_InitStructure;
// 	
// 	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; // TERM_ON
// 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
// 	  GPIO_Init(GPIOE, &GPIO_InitStructure);
// }



// void MG323B_PowerON(void)
// {
// 	//给GSM模块的电源控制脚一个下降沿启动模块	
// 	MG323B_RESET_DISABLE;  // disable reset
// 	MG323B_POWER_OFF;
// 		DelayMS(500);
// 		DelayMS(500);
// 		DelayMS(500);
// 		DelayMS(500);
// 		DelayMS(500);
// 	
// 	MG323B_POWER_ON;       // power on
// 		DelayMS(500);
// 		DelayMS(500);
// 		DelayMS(500);
// 	
// 	MG323B_TERM_ON_HIGH;   // 置高
// 	
// // 	OSTimeDlyHMSM(0,0,0,500); // minimum delay time  > 250ms
// 		DelayMS(500);
// 	
// 	MG323B_TERM_ON_LOW;
// // 	OSTimeDlyHMSM(0,0,1,0);	// minimum delay time  > 100ms
// 	DelayMS(1000);
// 	
// 	MG323B_TERM_ON_HIGH;
// }


// /*
// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
// ▇ 函数名：void MG323B_Configuration(void)								      ▇
// ▇ 函数功能：GSM模块工作模式配置																▇
// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
// */
// uint8_t MG323B_Configuration(void)
// {
// 	// 确认串口正常，波特率 =115200
// 	if(AT_CommandRun("AT\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;
// 	
// 	// 关闭回显
// 	if(AT_CommandRun("ATE0\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;
// 	

// // 	// 查询当前网络： CHINA MOBILE
// // 	if(AT_CommandRun("AT+COPS?\r\n", "OK",5,1000)==AT_ERROR)
// // 		return AT_ERROR;

// // 	// 查询当地网络信号强度，最大31，最小0
// // 	if(AT_CommandRun("AT+CSQ\r\n", "OK",5,1000)==AT_ERROR)
// // 		return AT_ERROR;
// 	
// 	// 查询当前网络注册状态
// // 	if(AT_CommandRun("AT+CREG?\r\n", "OK",5,1000)==AT_ERROR)
// // 		return AT_ERROR;	
// 	
// 	
// 	/*
// 		Manufacture: HUAWEI
// 		Model: MG323-B
// 		Revision: 11.210.09.06.00
// 		Number: 863679010068150
// 		+GCAP: +CGSM
// 	*/		
// // 	if(AT_CommandRun("ATI\r\n", "OK",5,1000)==AT_ERROR)
// // 		return AT_ERROR;	
// 	
// 	
// // 短信设置	
// // 	// 查询存储器状态
// // 	if(AT_CommandRun("AT+CPMS?\r\n", "OK", 5,1000)==AT_ERROR)
// // 		return AT_ERROR;		
// 	
// 	// txt
// // 	if(AT_CommandRun("AT+CMGF=1\r\n", "OK", 5,1000)==AT_ERROR)
// // 		return AT_ERROR;	
// // 	
// // 	// 删除SIM卡上的所有短信
// // 	if(AT_CommandRun("AT+CMGD=1,4\r\n", "OK", 10,1000)==AT_ERROR)
// // 		return AT_ERROR;	

// 	return AT_OK;
// }

// uint8_t MG323B_Init(void)
// {	
// 	// 确认串口正常，波特率 =115200
// 	
// 	if(AT_CommandRun("AT\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;
// 	
// 	// 关闭回显
// 	if(AT_CommandRun("ATE0\r\n", "OK", 5,1000)==AT_ERROR)
// 		return AT_ERROR;
// 	
// 	// 查询当前网络： CHINA MOBILE
// 	if(AT_CommandRun("AT+COPS?\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;

// 	// 查询当地网络信号强度，最大31，最小0
// 	if(AT_CommandRun("AT+CSQ\r\n", "OK",5,1000)==AT_ERROR)
// 		return AT_ERROR;


// 	// 查询当前网络注册状态
// // 	if(AT_CommandRun("AT+CREG?\r\n", "OK",5,1000)==AT_ERROR)
// // 		return AT_ERROR;	
// 	
// 	
// 	/*
// 		Manufacture: HUAWEI
// 		Model: MG323-B
// 		Revision: 11.210.09.06.00
// 		Number: 863679010068150
// 		+GCAP: +CGSM
// 	*/		
// // 	if(AT_CommandRun("ATI\r\n", "OK",5,1000)==AT_ERROR)
// // 		return AT_ERROR;	
// 	
// 	
// // 短信设置	
// // 	// 查询存储器状态
// // 	if(AT_CommandRun("AT+CPMS?\r\n", "OK", 5,1000)==AT_ERROR)
// // 		return AT_ERROR;		
// 	
// 	// txt
// // 	if(AT_CommandRun("AT+CMGF=1\r\n", "OK", 5,1000)==AT_ERROR)
// // 		return AT_ERROR;	
// // 	
// // 	// 删除SIM卡上的所有短信
// // 	if(AT_CommandRun("AT+CMGD=1,4\r\n", "OK", 10,1000)==AT_ERROR)
// // 		return AT_ERROR;	

// 	return AT_OK;
// }

// void MG323B_Close(void)
// {
// 	MG323B_POWER_OFF;
// 	DelayMS(500);
// 	DelayMS(500);
// 	DelayMS(500);
// 	DelayMS(500);
// // 	GPIO_ResetBits(GPIOE, GPIO_Pin_15);
// // 	GPIO_SetBits(GPIOE, GPIO_Pin_13);
// 			
// 	#ifdef DEBUG
// 	printf("> MG323B has been  closed!\r\n");
// 	#endif
// }



// void ResetGsm(void)
// {
// 	LED_BLUE_ON;
// 	if(MG323B_Init()) 
// 	{
// 		#ifdef DEBUG
// 		printf("> Initialization of MG323B is complete.\r\n");
// 		#endif
// 		GSM_TCP_Init();
// 	}
// 	else
// 	{
// 		#ifdef DEBUG
// 		printf("> Initialization of MG323B is failed!\r\n");
// 		#endif
// 		g_gsmStatus = 	GSM_STATUS_ERROR;
// 		LED_BLUE_OFF;
// 	}
// }


// void GSM_TCP_Init(void)
// {
// 	// GPRS_TCPIP 是否可用？
// 	uint8_t result = 0;
// 	result = GPRS_TCPIP_Config();
// 	if(result == AT_OK) 
// 	{
// 		#ifdef DEBUG
// 		printf("> Connect to the server.\r\n");  // 连接到GPRS-TCP
// 		#endif
// 		
// 		ConnectServerTcp();	  // 连接到服务器	

// 	}		
// 	else
// 	{
// 		#ifdef DEBUG
// 		printf("> Can not connect to the GPRS-TCP net.\r\n"); // 未连接到GPRS-TCP
// 		#endif
// 		g_gsmStatus =	GSM_STATUS_SMS;
// 	}
// }





// void ConnectServerTcp(void)
// {
// 	if (serverConnect < 2) 
// 	{
// 		//  the primary server
// 		GetServerIP(serverIP, 16, serverPort, 6);
// 	}
// 	else 
// 	{ // the standby server
// 		GetServerIPBack(serverIP, 16, serverPort, 6);
// 	}
// 	
// 	serverConnect++;

// 	serverConnect = serverConnect > 4 ? 0 : serverConnect;
// 	
// 	#ifdef DEBUG
// 	printf("> Server IP:  %s \r\n", serverIP);
// 	printf("> Server Port:%s \r\n", serverPort);
// 	#endif

// 	
// 	if(GPRS_TCPIP_Connect(serverIP, serverPort)) // max try times : 5
// 	{
// 		#ifdef DEBUG
// 		printf("> GSM_STATUS_TCP.\r\n");
// 		#endif
// 		g_gsmStatus 	= GSM_STATUS_TCP;
// 		serverConnect = 0;
// 		// 立即上传数据
// 		GPRS_TCPIP_Send(LinkMsg, sizeof(LinkMsg)) ; // 发送心跳包
// 		
// 		g_modbusReg4[modbusGSMRSSI] = GetSigalStrength();			 // 获取信号强度

// 		LocationBasedService();  // 获取基站信息
// 		
// 		Update_tcpip_Data(); // 更新数据
// 		
// 		
// 		GPRS_Data_Upload_Handler();		
// 		DelayMS(10);	// 10ms
// // 		
// 		#ifdef DEBUG
// 		printf("> TCPIP. Heartbeat.\r\n"); // 已连接到服务器
// 		#endif
// 	}
// 	else 
// 	{
// 		g_gsmStatus = 	GSM_STATUS_SMS;
// 		
// 		#ifdef DEBUG
// 		printf("> Failed to connect server.\r\n");  // 连接不到服务器
// 		#endif
// 	}
// }


