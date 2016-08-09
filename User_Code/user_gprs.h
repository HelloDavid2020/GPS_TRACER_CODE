#ifndef __USER_GPRS_H
#define __USER_GPRS_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "includes.h"
	
#define AT_NO_SIM_CARD  10
#define AT_PIN_LOCK			11	
	
#define LINK_STATUS_UNKNOWN		0x00
	
#define LINK_STATUS_TCP				0x01
#define UNABLE_TO_CONNECT   	0x06
	
#define LINK_STATUS_SMS				0x02
#define LINK_STATUS_BLIND     0x03
#define LINK_STATUS_ERROR			0x04
#define LINK_STATUS_NEWLINK		0x05	

#define ERR_GPRS_INIT				0x20
#define ERR_SIM_CARD				0x21
#define ERR_TCP_CONFIG			0x22
#define ERR_TCP_CONNECT			0x23
#define ERR_TCP_SEND				0x24
	

typedef struct
{
	uint8_t		InitFlag; // 初始化状态
	uint8_t   FirstSendFlag;
	uint8_t		InitStatus;
	uint8_t 	TcpInitStatus;  // tcpip 配置
	uint8_t 	LinkStatus; 		// gprs 链接状态

	uint8_t		SIMCardStatus;
	
	uint8_t		FlagTx; 				// gprs 发送标志
	uint8_t		TxDataLen; 			// gprs 发送长度
	uint8_t		TxData[256]; 		// gprs 发送缓存

	uint8_t		FlagRx; 				// gprs 接收标志
	uint8_t		RxDataLen; 			// gprs 发送缓存
	uint8_t 	RxData[256]; 		// gprs 接收缓存
	
	uint8_t		FlagTxTcpData; 				// gprs 数据发送标志
	uint8_t		FlagTxHeartBeat; 			// gprs 心跳包发送标志
	uint8_t 	Lbs[4]; 				// 基站信息
	
	uint8_t 	IpAddr[16]; 				// ip address
	uint8_t 	PortAddr[6]; 			// port address
	
	
	uint16_t 	SigalStrength;  // gprs 信号强度
	uint16_t  LinkTimeOut;    // 链接超时
	uint16_t  RetriesTimes;   // 链接次数
	
	uint16_t  WaitTime;	
	uint16_t  WaitTimeOut;	
	
	uint16_t  UploadTimeCount;
	uint16_t  HeartBeatTimeCount;
	
	uint16_t  UploadInterval;     // 接收时间间隔
	uint16_t  HeartBeatInterval;	 // 发送时间间隔	
	
}GPRS_DATA;	

extern GPRS_DATA Gprs;

typedef struct 
{
	char TxMsg[140];
	char RxMsg[140];

	uint8_t Length;


}SMS_DATA;

extern SMS_DATA SMS;


extern BOOL GprsTest;

extern void GetLbs(uint8_t * pLBS);
extern void GetSigalStrengthAndLBS(uint8_t TimeInterval);


extern void TurnOnGprs(void);
extern void TurnOffGprs(void);

extern uint8_t SIMCardCheck(void);

extern uint8_t TcpSend(uint8_t * Buffer,uint16_t BufferLen);
extern void TcpDataHandler(void);
extern uint8_t TcpSendData(uint8_t * Buffer,uint16_t BufferLen);
extern void TcpDataUpload(void);

extern void TcpKeepAlive(void);

extern void ModbusTCPPool( void );

extern uint8_t CheckTheNetworkConnection(void);

extern void Connect2TcpServer(void);
extern void GprsErrorModeHandler(void);






#ifdef __cplusplus
}
#endif 

#endif

