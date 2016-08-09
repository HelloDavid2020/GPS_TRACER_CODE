#include "user_swdt.h"

SOFT_WATCH_DOG_TIMER SoftWatchDogTimerList[MAX_SWDT_ID] = {0};
BOOL StopWDTFedMake = 0;

void WatchDogInit(void)
{

}

// 喂硬件狗
void FeedDog(void)
{



}

// 
void OSTimeTickHook (void)
{
	SoftWdtISR(); 
}

// check software watchdog status
void SoftWdtISR(void)
{
	SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
	uint8_t i = 0;

	if(StopWDTFedMake == TRUE)
	{
		return;
	}
	
	for(i=0; i<MAX_SWDT_ID; i++)
	{
		if(SoftWatchDogTimerPtr->watchDogState == SWDT_STAT_RUN)
		{
			if(SoftWatchDogTimerPtr->watchDogCount != SoftWatchDogTimerPtr->watchDogCountBack)
			{
				// 保存当前值
				SoftWatchDogTimerPtr->watchDogCountBack = SoftWatchDogTimerPtr->watchDogCount;
				
				// reload 看门狗计数上限 timeout
				SoftWatchDogTimerPtr->watchDogTime = SoftWatchDogTimerPtr->watchDogTimeOut;
			}
			else if(--SoftWatchDogTimerPtr->watchDogTime == 0)
			{ 
				// 当 watchDogTime == 0 时，软件看门狗溢出  需要复位系统
				StopWDTFedMake = TRUE;
				return;
			}
		}

		SoftWatchDogTimerPtr++;
	}

	FeedDog(); // 硬件喂狗
}			

void SoftWDTInit(void)
{

	memset(SoftWatchDogTimerList, 0, sizeof(SOFT_WATCH_DOG_TIMER)*MAX_SWDT_ID);
	StopWDTFedMake = 0;
	

	WatchDogInit( );// 硬件看门狗初始化
}


// SoftWdtInit(KEY_TASK_SWDT_ID,2000);
//SoftWdtFed(KEY_TASK_SWDT_ID);  

BOOL SoftWdtInit(SWDT_ID SwdtId, uint16_t TimerTop)
{
	SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
	uint16_t osTick = 0;

	if(SwdtId >= MAX_SWDT_ID)
	{
		return 0;
	}
	
	if(SoftWatchDogTimerPtr->watchDogState == SWDT_STAT_IDLE)
	{
		 SoftWatchDogTimerPtr += SwdtId;
//		 osTick = MsToOSTicks(TimerTop);//?mS???????????
		 SoftWatchDogTimerPtr->watchDogTimeOut = osTick;
		 SoftWatchDogTimerPtr->watchDogTime = osTick;
		 SoftWatchDogTimerPtr->watchDogState = SWDT_STAT_RUN;

		 return 1;
	}
	else
	{
		 return 0;
	}
}


//void SoftWdtFed(SWDT_ID SwdtId)
//{

//	SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
//	if(SwdtId >= MAX_SWDT_ID)
//	{
//		return;
//	}
//	SoftWatchDogTimerPtr += SwdtId;
//	SoftWatchDogTimerPtr->watchDogCount++;

//	
//	// 避免它们相等
//	if(SoftWatchDogTimerPtr->watchDogCount == SoftWatchDogTimerPtr->watchDogCountBack)
//	{
//		SoftWatchDogTimerPtr->watchDogCount++;
//  }

//}

void SoftWdtFed(SWDT_ID SwdtId)
{

	SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;

	if(SwdtId >= MAX_SWDT_ID)
	{
		return;
	}
	SoftWatchDogTimerPtr += SwdtId;

	SoftWatchDogTimerPtr->watchDogTime = SoftWatchDogTimerPtr->watchDogTimeOut;

}

void SuspenWdt(SWDT_ID SwdtId);
void RunWdt(SWDT_ID SwdtId);
