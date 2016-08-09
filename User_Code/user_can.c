#include "user_can.h"

uint8_t CanbusSendHeart1 = 0;
uint8_t CanbusSendHeart2 = 0;
uint16_t read_current  = 0;//当前读位置    
uint16_t write_current = 0;//当前写位置    
uint16_t packet_count  = 0; //有效包数量

CanRxMsg RxMessage[CANMAXFRAME];

CanTxMsg TxMessage;
CanRxMsg CANRxMessage;


// CAN_Config
/**
  * @brief  Configures the CAN.
  * @param  uint16_t baudrate: Kbps
  * @retval None
  
	CAN baudrate = PCLK1/((CAN_SJW +CAN_BS1 +  CAN_BS2)*CAN_Prescaler)
	if PCLK1=36MHz, CAN_SJW=1, CAN_BS1=8,CAN_BS2=7,CAN_Prescaler=9
	baudrate = PCLK1/((1 + 8 + 7) * 9) = 36MHz / 16 / 9 = 250Kbits
	
	*/
	
	
	
	
	
void CAN1_Config(uint16_t baudrate)
{
	GPIO_InitTypeDef				GPIO_InitStructure;
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
//   NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
   /* Configure CAN pin: RX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //
  /* CAN register init */
  CAN_DeInit(CAN1);
	
	  /* CAN1 cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	// PLCK1 = 18 MHz
	// CAN_SJW_1tq CAN_BS1_3tq CAN_BS2_2tq 	24 	// 250K
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//     
  CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;//
  CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;//
	
	switch(baudrate)
	{
		case  250: 	CAN_InitStructure.CAN_Prescaler = 12;	break;
		case  500: 	CAN_InitStructure.CAN_Prescaler = 12;	break;
		case 1000: 	CAN_InitStructure.CAN_Prescaler = 6; 	break;
		default:		CAN_InitStructure.CAN_Prescaler = 24;	break;
	}
	
  CAN_Init(CAN1, &CAN_InitStructure);
	
	#ifdef CAN_FILTER
/* CAN filter init */
// CAN_FilterNumber 0-13
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode  = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ( uint16_t)( CAN_STD_RX_ID_1 >> 13 );
	CAN_FilterInitStructure.CAN_FilterIdLow  = ( uint16_t )( CAN_STD_RX_ID_1 << 3 ) | CAN_ID_EXT;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0xFFFC;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 2;
	CAN_FilterInitStructure.CAN_FilterMode   = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale  = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ( uint16_t)( CAN_STD_RX_ID_2 >> 13 );
	CAN_FilterInitStructure.CAN_FilterIdLow  = ( uint16_t )( CAN_STD_RX_ID_2 << 3 ) | CAN_ID_EXT;
	
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFC;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 3;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ( uint16_t)( CAN_STD_RX_ID_3 >> 13 );
	CAN_FilterInitStructure.CAN_FilterIdLow  = ( uint16_t )( CAN_STD_RX_ID_3 << 3 ) | CAN_ID_EXT;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFC;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);	

	CAN_FilterInitStructure.CAN_FilterNumber = 4;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)( CAN_STD_RX_ID_4 >> 13 );
	CAN_FilterInitStructure.CAN_FilterIdLow  = (uint16_t)( CAN_STD_RX_ID_4 << 3 ) | CAN_ID_EXT;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFC;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);	


	CAN_FilterInitStructure.CAN_FilterNumber = 5;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ( uint16_t)( CAN_STD_RX_ID_5 >> 13 );
	CAN_FilterInitStructure.CAN_FilterIdLow  = ( uint16_t)( CAN_STD_RX_ID_5 << 3 ) | CAN_ID_EXT;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFC;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	
	#else
	
	CAN_FilterInitStructure.CAN_FilterNumber = 6;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)CAN_ID_RX1 << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (uint16_t)CAN_ID_RX1 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 7;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)CAN_ID_RX2 << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (uint16_t)CAN_ID_RX2 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	#endif
	
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

}

void User_CAN1_Init(void)
{
	POWER_ON_CAN;

			CAN1_Config(250);	

}


uint8_t BSP_Can_Read_buf(uint8_t * str)
{
  if(packet_count>0) //判断有效包总数
  {
	  if(read_current >CANMAXFRAME-1)
	     read_current=0;
	  memcpy(str,&RxMessage[read_current],sizeof(CanRxMsg));
	  read_current++;
	  packet_count--;
	  return 1;
  }
  return 0;
}



void BSP_Can_Write_buf(uint8_t * str)
{
   //把数据拷贝到发送区
   memcpy((char *)&TxMessage,str,sizeof(TxMessage));
   CAN_Transmit(CAN1,&TxMessage );
}

