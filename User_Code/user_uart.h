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

	 
	 
#define UART1_SEND_QUEUE_LENGTH      512     /* 给UART1发送数据队列分配的空间大小 */
#define UART1_RECV_QUEUE_LENGTH      1024     /* 给UART1接收数据队列分配的空间大小 */
#define UART2_SEND_QUEUE_LENGTH      512     /* 给UART1发送数据队列分配的空间大小 */
#define UART2_RECV_QUEUE_LENGTH      512     /* 给UART1接收数据队列分配的空间大小 */
#define UART3_SEND_QUEUE_LENGTH      512
#define UART3_RECV_QUEUE_LENGTH      512

	 
typedef struct 
{
    uint8_t     *Out;                   /* 指向数据输出位置         */
    uint8_t     *In;                    /* 指向数据输入位置         */
    uint8_t     *End;                   /* 指向Buf的结束位置        */
    uint16_t    NData;                  /* 队列中数据个数           */
    uint16_t    MaxData;                /* 队列中允许存储的数据个数 */     
    uint8_t     (* ReadEmpty)();        /* 读空处理函数             */
    uint8_t     (* WriteFull)();        /* 写满处理函数             */
    uint8_t     Buf[1];                 /* 存储数据的空间           */
} DataQueue; 

	 
	 
extern uint8_t   UART1_TXEN_flag;	        //发送中断标志位关闭
extern uint8_t   UART2_TXEN_flag;	        //发送中断标志位关闭
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
extern BOOL done_info_rx;// 接收完成标志
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


