#include "iap.h"

uint8_t CloseTcpip[12]="CloseTcpip.";    // 0x4F 0x4B 0x00 

uint8_t Response_Ok[3]="OK";    // 0x4F 0x4B 0x00 
uint8_t Response_Error[6]="ERROR"; // 0x45 0x52 0x00
uint8_t Response_Ack[2] ={'C',0}; // 0x45 0x52 0x00


FTP_SERVER_DOWNLOAD FtpDownload;

pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t FlashProtection = 0;

uint16_t PageSize = PAGE_SIZE;
uint32_t EraseCounter = 0x0;
uint32_t NbrOfPage = 0;
FLASH_Status FLASHStatus = FLASH_COMPLETE;
uint32_t RamSource= 0;

uint32_t FlashDestination = ApplicationAddress;


extern OS_TID t_GPRS;
extern OS_TID t_Monitor;
extern OS_TID t_GPS;
extern OS_TID t_CanMsg;
extern OS_TID t_Modbus;
extern OS_TID t_TheBlindZone;


DATA_BOOTLOADER bootloader;
REMOTE_MSG RemoteMsg;
FTP  Ftp;

uint8_t WriteBootloaderData(DATA_BOOTLOADER * bootloader)
{
	uint32_t writeData = 0;	
	uint32_t readData  = 0;	


	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	
	writeData = bootloader->UpdateFlag;
	
	switch(bootloader->JumpAddress)
	{
		case ADDR_APP1:
			bootloader->JumpApp = APP1;
			break;
		case ADDR_APP2:
			bootloader->JumpApp = APP2;
			break;
		default :
			bootloader->JumpApp = APP1;
			break;
	}	
	
	writeData = (writeData << 8) + bootloader->JumpApp;
	
	IntDisable();
	FLASH_Unlock();
	
	FLASH_ErasePage(ADDR_BOOT_DATA);//������Ӧ��ַ��1ҳ(2K)
	
	FLASH_ProgramWord(ADDR_BOOT_DATA, writeData);
	FLASH_Lock();
	IntEnable();
	readData = (*(u32 *)(ADDR_BOOT_DATA));
	if(readData == writeData)
	{
		 return 1;
	}
	else	 //
	{
		 return 0;
	}
}



void ReadBootloaderData(DATA_BOOTLOADER * bootloader)
{
	uint32_t data = 0;
	data = (*(u32 *)(ADDR_BOOT_DATA));
	
	bootloader->UpdateFlag = (data>> 8) & 0xFF;
	bootloader->JumpApp    = data & 0xFF;
	switch(bootloader->JumpApp)
	{
		case APP1:
			bootloader->JumpAddress = ADDR_APP1;
			break;
		case APP2:
			bootloader->JumpAddress = ADDR_APP2;
			break;
		default :
			bootloader->JumpAddress = ADDR_APP1;
			break;
	}

}

void IAP_Reset(uint32_t startAddr)
{
		JumpAddress = *(__IO uint32_t*) (startAddr + 4);

		//��ת���û�����
		Jump_To_Application = (pFunction) JumpAddress;
		//��ʼ���û�����Ķ�ջָ��
		__set_MSP(*(__IO uint32_t*) ApplicationAddress);
		Jump_To_Application();
}

// Զ������ ����ͨ�Žӿ�
int8_t IapRemoteInterface(uint8_t  * data)
{
	uint8_t i = 0;
	int8_t result = 0;

	if(data[0] != 0x7E) 
		return 0;
	
	RemoteMsg.Header  = data[0];
	RemoteMsg.SId     = data[1];
	RemoteMsg.DataLen = data[2];

	// ������, �����ݳ���  RemoteMsg.DataLen-3
	for(i=0; i < (RemoteMsg.DataLen-3); i++)
	{
		RemoteMsg.Data[i] += data[i+3];
	}	
	
	RemoteMsg.Checksum = RemoteMsg.Header + RemoteMsg.SId + RemoteMsg.DataLen;
	
	for(i=0; i < (RemoteMsg.DataLen-3); i++)
	{
		RemoteMsg.Checksum+= data[i+3];
	}
	
	// У����ȷ
	if(RemoteMsg.Checksum == data[RemoteMsg.DataLen])
	{
		if( TcpSendData(Response_Ok, sizeof(Response_Ok)) )						
			os_dly_wait(50);  // 10MS *50 = 500 mS
			switch(RemoteMsg.SId)  // Service Id
			{
				case UPDATE_COMMAND : 

				USART_ITConfig(USART2, USART_IT_TXE | USART_IT_RXNE, DISABLE);
				DMA_Cmd(DMA1_Channel6, DISABLE);	
				CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
				TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
				TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
				RTC_ITConfig(RTC_IT_SEC, DISABLE);	
				
//				os_tsk_delete(t_Monitor);
//				os_tsk_delete(t_GPS);
//				os_tsk_delete(t_CanMsg);
//				os_tsk_delete(t_Modbus);
//				os_tsk_delete(t_TheBlindZone);				
				
//				
//					ReadBootloaderData(&bootloader);

//					if(bootloader.JumpApp == APP1)
//						bootloader.JumpApp = APP2;
//					else
//						bootloader.JumpApp = APP1;
					bootloader.JumpApp = APP2;
					bootloader.UpdateFlag = 'Y';	
					WriteBootloaderData(&bootloader);
					
				FLASHStatus = EraseSectors();
//				os_dly_wait(200);

				if(TcpSendData(CloseTcpip, sizeof(CloseTcpip)))
				{
					os_dly_wait(100);  // 10MS *50 = 500 mS
					
					if(GPRS_TCPIP_DisConnect() == AT_OK)  // �Ͽ�TCP����
					{
						os_dly_wait(100);  // 10MS *50 = 500 mS
						bootloader.FtpFlag = ENABLE; // ��¼������ʹ��		
					}
				}					
					

					result = 1;
					break;
					
				case START_COMMAND : 
					TcpSendData(Response_Ack, 1);
					result = 1;
					break;	
				
				default : 
					result = 2; // unkown id 
					break;
			}
	}
	else
	{					
		TcpSendData(Response_Error, sizeof(Response_Error));
		result = -1;
	}
	
	memset(TCPReceiveBuf,0x00,sizeof(TCPReceiveBuf));
	return result;
}


uint8_t FtpInit(void)
{
// �����λ
	MG323B_RESET_ENABLE;
	os_dly_wait(50);  // 10MS *50 = 500 mS
	MG323B_RESET_DISABLE;
	
//-----------------------------------------------------------------------------		
// ����ҵ���ʼ��
	if(AT_CommandRun("at^sics=0,conType,GPRS0\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;	
	os_dly_wait(100);
	
	if(AT_CommandRun("at^sics=0,apn,cmnet\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;	
	os_dly_wait(100);
	
	if(AT_CommandRun("at^sics=0,inactTO,20\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(100);
	
	if(AT_CommandRun("AT+CGATT?\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;	
	os_dly_wait(100);
	
	if(AT_CommandRun("AT^SISS=1,srvType,Ftp\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;		
	os_dly_wait(100);
	
	if(AT_CommandRun("AT^SISS=1,conId,0\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;			
	os_dly_wait(100);
	
	if(AT_CommandRun("AT^SISS=1,address,\"ftp://test:test@115.47.44.207:21\"\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;	
	os_dly_wait(100);

//-----------------------------------------------------------------------------		
// ʹ��16������ת�����������ջ���
	if(AT_CommandRun("AT^IOMODE=1,0\r\n", "OK", 2,1000)==AT_ERROR)	// bin�ļ����أ� ʹ�û���

		return AT_ERROR;	
	os_dly_wait(100);
	
// �����ʽ����  AT^IPCFL=14,1
	if(AT_CommandRun("AT^IPCFL=14,1\r\n", "OK", 2,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(100);
	
// AT^IPCFL=5,50
	if(AT_CommandRun("AT^IPCFL=5,50\r\n", "OK", 2,1000)==AT_ERROR)
		return AT_ERROR;
	os_dly_wait(100);	
	
	if(AT_CommandRun("ATE0\r\n", "OK", 5,1000)==AT_ERROR)
		return AT_ERROR;		
	
//-----------------------------------------------------------------------------		
// ��ѯ��ʼ��״̬	
	AT_CommandSend(UART3SendBuf, "AT^SISS?\r\n");	
	os_dly_wait(300);
	return AT_OK;
}


// ��¼FTP
uint8_t FtpLogIn(void)
{
//	int32_t srvProfileId = 0;
//	int32_t dataLength = 0;

/*
OK
^SISW: 1,1,1460
^SIS: 1,4,230,User logged in, proceed.	
*/
// ��¼������
	AT_CommandSend(UART3SendBuf, "AT^SISO=1\r\n");//// ��Ӧ��ʱ�ȽϿ� ��ʱ�Ƚ���	
	os_dly_wait(500);
	
	AT_CommandRecv(UART3RecvBuf, 1000); // ��Ҫȥ��

	if(AT_CommandComp(ATRecvBuf,"^SIS: 1,4,230,User logged in, proceed."))		    //AT��Ӧ����
		return AT_OK;		
//	
	printf("> Ftp log in Error. >>\r\n");
	return AT_ERROR;	
	
}


// �˳�FTP
uint8_t FtpLogOut(void)
{
	if(AT_CommandRun("AT^SISC=1\r\n", "OK", 1,1000)==AT_ERROR)
		return AT_ERROR;
	
	return AT_OK;
}


void FtpHandler(void)
{
	Ftp.Status = FtpInit();
			
	if(Ftp.Status == AT_OK)
	{
		if(FtpLogIn() == AT_OK)
		{					
			Ftp.bufferRecFlag = ENABLE;
//					Ftp.ToggleFlag =1;
			AT_CommandSend(UART3SendBuf, "AT^FTPCMD=1,GET,\"GPS.bin\"\r\n");
			os_dly_wait(200);
//					Ftp.FlashEnable = ENABLE;
//					printf("> Write Flash Enable.\r\n");
		}
		Ftp.InitFlag = 1;
	}

}



void ReceiveFtpData(uint8_t ftpData)
{
	Ftp.Data[Ftp.recLen] = ftpData;
//	
//	if(Ftp.Data[Ftp.recLen] == 0X0D || Ftp.Data[Ftp.recLen] == 0X0A)
//	{
//		Ftp.recLen = 0;
//		return;
//	}
	Ftp.recLen ++;	
}
// FindLoggingMsgHeader
void ReceiveDownloadInfo(uint8_t ftpData)
{
	Ftp.Data[Ftp.recLen] = ftpData;
	
	if(Ftp.Data[Ftp.recLen] == 0X0D || Ftp.Data[Ftp.recLen] == 0X0A)
	{
		Ftp.recLen = 0;
		return;
	}
	
	if(Ftp.Data[Ftp.recLen] == 0X0D || Ftp.Data[Ftp.recLen] == 0X0A)
	{
		Ftp.bufferRecFlag = DISABLE;
		Ftp.ReceiveCompleteFlag = DONE;
//		LED_BLUE_OFF;
	}
	Ftp.recLen ++;	
		

}








uint8_t CheckFlashBlank(void)
{
    BlockNbr = (FlashDestination - 0x08000000) >> 12;
 
#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
    UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
#else /* USE_STM3210E_EVAL */
    if (BlockNbr < 62)
    {
        UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
    }
    else
    {
        UserMemoryMask = ((uint32_t)0x80000000);
    }
#endif /* (STM32F10X_MD) || (STM32F10X_MD_VL) */

    if ((FLASH_GetWriteProtectionOptionByte() & UserMemoryMask) != UserMemoryMask)
    {
        FlashProtection = 1;
    }
    else
    {
        FlashProtection = 0;
    }
		
		return FlashProtection;

}


/*******************************************************************************
  * @��������	FLASH_PagesMask
  * @����˵��   ����Falshҳ
  * @�������   Size:�ļ�����
  * @�������   ��
  * @���ز���   ҳ������
*******************************************************************************/
uint32_t FLASH_PagesMask(__IO uint32_t Size)
{
    uint32_t pagenumber = 0x0;
    uint32_t size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;

}

/*******************************************************************************
  * @��������	FLASH_DisableWriteProtectionPages
  * @����˵��   �Ӵ�Flashд����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void FLASH_DisableWriteProtectionPages(void)
{
    uint32_t useroptionbyte = 0, WRPR = 0;
    uint16_t var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
    FLASH_Status status = FLASH_BUSY;

    WRPR = FLASH_GetWriteProtectionOptionByte();

    //�����Ƿ�д����
    if ((WRPR & UserMemoryMask) != UserMemoryMask)
    {
        useroptionbyte = FLASH_GetUserOptionByte();

        UserMemoryMask |= WRPR;

        status = FLASH_EraseOptionBytes();

        if (UserMemoryMask != 0xFFFFFFFF)
        {
            status = FLASH_EnableWriteProtection((uint32_t)~UserMemoryMask);
        }
        //�ô�ѡ�����Ƿ��б��
        if ((useroptionbyte & 0x07) != 0x07)
        {
            //���±���ѡ����
            if ((useroptionbyte & 0x01) == 0x0)
            {
                var1 = OB_IWDG_HW;
            }
            if ((useroptionbyte & 0x02) == 0x0)
            {
                var2 = OB_STOP_RST;
            }
            if ((useroptionbyte & 0x04) == 0x0)
            {
                var3 = OB_STDBY_RST;
            }

            FLASH_UserOptionByteConfig(var1, var2, var3);
        }

        if (status == FLASH_COMPLETE)
        {
            printf("Write Protection disabled...\r\n");

            printf("...and a System Reset will be generated to re-load the new option bytes\r\n");
            //ϵͳ��λ���¼���ѡ����
            NVIC_SystemReset();
        }
        else
        {
            printf("Error: Flash write unprotection failed...\r\n");
        }
    }
    else
    {
        printf("Flash memory not write protected\r\n");
    }
}

FLASH_Status  EraseSectors(void)
{
	FLASH_Status FLASHStatus  = FLASH_COMPLETE;
	uint32_t time = 0;
	
	Ftp.DataLength = 204800;
	FlashDestination = ApplicationAddress; // APP���µ�ַ�����������ַ
	if(CheckFlashBlank() == FLASH_PROTECTION_DISABLE) // ��ֹд����
	{		
		FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		NbrOfPage = FLASH_PagesMask(Ftp.DataLength);	//������Ҫ����Flash��ҳ	

		printf("\r\n\r\n\r\n> ###### Erase Flash start. ######\r\n");
		debugTimeCount = 0;
		//����Flash
		FLASH_Unlock();	
		for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
		{
			FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
		}	
		FLASH_Lock();	
		time = debugTimeCount;
		printf("> ###### Erase Flash complete. Take time: %d mS. ######\r\n\r\n\r\n",time);

	}
	else
	{
		FLASHStatus = FLASH_ERROR_WRP;
	}
	return FLASHStatus;
}


void FlashTest(void)
{
	uint32_t i,j;
//	for(i = 0;i<204800;i++)
//	Ftp.Data[i] = i;
	

ENTER_CRITICAL_SECTION();
	FLASHStatus = EraseSectors();
EXIT_CRITICAL_SECTION();			
//	RamSource = (uint32_t)Ftp.Data;
//	for (j = 0; (j < Ftp.DataLength) && (FlashDestination <  (ApplicationAddress + 102400)); j += 4)
//	{
//		//�ѽ��յ������ݱ�д��Flash��
//		FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);

//		if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)
//		{
//			printf("> Error!\r\n");
//		}
//		FlashDestination += 4;
//		RamSource += 4;
//	}			

}

uint32_t GetFirmwareDataSize(uint8_t *buff)
{
	int a,b,c;
	
	// ���ݴ���1000 bytes ���� 1000 bytes �����bug
// ^SIS: 1,4,226,Transfer complete. 248 bytes transferred. 0.24 KB/sec.			a= 248,b=0
// ^SIS: 1,4,226,Transfer complete. 90,692 bytes transferred. 1,884.39 KB/sec. 		a= 90,b=692	
	sscanf((char *)buff, "complete. %d,%d", &a,&b);
	c= a*1000+b;
	return c;
}


// 			Ftp.FirmwareDataPaketNum = 

uint32_t GetFirmwareDataPaketNum(uint32_t FirmwareDataSize)
{

	uint32_t FirmwareDataPaketNum = 0x0;

    if ((FirmwareDataSize % PACKET_1K_SIZE) != 0)
    {
        FirmwareDataPaketNum = (FirmwareDataSize / PACKET_1K_SIZE) + 1;
    }
    else
    {
        FirmwareDataPaketNum = FirmwareDataSize / PACKET_1K_SIZE;
    }
    return FirmwareDataPaketNum;

}



int8_t DownloadInfo_Receive(uint16_t timeout)
{
	uint16_t i = 0;
	uint16_t timeoutCount = 0;
	
	while(strstr((void *)Ftp.Data,"^SIS: 1,4,226,Transfer complete.") == NULL)
	{
		timeoutCount ++;
		os_dly_wait(100); 
		
		if( timeoutCount > timeout) 
		{
			timeoutCount = 0;
			printf("> *** warning: Failed Download. ***\r\n");
			Ftp.FirmwareDataSize = 0;
			Ftp.FirmwareDataPaketNum = 0;
			return -1;
		}
	}
	
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
	printf("> Success download.\r\n");
	Ftp.DataIndex = FindStr((void *)Ftp.Data,"complete.");
	Ftp.DataIndex --; // ָ���±��һ, point to 'c'


	for(i = 0; (Ftp.Data[Ftp.DataIndex+i] != 'b'); i++ )
	Ftp.Data[i] = Ftp.Data[Ftp.DataIndex+i];

	Ftp.FirmwareDataSize = GetFirmwareDataSize(Ftp.Data);
	Ftp.FirmwareDataPaketNum = GetFirmwareDataPaketNum(Ftp.FirmwareDataSize);
	printf("> FirmwareDataSize: %d bytes, FirmwareDataPaketNum: %d\r\n",Ftp.FirmwareDataSize,Ftp.FirmwareDataPaketNum );
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n\r\n\r\n");
	
	return 1;	
}

void GetOnePacketData(void)
{
	FtpDownload.RecFlag = ENABLE;
	FtpDownload.Index = 0;
	FtpDownload.RecLen = 0;
	memset(FtpDownload.Data,0x00,sizeof(FtpDownload.Data));
	AT_CommandSend(UART3SendBuf, "AT^SISR=0,1024\r\n");
	os_dly_wait(10);
}


/*******************************************************************************
  * @��������	Receive_Packet
  * @����˵��   �ӷ��Ͷ˽���һ�����ݰ�
  * @�������   data ������ָ��
                length������
                timeout ����ʱʱ��
  * @�������   ��
  * @���ز���   ���յĽ��
                0: ��������
                -1: ��ʱ�������ݰ�����
                1: �û�ȡ��
*******************************************************************************/
int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
	uint16_t i, packet_size,index;
	uint8_t c;
	int a, datalen;
	uint32_t RamSource = 0;
// --------------------------------------------------------------------------------------	
	// ���ҿ�ͷ
	if(strstr((void *)data,"^SISR:") == NULL)
	{
		printf("\r\n> *** warning: No Finding a <HEADER>. ***\r\n");
		return ERROR;
	}

// ���ҽ�β, ���� '\0' �ͽ����ˡ��������޷����ҵ�OK
//	if(strstr((void *)data,"OK") == NULL)
//	{
//		printf("> *** warning: No Finding an <END>. ***\r\n");
//		return ERROR;
//	}
// --------------------------------------------------------------------------------------	

	
	index = FindStr((void *)data,"^SISR:");
	index --;
	if(index != 0)
	{
		for(i=0; i< (*length); i++)
		data[i] = data[index+i];
	}
	
	sscanf((char *)data, "^SISR: %d,%d", &a,&datalen);
	if(datalen >1024) 
	{
		datalen =0;
		printf("\r\n## Receive data erroe. Data size > 1024 bytes.##\r\n");
		return -1;

	}
	printf("\r\n> Packet data length: %4d bytes.\r\n\r\n",datalen);
// --------------------------------------------------------------------------------------	
	FtpDownload.DataLenSum += datalen;
	index = FindStr((void *)data,"\r\n");
	index ++;
	if(index != 0)
	{
		for(i=0; i< datalen; i++)
		data[i] = data[index+i];
	}	
	IntDisable();
	FLASH_Unlock();
	RamSource = (uint32_t)data;
	for (i = 0; (i < datalen) && (FlashDestination <  ApplicationAddress + APP_DATA_SIZE); i += 4)
	{
	//�ѽ��յ������ݱ�д��Flash��
		FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);

		if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)
		{
			return -2;
		}
		FlashDestination += 4;
		RamSource += 4;
	}	
	FLASH_Lock();
	IntEnable();


    return 0;
}



// �̼����ؽ���
int32_t Firmware_Receive (void)
{
	static uint32_t tempdata;
	
	FlashDestination = ApplicationAddress;  //��ʼ��Flash��ַ����
	tempdata = Ftp.FirmwareDataPaketNum;
	
	while(Ftp.FirmwareDataPaketNum != 0)
	{
		printf("\r\n\r\n\r\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
		printf("> Receive PacketNum: # %03d" ,++FtpDownload.NumofRecPackets);
		GetOnePacketData();
		os_dly_wait(200); // �ȴ�2��
		Receive_Packet(FtpDownload.Data, &FtpDownload.RecLen ,0);
		Ftp.FirmwareDataPaketNum --;
		
		if(Ftp.FirmwareDataPaketNum == 0)
			if(FtpDownload.NumofRecPackets == tempdata)
				if(FtpDownload.DataLenSum == Ftp.FirmwareDataSize)
				{
					printf("\r\n\r\n\r\n> @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \r\n");
					printf("> Download data size: %d bytes.\r\n",FtpDownload.DataLenSum);
					printf("> Device Firmware Upgrade Success!\r\n");
					printf("> @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \r\n");
					os_dly_wait(100);
					NVIC_SystemReset();
				}
	}

}

