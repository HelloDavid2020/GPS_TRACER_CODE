#include "user_i2c.h"


void I2C_delay(void)
{        
	// 20		- 330K
	// 40	 	- 200K ���β���
	// 50	 	- 132K
	// 80 	- 100K ��������
	// 100	- 80k  ��������
	
  uint8_t i=100; 
  while(i--) 
	{
		__nop();
	}
}

/**************************************
*	I2Cstart: keep SCL high, SDA high to low
*           ___________
*	SCL:                 \_____
*           ______
*	SDA:            \__________
*	��������ʱ:SDA,SCL��Ϊ�͵�ƽ
***************************************/
uint8_t I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_READ)return FALSE;  
	SDA_L;
	I2C_delay();
	if(SDA_READ)return FALSE;  

	SCL_L;
	return TRUE;
}


/**************************************
*	I2Cstop:keep SCL high, SDA low to high
*           ________________
*	SCL:
*                    ______
*	SDA:      ________/
*	��������ʱ:SDA,SCL��Ϊ�ߵ�ƽ
***************************************/
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
}


/**************************************
*	I2C �����ͷ�
*	����9��clk,�ڷ���stop�ź�
**************************************/

void I2C_Release(void)
{
	uint8_t i =9;
	
	while(i--)
	{
		SCL_H;
		I2C_delay();				
		SCL_L;
		I2C_delay();
	}
	I2C_Stop();
}



/**************************************
*   SDA keep low, SCL low->high->low
*           ______
*	SCL _____/      \______
*	SDA ___________________
*	��������ʱ:SDA,SCL��Ϊ�͵�ƽ
**************************************/
void I2C_Ack(void)
{      
	SCL_L;
	I2C_delay();
  
	SDA_L;
	I2C_delay();
	SCL_H;
	
	I2C_delay();
	SCL_L;
	I2C_delay();
}


/**************************************
*   SDA keep high,SCL low->high->low
*             ______
*   SCL _____/      \______
*       ___________________
*   SDA
*	��������ʱ:SDAΪ�ߵ�ƽ,SCLΪ�͵�ƽ
**************************************/
void I2C_NoAck(void)
{    
	SCL_L;
	I2C_delay();
	
	SDA_H;
	
	I2C_delay();
	SCL_H;
	
	I2C_delay();
	SCL_L;
	I2C_delay();
}

uint8_t I2C_WaitAck(void)         
{
	SCL_L;
	I2C_delay();
	
	SDA_H;                        
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_READ)
	{
		SCL_L;
		return FALSE;
	}
	SCL_L;
	return TRUE;
}

void I2C_SendByte(uint8_t SendByte) 
{
	u8 i=8;
	while(i--)
	{
		SCL_L;
		I2C_delay();		
		
		if(SendByte&0x80)
			SDA_H;  
		else 
			SDA_L; 		
		
		SendByte<<=1;

		I2C_delay();
		
		SCL_H;
		I2C_delay();
	}
	 SCL_L;

}


uint8_t I2C_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;                                
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
          SCL_H;
      I2C_delay();        
      if(SDA_READ)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}
