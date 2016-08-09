#include "user_gps.h"	 


/*
// Disable GLL 01
Length: 11, Data: B5 62 06 01 03 00 F0 01 00 FB 11 	

// Disable GSA 02
Length: 11, Data: B5 62 06 01 03 00 F0 02 00 FC 13 	

// Dsiable GSV 03
Length: 11, Data: B5 62 06 01 03 00 F0 03 00 FD 15 	



// Disable VTG 05
Length: 11, Data: B5 62 06 01 03 00 F0 05 00 FF 19 	
*/

uint8_t DisableGLL[11]={0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x01,0x00,0xFB,0x11};
uint8_t DisableGSA[11]={0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x02,0x00,0xFC,0x13};
uint8_t DisableGSV[11]={0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x03,0x00,0xFD,0x15};
uint8_t DisableVTG[11]={0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x05,0x00,0xFF,0x19};


uint8_t		GpsBuffer[512];
uint8_t 	gps_status = 0;
uint8_t 	gps_satelliteNum = 0;
uint8_t 	gps_dateTime[6] = {0};
uint8_t 	gps_speed = 0;
uint8_t 	gps_azimuth = 0;
uint16_t 	gps_altitude = 0;
uint16_t 	GpsBufferSize = 0;
uint32_t 	gps_latitude = 0;
uint32_t 	gps_longitude = 0;


uint8_t gps_buffer[NMEA_COUNT_MAX]={0}; 
//char FindString[80];
// char *pFindString="$GPRMC,015152.589,V,0000.0000,N,00000.0000,E,,,020504,,*11\r\n";
uint8_t GpsFlag = 0;
GPSINFO GpsInfo; // 解析数据包
GPSDATA GpsData; // 上传数据包


TIME UTC_Time; // 
TIME BTC_Time; // 北京时间


uint8_t Flag_GpsTimeGranted = NONE;
uint8_t Flag_getGpsTime = DISABLE;

uint32_t FlagTimeSynchronized = DISABLE;
uint32_t CountTimeSynchronized = 0;

void User_GpsPortInit(void)
{
	User_ComInit(2,	 9600u);	
	GpsDataInit();
	

	UART_sendPacket( USART2, DisableGLL, 11);// close GPGLL
	UART_sendPacket( USART2, DisableGSA, 11);// close GPGSA
	UART_sendPacket( USART2, DisableGSV, 11);// close GPGSV
	UART_sendPacket( USART2, DisableVTG, 11);// close GPVTG
}

//void UTC2BTC(uint8_t * utc_time)
void  UTC2BTC(void)
{
	BTC_Time.year	= UTC_Time.year;
	BTC_Time.mon 	= UTC_Time.mon;
	BTC_Time.day 	= UTC_Time.day;
	BTC_Time.hour = UTC_Time.hour;
	BTC_Time.min 	= UTC_Time.min;
	BTC_Time.sec 	= UTC_Time.sec;	
	
	BTC_Time.sec = BTC_Time.sec; //加一秒
	if(BTC_Time.sec >59)
	{
		BTC_Time.sec=0;
		BTC_Time.min++;
		if(BTC_Time.min>59)
		{
			BTC_Time.min=0;
			BTC_Time.hour++;
		}
	}	

	BTC_Time.hour+=8;
	if(BTC_Time.hour>23)
	{
		BTC_Time.hour-=24;
		BTC_Time.day+=1;
		if( (BTC_Time.mon==2) || (BTC_Time.mon==4) || (BTC_Time.mon==6) || (BTC_Time.mon==9) || (BTC_Time.mon==11) )
		{
			if(BTC_Time.mon>30)
			{
				BTC_Time.day=1;
				BTC_Time.mon++;
			}
		}
		else
		{
			if(BTC_Time.day>31)
			{
				BTC_Time.day=1;
				BTC_Time.mon++;
			}
		}
		if((BTC_Time.year % 4) == 0 )
		{
			if( (BTC_Time.day > 29 ) && (BTC_Time.mon==2))
			{
				BTC_Time.day =1;
				BTC_Time.mon ++;
			}
		}
		else
		{
			if((BTC_Time.day>28) && (BTC_Time.mon ==2))
			{
				BTC_Time.day=1;
				BTC_Time.mon++;
			}
		}
		if(BTC_Time.mon>12)
		{
			BTC_Time.mon-=12;
			BTC_Time.year++;
		}		
	}
	

}
// year	: 00-99                          
// month: 01-12                             
// day	: 01-31                           
//
uint8_t GetWeekFromDay(uint8_t year, uint8_t month, uint8_t day)
{
	uint8_t tempyear = year;
	uint8_t tempmon =  month;
	uint8_t tempday = day;
	
  uint8_t result = 0;
	
	if( tempmon == 1 || tempmon == 2 )  
    {
        tempmon += 12;
        if( tempyear> 0 )
            tempyear--;
        else
            tempyear = 4;
    }
		// 1 = mon
		result = 1+(( tempday + 2*tempmon + 3*(tempmon+1)/5 + tempyear + tempyear/4 ) %7);
    return result;
} 


void RT8025_Time_Synchronized(void)
{
// 	uint8_t * time = NULL;
	if(Flag_GpsTimeGranted == NONE)
	{
		if(uSystem.Bit.gps == GPS_VALID)
			if(FlagTimeSynchronized == ENABLE)
			{
				
				// UTC -- BTC
				UTC2BTC();
				TimeSet.year = DEC2HEX(BTC_Time.year);
				TimeSet.mon 	= DEC2HEX(BTC_Time.mon);
				TimeSet.day 	= DEC2HEX(BTC_Time.day);
				
				TimeSet.week = GetWeekFromDay(BTC_Time.year, BTC_Time.mon, BTC_Time.day);
				TimeSet.week =	DEC2HEX(TimeSet.week);
				
				TimeSet.hour = DEC2HEX(BTC_Time.hour);
				TimeSet.min 	= DEC2HEX(BTC_Time.min);
				TimeSet.sec 	= DEC2HEX(BTC_Time.sec);

				SetRx8025Time(TimeSet);	
				os_dly_wait(10);
				#ifdef DEBUG
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
				printf("> Time Synchronized.\r\n");
		
				ShowTime();
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");			
						os_dly_wait(100);

				#endif
				Flag_GpsTimeGranted = DONE;

			}

	}
	
	// 手动对时
	if(*(usRegHoldingBuf + 1827) != 0)
	{
		TimeSet.year = DEC2HEX(*(usRegHoldingBuf + 1820));
		TimeSet.mon 	= DEC2HEX(*(usRegHoldingBuf + 1821));
		TimeSet.day 	= DEC2HEX(*(usRegHoldingBuf + 1822));
		TimeSet.week =	DEC2HEX(*(usRegHoldingBuf + 1823));
		TimeSet.hour = DEC2HEX(*(usRegHoldingBuf + 1824));
		TimeSet.min 	= DEC2HEX(*(usRegHoldingBuf + 1825));
		TimeSet.sec 	= DEC2HEX(*(usRegHoldingBuf + 1826));
				
		SetRx8025Time(TimeSet);
		DelayMS(5);
		*(usRegHoldingBuf + 1827) = 0;

	}
		
}

void GpsDataInit(void)
{
// 	GpsInfo.UtcTime[6]		='\0';
	GpsInfo.Latitude[9]		='\0';
	GpsInfo.Longitude[10]	='\0';
	GpsInfo.Speed[5]			='\0';
	GpsInfo.Azimuth[5]		='\0';
	GpsInfo.UtcDate[6]		='\0';
	GpsInfo.Altitude[7]		='\0';
	
}
uint16_t FindStr(char *str,char *ptr)
{
	uint16_t index=0;
	char *STemp=NULL;
	char *PTemp=NULL;
	char *MTemp=NULL;
	if(0==str||0==ptr)
		return 0;
	for(STemp=str; *STemp!='\0'; STemp++)	 
	{
		index++;   
		MTemp=STemp;  

		for(PTemp=ptr;*PTemp!='\0';PTemp++)
		{	
			if(*PTemp!=*MTemp)
			break;
			MTemp++;
		}
		if(*PTemp=='\0')  
			break;
	}
	return index;
}



void GPSParse(void)
 {
 	uint8_t CommaNum=0; 
	uint8_t BufIndex=0;
 	uint8_t Sbuf;
	uint8_t *Pstr;
	uint16_t index;

	index= FindStr((void *)gps_buffer,"$GPRMC,");
	if(index)
	{
		CommaNum=0;
		Pstr=gps_buffer+index+6;	 
		do
		{
			Sbuf=*Pstr++;	

			switch(Sbuf)
			{
				case ',':
					CommaNum++;  
					BufIndex=0;
				break;
				
				default:
						switch(CommaNum)
						{
							case 0:
								GpsInfo.UtcTime[BufIndex]=Sbuf;
								GpsData.UTC_Time[3] = (GpsInfo.UtcTime[0] - '0') * 10 + (GpsInfo.UtcTime[1] - '0');
								GpsData.UTC_Time[4] = (GpsInfo.UtcTime[2] - '0') * 10 + (GpsInfo.UtcTime[3] - '0');
								GpsData.UTC_Time[5] = (GpsInfo.UtcTime[4] - '0') * 10 + (GpsInfo.UtcTime[5] - '0');
							
								UTC_Time.hour = GpsData.UTC_Time[3];
								UTC_Time.min 	= GpsData.UTC_Time[4];
								UTC_Time.sec 	= GpsData.UTC_Time[5];
							break; 
							
							case 1:
								GpsInfo.Status=Sbuf;
								if(GpsInfo.Status == 'A')
									GpsData.Status = GPS_VALID; // 有效定位
									
								else if(GpsInfo.Status == 'V')
									GpsData.Status = GPS_INVALID;  // 无效定位
								
							break;
							
							case 2:
								GpsInfo.Latitude[BufIndex]=Sbuf;	
								GpsInfo.Latitude[10]='\0';	
							
								GpsData.Latitude =	atof((void *)GpsInfo.Latitude);
							break;
							
							case 3:
								GpsInfo.LatitudeNS = Sbuf;
								if(GpsInfo.LatitudeNS == 'N')
									GpsData.LatitudeNS = NORTH_LATITUDE; //
								else if(GpsInfo.LatitudeNS == 'S')
									GpsData.LatitudeNS = SOUTH_LATITUDE;  // 

							
							break;
							
							case 4:
								GpsInfo.Longitude[BufIndex] = Sbuf;
								GpsInfo.Longitude[11] = '\0';
							
								GpsData.Longitude =	atof((void *)GpsInfo.Longitude);
							
							break;
							
							case 5:
								GpsInfo.LongitudeEW = Sbuf;	
								if(GpsInfo.LongitudeEW == 'E')
									GpsData.LongitudeEW = EAST_LONGTITUDE; //
								else if(GpsInfo.LongitudeEW == 'W')
									GpsData.LongitudeEW = WEST_LONGTITUDE;  // 							
							
							
							break;
							
							case 6:
								GpsInfo.Speed[BufIndex]=Sbuf;		
								GpsData.Speed =	atoi((void *)GpsInfo.Speed);
							
							break;
							
							case 7: 
								GpsInfo.Azimuth[BufIndex]=Sbuf;	 // 航向角
								GpsData.Azimuth =	atoi((void *)GpsInfo.Azimuth);
							
							break;
							
							case 8:
								GpsInfo.UtcDate[BufIndex]=Sbuf;	
								GpsData.UTC_Time[0] = (GpsInfo.UtcDate[4] - '0') * 10 + (GpsInfo.UtcDate[5] - '0');
								GpsData.UTC_Time[1] = (GpsInfo.UtcDate[2] - '0') * 10 + (GpsInfo.UtcDate[3] - '0');
								GpsData.UTC_Time[2] = (GpsInfo.UtcDate[0] - '0') * 10 + (GpsInfo.UtcDate[1] - '0');
							
								UTC_Time.day 	= GpsData.UTC_Time[2];
								UTC_Time.mon 	= GpsData.UTC_Time[1];
								UTC_Time.year	= GpsData.UTC_Time[0];							
							break;
							
							default:break;
						}
						BufIndex++;	//
				break;
			}
		}while(Sbuf!='*');
		
	}
	index= FindStr((void *)gps_buffer,"$GPGGA,");
	if(index)
	{
		CommaNum=0;
		Pstr=gps_buffer+index+6;
		do
		{
			Sbuf=*Pstr++ ;
			switch(Sbuf)
			{
				case ',':CommaNum++;
						 BufIndex=0;
						 break;
				default:
						switch(CommaNum)
						{
							case 0:break;
							case 1:break;
							case 2:break;
							case 3:break;
							case 4:break;
							case 5:break;
							case 6:
								GpsInfo.SatelliteNumS[BufIndex]= Sbuf;
								GpsInfo.SatelliteNumS[2]= '\0';
							
								GpsData.SatelliteNumS =	atoi((void *)GpsInfo.SatelliteNumS);
							break;
							
							case 7:break;
							
							case 8:
								GpsInfo.Altitude[BufIndex]=Sbuf; // 海拔
							
								GpsData.Altitude =	atoi((void *)GpsInfo.Altitude);

							break;
							default:break;
						}
						BufIndex++;
						break;
			}
		}while(Sbuf!='*');	
		

	} 
	
		#ifdef GPS
		printf("> NEMA:\r\n%s \r\n",gps_buffer);
		DelayMS(100);
		#endif

	
}

float LatToRad(uint8_t *Lat)
{
	float Rad;
	uint16_t Data;
	uint8_t i = 0;
	for(i=0; i<8; i++)
	 Lat[i] = Lat[i]-'0';
	
	Data=Lat[0]*10+Lat[1];
	Rad=Lat[2]*10+Lat[3]+Lat[5]*0.1+Lat[6]*0.01+Lat[7]*0.001+Lat[8]*0.0001;
	Rad=Rad/60;
	Rad=Rad+Data;
	return Rad;
				
 }
float LonToRad(uint8_t *Lon)
{
 	float Rad;
	uint16_t Data;
	uint8_t i = 0;
	for(i=0; i<9; i++)
	 Lon[i] = Lon[i]-'0';
	
	Data=Lon[0]*100+Lon[1]*10+Lon[2];
	Rad=Lon[3]*10+Lon[4]+Lon[6]*0.1+Lon[7]*0.01+Lon[8]*0.001+Lon[9]*0.0001;
	Rad=Rad/60;
	Rad=Rad+Data;
	return Rad;
}


void UpdateGpsData(void)
{	
	if(GpsData.Status == GPS_VALID)
	{
		LED_BLUE_ON;
	}
	else
	{
		LED_BLUE_OFF;
	}
	

	uSystem.Bit.gps = GpsData.Status;
	uSystem.Bit.latitude		= GpsData.LatitudeNS;
	uSystem.Bit.longtitude 	=	GpsData.LongitudeEW;	

	*(uint32_t*)(g_modbusReg4 + modbusGpsNoSV) 	= GpsData.SatelliteNumS;
	*(uint32_t*)(g_modbusReg4 + modbusGpsSpeed) = GpsData.Speed;
	*(uint32_t*)(g_modbusReg4 + modbusGpsdirection) = GpsData.Azimuth >> 1; // azimuth 
	*(uint32_t*)(g_modbusReg4 + modbusGpsHeight) 		= GpsData.Altitude;

	*(uint32_t*)(g_modbusReg4 + modbusGpsLatitude) = (uint32_t)(GpsData.Latitude * 100000L);
	*(uint32_t*)(g_modbusReg4 + modbusGpsLongitude) = (uint32_t)(GpsData.Longitude * 100000L);	

// 	g_modbusReg4[modbusGpsLatitude]		= (uint32_t)(GpsData.Latitude * 100000L);
// 	g_modbusReg4[modbusGpsLongitude]	= (uint32_t)(GpsData.Longitude * 100000L);	
	memcpy(g_modbusReg4 + modbusGpsTime, GpsData.UTC_Time, 6);	
	// 更新系统状态
	*(uint32_t*)(g_modbusReg4 + MODBUS_SystemStatus) = uSystem.StatusValue;	
}


void ShowGPS(void)
{
	printf("\r\n---- GPS Information ---\r\n");
	printf("> GpsState : %d \r\n",GpsData.Status);
	printf("> NumsofSat: %d\r\n", GpsData.SatelliteNumS);
	printf("> Longitude: %10d\r\n",(uint32_t)(GpsData.Longitude * 100000L));
	printf("> Latitude : %10d\r\n",(uint32_t)(GpsData.Latitude * 100000L));
	
	printf("> fNowLat: %.5f\r\n",TwoPointLocation.fNowLat);
	printf("> fNowLong : %.5f\r\n",TwoPointLocation.fNowLong);	
	printf("------------------------\r\n");
}

