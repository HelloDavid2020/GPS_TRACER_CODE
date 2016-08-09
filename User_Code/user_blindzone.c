#include "user_blindzone.h"

//uint8_t BlindZoneData[256] = {0};


BLIND_ZONE_DATA BlindZoneData;


uint32_t	FlashTotalLogNums = 0;  // 全部的 lognums

uint32_t 	FlashCurrentLogNums = 0;
uint16_t  FlashCurrentSector = 0;
uint16_t  EEPROMCurrentLogNums = 0;


uint32_t WriteErrCount = 0;
// 	FlashCurrentLogNums = GetLogNums();


uint32_t ReadTotalLogNums(void)
{
	uint8_t lognum[4] = {0};
	uint32_t tempTotalNums[4] = {0};
	uint32_t TotalNums = 0;

	ENTER_CRITICAL_SECTION();
	Fram_BufferRead(lognum, ADDR_TOTAL_LOG_NUMS*2, 4);
	EXIT_CRITICAL_SECTION();
//	TotalNums = (tempTotalNums[0] << 24) + (tempTotalNums[1] << 16) + (tempTotalNums[2] << 8) + tempTotalNums[3];
	DelayMS(1);

	tempTotalNums[0] = lognum[0];
	tempTotalNums[1] = lognum[1];
	tempTotalNums[2] = lognum[2];
	tempTotalNums[3] = lognum[3];
	TotalNums = (tempTotalNums[0] << 24) + (tempTotalNums[1] << 16) + (tempTotalNums[2] << 8) + tempTotalNums[3];

	return TotalNums;
}

void WriteTotalLogNums(uint32_t TotalNums)
{
	uint8_t lognum[4] = {0};
	
	lognum[0] = (TotalNums >> 24) & 0xFF;
	lognum[1] = (TotalNums >> 16) & 0xFF;
	lognum[2] = (TotalNums >>  8) & 0xFF;
	lognum[3] = (TotalNums      ) & 0xFF;

	ENTER_CRITICAL_SECTION();
	Fram_BufferWrite(lognum, ADDR_TOTAL_LOG_NUMS*2, 4);
	EXIT_CRITICAL_SECTION();
	DelayMS(1);

}

//uint16_t GetEEPROMCurrentLogNums(void)
//{
//	uint8_t lognum[2] = {0};
//	uint16_t tempTotalNums[2] = {0};
//	uint16_t currentlognum = 0;
//	Fram_BufferRead(lognum, ADDR_BLIND_DATA_EEPROM_LOG_NUMBERS*2, 2);
//	
//	tempTotalNums[0] = lognum[0];
//	tempTotalNums[1] = lognum[1];


//	currentlognum = (tempTotalNums[0] << 8) + tempTotalNums[1];

//	return currentlognum;

//}



//// save data to flash
//void SaveTheBlindZoneDataToFlash(void)
//{
////	BlindZoneData.TotalLogNums = ReadTotalLogNums();
//	
//	// 512 总共1024 *4K容量，即1024个扇区
//	// 0-511
//	BlindZoneData.LogNum = BlindZoneData.TotalLogNums % MAX_LOG_NUMS; // 数据最大存储条数

//	// 获取当前扇区号
//	BlindZoneData.SectorNum = BlindZoneData.LogNum / 16;
//	
//	// 若当前扇区为开始的第一条数据写入， 则擦除整个扇区。
//	if((BlindZoneData.LogNum%16) == 0)
//	{
//		SPI_FLASH_SectorErase(BlindZoneData.SectorNum);
//	}
//	
//	SPI_FLASH_BufferWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);    // 初始化log数量

//	BlindZoneData.TotalLogNums  ++;

////	BlindZoneData.TotalLogNums  ++;
////	WriteTotalLogNums(BlindZoneData.TotalLogNums);		
//	
//	
//}



//// 盲区数据存储
//void BlindZoneDataHandler(void)
//{
//	uint8_t  	tempTCPSendBuf[256] = {0};
//	uint8_t  	tempTCPSend_Num = 0;
//	
//	if (TCPSend_Num != 0) 
//	{
//		memcpy(tempTCPSendBuf,TCPSendBuf,TCPSend_Num);
//		
//		tempTCPSendBuf[TCPSend_Num] = 0;
//		tempTCPSend_Num = TCPSend_Num;
//		TCPSend_Num = 0;


//		
//		EEPROMCurrentLogNums = GetEEPROMCurrentLogNums();
////		EEPROMCurrentLogNums ++;
//		
//		EEPROMCurrentLogNums = EEPROMCurrentLogNums > 15 ? 0 : EEPROMCurrentLogNums; 
//		
//		
//		
//		
//		FlashTotalLogNums = FlashTotalLogNums > 0x7FFFFFFF ? 16 : FlashTotalLogNums;		
//		FlashTotalLogNums = FlashTotalLogNums < 1  ? 1 : FlashTotalLogNums;	

//		// 2000条数据存储 1-2000
//		FlashCurrentLogNums = FlashTotalLogNums % 2000;
//		
////		StoreLogNums(FlashCurrentLogNums);
//		
//		FlashCurrentSector = FlashCurrentLogNums >> 4;
//		
//// 		sector = (FlashCurrentLogNums)/16 ;
//		if(FlashCurrentLogNums%16 == 0) // 0,16,32,48,...
//		{
//// 			FlashCurrentSector = FlashCurrentSector > 128 ? 0 : FlashCurrentSector;		
//			SPI_FLASH_SectorErase(FlashCurrentSector);
//		}
//		
//		SPI_FLASH_BufferWrite(tempTCPSendBuf, FlashCurrentLogNums * FLASH_PAGE_SIZE, tempTCPSend_Num);    // 初始化log数量

//	}
//}

//void SpiFlashTest(void)
//{
//	static uint32_t ErrCount =0;
//	uint16_t num =0;
//	uint16_t i =0;		

//	for(num = 0; num<256; num++)
//		BlindZoneData.Writebuf[num] = rand(); // 随机数
//		
//	BlindZoneData.TotalLogNums = ReadTotalLogNums();
//	BlindZoneData.LogNum = BlindZoneData.TotalLogNums % MAX_LOG_NUMS;

//	
//	BlindZoneData.Writebuf[0] = ExRtcTime.sec;
//	BlindZoneData.Writebuf[1] = ExRtcTime.min;
//	BlindZoneData.Writebuf[2] = ExRtcTime.hour;
//	
//	
//	ENTER_CRITICAL_SECTION();
//	FlashBufferWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);    // 初始化log数量
//	EXIT_CRITICAL_SECTION();

//	printf("\r\nwritebuf---------------------------------------------\r\n");
//	
//	for(i =0; i <256; i++)
//	{
//		printf("0x%02X ",BlindZoneData.Writebuf[i]);
//	}
//	printf("\r\n-----------------------------------------------------\r\n");
//	os_dly_wait(2);
//	
//	memset(BlindZoneData.Readbuf,0x00,256);
//	ENTER_CRITICAL_SECTION();
//	FlashBufferRead (BlindZoneData.Readbuf,  (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);    // 初始化log数量
//	EXIT_CRITICAL_SECTION();
//	
//	printf("\r\nreadbuf----------------------------------------------\r\n");
//	for(i =0; i <256; i++)
//	{
//		printf("0x%02X ",BlindZoneData.Readbuf[i]);
//	}
//	printf("\r\n-----------------------------------------------------\r\n");
//	os_dly_wait(2);

//	if(Buffercmp(BlindZoneData.Writebuf, BlindZoneData.Readbuf, 256) != PASSED)
//		printf("  *** Error: %d  *** \r\n", ++ ErrCount);
//	
//	printf(" ### TotalLogNums: %d  ###\r\n", BlindZoneData.TotalLogNums);
//	
//	BlindZoneData.TotalLogNums ++;
//	WriteTotalLogNums(BlindZoneData.TotalLogNums);		

//}



int8_t  CheckSpiFlashWrite (uint8_t* writebuf, uint32_t writeAddr, uint16_t NumByteToWrite)
{
	int8_t result = 0;
	uint8_t readbuf[256] = {0};	
	uint16_t i =0;

	#ifdef DEBUG
	printf("\r\nwritebuf---------------------------------------------\r\n");
	for(i =0; i <256; i++)
	{
		printf("0x%02X ",*(writebuf+i));
	}
	printf("\r\n-----------------------------------------------------\r\n");
	os_dly_wait(2);
	#endif
	
	NumByteToWrite = 256;
	memset(BlindZoneData.Readbuf,0x00,NumByteToWrite);

//	ENTER_CRITICAL_SECTION(); 
	FlashBufferRead (readbuf, writeAddr, NumByteToWrite);    
//	EXIT_CRITICAL_SECTION();
	
	#ifdef DEBUG
	printf("\r\nreadbuf----------------------------------------------\r\n");
	for(i =0; i <256; i++)
	{
		printf("0x%02X ",readbuf[i]);
	}
	printf("\r\n-----------------------------------------------------\r\n");
	os_dly_wait(2);	
	#endif
	
	if(Buffercmp(writebuf, readbuf, NumByteToWrite) != PASSED)
	{
		result = FAILED;
		#ifdef DEBUG
		printf(" ****** Error: %d  ****** \r\n", ++ WriteErrCount);
		#endif
	}
	else
		result = PASSED;
	
	return result;
}

void SpiFlashTest(void)
{
	static uint32_t ErrCount =0;
	uint16_t num =0;
	uint16_t i =0;		

	for(num = 0; num<256; num++)
		BlindZoneData.Writebuf[num] = rand(); // 随机数
		
	BlindZoneData.TotalLogNums = ReadTotalLogNums();
	BlindZoneData.LogNum = BlindZoneData.TotalLogNums % MAX_LOG_NUMS;

	
	BlindZoneData.Writebuf[0] = ExRtcTime.sec;
	BlindZoneData.Writebuf[1] = ExRtcTime.min;
	BlindZoneData.Writebuf[2] = ExRtcTime.hour;
	
	
	ENTER_CRITICAL_SECTION();
	FlashBufferWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);    // 初始化log数量
	EXIT_CRITICAL_SECTION();


	

	CheckSpiFlashWrite(BlindZoneData.Writebuf, (BlindZoneData.LogNum * FLASH_PAGE_SIZE), 256);

	#ifdef DEBUG
	printf(" ### TotalLogNums: %d\r\n", BlindZoneData.TotalLogNums);
	#endif
	BlindZoneData.TotalLogNums ++;
	WriteTotalLogNums(BlindZoneData.TotalLogNums);		

}



