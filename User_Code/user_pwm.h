#ifndef __USER_PWM_H
#define __USER_PWM_H
#include "includes.h"

extern uint16_t pwm_frequency;
extern uint16_t pwm_duty;

typedef struct 
{
	uint32_t PwmOut1Freq;
	uint16_t PwmOut1Duty;
	uint16_t PwmOut2Duty;
	uint16_t PwmOut3Duty;
	uint16_t PwmOut4Duty;
}PWM_OUTPUT;

extern PWM_OUTPUT PwmOutput;
//typedef struct 
//{
//	uint16_t IC1Value;
//	uint16_t IC2Value;
//		
//	uint16_t DutyCycle;
//	uint16_t Frequency;
//}PWM_INPUT;

//extern PWM_INPUT ExPWM_Input;
extern uint16_t TIM4_ETR_Counts;
extern uint16_t TIM4_ETR_GetDropCounts;
//extern uint16_t Last_TIM4_ETR_GetDropCounts; // 当前计数
//extern uint16_t Prve_TIM4_ETR_GetDropCounts; // 前一计数

extern uint32_t ExPwmInputFrequency;

extern void UserPwmOutInit(uint16_t frequency, uint16_t duty);
extern void User_PWM_Init(uint16_t frequency, uint16_t duty);

extern void User_PWM_Capture_Init(void);

extern void PWM_Output(uint16_t frequency, uint16_t duty);
extern void User_PWM_Handler(void);

extern void ExPwmInputGpio(void);
#endif


