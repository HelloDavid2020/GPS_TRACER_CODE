#ifndef _GSM_GPRS_H_ 
#define _GSM_GPRS_H_ 

#include "includes.h"

#define  SIM900A       1
#define  MG323         2

#define  AT_OK         0xFF
#define  AT_ERROR      0


#define		RESULT_ERR		0
#define		RESULT_OK			1	
	
#define 	DONE 	1
#define 	NONE  0


#define Set_GSM_ONOFF   GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define Clr_GSM_ONOFF   GPIO_ResetBits(GPIOA, GPIO_Pin_8)

#define Set_GSM_Rst   GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define Clr_GSM_Rst   GPIO_ResetBits(GPIOA, GPIO_Pin_11)

#define TCP_LINK_BUFFER_SIZE 16 // ">TCP-LINK \r\n"

#define GSM_TCP_RECONNECT_TIMEOUT 60u
//#define GSM_HEART_SEND_TIMEOUT 60u
#define GSM_SMS_RECONNECT_TIMEOUT 600
//#define GSM_DATA_SEND_TIMEOUT 30u
#define GSM_SMS_SEND_TIMEOUT 60
#define MODBUS_CLEAR_TIMEOUT 10u

#define GSM_STATUS_UNKNOWN		0x00
#define GSM_STATUS_TCP				0x01
#define GSM_STATUS_SMS				0x02
#define GSM_STATUS_BLIND      0x03
#define GSM_STATUS_ERROR			0x04
#define GSM_STATUS_NEWLINK		0x05


#define MG323B_POWER_ON    		 	GPIO_ResetBits(GPIOE, GPIO_Pin_13)
#define MG323B_POWER_OFF   		 	GPIO_SetBits(GPIOE, GPIO_Pin_13)
 
// Èí¼þ¸´Î»
#define MG323B_RESET_DISABLE		GPIO_ResetBits(GPIOE, GPIO_Pin_14)
#define MG323B_RESET_ENABLE			GPIO_SetBits(GPIOE, GPIO_Pin_14)

#define MG323B_TERM_ON_LOW	 		GPIO_SetBits(GPIOE, GPIO_Pin_15)
#define MG323B_TERM_ON_HIGH	 	  GPIO_ResetBits(GPIOE, GPIO_Pin_15)
 
 
extern uint8_t serverIP[16];
extern uint8_t serverPort[6];
extern uint8_t serverConnect;


extern uint16_t ATRecv_Num;
extern uint8_t  ATRecvBuf[512];
extern uint16_t TCPSend_Num;
extern uint8_t  TCPSendBuf[256];
extern uint16_t TCPReceive_Num;
extern uint8_t  TCPReceiveBuf[256];
extern uint16_t TCPHeart_Num;
extern uint8_t  TCPHeartBuf[TCP_LINK_BUFFER_SIZE];

extern uint16_t heartTimeOut;
extern uint16_t tcpTimeOut;
extern uint16_t smsTimeOut;
extern uint32_t tcpDataTimeOut;
extern uint32_t smsDataTimeOut;
extern uint16_t clearModbusTimeOut;

extern uint8_t g_gsmStatus;
extern uint16_t g_reconnectTime;
// extern uint8_t g_needReportAlarm;


extern uint8_t LinkMsg[10];
extern uint8_t gsm_time_Count;


extern uint8_t Flag_TCP_Send_Data;
extern uint8_t Flag_SMS_Send_Data;
extern uint8_t Flag_TCP_Send_Heartbeat;

uint8_t AT_CommandRun(char *command,char *respond,uint8_t times,uint16_t timeout);
uint8_t AT_CommandSend(void *sendbuf, char *Command);
uint8_t AT_CommandRecv(void *recvbuf,uint16_t timeout); 
uint8_t AT_CommandComp(void *command, char *str);

extern void TcpPortMonitor(void);
//extern void System_Error_Model_Handler(void);

extern uint8_t GPRS_Data_Upload_Handler(void);
extern void SMS_Handler(void);

uint8_t TcpIpInit(void);
uint8_t GPRS_TCPIP_Connect(uint8_t * IP_ADDR,uint8_t * IP_PORT);
uint8_t GPRS_TCPIP_DisConnect(void);
uint8_t GPRS_TCPIP_NETShut(void);
extern uint8_t GPRS_TCPIP_Send(uint8_t * Buffer,uint16_t BufferLen);
uint8_t GPRS_TCPIP_Recv(void);
uint8_t GPRS_TCPIP_GetData(void);

extern uint8_t GPRS_TCPIP_Deal(void);

uint8_t SendSMSChar(char* pSMSRecipient, char* pSMSContent);
uint8_t SendSMS140(char* pSMSCenter, char* pSMSRecipient, char* pSMSContent);
uint8_t SendSMS70(char* pSMSCenter, char* pSMSRecipient, char* pSMSContent);
uint8_t GPRS_SMS_Recv(void);
uint8_t GPRS_SMS_Read(void);

extern void LocationBasedService(void);
extern uint16_t GetSigalStrength(void);


extern void MG323B_PowerON(void);
extern void EnablePowerKey(void);
extern void DisablePowerKey(void);
extern uint8_t MG323B_Configuration(void);

extern uint8_t MG323B_Init(void);
extern void MG323B_Close(void);


extern void ResetGsm(void);
extern void GSM_TCP_Init(void);
extern uint8_t LinkToTcpServer(void);



extern uint8_t Flag_Gprs_Init;
extern void User_GsmPortInit(void);	
extern uint8_t GprsInit(void);
extern uint8_t Check_TCPIP(void);

extern void	GSM_Power_Saver_Handler(void);

extern char* findData(char* src, int srcSize, char* target);
extern char* findDataFromEnd(char* src, int srcSize, char target);

#endif	  //_GSM_GPRS_H_
