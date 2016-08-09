/*
	�������������Ź�
*/

#include "user_swdg.h"

SOFT_WATCH_DOG_TIMER SoftTimerCountrList[MAX_SWDG_ID] = {0};
BOOL Feed_WDG_Flag = ENABLE;

// 
//void OSTimeTickHook (void)
//{
//	SoftWdgHandler(); 
//}

// check software watchdog status
void SoftWdgHandler(void)
{
	SOFT_WATCH_DOG_TIMER *SoftTimerCountrPtr = SoftTimerCountrList;
	uint8_t i = 0;

	if(Feed_WDG_Flag == DISABLE)
	{
		return;
	}
	
	for(i=0; i<MAX_SWDG_ID; i++)
	{
		if(SoftTimerCountrPtr->RunState == SWDT_STAT_RUN)
		{
			SoftTimerCountrPtr->TimerCount--;
			if( SoftTimerCountrPtr->TimerCount == 0 )
			{ 
				// �� TimerCount == 0 ʱ��������Ź����  ��Ҫ��λϵͳ
				Feed_WDG_Flag = DISABLE;
				return;
			}
		}

		SoftTimerCountrPtr++;
	}

}			

// �����״̬��ʼ��
void SoftWdgStateInit(void)
{
	memset(SoftTimerCountrList, 0, sizeof(SOFT_WATCH_DOG_TIMER)*MAX_SWDG_ID);
	Feed_WDG_Flag = ENABLE;  // 
}


// SoftWDG_Init(KEY_TASK_SWDG_ID,2000);
//SoftWdgFed(KEY_TASK_SWDG_ID);  

BOOL SoftWdtInit(SWDG_ID SwdtId, uint16_t timeout)
{
	SOFT_WATCH_DOG_TIMER *SoftTimerCountrPtr = SoftTimerCountrList;

	SoftTimerCountrPtr += SwdtId;
	
	if(SwdtId >= MAX_SWDG_ID)
	{
		return 0;
	}
	
	if(SoftTimerCountrPtr->RunState == SWDT_STAT_IDLE)
	{
//		 SoftTimerCountrPtr += SwdtId;
		
		 SoftTimerCountrPtr->TimeOut = timeout;
		 SoftTimerCountrPtr->TimerCount = SoftTimerCountrPtr->TimeOut;
		 SoftTimerCountrPtr->RunState = SWDT_STAT_RUN;

		 return 1;
	}
	else
	{
		 return 0;
	}
}


void FeedSoftWdg(SWDG_ID SwdtId)
{

	SOFT_WATCH_DOG_TIMER *SoftTimerCountrPtr = SoftTimerCountrList;

	if(SwdtId >= MAX_SWDG_ID)
	{
		return;
	}
	SoftTimerCountrPtr += SwdtId;  // ָ��ָ��

	SoftTimerCountrPtr->TimerCount = SoftTimerCountrPtr->TimeOut;

}


// ���������
void SuspenWdt(SWDG_ID SwdtId)
{
	SOFT_WATCH_DOG_TIMER *SoftTimerCountrPtr = SoftTimerCountrList;

	if(SwdtId >= MAX_SWDG_ID)
	{
		return;
	}
	SoftTimerCountrPtr += SwdtId;  // ָ��ָ��
	SoftTimerCountrPtr ->RunState = SWDT_STAT_SUSPEN;
}

// ����� ����
void RunWdt(SWDG_ID SwdtId)
{
	SOFT_WATCH_DOG_TIMER *SoftTimerCountrPtr = SoftTimerCountrList;

	if(SwdtId >= MAX_SWDG_ID)
	{
		return;
	}
	SoftTimerCountrPtr += SwdtId;  // ָ��ָ��
	SoftTimerCountrPtr ->RunState = SWDT_STAT_RUN;
}
