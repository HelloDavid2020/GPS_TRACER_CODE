#ifndef __USER_GPS_H
#define __USER_GPS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "includes.h"
	 
#define NMEA_COUNT_MAX 500
 
#define 	GPS_VALID   			1   // gps 定位
#define 	GPS_INVALID				0 	// gPS 未定位
	 
#define   NORTH_LATITUDE    1  // 北纬
#define   SOUTH_LATITUDE		0	 // 南纬
	 
#define   EAST_LONGTITUDE   1  // 东经
#define   WEST_LONGTITUDE	 	0  // 西经	 
	 

#define GPS_POWER_ON 		GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define GPS_POWER_OFF		GPIO_ResetBits(GPIOC, GPIO_Pin_2)
	 
	 

extern uint8_t 	GpsBuffer[512];
extern uint16_t GpsBufferSize;

extern uint8_t 	gps_satelliteNum;
extern int32u 	gps_latitude;
extern uint8_t 	gps_status ;

extern int32u 	gps_longitude;
extern uint8_t 	gps_speed;
extern uint8_t 	gps_azimuth;
extern uint16_t gps_altitude;
extern uint8_t 	gps_dateTime[6];
	 
typedef struct
{
	uint8_t UtcTime[6]; 
	uint8_t Status;   
	uint8_t Latitude[11];	
	uint8_t LatitudeNS; 
	uint8_t Longitude[12];	 
	uint8_t LongitudeEW;
	uint8_t Speed[6];	    
	uint8_t Azimuth[6];	
	uint8_t UtcDate[7]; 
	uint8_t Altitude[8];   
	uint8_t SatelliteNumS[3];//??? 
}GPSINFO;

typedef struct
{
	uint8_t SatelliteNumS; 
	uint8_t Status;  
	uint8_t Speed;	    
	uint8_t UTC_Time[7];  // UTC--->BTC
	
	uint8_t LatitudeNS; 
	uint8_t LongitudeEW;
	
	uint16_t Altitude;   
	uint16_t Azimuth;	  // 航向角
			
	double Latitude;	// 双精度
	double Longitude;	 
}GPSDATA;  // 上传数据

typedef struct
{
uint8_t sec;
uint8_t min;
uint8_t hour;
uint8_t day;
uint8_t mon;
uint8_t year;
uint8_t week;	
}TIME;  // 北京时间


extern uint8_t DisableGLL[11];
extern uint8_t DisableGSA[11];
extern uint8_t DisableGSV[11];
extern uint8_t DisableVTG[11];	

extern TIME UTC_Time; // 
extern TIME BTC_Time; // 北京时间


extern GPSINFO GpsInfo;
extern GPSDATA GpsData; // GPS上传数据包


extern uint8_t gps_buffer[NMEA_COUNT_MAX]; 

extern uint8_t GpsFlag;
extern uint8_t Flag_GpsTimeGranted;
extern uint8_t Flag_getGpsTime;

extern uint32_t FlagTimeSynchronized;
extern uint32_t CountTimeSynchronized;

extern void User_GpsPortInit(void);

extern void  UTC2BTC(void);

extern uint8_t GetWeekFromDay(uint8_t year, uint8_t month, uint8_t day);
extern u16 FindStrFindStr(char *str,char *ptr);
extern uint16_t FindStr(char *str,char *ptr);
extern void GPSParse(void);
extern void GpsDataInit(void);
extern void UpdateGpsData(void);

extern float LonToRad(uint8_t *Lon);
extern float LatToRad(uint8_t *Lat);
	
extern void RT8025_Time_Synchronized(void);
extern void ShowGPS(void);
	
	 
#ifdef __cplusplus
}
#endif 

#endif



