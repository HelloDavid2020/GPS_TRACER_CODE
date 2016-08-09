#ifndef __USER_REGISTERS_H
#define __USER_REGISTERS_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "includes.h"
	

extern uint16_t usRegInputBuf[256];
extern uint16_t *usRegHoldingBuf;	//一个测试用的 寄存器数组 地址0-7

extern uint16_t REG_INPUT_START,REG_HOLDING_START;
extern uint16_t REG_INPUT_NREGS,REG_HOLDING_NREGS;
extern uint16_t usRegInputStart,usRegHoldingStart;
	
#define TEMP_TCP_SERVER_IP			110
#define TEMP_TCP_SERVER_PORT 		114

#define TEMP_BACKUP_TCP_SERVER_IP 		115
#define TEMP_BACKUP_TCP_SERVER_PORT 	119

#define TCP_SERVER_IP 					100
#define TCP_SERVER_PORT 				104



#define BACKUP_TCP_SERVER_IP 		105
#define BACKUP_TCP_SERVER_PORT 	109


//---------------------------------------------
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
//#define DEVEICE_WORK_OR_SLEEP   252

//---------------------------------------------

#define TCP_LINK_HEART_BEAT_INTERVAL			228


#define TCP_UPLOAD_SECONDS_HIGH_ACC_ON		240
#define TCP_UPLOAD_SECONDS_LOW_ACC_ON			241

#define TCP_UPLOAD_SECONDS_HIGH_ACC_OFF		242
#define TCP_UPLOAD_SECONDS_LOW_ACC_OFF		243

#define DEVEICE_WORK_SECONDS_HIGH					244
#define DEVEICE_WORK_SECONDS_LOW					245

#define DEVEICE_SLEEP_SECONDS_HIGH				246
#define DEVEICE_SLEEP_SECONDS_LOW					247

#define DEVEICE_SLEEP_ENABLE	   					248
#define DEVEICE_RUN_OR_TEST     					249

//-------------------------------------------------------
#define ADDR_ADMIN_RIGHTS_ENABLE 420
#define ADDR_ADMIN_LOCK_STATE    421

// 实际锁车状态
#define ADDR_ADMIN_REAL_LOCK_STATE    422
#define ADDR_ADMIN_UNLOCK_ENABLE 420


#define ADDR_LOCK_LEVEL 				424



#define ADDR_BAT_LOW_VOLTAGE_VALUE 				431
#define ADDR_BAT_RECOVER_VOLTAGE_VALUE		432

#define ADDR_LI_BAT_LOW_VOLTAGE_VALUE 		433
#define ADDR_LI_BAT_RECOVER_VOLTAGE_VALUE	434	

#define COMM_TIMEOUT_GPRS   437
#define COMM_TIMEOUT_UART   438
#define COMM_TIMEOUT_CAN    439


// 远程数据发送时间记录
// 远程数据日志时间戳
#define ADDR_REMOTE_COMM_LOG_YEAR   440
#define ADDR_REMOTE_COMM_LOG_MON  	441
#define ADDR_REMOTE_COMM_LOG_DAY    442
#define ADDR_REMOTE_COMM_LOG_HOUR   443
#define ADDR_REMOTE_COMM_LOG_MIN    444
#define ADDR_REMOTE_COMM_LOG_SEC    445

// address 446 预留
#define ADDR_REMOTE_COMM_TIMEOUT_SETDAYS    446

#define ADDR_REMOTE_COMM_TIMEOUT_DAYS    447



#define ADDR_LAST_ENGINE_START_TIME   1830

#define ADDR_FIRST_ENGINE_START_TIME  1840

#define ADDR_FLAG_FIRST_ENGINE_START  1839




//VehicleWorkTimeOfOneDay
#define  ADDR_VEHICLE_WORK_TIME_OF_THE_PREV_DAY_HIGH	1846
#define  ADDR_VEHICLE_WORK_TIME_OF_THE_PREV_DAY_LOW 	1847

#define  ADDR_VEHICLE_WORK_TIME_OF_THE_CURRENT_DAY_HIGH	1848
#define  ADDR_VEHICLE_WORK_TIME_OF_THE_CURRENT_DAY_LOW 	1849

//-----------------------------------------------------
// Radius of center
//
//4318	2159	4321
//4322	2161	4325
//4326	2163	4329


#define ADDR_CENTER_POINT_RADIUS       4318
#define ADDR_CENTER_POINT_LONTITUDE    4322 
#define ADDR_CENTER_POINT_LATITUDE     4326 


#define ADDR_WATER_TEMP_HIGH           2110
#define ADDR_OIL_PRESS_LOW             2111 



// EEPROM 地址
#define ADDR_BLIND_DATA_ZONE_CONFIG    2250
#define ADDR_TOTAL_LOG_NUMS   2251

#define ADDR_BLIND_DATA_EEPROM_LOG_NUMBERS    				2253   // EEPROM  中存储的盲区数据条数 max = 16
#define BLIND_ZONE_DATA_BASE_ADDR				2254*2   // EEPROM  中存储的盲区数据的数据域地址






//-----------------------------------------------------
// TCP发送缓存器
#define ADDR_TCP_SENDBUF_VOL_BATTERY  122 // 蓄电池   
#define ADDR_TCP_SENDBUF_VOL_LIBAT    124 // Li电池   
#define ADDR_TCP_SENDBUF_WORKTIME   	126 // Li电池


extern uint32_t GetDeveiceUploadTime(void);



#ifdef __cplusplus
}
#endif 

#endif
