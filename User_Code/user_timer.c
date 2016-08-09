#include "user_timer.h"


uint32_t Timer2_count=0;


void User_Timer2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  		//����TIM2ʱ��
	TIM_DeInit(TIM2);						 			  		//��λTIM2��������������

	
	// 250ms 
	TIM_TimeBaseStructure.TIM_Period=5;                		//����ֵ���������ڸ��������ж�
	TIM_TimeBaseStructure.TIM_Prescaler= (36 - 1);     		//��Ƶ��,��ÿ36M/36000����һ��
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                 		//�������жϱ�־
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2, ENABLE);                                		//������ʱ��
	
}


/* �ж�����Ϊ1 s */
void User_Timer3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	TIM_DeInit(TIM3);
	
	// 25ms 
	TIM_TimeBaseStructure.TIM_Period=1;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* ʱ��Ԥ��Ƶ�� 36M/36 */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		/* �������жϱ�־ */
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);																		/* ����ʱ�� */

	#ifdef DEBUG
	printf("> TIMER3 init.\r\n");
	#endif	  
		
}

