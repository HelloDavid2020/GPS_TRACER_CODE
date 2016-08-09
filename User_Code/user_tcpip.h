#ifndef __USER_TCPIP_H
#define __USER_TCPIP_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "includes.h"
	
	
typedef struct
{
	BOOL LinkStatus; // 0:无法链接服务器 1： 链接到服务器
	
	
	uint8_t TxData[256]; // 数据发送缓存
	uint8_t RxData[256]; // 数据接收缓存
	
	uint16_t TxDataLen; // 数据发送长度
	uint16_t RxDataLen; // 数据发送长度
		
	uint8_t	old_serverIP[16];
	uint8_t old_serverPort[6];

	uint8_t	new_serverIP[16];
	uint8_t new_serverPort[6];	 
}TCP_DATA;	

extern TCP_DATA Tcp;
	
extern uint8_t	old_serverIP[16];
extern uint8_t 	old_serverPort[6];

extern uint8_t	new_serverIP[16];
extern uint8_t 	new_serverPort[6];	 


	
extern int8_t 	Check_ip_port(void);
extern uint8_t 	Check_GPRS_Status(void);	
extern int8_t 	Close_GPRS_tcpip(void);
extern void  		Reestablish_the_connection(void);
	
extern void	Update_tcpip_Data(void);
extern void Update_SMS_Data(void);	

extern	void RemoteCommBreakdownHandler(void);	


extern void ReloadGprsData(TCP_DATA * tcp);
extern int8_t SendGprsDataPackage(TCP_DATA * tcp);
extern void SaveGprsDataToFlash(TCP_DATA * tcp);


	
#ifdef __cplusplus
}
#endif 

#endif

