#ifndef __USER_CAN_H
#define __USER_CAN_H

#include "includes.h"
#ifdef __cplusplus
 extern "C" {
#endif 
	 
// SJW 					BS1 					BS2 					Prescaler
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	6 		// 1M
// CAN_SJW_1tq 	CAN_BS1_4tq 	CAN_BS2_3tq 	5 		// 900K
// CAN_SJW_1tq 	CAN_BS1_5tq 	CAN_BS2_3tq 	5 		// 800K
// CAN_SJW_1tq 	CAN_BS1_6tq 	CAN_BS2_3tq 	6 		// 600K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	12 		// 500K
// CAN_SJW_1tq 	CAN_BS1_5tq 	CAN_BS2_3tq 	10 		// 400K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	20 		// 300K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq	 	24 		// 250K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	30 		// 200K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	40 		// 150K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	48 		// 125K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	60 		// 100K
// CAN_SJW_1tq 	CAN_BS1_4tq 	CAN_BS2_3tq 	50 		// 90K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	75 		// 80K
// CAN_SJW_1tq 	CAN_BS1_6tq 	CAN_BS2_3tq 	60 		// 60K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	120 	// 50K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	150 	// 40K
// CAN_SJW_1tq 	CAN_BS1_6tq 	CAN_BS2_3tq 	120 	// 30K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	300 	// 20K
// CAN_SJW_1tq 	CAN_BS1_3tq 	CAN_BS2_2tq 	600 	// 10K
// CAN_SJW_2tq 	CAN_BS1_6tq 	CAN_BS2_4tq 	600 	// 5K
// CAN_SJW_2tq 	CAN_BS1_6tq 	CAN_BS2_4tq 	1000	// 3K
// CAN_SJW_2tq 	CAN_BS1_10tq 	CAN_BS2_6tq 	1000 	// 2K
	 
#define POWER_ON_CAN 		GPIO_SetBits (GPIOD, GPIO_Pin_0)
#define POWER_OFF_CAN	 	GPIO_RestBits(GPIOD, GPIO_Pin_0)

#define  CANMAXFRAME 2

#define CAN_STD_RX_ID_1 				0x1BFEFF17
#define CAN_STD_RX_ID_2 				0x1BFEFC17
#define CAN_STD_RX_ID_3 				0x1BF00417
#define CAN_STD_RX_ID_4 				0x1BFEE717
#define CAN_STD_RX_ID_5 				0x1BFED917
#define CAN_STD_RX_ID_6 				0x1F6
#define CAN_STD_RX_ID_7 				0x1F7
#define CAN_STD_RX_ID_A 				0x200
#define CAN_STD_RX_ID_B 				0x201
#define CAN_STD_RX_ID_C 				0x202
#define CAN_STD_RX_ID_E 				0x286
#define CAN_STD_RX_ID_D 				0x1FD

extern uint8_t CanbusSendHeart1;
extern uint8_t CanbusSendHeart2 ;	 

extern uint16_t read_current;//当前读位置    
extern uint16_t write_current;//当前写位置    
extern uint16_t packet_count; //有效包数量
extern CanRxMsg RxMessage[CANMAXFRAME];
extern CanTxMsg TxMessage;
extern CanRxMsg CANRxMessage;
	 

	 
extern void CAN1_Config(uint16_t baudrate);
extern void User_CAN1_Init(void);
extern void BSP_Can_Write_buf(uint8_t * str);
extern uint8_t BSP_Can_Read_buf(uint8_t * str);
extern void BSP_Can_Write_buf(uint8_t * str);
extern uint8_t BSP_Can_Read_buf(uint8_t * str);
extern void CanbusSendData(void);
extern void CAN_MSG_Handler(void);

#ifdef __cplusplus
}
#endif 

#endif

