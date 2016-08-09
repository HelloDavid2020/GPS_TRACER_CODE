#ifndef  _USER_QUEUE_H
#define  _USER_QUEUE_H
 
#include "includes.h"
	 
// #ifndef  uint8_t
// #define  uint8_t         uint8_t		/* ���ݶ����д洢����������     */

// #endif
/********************************************************************************************************/
#ifndef NOT_OK
#define NOT_OK              0xff        /* ��������               */
#endif

#ifndef NULL
#define NULL                   0ul
#endif 

#define QUEUE_OK            1           /* �����ɹ�                */
#define QUEUE_FULL          2           /* ������                  */
#define QUEUE_EMPTY         4           /* ������                  */ 

// typedef struct 
// {
//     uint8_t     *Out;                   /* ָ���������λ��         */
//     uint8_t     *In;                    /* ָ����������λ��         */
//     uint8_t     *End;                   /* ָ��Buf�Ľ���λ��        */
//     uint16_t    NData;                  /* ���������ݸ���           */
//     uint16_t    MaxData;                /* ����������洢�����ݸ��� */     
//     uint8_t     (* ReadEmpty)();        /* ���մ�����             */
//     uint8_t     (* WriteFull)();        /* д��������             */
//     uint8_t     Buf[1];                 /* �洢���ݵĿռ�           */
// } DataQueue; 



/********************************************************************************************************/
#define EN_QUEUE_WRITE            1     /* ��ֹ(0)������(1)FIFO��������       */
#define EN_QUEUE_WRITE_FRONT      1     /* ��ֹ(0)������(1)LIFO��������       */
#define EN_QUEUE_NDATA            1     /* ��ֹ(0)������(1)ȡ�ö���������Ŀ   */
#define EN_QUEUE_SIZE             1     /* ��ֹ(0)������(1)ȡ�ö������������� */
#define EN_QUEUE_FLUSH            1     /* ��ֹ(0)������(1)��ն���           */

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
