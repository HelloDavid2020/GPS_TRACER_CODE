#ifndef _COMMON_H_ 
#define _COMMON_H_ 

#include "includes.h"


extern const uint16_t Table_GBK2UCS2_0xA1_0xA9[];
extern const uint16_t Table_GBK2UCS2_0XB0_0XF7[];

extern void data2ascii(char * des, char * src, int size);
extern void ascii2data(char * des, char * src, int size);

int8u  Compstr(char *str1,char *str2, int8u num);
int16u Encode_GBK2_UCS2(const char* pSource, int16u nSourceLen, char* pDest,int16u nDestLen);

#endif	  //_COMMON_H_ 
