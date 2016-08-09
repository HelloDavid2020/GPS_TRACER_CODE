#include "user_uart.h"0

uint16_t usart1_rx_len =0;//
BOOL flag_usart1_rx =0;//
BOOL flag_usart1_rx_done=0;//

uint16_t usart2_rx_len =0;//
BOOL flag_usart2_rx =0;//

uint8_t info_rx[1024]={0};
uint16_t info_rx_len =0;//

uint8_t csq_rx[1024]={0};
uint16_t csq_rx_len =0;//
BOOL flag_csq_rx =0;//

BOOL flag_info_rx =0;//
BOOL en_info_rx=0;  // 允许接收标志
BOOL done_info_rx=0;// 接收完成标志
BOOL flag_info_dis=0;//lcd显示标志

uint16_t usart3_rx_len=0;
BOOL flag_usart3_rx=0;
BOOL flag_usart3_rx_done=0;


uint8_t  UART1SendBuf[UART1_SEND_QUEUE_LENGTH/sizeof(uint8_t)] = {0};
uint8_t  UART1RecvBuf[UART1_RECV_QUEUE_LENGTH/sizeof(uint8_t)] = {0};
uint8_t  UART2SendBuf[UART2_SEND_QUEUE_LENGTH/sizeof(uint8_t)] = {0};
uint8_t  UART2RecvBuf[UART2_RECV_QUEUE_LENGTH/sizeof(uint8_t)] = {0};
uint8_t  UART3SendBuf[UART3_SEND_QUEUE_LENGTH/sizeof(uint8_t)] = {0};
uint8_t  UART3RecvBuf[UART3_RECV_QUEUE_LENGTH/sizeof(uint8_t)] = {0};

uint8_t   UART1_TXEN_flag = 0;	   //发送中断标志位关闭
uint8_t   UART2_TXEN_flag = 0;
uint8_t   UART3_TXEN_flag = 0;

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	
	USART1->SR; //
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}



void UART_sendPacket(USART_TypeDef* USARTx, uint8_t *buffer, uint8_t len)
{

	uint8_t	sendIndex	=len;
	uint8_t	*pData	=NULL;
	pData = buffer;
	
	while(sendIndex--)
	{	
		USARTx->SR; //
		USART_SendData(USARTx,*pData++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}



// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
/*******************************************************************************
  * @函数名称	SerialPutChar
  * @函数说明   串口发送一个字符
  * @输入参数   C:需发送的字符
  * @输出参数   i无
  * @返回参数   无
*******************************************************************************/
void SerialPutChar(uint8_t c)
{
		USART2->SR; // 防止第一次发送时的乱码

    USART_SendData(USART2, c);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {
    }
}

/*******************************************************************************
  * @函数名称	SerialPutChar
  * @函数说明   串口发送一个字符串
  * @输入参数   *s:需发送的字符串
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void Serial_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
        SerialPutChar(*s);
        s++;
    }
}
// ▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇





/********************************************************************************************************
**函数信息 ：Uart1WriteFull                       // WAN.CG // 2014.4.16
**功能描述 ：UART1数据队列写满处理程序 
**输入参数 ：Buf:指向数据队列
**           Data:将要入队的数据
**输出参数 ：TRUE  :成功
**           FALSE:失败
**调用提示 ：
********************************************************************************************************/
uint8_t UartWriteFull(DataQueue *Buf, uint8_t Data)
{
    uint8_t temp;
    QueueRead(&temp,(void *)Buf);                   /*数据队列满，将最先接收的数据覆盖*/

    return QueueWrite((void *)Buf, Data);           /* 数据重新入队 */
}
/*********************************************************************************************************
**函数信息 ：QueueWriteStr	                 // WAN.CG // 2014.4.16
** 功能描述: 向指定的数据队列写入字符串
** 输  　入: Data:要写入的数据的指针
** 输　  出: 无
** 全局变量: 无
** 调用模块: QueueWrite
********************************************************************************************************/
void QueueWriteStr(void *buf, char *str)
{  
   while(*str!='\0')	
   {	
      	 QueueWrite(buf, *str);      /* 数据入队 */		
		 str++;
   }
}
void USART1_Init(uint32_t Baudrate)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
// 	NVIC_InitTypeDef         NVIC_InitStructure;
	USART_InitTypeDef        USART_InitStructure;
	
	
	/* Enable USART1, GPIOA and AFIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	/* Enable USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_DeInit(USART1);
	
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);


	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
// USART_ITConfig(USART1, USART_IT_TXE | USART_IT_RXNE, ENABLE);




//	//创建UART1发送数据队列
//	QueueCreate((void *)UART1SendBuf, sizeof(UART1SendBuf), NULL, (uint8_t (*)())UartWriteFull);
// 	//创建UART1接收数据队列
//	QueueCreate((void *)UART1RecvBuf, sizeof(UART1RecvBuf), NULL, (uint8_t (*)())UartWriteFull);


/* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}



void USART2_Init(uint32_t Baudrate)
{

	GPIO_InitTypeDef         GPIO_InitStructure;
// 	NVIC_InitTypeDef         NVIC_InitStructure;
	USART_InitTypeDef        USART_InitStructure;
//	DMA_InitTypeDef DMA_InitStructure;
	
  /* Enable USART2, GPIOA and AFIO clocks */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* Enable the USART2 Pins Software Remapping */
//   GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);



//	/* Configure USART2 Tx (PD5) as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);	

//  /* Configure USART2 Rx (PD6) as input floating */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);




///* Enable DMA1 clock */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	DMA_DeInit(DMA1_Channel6); 
//	/* Periph addr is usart rx data register address  */
//	DMA_InitStructure.DMA_PeripheralBaseAddr    = USART2_BASE+4;
//	DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)gps_buffer;
//	DMA_InitStructure.DMA_BufferSize            = NMEA_COUNT_MAX;

//	/* Usart rx data reg need not increment, rx buffer nned */
//	DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
//	/* Once transfer one byte and using cycle mode  */
//	DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;           
//	/* Using highest priority, transfer direction is perihp to memory(USART to memory)  */    
//	DMA_InitStructure.DMA_Priority              = DMA_Priority_VeryHigh;
//	DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
//	DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
//	DMA_Init(DMA1_Channel6, &DMA_InitStructure);	
//	
//	/* Enable USART2 RX DMA channel */
//	DMA_Cmd(DMA1_Channel6, ENABLE);	
	
	USART_DeInit(USART2);
	
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	/* Enable USART DMA RX request */
//	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
//	
//	QueueCreate((void *)UART2SendBuf, sizeof(UART2SendBuf), NULL, (uint8_t (*)())UartWriteFull);
//	QueueCreate((void *)UART2RecvBuf, sizeof(UART2RecvBuf), NULL, (uint8_t (*)())UartWriteFull);
	
  /* Enable USART2 */
  USART_Cmd(USART2, ENABLE);
//	USART_ClearFlag(USART2, USART_FLAG_TC);
}


/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ TXD 使用OD门输出，为了兼容MG323B的2.82V电平			    ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
*/
void USART3_Init(uint32_t Baudrate)
{

	GPIO_InitTypeDef         GPIO_InitStructure;
// 	NVIC_InitTypeDef         NVIC_InitStructure;
	USART_InitTypeDef        USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// USART3 remap
//	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
	
	
//	
//	/* Configure USART3 Tx (PD.08) as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);	

//  /* Configure USART3 Rx (PD.09) as input floating */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

	
	/* Configure USART3 Tx (PD.08) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	

  /* Configure USART3 Rx (PD.09) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_DeInit(USART3);
	
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
// USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
//	
//	QueueCreate((void *)UART3SendBuf, sizeof(UART3SendBuf), NULL, (uint8_t (*)())UartWriteFull);
//	QueueCreate((void *)UART3RecvBuf, sizeof(UART3RecvBuf), NULL, (uint8_t (*)())UartWriteFull);

  /* Enable USART3 */
  USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
}

void UART4_Init(uint32_t Baudrate)
{

  GPIO_InitTypeDef GPIO_InitStructure; 
  USART_InitTypeDef USART_InitStructure; 	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);//UART4

	
	/* Configure UART4 Tx (PC.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	

  /* Configure UART4 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	USART_DeInit(UART4);
	
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(UART4, &USART_InitStructure);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
// 	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
  /* Enable UART4 */
  USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4, USART_FLAG_TC);
}


void UART5_Init(uint32_t Baudrate)
{

  GPIO_InitTypeDef GPIO_InitStructure; 
  USART_InitTypeDef USART_InitStructure; 	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//UART4

	
	/* Configure UART5 Tx (PC.12) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
  /* Configure UART5 Rx (PD.02) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	
	USART_DeInit(UART5);
	
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(UART5, &USART_InitStructure);
// USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
// USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
  /* Enable UART5 */
  USART_Cmd(UART5, ENABLE);
	USART_ClearFlag(UART5, USART_FLAG_TC);
}

void User_ComInit(uint8_t ComNum, uint32_t BaudRate)
{
	switch(ComNum)
	{
		case 1:   
			USART1_Init(BaudRate);
			break;  
		case 2:   
			USART2_Init(BaudRate);
			break;   
		case 3:   
			USART3_Init(BaudRate);
			break;     
		case 4:   
			UART4_Init(BaudRate);
			break;  
		case 5:   
			UART5_Init(BaudRate);
			break;  
              
     default:  break; 
	}
}




