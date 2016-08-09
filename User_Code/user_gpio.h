#ifndef _USER_GPIO_H_
#define _USER_GPIO_H_

#include "stm32f10x.h"
#include "user_timedelay.h"

#include <stdbool.h>


#define I0_0_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0 )
#define I0_1_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define I0_2_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2 )
#define I0_3_STATUS		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3 )
#define I0_4_STATUS		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4 )
#define I0_5_STATUS		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5 )
#define I0_6_STATUS		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)
#define I0_7_STATUS		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8)
#define I0_8_STATUS		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)


#define I1_0_STATUS		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1 )
#define I1_1_STATUS		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9 )
#define I1_2_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6 )
#define I1_3_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7 )

// Q0.0 -- PA3
// Q0.1 -- PA2
// Q0.2 -- PA1
// Q0.3 -- PA0
// Q0.4 -- PB1
// Q0.5 -- PB0
// Q0.6 -- PD0
// Q0.7 -- PC13

#define Q0_0 0
#define Q0_1 1
#define Q0_2 2
#define Q0_3 3
#define Q0_4 4
#define Q0_5 5
#define Q0_6 6
#define Q0_7 7


//  EXT_OUT4---LED_RED  	--- PC9
//  EXT_OUT3---BEEP  			--- PC8
//  EXT_OUT2---LED_GREEN  --- PC7
//  EXT_OUT1---LED_BLUE  	--- PC6
#define EXT_INT_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define EXT_INT_LOW()  GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define BLE_KEY_SET()  GPIO_SetBits(GPIOE, GPIO_Pin_11)
#define BLE_KEY_RESET()  GPIO_ResetBits(GPIOE, GPIO_Pin_11)


#define LED_PORT		GPIOC
#define LED_RED     GPIO_Pin_15
#define LED_GREEN   GPIO_Pin_3
#define LED_BLUE    GPIO_Pin_0
#define BEEP        GPIO_Pin_8

#define BEEP_ON()        GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define BEEP_OFF()       GPIO_SetBits(GPIOC, GPIO_Pin_8)



#define COMM_POWER_ON   GPIO_SetBits(GPIOD, GPIO_Pin_0)
#define COMM_POWER_OFF	GPIO_ResetBits(GPIOD, GPIO_Pin_0)

#define LED_RED_ON  GPIO_ResetBits(GPIOA, LED_RED)
#define LED_RED_OFF GPIO_SetBits(GPIOA, LED_RED)

#define LED_BLUE_ON  GPIO_ResetBits(LED_PORT, LED_BLUE)
#define LED_BLUE_OFF GPIO_SetBits(LED_PORT, LED_BLUE)

#define LED_GREEN_ON  GPIO_ResetBits(LED_PORT, LED_GREEN)
#define LED_GREEN_OFF GPIO_SetBits(LED_PORT, LED_GREEN)

#define LED_RED_TOGGLE()    GPIO_Toggle(LED_PORT,LED_RED)
#define LED_GREEN_TOGGLE()  GPIO_Toggle(LED_PORT,LED_GREEN)


#define ACC_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
#define MOTO_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
//#define SHELL_STATUS	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
//#define ANT_STATUS		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)

#define IO1_STATUS		GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)
#define IO2_STATUS		GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)
#define IO3_STATUS		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define IO4_STATUS		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)


//typedef struct 
//{
//	bool In0_0;
//	bool In0_1;
//	bool In0_2;
//	bool In0_3;
//	bool In0_4;
//	bool In0_5;	
//	bool In0_6;
//	bool In0_7;	
//	bool In1_0;
//	bool In1_1;	
//	bool In1_2;	
//	bool In1_3;	
//}PORT_INPUT ;
	


	
typedef union 
{
	struct
	{
		uint16_t In0_0:1;  // M
		uint16_t In0_1:1;  //¡ý
		uint16_t In0_2:1;  // PWR
		uint16_t In0_3:1;  // F1
		uint16_t In0_4:1;  // F2
		uint16_t In0_5:1;	 // F3
		uint16_t In0_6:1;  // A
		uint16_t In0_7:1;	 // B
		uint16_t In0_8:1;	 // C
		uint16_t In1_0:1;
		uint16_t In1_1:1;	
		uint16_t In1_2:1;	
		uint16_t In1_3:1;
		uint16_t rsd:3;

	} Bit;		
	uint16_t Value;
} PORT_INPUT;

	
typedef union 
{
	struct
	{
		uint8_t Q0 :1;
		uint8_t Q1 :1;
		uint8_t Q2 :1;
		uint8_t Q3 :1;
		uint8_t Q4 :1;
		uint8_t Q5 :1;
		uint8_t Q6 :1;
		uint8_t Q7 :1;
	} Bit;		
	uint8_t Value;
}PORT_OUTPUT;


extern PORT_INPUT   ExInput;
extern PORT_OUTPUT  ExOutput;

extern void exit_wake_up(void);
extern void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void User_GpioInit(void); 

extern void BSP_GPIO_IO1_Set(uint8_t enabled);
extern void BSP_GPIO_IO2_Set(uint8_t enabled);
extern void BSP_GPIO_IO3_Set(uint8_t enabled);
extern void BSP_GPIO_IO4_Set(uint8_t enabled);
extern void LED_SystemRun(void);

extern void OutputControl(PORT_OUTPUT Output);
extern void DetectPortInput(void);

#endif


