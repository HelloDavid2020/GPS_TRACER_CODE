#include "user_tcpip.h"

uint8_t	old_serverIP[16] ={0};
uint8_t old_serverPort[6]={0};

uint8_t	new_serverIP[16] ={0};
uint8_t new_serverPort[6]={0};


TCP_DATA Tcp;

// 返回 0 未改变，不为0 则发生改变
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


// 产看当前GPRS状态
uint8_t Check_GPRS_Status(void)
{

	// 查询GPRS连接状态
	if(AT_CommandRun("AT+CGATT?\r\n", "OK",5,1000)==AT_ERROR)
		return AT_ERROR;
	
	// 查询GPRS初始化是否成功
	if(AT_CommandRun("AT%ETCPIP?\r\n", "OK",5,5000)==AT_ERROR)
		return AT_ERROR;	

	return AT_OK;
}


int8_t Close_GPRS_tcpip(void)
{
   //AT+CIPCLOSE:断开连接
	if(AT_CommandRun("at%ipclose\r\n", "CLOSE OK", 1,50000)==AT_ERROR)
		return AT_ERROR; 

	return AT_OK;  
}

// 重新建立连接
void  Reestablish_the_connection(void)
{
	// 结束当前连接
	if(Check_GPRS_Status() == AT_OK)
	{
		Close_GPRS_tcpip();
	}
	else
	{
		GSM_TCP_Init();
	}

	
// 建立新连接
}

// 更新TCPIP上传数据 获取tcpip发送的数据及数据长度
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
	
		allSize += longth; // 总的数据长度 计算
		
		// 获取下一 数据地址 和 数据长度
		i++;		
		
		address = ((int16u)(g_modbusReg5[i << 2])       << 8)	+ 	g_modbusReg5[(i << 2 ) + 1];
		longth 	= ((int16u)(g_modbusReg5[(i << 2) + 2]) << 8) + 	g_modbusReg5[(i << 2 ) + 3];
	}
	
	
	g_modbusSendBufTcp[0] 		= 0x45;
	g_modbusSendBufTcp[1] 		= allSize;
	g_modbusSendBufNumberTcp 	= allSize + 2;  // 外加2个结束符
	
	// void*memcpy(void*dest,constvoid*src,size_tn)
	memcpy(&g_modbusSendBufTcp[50], g_modbusReg4 + ADDR_CAN_DATA, 48);

	// add Li-battery voltage
	// Low byte High byte 
	// 低位在前 高位在后
	
	*(uint16_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_VOL_BATTERY) = (uint16_t)AdcSensor.power;
	
	// add Li-battery voltage
	*(uint16_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_VOL_LIBAT) = (uint16_t)AdcSensor.battery;
	
	// add worktime 
//	*(uint32_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_WORKTIME) = *(uint32_t *)(g_modbusReg4 + modbusTimeACC);
	*(uint32_t *)(g_modbusSendBufTcp + ADDR_TCP_SENDBUF_WORKTIME) = VehicleWorkTimeOfOneDay;
	
	// 是否有 其他是数据在发送
	if (g_modbusSendBufNumberTcp + TCPSend_Num > 256)
		os_dly_wait(10);

	memcpy(TCPSendBuf+TCPSend_Num,g_modbusSendBufTcp,g_modbusSendBufNumberTcp);
	
	TCPSend_Num += g_modbusSendBufNumberTcp;
	
	memset(g_modbusSendBufTcp,0,MODBUS_BUFFER_SIZE);
	g_modbusSendBufNumberTcp = 0;

}

// 
// 更新Flash盲区数据
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
	
		allSize += longth; // 总的数据长度 计算
		
		// 获取下一 数据地址 和 数据长度
		i++;		
		
		address = ((int16u)(g_modbusReg5[i << 2])       << 8)	+ 	g_modbusReg5[(i << 2 ) + 1];
		longth 	= ((int16u)(g_modbusReg5[(i << 2) + 2]) << 8) + 	g_modbusReg5[(i << 2 ) + 3];
	}
	
	
	g_modbusSendBufTcp[0] 		= 0x49;
	g_modbusSendBufTcp[1] 		= allSize;
	g_modbusSendBufNumberTcp 	= allSize + 2;  // 外加2个结束符
	
	// 是否有 其他是数据在发送
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

// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
// 发送GPRS数据包

// 更新TCPIP上传数据 获取tcpip发送的数据及数据长度
void ReloadGprsData(TCP_DATA * tcp) 
{
	uint16_t	dataAddr = 0; // 寄存器数据地址
	uint16_t	dataLen	 = 0;  // 寄存器数据长度
	uint16_t	dataSize = 0; // 重新加载的数据 总大小
	uint16_t	i = 0;
	
	dataAddr = ((uint16_t)(g_modbusReg5[i << 2])       << 8)  + 	g_modbusReg5[(i << 2 ) + 1];
	dataLen  = ((uint16_t)(g_modbusReg5[(i << 2) + 2]) << 8)	 +  g_modbusReg5[(i << 2 ) + 3];
	
	

	
	while(dataLen != 0) 
	{
		// 前两个字节为 header 0x45 0x84
		memcpy(tcp->TxData + 2 + dataSize, g_modbusReg4 + dataAddr, dataLen);
		dataSize += dataLen; // 总的数据长度 计算
		
		// 获取下一 数据地址 和 数据长度
		i++;		
		dataAddr = ((uint16_t)(g_modbusReg5[i << 2])       << 8) + 	g_modbusReg5[(i << 2 ) + 1];
		dataLen  = ((uint16_t)(g_modbusReg5[(i << 2) + 2]) << 8) + 	g_modbusReg5[(i << 2 ) + 3];
	}
	
	tcp->TxData[0]	= 0x45;
	tcp->TxData[1] = dataSize;
	tcp->TxDataLen = dataSize + 2;  // 外加2个结束符
	
	
	
	
	
	memcpy(&tcp->TxData[50], g_modbusReg4 + ADDR_CAN_DATA, 48);

	// add Li-battery voltage
	// Low byte High byte 
	// 低位在前 高位在后
	
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

		// 保存最后一次发送的时间
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
	FlashBufferWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);    // 初始化log数量

	CheckSpiFlashWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);

	#ifdef DEBUG
	printf(" ### TotalLogNums: %d\r\n", BlindZoneData.TotalLogNums);
	#endif
	BlindZoneData.TotalLogNums ++;
	WriteTotalLogNums(BlindZoneData.TotalLogNums);	
}


// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇



