// TheGpsDistanceCalculate
#include "user_distance.h"
TWO_POINT_LOCATION TwoPointLocation;

double lat1 = 39.90744;
double lng1 = 116.41615;
double lat2 = 39.90744;
double lng2 = 116.30746;


uint32_t distPerDegree[46][2] = 
{
{110574,111319},{110575,111251},{110579,111049},{110586,110713},{110596,110243},{110608,109720},{110622,108902},{110639,108033},{110659,107034},
{110681,105905},{110705,104647},{110731,103300},{110759,101752},{110789,100118},{110820,98362}, {110853,96486}, {110888,94493}, {110923,92385},
{110960,90164}, {110998,87832}, {111036,85394}, {111074,82741}, {111113,80207}, {111152,77464}, {111191,74626}, {111230,71696}, {111268,68678},
{111305,65576}, {111342,62392}, {111378,59226}, {111412,55800}, {111445,52398}, {111477,48932}, {111507,45405}, {111535,41822}, {111561,38186},
{111585,34504}, {111607,30779}, {111626,27016}, {111643,23219}, {111658,19346}, {111670,15543}, {111680,11675}, {111686,7791},  {111690,3898},
{111133,0}
};


//DisPerLat = *(distPerDegree_P+(DegreeOfLat>>1)*2);//???????????????????????????
//DisPerLong = *(distPerDegree_P+(DegreeOfLat>>1)*2+1);//???????????????????????????							                
//DisToDes = GetTwoPointDis(destipoint,newpoint);//??????????????

//            NowLat = FloatTransformToDecimal(DataToControler1.Latitude);//??????????????????????
//            NowLong = FloatTransformToDecimal(DataToControler1.Longitude);//??????????????????????
//            DegreeOfLat = DegreeOfLatTemp;
//mod_ss

double radian(double d)
{
	 return d * PI / 180.0;  
}

double get_distance(double lat1, double lng1, double lat2, double lng2)
{
	double radLat1 = radian(lat1);
	double radLat2 = radian(lat2);
	
	double radLon1 = radian(lng1);
	double radLon2 = radian(lng2);	
	
	double a = 0;
	double b = 0;
	double dst = 0;
	
	if(radLat1 >= radLat2)
		a = radLat1 - radLat2;
	else
		a = radLat2 - radLat1;
		
	if(radLon1 >= radLon2)
		b = radLon1 - radLon2;
	else
		b = radLon2 - radLon1;
	
	dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));

	dst = dst * EARTH_RADIUS;
//	dst= round(dst * 10000) / 10000;
	dst = dst*1000; // 单位米
	return dst;
}

DWORD mod_ss(DWORD Temp1,DWORD Temp)
{
	DWORD i,l,rank;
	i = 0;
	l = 0;
	rank = 0;
	if((Temp1 == 0)&&(Temp!=0))
	{
		return 0;
	}
	else if(Temp == 0)
	{
		return 0;	
	}
	else
	{
		while(!(Temp1&0x80000000))
		{
			i++;
			Temp1<<=1;
		}
		rank = 32-i;
		i = 0;
		for(;rank>0;rank--)
		{
			if(Temp1&0x80000000)
			{
				l = l|0x01;
			}
			Temp1<<=1;
			if(l >= Temp)
			{
				i = i|0x01;
				l = l-Temp;
			}
			if(rank>1)
			{
				i<<=1;
				l<<=1;
			}
		}
	}
	return i;
}


//NowLong = FloatTransformToDecimal(DataToControler1.Longitude);//??????????????????????
//            DegreeOfLat = DegreeOfLatTemp;

// 单精度浮点数转换为10进制数
WORD FloatTransformToDecimal(DWORD temp)
{
	DWORD H,Temp,temptemp;
	DWORD i,l,m;
	H = 0;
	Temp = 0;
	temptemp = temp;
	i = 0;
	l = 0;
	m = 0;
        if(temp&0x80000000)
        {
          
        }
        else
        {
	  i = (((temp<<1) & 0xff000000)>>24);
	  if((i>=127)&&(i<=255))
	  {
		l = i - 127;
		temp = ((temp<<1) & 0x00ffffff)<<(8+l);
		H = 0x00000002;//?????1,???????,??????????  ????
		for(;m<l;m++)
		{
			if((temptemp<<(m+1))&0x00800000)//?????????
			{
				H = H|0x00000001;
			}
			if(m<(l-1))
			{
				H<<=1;
			}
		}//??????	
		for(m = 0;m<(23-l);m++)
		{
//			if(temp&0x80000000)
//			Temp = Temp+(*(FloatToDeci_P+m));
//			//if(m<15)
//			//{
//				temp <<= 1;
			//}
		}
		temp = Temp;
		Temp = mod_ss(temp,10);//???Temp/10,???????????
		Temp = H*100000+Temp;	
	}
	else if((i>0)&&(i<127))//-------------------------------------------???
	{
		l = 127 - i;
		temp = ((temp<<1) & 0x00ffffff)<<(8-l);		
		H = 0;
		
		for(m = 0;m<16;m++)
		{
			if(temp&0x80000000)
//			Temp = Temp+(*(FloatToDeci_P+m+l));
			if(m<15)
			{
				temp <<= 1;
			}
		}
		Temp = mod_ss(Temp,10);//???Temp/10,???????????
		//H = Temp;//-----------------------------------------------------???
	}
	else 
	{
		Temp = 0;
	}
     }
	return Temp;
}


void GetTwoPointDis(void)
{	
		uint32_t tempbuf[2]={0};

		if(uSystem.Bit.gps == GPS_VALID)
		{
			TwoPointLocation.fNowLat = LatToRad(GpsInfo.Latitude);
			TwoPointLocation.fNowLong = LonToRad(GpsInfo.Longitude);
		}
		else
		{
			TwoPointLocation.fNowLat  = 39.976478;
			TwoPointLocation.fNowLong =116.339964;
		}
		
		
		tempbuf[0]= *(uint32_t*)(usRegHoldingBuf + 2159) >> 16; //高位
		tempbuf[1]= *(uint32_t*)(usRegHoldingBuf + 2159) & 0xff; // 
		TwoPointLocation.uDesLong = (tempbuf[1] << 16) + tempbuf[0];
		
		tempbuf[0]= *(uint32_t*) (usRegHoldingBuf + 2161) >> 16; //高位
		tempbuf[1]= *(uint32_t*) (usRegHoldingBuf + 2161) & 0xff; // 		
		TwoPointLocation.uDesLat  = (tempbuf[1] << 16) + tempbuf[0];
		

		tempbuf[0]= *(uint32_t*)(usRegHoldingBuf + 2163) >> 16; //高位
		tempbuf[1]= *(uint32_t*)(usRegHoldingBuf + 2163) & 0xff; // 
		TwoPointLocation.WarnDis  = (tempbuf[1] << 16) + tempbuf[0];

		TwoPointLocation.fDesLong  = TwoPointLocation.uDesLong  / 10000.0L;
		TwoPointLocation.fDesLat   = TwoPointLocation.uDesLat   / 10000.0L;
		TwoPointLocation.Distance = (uint32_t)get_distance(TwoPointLocation.fNowLat,TwoPointLocation.fNowLong, TwoPointLocation.fDesLat, TwoPointLocation.fDesLong);

		if(TwoPointLocation.Distance >= TwoPointLocation.WarnDis )
			DeveiceAlarm.Bit.BeyondTheMark = 0;
		else
			DeveiceAlarm.Bit.BeyondTheMark = 0;
		
}

void get2dis(void)
{


	


}
