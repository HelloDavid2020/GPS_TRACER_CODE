#include "includes.h"


// #ifdef DEBUG
// 	UartMode g_uartMode = Command;//Command;//Modbus;
// #else 
// 	UartMode g_uartMode = Modbus;//Command;//Modbus;
// #endif

	UartMode g_uartMode = Command;//Command;//Modbus;


char debugLog[512];
void DebugLog(char *c)
{
	if (g_uartMode != Modbus)
	{
		QueueWriteStr(UART1SendBuf, c);
		OSTimeDly(10);
	}
}
char debugLogBuf[512];
void DebugData(char *c, uint32_t length)
{
	uint8_t highData;
	uint8_t lowData;
	uint32_t i = 0;
	if (g_uartMode == Modbus)
		return;
	
	DebugLog("\r\n");
	while (length > 0) {
		highData = *c >> 4;
		lowData = *c & 0xf;
		if (highData < 10)
			debugLogBuf[i] = highData - 0 + '0';
		else
			debugLogBuf[i] = highData - 10 + 'a';
		i++;
		if (lowData < 10)
			debugLogBuf[i] = lowData - 0 + '0';
		else
			debugLogBuf[i] = lowData - 10 + 'a';
		i++;
		length--;
		c++;
		if (i == 200) {
			debugLogBuf[i] = 0;
			i = 0;

			
			if((!UART1_TXEN_flag)&&(QueueNData(UART1SendBuf)))		   //如果，发送中断关闭，同时发送区有数据，开发送中断
			{  	
				UART1_TXEN_flag = 1;
				USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
			}
			while (UART1_TXEN_flag == 1)
				OSTimeDly(1);
			
			QueueWriteStr(UART1SendBuf, debugLogBuf);
			DebugLog("\r\n");
		}
	}	
	debugLogBuf[i] = 0;
	QueueWriteStr(UART1SendBuf, debugLogBuf);
	OSTimeDly(1);
	DebugLog("\r\n");
}


void GSMInit()
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_13);
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	GPIO_ResetBits(GPIOE, GPIO_Pin_15);
	DelayMS(500);
	GPIO_SetBits(GPIOE, GPIO_Pin_15);
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);
	GPIO_ResetBits(GPIOE, GPIO_Pin_15);
}

void GSMClose(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_15);
	DelayMS(500);
	DelayMS(500);
	DelayMS(500);

	GPIO_ResetBits(GPIOE, GPIO_Pin_15);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
}
void TestI2C(void)
{
	char rev[16] = {0};
	char send[16] = {0};
// 	char *ptr = send;
	int i;
	DebugData(send, 16);

	Fram_BufferWrite((uint8_t *)send, 0, 16);
	Fram_BufferRead((uint8_t *)rev, 0, 16);

	DebugData(rev, 16);
	for(i = 0; i < 16; i++)
	{
		send[i] = i;
	}
	DebugData(send, 16);
	Fram_BufferWrite((uint8_t *)send, 0, 16);
	Fram_BufferRead((uint8_t *)rev, 0, 16);
	DebugData(rev, 16);
}
void TestI2C_Read(void)
{
	int i = 0;
	char rev[256];
	while (i < 8*1024) 
	{
		Fram_BufferRead((uint8_t *)rev, i, 256);
		DebugData(rev, 256);
		i += 256;
		OSTimeDly(1);
	}
}
uint8_t RcvBuf[1024];
uint8_t SendBuf[1024];
void TestSpi(void)
{
	uint32_t idData;
	
	uint32_t i;
	
	for (i = 0; i < 1024; i++)
		RcvBuf[i] = 2;
	for (i = 0; i < 1024; i++)
		SendBuf[i] = i;
	spiFLASH_RdID(Manu_ID, &idData);
	DebugData((char*)&idData, 4);
	spiFLASH_RdID(Dev_ID, &idData);
	DebugData((char*)&idData, 4);
	
	DebugData((char*)RcvBuf, 1024);
	//spiFLASH_Erase(0);
	//spiFLASH_Erase(1);
    if (BSP_Flash_Write_Buffer(256,RcvBuf,1024) == OK) {
		if (BSP_spiFLASH_RD(256,RcvBuf,1024) == OK)
			DebugData((char*)RcvBuf, 1024);
    }
	DebugData((char*)SendBuf, 1024);
    if (BSP_Flash_Write_Buffer(256,SendBuf,1024) == OK) {
		if (BSP_spiFLASH_RD(256,RcvBuf,1024) == OK)
			DebugData((char*)RcvBuf, 1024);
    }
}
void TestSpi_Read(void)
{
	uint32_t i;
	char spiTemp[256];
	for (i = 0; i < ADDR_MAX + 1; i += 256) {
		BSP_spiFLASH_RD(i, (uint8_t *)spiTemp, 256);
		OSTimeDly(1);
		DebugData(spiTemp, 256);
		DebugData((char*)&i, 4);
	}
}
void TestFlashRead(void)
{
	char temp[140];
	uint8_t i;
	for (i = 0; i < 140; i++)
		temp[i] = 0;
	FlashReadData(temp, 140);
	DebugData(temp, 140);
	FlashCleanData(140);
}
void TestFlashWrite(void)
{
	char temp[140];
	uint8_t i;
	for (i = 0; i < 140; i++)
		temp[i] = i;
	FlashWriteData(temp, 140);
}
void TestFlashReadAll(void)
{
	uint32_t i = 0;
	uint8_t temp[128];
	
	while (i < 1024 * 8) {
		sprintf((void *)temp, "%d\r\n", i);
		DebugLog((void *)temp);
		BSP_spiFLASH_RD(i, (uint8_t *)temp, 128);
		i += 128;
		DebugData((void *)temp, 128);
		OSTimeDly(3);
	}
}
void TestFlashInit(void)
{
// 	FlashInit();
	User_FlashInit();
}
void TestFlashInfo(void)
{
	FlashInfo();
}
void SaveData(void)
{

}
void LoadData(void)
{

}

void GetTimer(void)
{
	Read_Time();
	printf("> Current Time: %02d-%02d-%02d %02d:%02d:%02d\r\n",RTC_TIMER.Years,RTC_TIMER.Months,RTC_TIMER.Days,RTC_TIMER.Hours,RTC_TIMER.Minutes,RTC_TIMER.Seconds);
}
void SetTimer(char *c, uint8_t length)
{
	char buf[64];
	char des[7];

	if (length != 14)
		return;
	DebugLog(c);
	DebugData(c, 14);
	ascii2data(des, c, length);
	DebugData(des, 7);

	RTC_TIMER.Years = des[0] * 256;
	RTC_TIMER.Years += des[1];
	RTC_TIMER.Months = des[2];
	RTC_TIMER.Days = des[3];
	RTC_TIMER.Hours = des[4];
	RTC_TIMER.Minutes = des[5];
	RTC_TIMER.Seconds = des[6];

	/*RTC_TIMER.Years = ((des[0] & 0xf) + (des[0] >> 4 ) * 10) * 100;
	RTC_TIMER.Years += (des[1] & 0xf) + (des[1] >> 4 ) * 10;
	RTC_TIMER.Months = (des[2] & 0xf) + (des[2] >> 4 ) * 10;
	RTC_TIMER.Days = (des[3] & 0xf) + (des[3] >> 4 ) * 10;
	RTC_TIMER.Hours = (des[4] & 0xf) + (des[4] >> 4 ) * 10;
	RTC_TIMER.Minutes = (des[5] & 0xf) + (des[5] >> 4 ) * 10;
	RTC_TIMER.Seconds = (des[6] & 0xf) + (des[6] >> 4 ) * 10;
	
	*/
    
	sprintf(buf,"Set Time:%d/%d/%d %d:%d:%d\r\n",RTC_TIMER.Years,RTC_TIMER.Months,RTC_TIMER.Days,RTC_TIMER.Hours,RTC_TIMER.Minutes,RTC_TIMER.Seconds);
	DebugLog(buf);

// 	Set_Time();
}

uint8_t commandLength = 0;
char CommandString[24];
void InputCommand(char c)
{
	CommandString[commandLength] = c;
	if (c == '\r' || c == '\n') {
		CommandString[commandLength] = 0;
		commandLength = 0;
	}
	else {
		commandLength++;
		if (commandLength > 24)
			commandLength = 0;
		return;
	}
	
	if (strcmp(CommandString, "Modbus") == 0) {
		DebugLog("\r\nCurrentMode:Modebus\r\n");
		g_uartMode = Modbus;
	}
	else if (strcmp(CommandString, "InputSend") == 0) {
		DebugLog("\r\nCurrentMode:InputSend\r\n");
		g_uartMode = InputSend;
	}
	else if (strcmp(CommandString, "DebugGSM") == 0) {
		DebugLog("\r\nCurrentMode:DebugGSM\r\n");
		GSMInit();
		g_uartMode = DebugGSM;
	}
	else if (strcmp(CommandString, "DebugGPS") == 0) {
		DebugLog("\r\nCurrentMode:DebugGPS\r\n");
		g_uartMode = DebugGPS;
	}
	else if (strcmp(CommandString, "Mode") == 0) {
		DebugLog("\r\nCurrentMode:");
		DebugData((&(g_uartMode)), 1);
		DebugLog("\r\n");	
	}
	else if (strcmp(CommandString, "Command") == 0) {
		DebugLog("\r\nCurrentMode:Commandr\n");
		g_uartMode = Command;
	}
	else if (strcmp(CommandString, "i2c") == 0) {
		TestI2C_Read();
	}
	else if (strcmp(CommandString, "i2cTest") == 0) {
		TestI2C();
	}
	else if (strcmp(CommandString, "flashr") == 0) {
		TestFlashRead();;
	}
	else if (strcmp(CommandString, "flashinit") == 0) {
		TestFlashInit();;
	}
	else if (strcmp(CommandString, "flashall") == 0) {
		TestFlashReadAll();;
	}
	else if (strcmp(CommandString, "flashw") == 0) {
		TestFlashWrite();;
	}
	else if (strcmp(CommandString, "flashinfo") == 0) {
		TestFlashInfo();;
	}
	else if (strcmp(CommandString, "spi") == 0) {
		TestSpi();
	}
	else if (strcmp(CommandString, "spiTest") == 0) {
		TestSpi_Read();
	}
	else if (strstr(CommandString, "st201") != NULL) {
		SetTimer(CommandString + 2, 14);
	}
	else if (strstr(CommandString, "time") != NULL) {
		GetTimer();
	}
	else if (strstr(CommandString, "save") != NULL) {
		SaveData();
	}
	else if (strstr(CommandString, "load") != NULL) {
		LoadData();
	}
	else
		DebugLog("\r\nHelp\r\n");	
}



