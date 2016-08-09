#ifndef __MODBUS_H
#define __MODBUS_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "includes.h"

#define MAX_TX_BUF_SIZE  (0x7d << 1)	 // 250



#define MODBUS_REG4_SIZE 		5120
#define MODBUS_REG5_SIZE 		1024
#define MODBUS_REG6_SIZE		512
#define MODBUS_BUFFER_SIZE 	256

// ¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~

#define MODBUS_COMMAND 258

// ¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~

#define MODBUS_SystemStatus  84

#define HOMER_3X_DEVIDE_ID	 0

#define modbusTerminalBaseStatus 85
#define modbusSMSNumberBack 100
#define modbusSMSNumberAlarm 128
#define modbusSMSNumberConcrol 142
#define modbusServerIPBack 320
#define modbusServerIP 188
#define modbusServerPort 452
#define modbusServerPortBack 454
#define modbusTcpHeartInterval 456



#define TCP_REPORT_TIME_HOUR_ACC_ON		240
#define TCP_REPORT_TIME_MIN_ACC_ON		241
#define TCP_REPORT_TIME_SEC_ACC_ON		242

#define TCP_REPORT_TIME_HOUR_ACC_OFF	243
#define TCP_REPORT_TIME_MIN_ACC_OFF		244
#define TCP_REPORT_TIME_SEC_ACC_OFF   245

#define DEVEICE_WORK_TIME_HOUR	246
#define DEVEICE_WORK_TIME_MIN		247
#define DEVEICE_WORK_TIME_SEC   248

#define DEVEICE_SLEEP_TIME_HOUR	249
#define DEVEICE_SLEEP_TIME_MIN	250
#define DEVEICE_SLEEP_TIME_SEC  251
#define DEVEICE_WORK_OR_SLEEP   252


#define modbusTcpReportIntervalAccOn 500
#define modbusTcpReportIntervalAccOff 502
#define modbusSleepDelay 506
#define modbusSleepTime 508
#define modbusLockTimeNoSignal 510
#define modbusAlarmTimeNoPLC 512
#define modbusDefaultSet 516


#define Countstandby 518

#define modbusGpsLatitude 750
#define modbusGpsLongitude 754
#define modbusGpsSpeed 758
#define modbusGpsdirection 760
#define modbusGpsHeight 762
#define modbusGpsTime 764
#define modbusGpsNoSV 770

#define modbusGSMRSSI 778

#define modbusLBSMessage 780

#define modbusPWM 828
#define modbusAD1 858
#define modbusAD2 860
#define modbusPower 852
#define modbusBattery 854
// #define modbusShell 856
#define modbusAntenna 800
#define modbusShell 	801

#define modbusACC 802
#define modbusMoto 804

#define RELAY_A_SET    	822
#define RELAY_A_STATUS 	823

#define RELAY_B_SET 		824
#define RELAY_B_STATUS 	825

#define RELAY_C_SET 		826
#define RELAY_C_STATUS	827

#define RELAY_D_SET 		828
#define RELAY_D_STATUS	829


#define modbusTimeACC 3700
#define modbusTimeDay 3704
#define modbusTimeNum 3708
#define modbusTimeData 3712

#define MODBUS_OK 1;
#define MODBUS_ERROR 0;

#define modbusCanEnabled 900
#define modbusCanBaudrate 902
#define modbusCanNumber 904
#define modbusCanID1 906
#define modbusCanFrame 1418





#define modbusCanSendHeart1 3082
#define modbusCanSendID1 3084
#define modbusCanSendData1 3088
#define modbusCanSendHeart2 3096
#define modbusCanSendID2 3098
#define modbusCanSendData2 3102


#define MIN_SLEEP_TIME 60  // >= 60


extern uint8_t g_modbusReg4[MODBUS_REG4_SIZE];//
extern uint8_t g_modbusReg5[MODBUS_REG5_SIZE];//tcp
extern uint8_t g_modbusReg6[MODBUS_REG6_SIZE];//sms

extern int32u reportHeartTimeOut;
extern int32u reportAccOffTimeOut;
extern int32u reportAccOnTimeOut;
extern int32u DeviceWorkTime;
extern int32u DeviceSleepTime;
extern int32u lockNoSignalTimeOut;
extern int32u alarmNoPLCTimeOut;

extern uint8_t g_modbusReceiveBufTcp[MODBUS_BUFFER_SIZE];
extern uint8_t g_modbusSendBufTcp[MODBUS_BUFFER_SIZE];
extern int16u g_modbusReceiveBufNumberTcp;
extern int16u g_modbusSendBufNumberTcp;

extern uint8_t g_modbusReceiveBufSerial[MODBUS_BUFFER_SIZE];
extern uint8_t g_modbusSendBufSerial[MODBUS_BUFFER_SIZE];
extern int16u g_modbusReceiveBufNumberSerial;
extern int16u g_modbusSendBufNumberSerial;

extern int16u modbusAddressTcp;
extern int16u modbusLongthTcp;
extern int16u modbusAddressSerial;
extern int16u modbusLongthSerial;

extern int8u modbusTerminalID;
extern int8u modbusTerminalIDOn;

extern uint8_t Modbus_TCP_Handler(uint8_t *pIn, uint32_t inSize);
extern void	ModbusTcpReportByTime(void);
extern void	ModbusSmsReportByTime(void);

extern void LoadModbus(void);

extern void EraseEEPROM(void);
extern void LoadModbus(void);
extern void CleanFlash(void);
extern void AutoSaveModbus4(void);
extern void SaveModbus4(void);
extern void SaveModbus5(void);
extern void SaveModbus6(void);

extern void SaveEEPROM(void);
//extern void ModbusCommand(void);
extern uint8_t REGISTER_MODBUS5_BUFFER[100];

extern void InitModbusTcp(void);
extern void InitModbusSendTcp(void);
extern void InitModbusSerial(void);

extern void InitModbusSendSerial(void);
extern void InitModbus(int8u type);
extern int8u ModbusReportErrorTcp(int data);
extern int8u ModbusReportErrorTcp(int data);
extern int8u ModbusSendDataTcp(void);


extern int8u ModbusReportErrorSerial(int data);
extern int8u ModbusSendDataSerial(void);
extern int8u ModbusReportError(int data, int8u type);
extern uint8_t ModbusSendData(uint8_t type);
extern uint8_t ModbusInput(uint8_t *pIn, uint32_t inSize, uint8_t * g_modbusReceiveBuf, uint8_t * g_modbusSendBuf, int16u* g_modbusReceiveBufNumber, uint16_t * g_modbusSendBufNumber, uint8_t type, uint16_t * longth, uint16_t * address);
extern uint8_t ModbusInputSerial(uint8_t *pIn, uint32_t inSize);
extern uint8_t ModbusInputTcp(uint8_t *pIn, uint32_t inSize);


extern void LoadModbusDefault(void);
extern void GetServerIP(uint8_t * ip, int8u ipLen, uint8_t* port, uint8_t portLen);
extern void GetServerIPBack(uint8_t* ip, uint8_t ipLen, uint8_t* port, int8u portLen);

extern void Modbus_Handler(void);


#ifdef __cplusplus
}
#endif 

#endif


