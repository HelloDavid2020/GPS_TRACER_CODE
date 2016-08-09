#include "user_tcpip.h"

uint8_t	old_serverIP[16] ={0};
uint8_t old_serverPort[6]={0};

uint8_t	new_serverIP[16] ={0};
uint8_t new_serverPort[6]={0};


TCP_DATA Tcp;

// ���� 0 δ�ı䣬��Ϊ0 �����ı�
int8_t Check_ip_port(void)
{
	int8_t reslut = 0;
	#ifdef DEBUG
	uint8_t buf[]= "> port change.";
	#endif
	GetServerIP(new_serverIP, 16, new_serverPort, 6);
	reslut = strcmp( (void *)old_serverPort, (void *)new_serverPort );
	
	if(reslut != 0)
	{
		memcpy(serverPort,new_serverPort,6);
		memcpy(old_serverPort,serverPort,6);
//		MG323B.Bit.portChange = 1;
		#ifdef DEBUG
		GPRS_TCPIP_Send(buf, sizeof(buf));
		#endif
		GPRS_TCPIP_DisConnect();
		os_dly_wait(100);
		
	}
	else
	{
//		MG323B.Bit.portChange = 0;
	}
	return  reslut;
}


// ������ǰGPRS״̬
uint8_t Check_GPRS_Status(void)
{

	// ��ѯGPRS����״̬
	if(AT_CommandRun("AT+CGATT?\r\n", "OK",5,1000)==AT_ERROR)
		return AT_ERROR;
	
	// ��ѯGPRS��ʼ���Ƿ�ɹ�
	if(AT_CommandRun("AT%ETCPIP?\r\n", "OK",5,5000)==AT_ERROR)
		return AT_ERROR;	

	return AT_OK;
}


int8_t Close_GPRS_tcpip(void)
{
   //AT+CIPCLOSE:�Ͽ�����
	if(AT_CommandRun("at%ipclose\r\n", "CLOSE OK", 1,50000)==AT_ERROR)
		return AT_ERROR; 

	return AT_OK;  
}

// ���½�������
void  Reestablish_the_connection(void)
{
	// ������ǰ����
	if(Check_GPRS_Status() == AT_OK)
	{
		Close_GPRS_tcpip();
	}
	else
	{
		GSM_TCP_Init();
	}

	
// ����������
}

// ����TCPIP�ϴ����� ��ȡtcpip���͵����ݼ����ݳ���
void Update_tcpip_Data(void) // 0x45
{
	uint16_t	address = 0;
	uint16_t	longth 	= 0;
	uint8_t		allSize = 0;
	uint16_t	i = 0;
// 	uint16_t	m = 0;
	
	address = ((uint16_t)(g_modbusReg5[i << 2])       << 8)  + 	g_modbusReg5[(i << 2 ) + 1];
	longth 	= ((uint16_t)(g_modbusReg5[(i << 2) + 2]) << 8)	 +  g_modbusReg5[(i << 2 ) + 3];

	while(longth != 0) 
	{
		if (longth > MAX_TX_BUF_SIZE) 
		{
			ModbusReportErrorTcp(0x45);
			memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
			g_modbusSendBufNumberTcp = 0;

		}
		if ( (address + longth) > (MODBUS_REG4_SIZE - 1) ) 
		{
			ModbusReportErrorTcp(0x45);
			memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
			g_modbusSendBufNumberTcp = 0;

		}

		memcpy(g_modbusSendBufTcp + allSize + 2, g_modbusReg4+address, longth);
	
		allSize += longth; // �ܵ����ݳ��� ����
		
		// ��ȡ��һ ���ݵ�ַ �� ���ݳ���
		i++;		
		
		address = ((int16u)(g_modbusReg5[i << 2])       << 8)	+ 	g_modbusReg5[(i << 2 ) + 1];
		longth 	= ((int16u)(g_modbusReg5[(i << 2) + 2]) << 8) + 	g_modbusReg5[(i << 2 ) + 3];
	}
	
	
	g_modbusSendBufTcp[0] 		= 0x45;
	g_modbusSendBufTcp[1] 		= allSize;
	g_modbusSendBufNumberTcp 	= allSize + 2;  // ���2��������
	
	// void*memcpy(void*dest,constvoid*src,size_tn)
	memcpy(&g_modbusSendBufTcp[50], g_modbusReg4 + ADDR_CAN_DATA, 48);

	// add Li-battery voltage
	// Low byte High byte 
	// ��λ��ǰ ��λ�ں�
	
	*(uint16_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_VOL_BATTERY) = (uint16_t)AdcSensor.power;
	
	// add Li-battery voltage
	*(uint16_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_VOL_LIBAT) = (uint16_t)AdcSensor.battery;
	
	// add worktime 
//	*(uint32_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_WORKTIME) = *(uint32_t *)(g_modbusReg4 + modbusTimeACC);
	*(uint32_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_WORKTIME) = VehicleWorkTimeOfOneDay;
	
	// �Ƿ��� �����������ڷ���
	if (g_modbusSendBufNumberTcp + TCPSend_Num > 256)
		os_dly_wait(10);

	memcpy(TCPSendBuf+TCPSend_Num,g_modbusSendBufTcp,g_modbusSendBufNumberTcp);
	
	TCPSend_Num += g_modbusSendBufNumberTcp;
	
	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberTcp = 0;

}

// 
// ����Flashä������
void Update_Flash_Data(void) // 0x49
{
	uint16_t	address = 0;
	uint16_t	longth 	= 0;
	uint8_t		allSize = 0;
	uint16_t	i = 0;
// 	uint16_t	m = 0;
	
	address = ((uint16_t)(g_modbusReg5[i << 2])       << 8)  + 	g_modbusReg5[(i << 2 ) + 1];
	longth 	= ((uint16_t)(g_modbusReg5[(i << 2) + 2]) << 8)	 +  g_modbusReg5[(i << 2 ) + 3];

	while(longth != 0) 
	{
		if (longth > MAX_TX_BUF_SIZE) 
		{
			ModbusReportErrorTcp(0x45);
			memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
			g_modbusSendBufNumberTcp = 0;

		}
		if ( (address + longth) > (MODBUS_REG4_SIZE - 1) ) 
		{
			ModbusReportErrorTcp(0x45);
			memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
			g_modbusSendBufNumberTcp = 0;

		}

		memcpy(g_modbusSendBufTcp + allSize + 2, g_modbusReg4+address, longth);
	
		allSize += longth; // �ܵ����ݳ��� ����
		
		// ��ȡ��һ ���ݵ�ַ �� ���ݳ���
		i++;		
		
		address = ((int16u)(g_modbusReg5[i << 2])       << 8)	+ 	g_modbusReg5[(i << 2 ) + 1];
		longth 	= ((int16u)(g_modbusReg5[(i << 2) + 2]) << 8) + 	g_modbusReg5[(i << 2 ) + 3];
	}
	
	
	g_modbusSendBufTcp[0] 		= 0x49;
	g_modbusSendBufTcp[1] 		= allSize;
	g_modbusSendBufNumberTcp 	= allSize + 2;  // ���2��������
	
	// �Ƿ��� �����������ڷ���
	if (g_modbusSendBufNumberTcp + TCPSend_Num > 256)
		os_dly_wait(10);

	memcpy(TCPSendBuf+TCPSend_Num,g_modbusSendBufTcp,g_modbusSendBufNumberTcp);
	TCPSend_Num += g_modbusSendBufNumberTcp;
	
	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberTcp = 0;

}


// g_modbusReg6
void Update_SMS_Data(void)//0x46
{
	uint16_t	address	= 0;
	uint16_t 	longth 	= 0;
	uint16_t 	allSize	= 0;
	uint16_t 	i	= 0;
// 	uint16_t 	m	= 0;
	
	address = ((uint16_t)(g_modbusReg6[i << 2]) << 8) 			+ g_modbusReg6[(i << 2 ) + 1];
	longth 	= ((uint16_t)(g_modbusReg6[(i << 2) + 2]) << 8) + g_modbusReg6[(i << 2 ) + 3];
	
	
	while(longth != 0) 
	{
		if (longth > MAX_TX_BUF_SIZE ) 
		{
			ModbusReportErrorTcp(0x46);
			memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
			g_modbusSendBufNumberTcp = 0;
		}
		if( (address + longth) > (MODBUS_REG4_SIZE - 1) )
		{
			ModbusReportErrorTcp(0x46);
			memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
			g_modbusSendBufNumberTcp = 0;
		}

		memcpy(g_modbusSendBufTcp + allSize + 2, g_modbusReg4+address, longth);
		
		i++;
		allSize += longth;

		address = ((uint16_t)(g_modbusReg6[i << 2]) << 8) 			+ g_modbusReg6[(i << 2 ) + 1];
		longth 	= ((uint16_t)(g_modbusReg6[(i << 2) + 2]) << 8) + g_modbusReg6[(i << 2 ) + 3];
	}
	
	g_modbusSendBufTcp[0] = 0x46;
	g_modbusSendBufTcp[1] = allSize;
	g_modbusSendBufNumberTcp = allSize + 2;
	
	memcpy(TCPSendBuf+TCPSend_Num,g_modbusSendBufTcp,g_modbusSendBufNumberTcp);
	TCPSend_Num += g_modbusSendBufNumberTcp;		

	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberTcp = 0;
	
}

// �~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
// ����GPRS���ݰ�

// ����TCPIP�ϴ����� ��ȡtcpip���͵����ݼ����ݳ���
void ReloadGprsData(TCP_DATA * tcp) 
{
	uint16_t	dataAddr = 0; // �Ĵ������ݵ�ַ
	uint16_t	dataLen	 = 0;  // �Ĵ������ݳ���
	uint16_t	dataSize = 0; // ���¼��ص����� �ܴ�С
	uint16_t	i = 0;
	
	dataAddr = ((uint16_t)(g_modbusReg5[i << 2])       << 8)  + 	g_modbusReg5[(i << 2 ) + 1];
	dataLen  = ((uint16_t)(g_modbusReg5[(i << 2) + 2]) << 8)	 +  g_modbusReg5[(i << 2 ) + 3];
	
	

	
	while(dataLen != 0) 
	{
		// ǰ�����ֽ�Ϊ header 0x45 0x84
		memcpy(tcp->TxData + 2 + dataSize, g_modbusReg4 + dataAddr, dataLen);
		dataSize += dataLen; // �ܵ����ݳ��� ����
		
		// ��ȡ��һ ���ݵ�ַ �� ���ݳ���
		i++;		
		dataAddr = ((uint16_t)(g_modbusReg5[i << 2])       << 8) + 	g_modbusReg5[(i << 2 ) + 1];
		dataLen  = ((uint16_t)(g_modbusReg5[(i << 2) + 2]) << 8) + 	g_modbusReg5[(i << 2 ) + 3];
	}
	
	tcp->TxData[0]	= 0x45;
	tcp->TxData[1] = dataSize;
	tcp->TxDataLen = dataSize + 2;  // ���2��������
	
	
	
	
	
	memcpy(&tcp->TxData[50], g_modbusReg4 + ADDR_CAN_DATA, 48);

	// add Li-battery voltage
	// Low byte High byte 
	// ��λ��ǰ ��λ�ں�
	
	*(uint16_t *)(tcp->TxData + ADDR_TCP_SENDBUF_VOL_BATTERY) = (uint16_t)AdcSensor.power;
	
	// add Li-battery voltage
	*(uint16_t *)(tcp->TxData + ADDR_TCP_SENDBUF_VOL_LIBAT) = (uint16_t)AdcSensor.battery;
	
	// add worktime 
//	*(uint32_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_WORKTIME) = *(uint32_t *)(g_modbusReg4 + modbusTimeACC);
	*(uint32_t *)(tcp->TxData + ADDR_TCP_SENDBUF_WORKTIME) = VehicleWorkTimeOfOneDay;

	if( (dataLen > MAX_TX_BUF_SIZE) || ((dataAddr + dataLen) > (MODBUS_REG4_SIZE - 1) ) )
	{
		tcp->TxData[0] = 0x45;
		tcp->TxData[1] = 0x81;
		tcp->TxDataLen = 4;
	}
}



int8_t SendGprsDataPackage(TCP_DATA * tcp)
{
	int8_t result = OK;
	uint16_t i = 0;
//	ReloadGprsData(&Tcp);

	if(TcpSendData(tcp->TxData,tcp->TxDataLen))
	{
		result = OK;
		#ifdef DEBUG
		printf("> Data Upload OK!\r\n");
		for(i = 0; i < tcp->TxDataLen; i++)
		{
			printf("0x%02x ",tcp->TxData[i]);
		}
		printf(" \r\n");			
		#endif

		// �������һ�η��͵�ʱ��
		LogDate.UpdateLogDateEnable = ENABLE;
	}
	else
	{
		result = ERROR;
		#ifdef DEBUG
		printf("> Data Upload Fail!\r\n");
		#endif

		Gprs.LinkStatus = LINK_STATUS_ERROR;
	}
	
	return result;

}


void SaveGprsDataToFlash(TCP_DATA * tcp)
{
	BlindZoneData.TotalLogNums = ReadTotalLogNums();
	BlindZoneData.LogNum = BlindZoneData.TotalLogNums % MAX_LOG_NUMS;
	
	memcpy(BlindZoneData.Writebuf, tcp->TxData, tcp->TxDataLen);
	 
// ******************************************	******	******	******		
	BlindZoneData.Writebuf[0] =  0x46;  

//	BlindZoneData.Writebuf[tcp->TxDataLen] =  BlindZoneData.TotalLogNums;  
// ******************************************	******	******	******		
	FlashBufferWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);    // ��ʼ��log����

	CheckSpiFlashWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);

	#ifdef DEBUG
	printf(" ### TotalLogNums: %d\r\n", BlindZoneData.TotalLogNums);
	#endif
	BlindZoneData.TotalLogNums ++;
	WriteTotalLogNums(BlindZoneData.TotalLogNums);	
}


// �~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~



