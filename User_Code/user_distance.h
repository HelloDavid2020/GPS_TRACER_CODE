#ifndef __USER_DISTANCE_H
#define __USER_DISTANCE_H

#include "includes.h"

#define PI                      3.1415926
#define EARTH_RADIUS            6378.137        //??????



typedef struct 
{
	 char 	DesLatBuffer[11];
	 char 	DesLonBuffer[11];
	
	float  fNowLat;
	float  fNowLong;
	
	float  fDesLattemp;
	float  fDesLongtemp;	

	float  fDesLat;
	float  fDesLong;
	
	uint32_t  uNowLat;
	uint32_t  uNowLong;

	uint32_t  uDesLat;
	uint32_t  uDesLong;	
	
	uint32_t  DisPerLat;
	uint32_t  DisPerLon;

	double  SquareX; // x square
	double  SquareY;
	
	double  x;
	double  y;
	
	double  Distance;
	
	double WarnDis;
	
	
}TWO_POINT_LOCATION;
extern TWO_POINT_LOCATION TwoPointLocation;

extern double lat1;
extern double lng1;
extern double lat2;
extern double lng2;


extern uint32_t distPerDegree[46][2];

extern void GetTwoPointDis(void);

extern double radian(double d);
extern double get_distance(double lat1, double lng1, double lat2, double lng2);
extern void get2dis(void);

#endif



