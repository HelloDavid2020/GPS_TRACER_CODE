#include "includes.h"

uint8_t g_modbusReg4[MODBUS_REG4_SIZE];//
uint8_t g_modbusReg5[MODBUS_REG5_SIZE];//tcp
uint8_t g_modbusReg6[MODBUS_REG6_SIZE];//sms

int32u reportHeartTimeOut;
int32u reportAccOffTimeOut;
int32u reportAccOnTimeOut;
int32u DeviceWorkTime;
int32u DeviceSleepTime;
int32u lockNoSignalTimeOut;
int32u alarmNoPLCTimeOut;

uint8_t g_modbusReceiveBufTcp[MODBUS_BUFFER_SIZE];
uint8_t g_modbusSendBufTcp[MODBUS_BUFFER_SIZE];
int16u g_modbusReceiveBufNumberTcp = 0;
int16u g_modbusSendBufNumberTcp = 0;

uint8_t g_modbusReceiveBufSerial[MODBUS_BUFFER_SIZE];
uint8_t g_modbusSendBufSerial[MODBUS_BUFFER_SIZE];
int16u g_modbusReceiveBufNumberSerial = 0;
int16u g_modbusSendBufNumberSerial = 0;

int16u modbusAddressTcp = 0;
int16u modbusLongthTcp = 0;
int16u modbusAddressSerial = 0;
int16u modbusLongthSerial = 0;

int8u modbusTerminalID;
int8u modbusTerminalIDOn = 0;
uint8_t REGISTER_MODBUS5_BUFFER[100]=
{
	0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x02, 0x00, 0x54, 
	0x00, 0x04, 0x02, 0x30, 0x00, 0x04, 0x02, 0xee, 0x00, 0x16, 
	0x03, 0x0a, 0x00, 0x06, 0x03, 0x88, 0x00, 0x02, 0x05, 0x8b, 
	0x00, 0x08, 0x05, 0x98, 0x00, 0x08, 0x05, 0xa5, 0x00, 0x08, 
	0x05, 0xb2, 0x00, 0x08, 0x05, 0xbf, 0x00, 0x08, 0x05, 0xcc, 
	0x00, 0x08, 0x05, 0xd9, 0x00, 0x08, 0x05, 0xe6, 0x00, 0x08, 
	0x05, 0xf3, 0x00, 0x08, 0x06, 0x00, 0x00, 0x08, 0x0e, 0x74, 
	0x00, 0x04, 0x00
};



void GetServerIP(uint8_t * ip, int8u ipLen, uint8_t* port, uint8_t portLen)
{
	if (ipLen < 16 || portLen < 6)
		return;
	ip[0] = g_modbusReg4[modbusServerIP] / 100;
	ip[1] = (g_modbusReg4[modbusServerIP] - ip[0] * 100 ) / 10;
	ip[2] = g_modbusReg4[modbusServerIP] - ip[0] * 100 - ip[1] * 10;

	ip[0] += '0';
	ip[1] += '0';
	ip[2] += '0';
	
	ip[3] = '.';

	ip[4] = g_modbusReg4[modbusServerIP + 1] / 100;
	ip[5] = (g_modbusReg4[modbusServerIP + 1] - ip[4] * 100 ) / 10;
	ip[6] = g_modbusReg4[modbusServerIP + 1] - ip[4] * 100 - ip[5] * 10;
	
	ip[4] += '0';
	ip[5] += '0';
	ip[6] += '0';

	ip[7] = '.';

	ip[8] = g_modbusReg4[modbusServerIP + 2] / 100;
	ip[9] = (g_modbusReg4[modbusServerIP + 2] - ip[8] * 100 ) / 10;
	ip[10] = g_modbusReg4[modbusServerIP + 2] - ip[8] * 100 - ip[9] * 10;
	
	ip[8] += '0';
	ip[9] += '0';
	ip[10] += '0';

	ip[11] = '.';

	ip[12] = g_modbusReg4[modbusServerIP + 3] / 100;
	ip[13] = (g_modbusReg4[modbusServerIP + 3] - ip[12] * 100 ) / 10;
	ip[14] = g_modbusReg4[modbusServerIP + 3] - ip[12] * 100 - ip[13] * 10;
	
	ip[12] += '0';
	ip[13] += '0';
	ip[14] += '0';

	ip[15] = 0;
	
	port[0] = *(int16u*)(g_modbusReg4 + modbusServerPort) / 10000;
	port[1] = (*(int16u*)(g_modbusReg4 + modbusServerPort) - port[0] * 10000) / 1000;
	port[2] = (*(int16u*)(g_modbusReg4 + modbusServerPort) - port[0] * 10000 - port[1] * 1000) / 100;
	port[3] = (*(int16u*)(g_modbusReg4 + modbusServerPort) - port[0] * 10000 - port[1] * 1000 - port[2] * 100) / 10;
	port[4] = *(int16u*)(g_modbusReg4 + modbusServerPort) - port[0] * 10000 - port[1] * 1000 - port[2] * 100 - port[3] * 10;
	port[0] += '0';
	port[1] += '0';
	port[2] += '0';
	port[3] += '0';
	port[4] += '0';
	port[5] = 0;
	
	
}
void GetServerIPBack(uint8_t* ip, uint8_t ipLen, uint8_t* port, int8u portLen)
{
	if (ipLen < 16 || portLen < 6)
		return;
	ip[0] = g_modbusReg4[modbusServerIPBack] / 100;
	ip[1] = (g_modbusReg4[modbusServerIPBack] - ip[0] * 100 ) / 10;
	ip[2] = g_modbusReg4[modbusServerIPBack] - ip[0] * 100 - ip[1] * 10;

	ip[0] += '0';
	ip[1] += '0';
	ip[2] += '0';
	
	ip[3] = '.';

	ip[4] = g_modbusReg4[modbusServerIPBack + 1] / 100;
	ip[5] = (g_modbusReg4[modbusServerIPBack + 1] - ip[4] * 100 ) / 10;
	ip[6] = g_modbusReg4[modbusServerIPBack + 1] - ip[4] * 100 - ip[5] * 10;
	
	ip[4] += '0';
	ip[5] += '0';
	ip[6] += '0';

	ip[7] = '.';

	ip[8] = g_modbusReg4[modbusServerIPBack + 2] / 100;
	ip[9] = (g_modbusReg4[modbusServerIPBack + 2] - ip[8] * 100 ) / 10;
	ip[10] = g_modbusReg4[modbusServerIPBack + 2] - ip[8] * 100 - ip[9] * 10;
	
	ip[8] += '0';
	ip[9] += '0';
	ip[10] += '0';

	ip[11] = '.';

	ip[12] = g_modbusReg4[modbusServerIPBack + 3] / 100;
	ip[13] = (g_modbusReg4[modbusServerIPBack + 3] - ip[12] * 100 ) / 10;
	ip[14] = g_modbusReg4[modbusServerIPBack + 3] - ip[12] * 100 - ip[13] * 10;
	
	ip[12] += '0';
	ip[13] += '0';
	ip[14] += '0';

	ip[15] = 0;
	
	port[0] = *(int16u*)(g_modbusReg4 + modbusServerPortBack) / 10000;
	port[1] = (*(int16u*)(g_modbusReg4 + modbusServerPortBack) - port[0] * 10000) / 1000;
	port[2] = (*(int16u*)(g_modbusReg4 + modbusServerPortBack) - port[0] * 10000 - port[1] * 1000) / 100;
	port[3] = (*(int16u*)(g_modbusReg4 + modbusServerPortBack) - port[0] * 10000 - port[1] * 1000 - port[2] * 100) / 10;
	port[4] = *(int16u*)(g_modbusReg4 + modbusServerPortBack) - port[0] * 10000 - port[1] * 1000 - port[2] * 100 - port[3] * 10;
	port[0] += '0';
	port[1] += '0';
	port[2] += '0';
	port[3] += '0';
	port[4] += '0';
	port[5] = 0;
}
//eprom all 8*1024
//256
void LoadModbus(void)
{
	int16u i;
// 	int8u temp[16];
	for (i = 0; i < MODBUS_REG4_SIZE; i += 256)
		Fram_BufferRead(&g_modbusReg4[0] + i, i, 256);
	for (i = 0; i < MODBUS_REG5_SIZE; i += 256)
		Fram_BufferRead(&g_modbusReg5[0] + i, MODBUS_REG4_SIZE + i, 256);
	for (i = 0; i < MODBUS_REG6_SIZE; i += 256)
		Fram_BufferRead(&g_modbusReg6[0] + i, MODBUS_REG4_SIZE + MODBUS_REG5_SIZE + i, 256);

	memcpy(g_modbusReg5,REGISTER_MODBUS5_BUFFER,80);


}
void EraseEEPROM(void)
{
	uint16_t i = 0;
	uint8_t  temp[256] = {0};

	for (i = 0; i < MODBUS_REG4_SIZE; i += 256)
		Fram_BufferWrite(temp, i, 256);
	for (i = 0; i < MODBUS_REG5_SIZE; i += 256)
		Fram_BufferWrite(temp, MODBUS_REG4_SIZE + i, 256);
	for (i = 0; i < MODBUS_REG6_SIZE; i += 256)
		Fram_BufferWrite(temp, MODBUS_REG4_SIZE + MODBUS_REG5_SIZE + i, 256);

}
void CleanFlash(void)
{
	uint16_t i = 0;
	uint8_t  temp[256] = {0};

	for (i = 0; i < eprom_data_size; i += 256)
		Fram_BufferWrite(temp, eprom_data_address, 256);
	Modbus_FlashInit();
}


void LoadModbusDefault(void)
{
	g_modbusReg4[modbusTerminalBaseStatus] = 6;
	memcpy(g_modbusReg4 + HOMER_3X_DEVIDE_ID, 		"4901213M", 8);
	memcpy(g_modbusReg4 + modbusSMSNumberBack, 		"8613021286541", 13);
	memcpy(g_modbusReg4 + modbusSMSNumberAlarm, 	"8613021286541", 13);
	memcpy(g_modbusReg4 + modbusSMSNumberConcrol, "8613021286541", 13);

	*(int16u*)(g_modbusReg4 + modbusServerPort) 		= 7778;
	*(int16u*)(g_modbusReg4 + modbusServerPortBack) = 7778;   // backup port

	
	g_modbusReg4[modbusServerIP] 			= 124;
	g_modbusReg4[modbusServerIP + 1] 	= 202;
	g_modbusReg4[modbusServerIP + 2] 	= 198;
	g_modbusReg4[modbusServerIP + 3] 	= 242;
	
//	g_modbusReg4[modbusServerIP] 			= 115;
//	g_modbusReg4[modbusServerIP + 1] 	= 47;
//	g_modbusReg4[modbusServerIP + 2] 	= 44;
//	g_modbusReg4[modbusServerIP + 3] 	= 207;


	g_modbusReg4[modbusServerIPBack] 			= g_modbusReg4[modbusServerIP];
	g_modbusReg4[modbusServerIPBack + 1] 	= g_modbusReg4[modbusServerIP + 1];
	g_modbusReg4[modbusServerIPBack + 2] 	= g_modbusReg4[modbusServerIP + 2];
	g_modbusReg4[modbusServerIPBack + 3] 	= g_modbusReg4[modbusServerIP + 3];	

	*(int16u*)(g_modbusReg4 + modbusTcpHeartInterval) = 30;//120;
	*(int16u*)(g_modbusReg4 + modbusTcpReportIntervalAccOn) = 60;//120;
	*(int16u*)(g_modbusReg4 + modbusTcpReportIntervalAccOff) = 60;//10800;// 3 hour
	*(int16u*)(g_modbusReg4 + modbusSleepDelay) = 600;
	*(int16u*)(g_modbusReg4 + modbusSleepTime) = 300;//
	
	
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_HIGH_ACC_ON) = 0;
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_LOW_ACC_ON) = 60;
	
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_HIGH_ACC_OFF) = 0;
	*(usRegHoldingBuf + TCP_UPLOAD_SECONDS_LOW_ACC_OFF) = 60;
	
	*(usRegHoldingBuf + DEVEICE_WORK_SECONDS_HIGH) = 0;
	*(usRegHoldingBuf + DEVEICE_WORK_SECONDS_LOW) = 300;
	
	*(usRegHoldingBuf + DEVEICE_SLEEP_SECONDS_HIGH) = 0;
	*(usRegHoldingBuf + DEVEICE_SLEEP_SECONDS_LOW) = 300;
	
	*(usRegHoldingBuf + DEVEICE_SLEEP_ENABLE) = 0; // 不允许休眠
	*(usRegHoldingBuf + DEVEICE_RUN_OR_TEST) = 1;	 // 测试模式
	
	
	*(usRegHoldingBuf + ADDR_ADMIN_LOCK_STATE)	 = 1;
	
	*(usRegHoldingBuf + COMM_TIMEOUT_GPRS) = 600;  // Add: 437, 600S
	*(usRegHoldingBuf + COMM_TIMEOUT_UART) = 15; // Add: 438, 15S
	*(usRegHoldingBuf + COMM_TIMEOUT_CAN) = 15; // Add: 439, a5S
	
	
//	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_YEAR) 	= 13;
//	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_MON)		= 8;
//	*(usRegHoldingBuf + ADDR_REMOTE_COMM_LOG_DAY)   = 2;	

	g_modbusReg4[modbusCanEnabled] 	= 1;
	g_modbusReg4[modbusCanBaudrate] = 250;
	*(int32u*)(g_modbusReg4 + modbusCanSendID1) = 0x183;
	g_modbusReg4[modbusCanSendHeart1] = 3;
	g_modbusReg4[modbusCanSendData1] 	= 1;
	
	*(int32u*)(g_modbusReg4 + modbusCanSendID2) = 0x283;
	g_modbusReg4[modbusCanSendHeart2] = 1;
	g_modbusReg4[modbusCanSendData2] 	= 2;

	g_modbusReg4[modbusCanNumber] = 2;
	*(int32u*)(g_modbusReg4 + modbusCanID1) = 0x383;
	*(int32u*)(g_modbusReg4 + modbusCanID1 + 4) = 0x483;

}

// 保存统计工时
void AutoSaveModbus4(void)
{
	Fram_BufferWrite(g_modbusReg4 + 3700, 3700, 252);
	
}
void SaveModbus4(void)
{
	int16u i;
	for (i = 0; i < MODBUS_REG4_SIZE; i += 256)
		Fram_BufferWrite(g_modbusReg4 + i, i, 256);
}
void SaveModbus5(void)
{
	int16u i;
	for (i = 0; i < MODBUS_REG5_SIZE; i += 256)
		Fram_BufferWrite(g_modbusReg5 + i, MODBUS_REG4_SIZE + i, 256);
}
void SaveModbus6(void)
{
	int16u i;
	for (i = 0; i < MODBUS_REG6_SIZE; i += 256)
		Fram_BufferWrite(g_modbusReg6 + i, MODBUS_REG4_SIZE + MODBUS_REG5_SIZE + i, 256);
}
void SaveEEPROM(void)
{
	SaveModbus4();
	SaveModbus5();
	SaveModbus6();
}




/*********************************************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
**调用提示 ：
********************************************************************************************************/
void InitModbusTcp(void)
{

	memset(g_modbusReceiveBufTcp,0,MODBUS_BUFFER_SIZE);
	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	
	g_modbusReceiveBufNumberTcp = 0;
	g_modbusSendBufNumberTcp = 0;
	modbusAddressTcp = 0;
	modbusLongthTcp = 0;
}
void InitModbusSendTcp(void)
{
	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberTcp = 0;
}
void InitModbusSerial(void)
{
	memset(g_modbusReceiveBufSerial,0,MODBUS_BUFFER_SIZE);
	memset(g_modbusSendBufSerial,0,MODBUS_BUFFER_SIZE);
	
	g_modbusReceiveBufNumberSerial = 0;
	g_modbusSendBufNumberSerial = 0;
	modbusAddressSerial = 0;
	modbusLongthSerial = 0;

	modbusTerminalIDOn = 0;
}

void InitModbusSendSerial(void)
{
	memset(g_modbusSendBufSerial,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberSerial = 0;
}
void InitModbus(int8u type)
{
	if (type == 0)
		InitModbusTcp();
	else if (type == 1)
		InitModbusSerial();
}
int8u ModbusReportErrorTcp(int data)
{
// 	#if OS_CRITICAL_METHOD == 3         
// 		OS_CPU_SR  cpu_sr;
// 	#endif
	char reportData[2];
	reportData[0] = 0x80 + data;
	reportData[1] = 0x01;
	
	while(TCPSend_Num > 127)
		os_dly_wait(1);
// 	OS_ENTER_CRITICAL();
	TCPSendBuf[TCPSend_Num] = reportData[0];
	TCPSend_Num++;
	TCPSendBuf[TCPSend_Num] = reportData[1];
	TCPSend_Num++;
	TCPSendBuf[TCPSend_Num] = 0;
// 	OS_EXIT_CRITICAL();
	return MODBUS_OK;
}
int8u ModbusSendDataTcp(void)
{
// 	int i;
// 	#if OS_CRITICAL_METHOD == 3         
// 		OS_CPU_SR  cpu_sr;
// 	#endif
	if (g_modbusSendBufNumberTcp + TCPSend_Num > 256)
		os_dly_wait(10);
		
// 	OS_ENTER_CRITICAL();

	memcpy(TCPSendBuf+TCPSend_Num,g_modbusSendBufTcp,g_modbusSendBufNumberTcp);
	TCPSend_Num += g_modbusSendBufNumberTcp;	
	
	return MODBUS_OK;
}
int8u ModbusReportErrorSerial(int data)
{
	uint8_t  reportData[5];
	uint16_t crc;
	uint8_t  i;
	reportData[0] = modbusTerminalID;
	reportData[1] = 0x80 + data;
	reportData[2] = 0x01;
	crc = usMBCRC16(reportData, 3);
	reportData[4] = crc >> 8;
	reportData[3] = crc & 0xff;
	
	for (i = 0; i < 5; i++)
		QueueWrite((void *)UART1SendBuf, reportData[i]);	
	return MODBUS_OK;
}
int8u ModbusSendDataSerial(void)
{
	int16u i;
	int16u crc;
	g_modbusSendBufSerial[0] = modbusTerminalID;
	g_modbusSendBufNumberSerial++;
	crc = usMBCRC16(g_modbusSendBufSerial, g_modbusSendBufNumberSerial);
	
	for(i = 0; i < g_modbusSendBufNumberSerial; i++) 
	{
		QueueWrite((void *)UART1SendBuf, g_modbusSendBufSerial[i]);	
	}
	QueueWrite((void *)UART1SendBuf, crc & 0xff);
	QueueWrite((void *)UART1SendBuf, crc >> 8);
	return MODBUS_OK;
}
int8u ModbusReportError(int data, int8u type)
{
	uint8_t result = 0;
	if (type == 0)
		result = ModbusReportErrorTcp(data);
	else if (type == 1)
		result = ModbusReportErrorSerial(data);
	
	return result;
}

uint8_t ModbusSendData(uint8_t type)
{
	uint8_t result = 0;
	
	if (type == 0)
		result = ModbusSendDataTcp();
	else if (type == 1)
		result = ModbusSendDataSerial();
	
	return result;
}

uint8_t ModbusInput(uint8_t *pIn, uint32_t inSize, uint8_t * g_modbusReceiveBuf, uint8_t * g_modbusSendBuf, int16u* g_modbusReceiveBufNumber, uint16_t * g_modbusSendBufNumber, uint8_t type, uint16_t * longth, uint16_t * address)
{
	uint16_t i = 0;
	uint16_t receiveSize = 0;
	uint8_t  tempbuf[4] = {0};
	
	while (receiveSize < inSize) 
	{
		if (*g_modbusReceiveBufNumber >= MODBUS_BUFFER_SIZE)
			InitModbus(type);
		g_modbusReceiveBuf[*g_modbusReceiveBufNumber] = *((pIn) + receiveSize);
		
		(*g_modbusReceiveBufNumber)++;
			receiveSize++;
		
		if (g_modbusReceiveBuf[0] == 0x03) 
		{//read
			if (*g_modbusReceiveBufNumber > 4) 
			{
				*address	= g_modbusReceiveBuf[2] + ((uint16_t)(g_modbusReceiveBuf[1]) << 8);
				*longth		= g_modbusReceiveBuf[4] + ((uint16_t)(g_modbusReceiveBuf[3]) << 8);
				
				if (type == 1) 
				{
					if (*g_modbusReceiveBufNumber > 6) 
					{
						if (usMBCRC16(&g_modbusReceiveBuf[0] - 1, 8)) 
						{
							InitModbus(type);
							ModbusReportError(0x3, type);
							continue;
						}
					}
					else
						continue;
				}
				if ((((*address + *longth) << 1) > MODBUS_REG4_SIZE) || (*longth > 125)) 
				{
					InitModbus(type);
					ModbusReportError(0x3, type);
				}
				else 
				{
					if (type == 1) 
					{
						g_modbusSendBuf++;
					}
					g_modbusSendBuf[0] = 0x03;
					g_modbusSendBuf[1] = *longth << 1;
					memcpy(g_modbusSendBuf+2, g_modbusReg4+(*address << 1), (*longth << 1));
					
					tempbuf[0] = g_modbusSendBuf[2]; 
					tempbuf[1] = g_modbusSendBuf[3]; 
					tempbuf[2] = g_modbusSendBuf[4]; 
					tempbuf[3] = g_modbusSendBuf[5]; 					
					
					g_modbusSendBuf[2] = tempbuf[1]; 
					g_modbusSendBuf[3] = tempbuf[0]; 
					
					g_modbusSendBuf[4] = tempbuf[3];
					g_modbusSendBuf[5] = tempbuf[2]; 
					
					*g_modbusSendBufNumber = (*longth << 1) + 2;
					
//					ModbusSendData(type);
					TcpSendData(g_modbusSendBuf, *g_modbusSendBufNumber);
//					InitModbus(type);
					InitModbusTcp();
				}
			}
			continue;
		}


		// 最多接收5个
		if (g_modbusReceiveBuf[0] == 0x06) 
		{//write single
			if (*g_modbusReceiveBufNumber > 4) 
			{
				if (type == 1) 
				{
					if (*g_modbusReceiveBufNumber > 6) 
					{
						if (usMBCRC16(g_modbusReceiveBuf - 1, 8)) 
						{
							InitModbus(type);
							ModbusReportError(0x6, type);
							continue;
						}
					}
					else
						continue;
				}
				
				*address = g_modbusReceiveBuf[2] + ((int16u)(g_modbusReceiveBuf[1]) << 8);
				if ((*address << 1) >= MODBUS_REG4_SIZE) 
				{
					InitModbus(type);
					ModbusReportError(0x6, type);
				}
				else 
				{
					if (type == 1) 
					{
						g_modbusSendBuf++;
					}
					// 更新寄存器数据,MODBUS 寄存器显示的是高位在后 需要逆转一下
					g_modbusReg4[((*address) << 1)] 		= g_modbusReceiveBuf[4];
					g_modbusReg4[((*address) << 1) + 1] = g_modbusReceiveBuf[3];	

					TcpSendData(g_modbusReceiveBuf, 5);
//					InitModbus(type);	
					InitModbusTcp();
				}
			}
			continue;
		}
		
		// 写多个寄存器
		if (g_modbusReceiveBuf[0] == 0x10) 
		{//write mul
			if (*g_modbusReceiveBufNumber > 4 && *longth == 0)
			{
				*address = g_modbusReceiveBuf[2] + ((int16u)(g_modbusReceiveBuf[1]) << 8);
				*longth = g_modbusReceiveBuf[4] + ((int16u)(g_modbusReceiveBuf[3]) << 8);
				if (((*address + *longth) << 1) > MODBUS_REG4_SIZE || *longth > 0x7d) 
				{
					InitModbus(type);
					ModbusReportError(0x10, type);
				}
			}
			else 
			{
				if (*g_modbusReceiveBufNumber >= ((*longth << 1) + 6)) 
				{
					if (type == 1) {
						if (*g_modbusReceiveBufNumber >= ((*longth << 1) + 8)) 
						{
							if (usMBCRC16(g_modbusReceiveBuf - 1, *g_modbusReceiveBufNumber + 3)) 
							{
								InitModbus(type);
								ModbusReportError(0x10, type);
								continue;
							}
						}
						else
							continue;
					}
					tempbuf[0] = g_modbusReceiveBuf[6]; 
					tempbuf[1] = g_modbusReceiveBuf[7]; 
					tempbuf[2] = g_modbusReceiveBuf[8]; 
					tempbuf[3] = g_modbusReceiveBuf[9]; 

					
					g_modbusReceiveBuf[7] = tempbuf[0];
					g_modbusReceiveBuf[6] = tempbuf[1];
					
					g_modbusReceiveBuf[9] = tempbuf[2];
					g_modbusReceiveBuf[8] = tempbuf[3];					
					
					for(i = 0; i < (*longth << 1); i++) 
					{
						g_modbusReg4[(*address << 1) + i] = g_modbusReceiveBuf[6 + i];
					}
					if (type == 1) 
						g_modbusSendBuf++;
					

					TcpSendData(g_modbusReceiveBuf, 5);
					InitModbusTcp();		
//					InitModbus(type);
				}
			}
			continue;
		}
		
		// 扩展指令
		if (g_modbusReceiveBuf[0] == 0x41) 
		{
			if (*g_modbusReceiveBufNumber > 4) 
			{
				*address = g_modbusReceiveBuf[2] + ((int16u)(g_modbusReceiveBuf[1]) << 8);
				*longth = g_modbusReceiveBuf[4] + ((int16u)(g_modbusReceiveBuf[3]) << 8);
				//sprintf(tempbuf, "%d %d %d %d\r\n", address, longth, ((int16u)(g_modbusReceiveBuf[1]) << 8), ((int16u)(g_modbusReceiveBuf[3]) << 8));
				//DebugLog(tempbuf);
				if (((*address + *longth) << 1) > MODBUS_REG5_SIZE || *longth > 0x7d) {
					InitModbus(type);
					ModbusReportError(0x41, type);
				}
				else {
					if (type == 1) {
						g_modbusSendBuf++;
					}
					g_modbusSendBuf[0] = 0x41;
					g_modbusSendBuf[1] = *longth << 1;
					for(i = 0; i < (*longth << 1); i++) {
						g_modbusSendBuf[i + 2] = g_modbusReg5[(*address << 1) + i];
					}
					*g_modbusSendBufNumber = (*longth << 1) + 2;
					ModbusSendData(type);
					InitModbus(type);
				}
			}
			continue;
		}
		
		
		if (g_modbusReceiveBuf[0] == 0x42) 
		{
			if (*g_modbusReceiveBufNumber > 4 && *longth == 0) 
			{
				*address = g_modbusReceiveBuf[2] + ((int16u)(g_modbusReceiveBuf[1]) << 8);
				*longth = g_modbusReceiveBuf[4] + ((int16u)(g_modbusReceiveBuf[3]) << 8);
				
				if (((*address + *longth) << 1) > MODBUS_REG5_SIZE || *longth > 0x7d) {
					InitModbus(type);
					
					ModbusReportError(0x42, type);
				}
			}
			else if (*g_modbusReceiveBufNumber == ((*longth << 1) + 6)) {
				for(i = 0; i < (*longth << 1); i++) {
					g_modbusReg5[(*address << 1) + i] = g_modbusReceiveBuf[6 + i];
				}
				if (type == 1) {
					g_modbusSendBuf++;
				}
				g_modbusSendBuf[0] = 0x42;
				g_modbusSendBuf[1] = g_modbusReceiveBuf[1];
				g_modbusSendBuf[2] = g_modbusReceiveBuf[2];
				g_modbusSendBuf[3] = g_modbusReceiveBuf[3];
				g_modbusSendBuf[4] = g_modbusReceiveBuf[4];
				*g_modbusSendBufNumber = 5;
				ModbusSendData(type);
				InitModbus(type);
			}
			continue;
		}
		if (g_modbusReceiveBuf[0] == 0x43) 
		{
			if (*g_modbusReceiveBufNumber > 4) {
				*address = g_modbusReceiveBuf[2] + ((int16u)(g_modbusReceiveBuf[1]) << 8);
				*longth = g_modbusReceiveBuf[4] + ((int16u)(g_modbusReceiveBuf[3]) << 8);
				if (((*address + *longth) << 1) > MODBUS_REG6_SIZE || *longth > 0x7d) {
					InitModbus(type);
					ModbusReportError(0x43, type);
				}
				else {
					if (type == 1) {
						g_modbusSendBuf++;
					}
					g_modbusSendBuf[0] = 0x43;
					g_modbusSendBuf[1] = *longth << 1;
					for(i = 0; i < (*longth << 1); i++) {
						g_modbusSendBuf[i + 2] = g_modbusReg6[(*address << 1) + i];
					}
					*g_modbusSendBufNumber = (*longth << 1) + 2;
					ModbusSendData(type);
					InitModbus(type);
				}
			}
			continue;
		}
		if (g_modbusReceiveBuf[0] == 0x44) 
		{
			if (*g_modbusReceiveBufNumber > 4 && *longth == 0) 
			{
				*address = g_modbusReceiveBuf[2] + ((int16u)(g_modbusReceiveBuf[1]) << 8);
				*longth = g_modbusReceiveBuf[4] + ((int16u)(g_modbusReceiveBuf[3]) << 8);
				if (((*address + *longth) << 1) > MODBUS_REG6_SIZE || *longth > 0x7d) {
					InitModbus(type);
					ModbusReportError(0x44, type);
				}
			}
			else if (*g_modbusReceiveBufNumber == ((*longth << 1) + 6)) 
			{
				for(i = 0; i < (*longth << 1); i++) 
				{
					g_modbusReg6[(*address << 1) + i] = g_modbusReceiveBuf[6 + i];
				}
				if (type == 1) 
				{
					g_modbusSendBuf++;
				}
				g_modbusSendBuf[0] = 0x44;
				g_modbusSendBuf[1] = g_modbusReceiveBuf[1];
				g_modbusSendBuf[2] = g_modbusReceiveBuf[2];
				g_modbusSendBuf[3] = g_modbusReceiveBuf[3];
				g_modbusSendBuf[4] = g_modbusReceiveBuf[4];
				*g_modbusSendBufNumber = 5;
				ModbusSendData(type);
				InitModbus(type);
			}
			continue;
		}
		
		
		ModbusReportError(g_modbusReceiveBuf[0], type);
		InitModbus(type);
		continue;
	}
	return MODBUS_OK;
}


uint8_t ModbusInputSerial(uint8_t *pIn, uint32_t inSize)
{
	uint8_t  result = 0;
	if(clearModbusTimeOut == 0) 
	{
		memset(g_modbusReceiveBufSerial,	0,	MODBUS_BUFFER_SIZE);
		memset(g_modbusSendBufSerial,			0,	MODBUS_BUFFER_SIZE);
		
		
		g_modbusReceiveBufNumberSerial = 0;
		g_modbusSendBufNumberSerial = 0;
		modbusAddressSerial = 0;
		modbusLongthSerial = 0;

		modbusTerminalIDOn = 0;
	}

	clearModbusTimeOut = MODBUS_CLEAR_TIMEOUT;

	if (modbusTerminalIDOn == 0) 
	{
		modbusTerminalID = *pIn;
		modbusTerminalIDOn = 1;
		g_modbusReceiveBufSerial[0] = *pIn;
		pIn++;
		inSize--;
		return (0);
	}
	
	result = ModbusInput(pIn, inSize, g_modbusReceiveBufSerial + 1, &g_modbusSendBufSerial[0], &g_modbusReceiveBufNumberSerial, &g_modbusSendBufNumberSerial, 1, &modbusLongthSerial, &modbusAddressSerial);
	return result;
}

uint8_t Modbus_TCP_Handler(uint8_t *pIn, uint32_t inSize)
{	
	uint8_t result = 0;
	uint8_t * temp = NULL;
	temp = pIn;

	
	if(inSize > 0)
	{
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
												&modbusLongthTcp, &modbusAddressTcp);
	
		* temp 	 = 0;		
		inSize = 0;
	}
	
	
	return result;
}


/*

uint8_t Modbus_TCP_Handler(uint8_t *pIn, uint32_t inSize)
{	
	uint8_t result = 0;
	uint8_t * temp = NULL;
	temp = pIn;
// 	uint8_t buffer[1] = {0};

	
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

*/

// 更新TCPIP上传数据
void ModbusTcpReportByTime(void)//0x45
{
	int16u address = 0;
	int16u longth = 0;
	int16u allSize = 0;
	int16u i = 0;
	int16u m;
	
	address = ((int16u)(g_modbusReg5[i << 2]) << 8) + g_modbusReg5[(i << 2 ) + 1];
	longth 	= ((int16u)(g_modbusReg5[(i << 2) + 2]) << 8) + g_modbusReg5[(i << 2 ) + 3];

	while(longth != 0) 
	{
		if (longth > MAX_TX_BUF_SIZE) 
		{
			ModbusReportErrorTcp(0x45);
			InitModbusSendTcp();
		}
		if (address + longth > MODBUS_REG4_SIZE - 1) {
			ModbusReportErrorTcp(0x45);
			InitModbusSendTcp();
		}
		for(m = 0; m < longth; m++) 
		{
			g_modbusSendBufTcp[allSize + 2 + m] = g_modbusReg4[address + m];
		}
		i++;
		allSize += longth;
		address = ((int16u)(g_modbusReg5[i << 2]) << 8) + g_modbusReg5[(i << 2 ) + 1];
		longth 	= ((int16u)(g_modbusReg5[(i << 2) + 2]) << 8) + g_modbusReg5[(i << 2 ) + 3];
	}
	
	g_modbusSendBufTcp[0] = 0x45;
	g_modbusSendBufTcp[1] = allSize;
	g_modbusSendBufNumberTcp = allSize + 2;
	
	if (g_modbusSendBufNumberTcp + TCPSend_Num > 256)
		os_dly_wait(10);
		
	memcpy(TCPSendBuf+TCPSend_Num,g_modbusSendBufTcp,g_modbusSendBufNumberTcp);
	TCPSend_Num += g_modbusSendBufNumberTcp;	
	
	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberTcp = 0;
	
}


void ModbusSmsReportByTime(void)//0x46
{
	uint16_t	address	= 0;
	uint16_t 	longth 	= 0;
	uint16_t 	allSize	= 0;
	uint16_t 	i	= 0;
	uint16_t 	m	= 0;
	
	address = ((uint16_t)(g_modbusReg6[i << 2]) << 8) 			+ g_modbusReg6[(i << 2 ) + 1];
	longth 	= ((uint16_t)(g_modbusReg6[(i << 2) + 2]) << 8) + g_modbusReg6[(i << 2 ) + 3];
	
	
	while(longth != 0) 
	{
		if (longth > MAX_TX_BUF_SIZE ) 
		{
			ModbusReportErrorTcp(0x46);
			InitModbusSendTcp();
		}
		if( (address + longth) > (MODBUS_REG4_SIZE - 1) )
		{
			ModbusReportErrorTcp(0x46);
			InitModbusSendTcp();
		}
		
		for(m = 0; m < longth; m++) 
		{
			g_modbusSendBufTcp[allSize + 2 + m] = g_modbusReg4[address + m];
		}
		
		i++;
		allSize += longth;
		address = ((int16u)(g_modbusReg6[i << 2]) << 8) + g_modbusReg6[(i << 2 ) + 1];
		longth = ((int16u)(g_modbusReg6[(i << 2) + 2]) << 8) + g_modbusReg6[(i << 2 ) + 3];
	}
	
	g_modbusSendBufTcp[0] = 0x46;
	g_modbusSendBufTcp[1] = allSize;
	g_modbusSendBufNumberTcp = allSize + 2;
	
	memcpy(TCPSendBuf+TCPSend_Num,g_modbusSendBufTcp,g_modbusSendBufNumberTcp);
	TCPSend_Num += g_modbusSendBufNumberTcp;		

	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberTcp = 0;
	
}

void Modbus_Handler(void)
{
	uint16_t command = 0;
	if(*(usRegHoldingBuf + MODBUS_COMMAND) != 0)
	{
		command = *(usRegHoldingBuf + MODBUS_COMMAND);
		*(usRegHoldingBuf + MODBUS_COMMAND) = 0;
		
		switch(command) 
		{
			case 1:
				SaveEEPROM();
				break;//save all
			case 2:
				LoadModbus();
				break;//load
			case 3:
				LoadModbusDefault();
				break;//default
			case 4:
				SaveModbus4();
				break;//save 1
			case 5:
				SaveModbus5();
				break;//save 2
			case 6:
				SaveModbus6();
				break;//save 3
			case 7:
				EraseEEPROM();
				break;
			case 8:
				CleanFlash();
				break;
			case 9:
	// 			g_uartMode = Command;
				break;
			case 10:
	//			g_modbusReg4[modbusDefaultSet] = 0;//reboot
				break;
			
			default: 
				break;
		}
	
	}
	
}


