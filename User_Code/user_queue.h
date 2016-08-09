#ifndef  _USER_QUEUE_H
#define  _USER_QUEUE_H
 
#include "includes.h"
	 
// #ifndef  uint8_t
// #define  uint8_t         uint8_t		/* 数据队列中存储的数据类型     */

// #endif
/********************************************************************************************************/
#ifndef NOT_OK
#define NOT_OK              0xff        /* 参数错误               */
#endif

#ifndef NULL
#define NULL                   0ul
#endif 

#define QUEUE_OK            1           /* 操作成功                */
#define QUEUE_FULL          2           /* 队列满                  */
#define QUEUE_EMPTY         4           /* 无数据                  */ 

// typedef struct 
// {
//     uint8_t     *Out;                   /* 指向数据输出位置         */
//     uint8_t     *In;                    /* 指向数据输入位置         */
//     uint8_t     *End;                   /* 指向Buf的结束位置        */
//     uint16_t    NData;                  /* 队列中数据个数           */
//     uint16_t    MaxData;                /* 队列中允许存储的数据个数 */     
//     uint8_t     (* ReadEmpty)();        /* 读空处理函数             */
//     uint8_t     (* WriteFull)();        /* 写满处理函数             */
//     uint8_t     Buf[1];                 /* 存储数据的空间           */
// } DataQueue; 



/********************************************************************************************************/
#define EN_QUEUE_WRITE            1     /* 禁止(0)或允许(1)FIFO发送数据       */
#define EN_QUEUE_WRITE_FRONT      1     /* 禁止(0)或允许(1)LIFO发送数据       */
#define EN_QUEUE_NDATA            1     /* 禁止(0)或允许(1)取得队列数据数目   */
#define EN_QUEUE_SIZE             1     /* 禁止(0)或允许(1)取得队列数据总容量 */
#define EN_QUEUE_FLUSH            1     /* 禁止(0)或允许(1)清空队列           */

extern uint8_t  QueueCreate  (void *Buf,uint32_t SizeOfBuf,uint8_t (* ReadEmpty)(),uint8_t (* WriteFull)());
extern uint8_t  QueueRead(uint8_t *Ret, void *Buf);
extern uint8_t  QueueReadOnly(uint8_t *Ret, void *Buf);        
extern uint8_t  QueueWrite(void *Buf, uint8_t Data);
extern uint8_t  QueueWriteFront(void *Buf, uint8_t Data);
extern uint16_t QueueNData(void *Buf); 
extern uint16_t QueueSize(void *Buf);
extern void   	QueueFlush(void *Buf);

#endif	
/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/
