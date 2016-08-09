/*
	多任务的软件看门狗
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
				// 当 TimerCount == 0 时，软件看门狗溢出  需要复位系统
				Feed_WDG_Flag = DISABLE;
				return;
			}
		}

		SoftTimerCountrPtr++;
	}

}			

// 软件狗状态初始化
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
	SoftTimerCountrPtr += SwdtId;  // 指针指向

	SoftTimerCountrPtr->TimerCount = SoftTimerCountrPtr->TimeOut;

}


// 软件狗挂起
void SuspenWdt(SWDG_ID SwdtId)
{
	SOFT_WATCH_DOG_TIMER *SoftTimerCountrPtr = SoftTimerCountrList;

	if(SwdtId >= MAX_SWDG_ID)
	{
		return;
	}
	SoftTimerCountrPtr += SwdtId;  // 指针指向
	SoftTimerCountrPtr ->RunState = SWDT_STAT_SUSPEN;
}

// 软件狗 运行
void RunWdt(SWDG_ID SwdtId)
{
	SOFT_WATCH_DOG_TIMER *SoftTimerCountrPtr = SoftTimerCountrList;

	if(SwdtId >= MAX_SWDG_ID)
	{
		return;
	}
	SoftTimerCountrPtr += SwdtId;  // 指针指向
	SoftTimerCountrPtr ->RunState = SWDT_STAT_RUN;
}
