#ifndef _IAP_H
#define _IAP_H

#include "includes.h"

#ifndef FLASH_PROTECTION_ENABLE
#define FLASH_PROTECTION_ENABLE 1
#endif

#ifndef FLASH_PROTECTION_DISABLE
#define FLASH_PROTECTION_DISABLE 0
#endif



#define PACKET_HEADER           (17)  // ^SISR: 0,1024 ,max size 17
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)




// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
#define APP1   0xA1
#define APP2   0xA2
#define BOOTLOADER 0x0B 

#define ADDR_APP1  0x08005800
#define ADDR_APP2  0x08042C00
// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇


#define APP_BASE_Address    APP_JUMP_ADD	   
#define JUMP_Address        0x08001000 

// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
#define ADDR_BOOT_DATA     0x08005000  
// 跳转地址标志 
// 0xA1 --- 跳转到APP1  
// 0xA2 --- 跳转到APP2 
#define ADDR_JUMP_APP      0x08005000  


// 是否更新标志 
// 'Y' --- 进行更新； 
// 'N' --- 不进行更新
#define ADDR_UPDATE_FLAG    0x08005001  
// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇

/* 类型声明 -----------------------------------------------------------------*/
typedef  void (*pFunction)(void);

//* 宏 ------------------------------------------------------------------------*/
#define CMD_STRING_SIZE       128

#define ApplicationAddress    0x8042C00
#define APP_DATA_SIZE         0x0032000     // 204800 bytes

#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
 #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
 #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
#elif defined STM32F10X_CL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
#elif defined STM32F10X_HD || defined (STM32F10X_HD_VL)
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
#elif defined STM32F10X_XL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x100000) /* 1 MByte */
#else 
 #error "Please select first the STM32 device to be used (in stm32f10x.h)"    
#endif

//计算上传文件大小 
#define FLASH_IMAGE_SIZE                   (uint32_t) (FLASH_SIZE - (ApplicationAddress - 0x08000000))

#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

#define SerialPutString(x) Serial_PutString((uint8_t*)(x))


#define UPDATE        1
#define NOT_UPDATE    0


#define DOWNLOAD_START  0
#define DOWNLOAD_END    1
#define DOWNLAOD_DONE   2


#define UPDATE_HEADER    0x7E
#define UPDATE_COMMAND   0x6F
#define START_COMMAND    0x70

#define CRC16_HIGH_BYTE  0x9E
#define CRC16_LOW_BYTE   0x3A


#define PROTOCOL_MODBUS  0x00





typedef struct
{
	uint8_t  UpdateFlag;
	uint8_t  FtpFlag;
	uint8_t  JumpApp;
  uint32_t JumpAddress;
}DATA_BOOTLOADER;
extern DATA_BOOTLOADER bootloader;


typedef struct
{
	uint8_t  Header;
	uint8_t  SId; // service id
	uint8_t  DataLen;  
	uint8_t  Data[50];  
	uint8_t  Checksum;
}REMOTE_MSG;

extern REMOTE_MSG RemoteMsg;



typedef struct 
{
	BOOL ToggleFlag;
	BOOL InitFlag;
	BOOL FlashEnable; // 编程使能
	uint8_t Status;
	uint8_t EndofData; // 
	BOOL DownloadFlag;
	BOOL bufferRecFlag; 
	BOOL ReceiveCompleteFlag;
	
	uint8_t Data[512];
	uint32_t DataIndex; // 数据指针
	uint32_t DataLength; // 数据长度，从接受缓存中提取出
	uint32_t recLen;     // 接收数据长度，用于接受缓存
	uint32_t FirmwareDataSize;	
	uint32_t FirmwareDataPaketNum; // 数据包数量
 //
	
}FTP;
extern FTP  Ftp;

typedef struct 
{
	BOOL StartFlag;
	BOOL EndFlag;
	
	BOOL RecFlag; // 允许接受标志
	uint8_t  NumofRecPackets; //  the number of received packets
	
	uint8_t Data[1200]; // 数据包缓存

	int32_t RecLen;     // 接收数据长度，用于接受缓存
	int32_t Index; // 数据指针
	int32_t DataLenSum; // 数据总和

	
}FTP_SERVER_DOWNLOAD;
extern FTP_SERVER_DOWNLOAD FtpDownload;


extern uint32_t BlockNbr;
extern uint32_t UserMemoryMask;
extern __IO uint32_t FlashProtection;

extern uint16_t PageSize;
extern uint32_t EraseCounter;
extern uint32_t NbrOfPage;
extern FLASH_Status FLASHStatus;
extern uint32_t RamSource;
extern uint32_t FlashDestination;











extern uint8_t Response_Ok[3];
extern uint8_t Response_Error[6];
extern uint8_t CloseTcpip[12];
extern uint8_t Response_Ack[2];

extern void ReadBootloaderData(DATA_BOOTLOADER * bootloader);
extern uint8_t WriteBootloaderData(DATA_BOOTLOADER * bootloader);
extern void IAP_Reset(uint32_t startAddr);
extern int8_t IapRemoteInterface(uint8_t  * data);
extern uint8_t FtpInit(void);

extern uint8_t FtpLogIn(void);
extern uint8_t FtpLogOut(void);

extern void FtpHandler(void);
extern void ReceiveFtpData(uint8_t ftpData);

extern uint32_t FLASH_PagesMask(__IO uint32_t Size);
extern uint8_t CheckFlashBlank(void);
extern void FLASH_DisableWriteProtectionPages(void);

extern FLASH_Status  EraseSectors(void);
extern void FlashTest(void);
extern uint32_t GetFirmwareDataSize(uint8_t *buff);
extern uint32_t GetFirmwareDataPaketNum(uint32_t FirmwareDataSize);
extern int8_t DownloadInfo_Receive(uint16_t timeout);

extern int32_t Firmware_Receive (void);
extern void GetOnePacketData(void);
extern int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout);

#endif  /* _COMMON_H */

/*******************************文件结束***************************************/



