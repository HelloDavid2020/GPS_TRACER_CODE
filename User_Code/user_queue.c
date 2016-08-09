#include "user_queue.h"

/*********************************************************************************************************
** ��������: QueueCreate
** ��������: ��ʼ�����ݶ���
** ��  ����: Buf      ��Ϊ���з���Ĵ洢�ռ��ַ
**           SizeOfBuf��Ϊ���з���Ĵ洢�ռ��С���ֽڣ�
**           ReadEmpty��Ϊ���ж���ʱ�������
**           WriteFull��Ϊ����д��ʱ�������
** ��  ����: NOT_OK:��������
**           QUEUE_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8_t QueueCreate(void *Buf, uint32_t SizeOfBuf, uint8_t (* ReadEmpty)(), uint8_t (* WriteFull)())
{ 
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
	    DataQueue *Queue;
    
    if (Buf != NULL && SizeOfBuf >= (sizeof(DataQueue)))        /* �жϲ����Ƿ���Ч */
    {
        Queue = (DataQueue *)Buf;

// 			__disable_irq();
                                                                /* ��ʼ���ṹ������ */
        Queue->MaxData = (SizeOfBuf - (uint32_t)(((DataQueue *)0)->Buf)) / 
                         sizeof(uint8_t);               /* ������п��Դ洢��������Ŀ */
        Queue->End = Queue->Buf + Queue->MaxData;               /* �������ݻ���Ľ�����ַ */
        Queue->Out = Queue->Buf;
        Queue->In = Queue->Buf;
        Queue->NData = 0;
        Queue->ReadEmpty = ReadEmpty;							/* ���ж��մ����������û����� */
        Queue->WriteFull = WriteFull;							/* ����д�������������û����� */

//         __enable_irq();

        return QUEUE_OK;
    }
    else
    {
        return NOT_OK;
    }
}	

/*********************************************************************************************************
** ��������: QueueRead
** ��������: ��ȡ�����е�����
** ��  ����: Ret:�洢���ص���Ϣ�ĵ�ַ
**           Buf:ָ����е�ָ��
** ��  ����: NOT_OK     ����������
**           QUEUE_OK   ���յ���Ϣ
**           QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
********************************************************************************************************/
uint8_t QueueRead(uint8_t *Ret, void *Buf)
{
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
    uint8_t err;
    DataQueue *Queue;

    err = NOT_OK;
    if (Buf != NULL)                                            /* �����Ƿ���Ч */
    {                                                           /* ��Ч */
        Queue = (DataQueue *)Buf;
        
//         __disable_irq();
        
        if (Queue->NData > 0)                                   /* �����Ƿ�Ϊ�� */
        {                                                       /* ����         */
            *Ret = Queue->Out[0];                               /* ���ݳ���     */
            Queue->Out++;                                       /* ��������ָ�� */
            if (Queue->Out >= Queue->End)
            {
                Queue->Out = Queue->Buf;
            }
            Queue->NData--;                                     /* ���ݼ���      */
            err = QUEUE_OK;
        }
        else
        {                                                       /* ��              */
            err = QUEUE_EMPTY;
            if (Queue->ReadEmpty != NULL)                       /* �����û������� */
            {
                err = Queue->ReadEmpty(Ret, Queue);
            }
        }
//         __enable_irq();
    }
    return err;
}

/*********************************************************************************************************
** ��������: QueueReadOnly
** ��������: ��ȡ�����е�����,�����ı�ԭ���ݶ���
** ��  ����: Ret:�洢���ص���Ϣ�ĵ�ַ
**           Buf:ָ����е�ָ��
** ��  ����: NOT_OK     ����������
**           QUEUE_OK   ���յ���Ϣ
**           QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
********************************************************************************************************/
uint8_t QueueReadOnly(uint8_t *Ret, void *Buf)
{
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
    uint8_t err;
    DataQueue *Queue;

    err = NOT_OK;
    if (Buf != NULL)                                            /* �����Ƿ���Ч */
    {                                                           /* ��Ч */
        Queue = (DataQueue *)Buf;
        
//         __disable_irq();
        
        if (Queue->NData > 0)                                   /* �����Ƿ�Ϊ�� */
        {                                                       /* ����         */
            *Ret = Queue->Out[0];                               /* ���ݳ���     */
            err = QUEUE_OK;
        }
        else
        {                                                       /* ��              */
            err = QUEUE_EMPTY;
        }
        
//         __enable_irq();
    }
    return err;
}

/*********************************************************************************************************
** ��������: QueueWrite
** ��������: FIFO��ʽ��������
** ��  ����: Buf :ָ����е�ָ��
**           Data:��Ϣ����
** ��  ����: NOT_OK   :��������
**           QUEUE_FULL:������
**           QUEUE_OK  :���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
********************************************************************************************************/
#if EN_QUEUE_WRITE > 0 
uint8_t QueueWrite(void *Buf, uint8_t Data)
{
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
    uint8_t err;
    DataQueue *Queue;

    err = NOT_OK;
    if (Buf != NULL)                                                    /* �����Ƿ���Ч */
    {
        Queue = (DataQueue *)Buf;
		
//         __disable_irq();
        
        if (Queue->NData < Queue->MaxData)                              /* �����Ƿ���  */
        {                                                               /* ����        */
            Queue->In[0] = Data;                                        /* �������    */
            Queue->In++;                                                /* �������ָ��*/
            if (Queue->In >= Queue->End)
            {
                Queue->In = Queue->Buf;
            }
            Queue->NData++;                                             /* ��������    */
            err = QUEUE_OK;
        }
        else
        {                                                               /* ��           */
            err = QUEUE_FULL;
            if (Queue->WriteFull != NULL)                               /* �����û������� */
            {
                err = Queue->WriteFull(Queue, Data);
            }
        }
//         __enable_irq();
    }
    return err;
}
#endif

/*********************************************************************************************************
** ��������: QueueWriteFront
** ��������: LIFO��ʽ��������
** ��  ����: Buf:ָ����е�ָ��
**           Data:��Ϣ����
** �䡡  ��: QUEUE_FULL:������
**           QUEUE_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
********************************************************************************************************/
#if EN_QUEUE_WRITE_FRONT > 0	   
uint8_t QueueWriteFront(void *Buf, uint8_t Data)
{
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
    uint8_t err;
    DataQueue *Queue;

    err = NOT_OK;
    if (Buf != NULL)                                                    /* �����Ƿ���Ч */
    {
        Queue = (DataQueue *)Buf;
        
//         __disable_irq();
        
        if (Queue->NData < Queue->MaxData)                              /* �����Ƿ���  */
        {                                                               /* ���� */
            Queue->Out--;                                               /* ��������ָ�� */
            if (Queue->Out < Queue->Buf)
            {
                Queue->Out = Queue->End - 1;
            }
            Queue->Out[0] = Data;                                       /* �������     */
            Queue->NData++;                                             /* ������Ŀ���� */
            err = QUEUE_OK;
        }
        else
        {                                                               /* ��           */
            err = QUEUE_FULL;
            if (Queue->WriteFull != NULL)                               /* �����û������� */
            {
                err = Queue->WriteFull(Queue, Data);
            }
        }
//         __enable_irq();
    }
    return err;
}
#endif

/*********************************************************************************************************
** ��������: QueueNData
** ��������: ȡ�ö�����������
** ��  ����: Buf:ָ����е�ָ��
** ��  ����: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
********************************************************************************************************/
#if EN_QUEUE_NDATA > 0					  
uint16_t QueueNData(void *Buf)
{
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
    uint16_t temp;
    
    temp = 0;                                                   /* ������Ч����0 */
    if (Buf != NULL)
    {
//         __disable_irq();
        temp = ((DataQueue *)Buf)->NData;
//         __enable_irq();
    }
    return temp;
} 
#endif	

/*********************************************************************************************************
** ��������: QueueSize
** ��������: ȡ�ö���������
** ��  ����: Buf:ָ����е�ָ��
** ��  ����: ����������
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
********************************************************************************************************/ 
#if EN_QUEUE_SIZE > 0				   
uint16_t QueueSize(void *Buf)
{
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
    uint16_t temp;
    
    temp = 0;                                                   /* ������Ч����0 */
    if (Buf != NULL)
    {
//         __disable_irq();
        temp = ((DataQueue *)Buf)->MaxData;
//         __enable_irq();
    }
    return temp;
}	  
#endif

/*********************************************************************************************************
** ��������: OSQFlush
** ��������: ��ն���
** �䡡  ��: Buf:ָ����е�ָ��
** ��  ����: ��
** ȫ�ֱ���: ��
** ����ģ��: __disable_irq,__enable_irq
********************************************************************************************************/
#if EN_QUEUE_FLUSH > 0								 
void QueueFlush(void *Buf)
{
// #if OS_CRITICAL_METHOD == 3         
// 	OS_CPU_SR  cpu_sr;
// #endif
    DataQueue *Queue;
    
    if (Buf != NULL)                                                /* �����Ƿ���Ч */
    {                                                               /* ��Ч         */
        Queue = (DataQueue *)Buf;
//         __disable_irq();
        Queue->Out = Queue->Buf;
        Queue->In = Queue->Buf;
        Queue->NData = 0;                                           /* ������ĿΪ0 */
//         __enable_irq();
    }
} 
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
