#ifndef __USER_UART_H
#define __USER_UART_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "includes.h"
	 
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

	 
	 
#define UART1_SEND_QUEUE_LENGTH      512     /* ��UART1�������ݶ��з���Ŀռ��С */
#define UART1_RECV_QUEUE_LENGTH      1024     /* ��UART1�������ݶ��з���Ŀռ��С */
#define UART2_SEND_QUEUE_LENGTH      512     /* ��UART1�������ݶ��з���Ŀռ��С */
#define UART2_RECV_QUEUE_LENGTH      512     /* ��UART1�������ݶ��з���Ŀռ��С */
#define UART3_SEND_QUEUE_LENGTH      512
#define UART3_RECV_QUEUE_LENGTH      512

	 
typedef struct 
{
    uint8_t     *Out;                   /* ָ���������λ��         */
    uint8_t     *In;                    /* ָ����������λ��         */
    uint8_t     *End;                   /* ָ��Buf�Ľ���λ��        */
    uint16_t    NData;                  /* ���������ݸ���           */
    uint16_t    MaxData;                /* ����������洢�����ݸ��� */     
    uint8_t     (* ReadEmpty)();        /* ���մ�����             */
    uint8_t     (* WriteFull)();        /* д��������             */
    uint8_t     Buf[1];                 /* �洢���ݵĿռ�           */
} DataQueue; 

	 
	 
extern uint8_t   UART1_TXEN_flag;	        //�����жϱ�־λ�ر�
extern uint8_t   UART2_TXEN_flag;	        //�����жϱ�־λ�ر�
extern uint8_t   UART3_TXEN_flag;

extern uint16_t usart2_rx_len;
extern BOOL flag_usart2_rx;
extern uint16_t usart1_rx_len;
extern BOOL flag_usart1_rx;
extern BOOL flag_usart1_rx_done;

extern uint16_t usart3_rx_len;
extern BOOL flag_usart3_rx;
extern BOOL flag_usart3_rx_done;

extern uint8_t info_rx[1024];
extern uint16_t info_rx_len;//
extern BOOL flag_info_rx;//

extern uint8_t csq_rx[1024];
extern uint16_t csq_rx_len ;//
extern BOOL flag_csq_rx ;//

extern BOOL en_info_rx;//
extern BOOL done_info_rx;// ������ɱ�־
extern BOOL flag_info_dis;
extern uint8_t  UART1SendBuf[UART1_SEND_QUEUE_LENGTH/sizeof(uint8_t)];
extern uint8_t  UART1RecvBuf[UART1_RECV_QUEUE_LENGTH/sizeof(uint8_t)];
extern uint8_t  UART2SendBuf[UART2_SEND_QUEUE_LENGTH/sizeof(uint8_t)];
extern uint8_t  UART2RecvBuf[UART2_RECV_QUEUE_LENGTH/sizeof(uint8_t)];
extern uint8_t  UART3SendBuf[UART3_SEND_QUEUE_LENGTH/sizeof(uint8_t)];
extern uint8_t  UART3RecvBuf[UART3_RECV_QUEUE_LENGTH/sizeof(uint8_t)];


extern void USART1_Init(uint32_t Baudrate);
extern void USART2_Init(uint32_t Baudrate);
extern void USART3_Init(uint32_t Baudrate);

extern void User_ComInit(uint8_t ComNum, uint32_t BaudRate);
// extern void UARTInit(void);
extern void QueueWriteStr(void *buf, char *str);
extern uint8_t UartWriteFull(DataQueue *Buf, uint8_t Data);		 
	 
extern PUTCHAR_PROTOTYPE;
extern void UART_sendPacket(USART_TypeDef* USARTx, uint8_t *buffer, uint8_t len);	 

extern void SerialPutChar(uint8_t c);
extern void Serial_PutString(uint8_t *s);



	 
#ifdef __cplusplus
}
#endif	 	 
	 
#endif	 


