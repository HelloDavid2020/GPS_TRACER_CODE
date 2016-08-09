 #include "includes.h"

// Èí¼þ ¿´ÃÅ¹·×´Ì¬ 
//typedef enum
//{
//	SWDT_IDLE,   
//	SWDT_SUSPEN, 
//	SWDT_RUN,
//	SWDT_ERR,
//}SWDT_STAT;
typedef enum{
        SWDT_STAT_IDLE,                                //???????
        SWDT_STAT_SUSPEN,                        //???????
        SWDT_STAT_RUN                                //???????
}SWDT_STAT;
 
typedef struct
{
	uint32_t CurCnt, LastCnt;
	BOOL RunState;
	uint32_t taskID;
} STRUCT_WATCH_DOG;

typedef struct soft_wach_dog_timer{
        uint16_t watchDogTimeOut;                //?????????
        uint16_t watchDogTime;                //??????
        uint8_t  watchDogCountBack;        //??????????
        uint8_t  watchDogCount;                //????????
        SWDT_STAT  watchDogState;        //????????
        uint8_t  NOUSE8;
}SOFT_WATCH_DOG_TIMER;


typedef enum{//??????????ID
        KEY_TASK_SWDT_ID,
        MP3_TASK_SWDT_ID,
        MAX_SWDT_ID
}SWDT_ID;


extern void FeedDog(void);
extern void SoftWdtISR(void);




