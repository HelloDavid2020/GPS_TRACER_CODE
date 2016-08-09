#ifndef __USER_BLINDZONE_H
#define __USER_BLINDZONE_H

#include "includes.h"

#define MAX_LOG_NUMS  512

typedef struct
{	
	uint8_t TxCount;
	BOOL		TxFlag;
	
	uint8_t Writebuf[256];
	uint8_t Readbuf[256];
	uint8_t DataLen; // 

	uint32_t LogNum;     // ��ǰlog num
	uint32_t SectorNum;  // ��ǰ����
	uint32_t SectorAddr; // ��ǰ������ַ
	uint32_t TotalLogNums; // eeprom�д洢���ܵ�lognums��
	uint32_t wTotalLogNums; // eeprom�д洢���ܵ�lognums��
	uint32_t rTotalLogNums; // eeprom�д洢���ܵ�lognums��
}BLIND_ZONE_DATA;

extern BLIND_ZONE_DATA BlindZoneData;



//extern uint8_t BlindZoneData[256];

extern uint32_t	FlashTotalLogNums;
extern uint32_t FlashCurrentLogNums;
extern uint16_t	FlashCurrentSector;
extern uint16_t EEPROMCurrentLogNums;

extern uint32_t WriteErrCount;

extern uint32_t ReadTotalLogNums(void);
extern void WriteTotalLogNums(uint32_t TotalNums);

extern uint16_t GetEEPROMCurrentLogNums(void);
extern void SetEEPROMCurrentLogNums(uint16_t currentlognum);

extern void SaveTheBlindZoneDataToEEPROM(void);

extern void SaveTheBlindZoneDataToFlash(void);

extern void BlindZoneDataHandler(void);
extern void SpiFlashTest(void);
extern int8_t  CheckSpiFlashWrite(uint8_t* writebuf, uint32_t writeAddr, uint16_t NumByteToWrite);








#endif

