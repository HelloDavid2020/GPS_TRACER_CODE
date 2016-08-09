#include "user_can_protocol.h"

VEHICLE Vehicle;
CanTxMsg msg_beat;
uint8_t Flag_SendBeatMsg = DISABLE;

uint8_t Flag_CanMsgTx = DISABLE;
uint32_t Count_CanMsgTx = 0;


uint8_t CanMsgTxMaxNum = 5; // 需要发送的CAN数据数目
uint8_t CanMsgTxCurNum = 0; // 需要发送的CAN当前Num

uint8_t CanMsgTxBuf[CAN_TX_MSG_MAX_NUM][8]={0}; // CAN MSG 发送缓存区
CanTxMsg CanTxMsgBuf[CAN_TX_MSG_MAX_NUM];

union CONTROLLER_ALARM ControllerAlarm;


void BeatCanMsgInit(void)
{
	msg_beat.StdId = 0x1FA;
	msg_beat.ExtId = 0x00;
	msg_beat.RTR = CAN_RTR_DATA; 
	msg_beat.IDE = CAN_ID_STD;  
	msg_beat.DLC = 8;	
	memset(msg_beat.Data,0x00,8);
}


void User_TransmitBeatCanMsg(uint8_t data)
{
		msg_beat.StdId = 0x1FA;
		msg_beat.ExtId = 0x00;
		msg_beat.RTR = CAN_RTR_DATA; 
		msg_beat.IDE = CAN_ID_STD;  
		msg_beat.DLC = 8;	


	
		data &= 0xff;
		if( data <= 127) 	
		{
			msg_beat.Data[0] = data  + 66;
		}
		else 	
		{
			msg_beat.Data[0] = data  + 88;
		}
		msg_beat.Data[1] = 0;  //rand()
		msg_beat.Data[2] = 0;
		msg_beat.Data[3] = 0;
		msg_beat.Data[4] = 0;
		msg_beat.Data[5] = 0;
		msg_beat.Data[6] = 0;		
		msg_beat.Data[7] = 0;		
		CAN_Transmit(CAN1,&msg_beat );
		
		memcpy(g_modbusReg4 + ADDR_CAN_MSG03, &msg_beat.Data[0], 8);


}

/*
	ID			
	0x100		TX
	0x101		TX
	0x104		TX
	0x105		TX
	0x106		TX
*/




// CAN 接受数据包解析
void User_CAN_RX_MSG_Handler(CanRxMsg * msg_rece)
{
	switch(msg_rece->StdId)
	{
		// 0x102
		case CAN_ID_RX1:
		ExOutput.Value  = msg_rece->Data[0]&0x7F;

		break;
		
		// 0x103
		case CAN_ID_RX2:
//			GPIO_Toggle(LED_PORT,LED_GREEN);
		PwmOutput.PwmOut1Duty = msg_rece->Data[0];
		PwmOutput.PwmOut2Duty = msg_rece->Data[1];
		PwmOutput.PwmOut3Duty = msg_rece->Data[2];
		PwmOutput.PwmOut4Duty = msg_rece->Data[3];
		
//		TIM5->CCR1 =  PwmOutput.PwmOut4Duty;		// PWM4
//		TIM5->CCR2 =  PwmOutput.PwmOut3Duty;   	// PWM3
//		TIM5->CCR3 =  PwmOutput.PwmOut2Duty;   	// PWM2
//		TIM5->CCR4 =  PwmOutput.PwmOut1Duty;   	// PWM1
		
		break;

		default: 				
		break;
	}
}

/*
	ID			
0x00000101	
0x00000105
0x00000100
0x00000104
0x00000106

*/

void CanTxMsgHandler(void)
{
	uint32_t timeout  = 0xFFF;
	uint32_t timecount = timeout;
	if(ADC1Conversion.Power < 0.0)      ADC1Conversion.Power = 0.0;
	if(ADC1Conversion.ExVin_N16 < 0.0)  ADC1Conversion.ExVin_N16 = 0.0;
	if(ADC1Conversion.ExVin_N17 < 0.0 ) ADC1Conversion.ExVin_N17 = 0.0;
	if(ADC1Conversion.ExIin_N18 < 0.0)  ADC1Conversion.ExIin_N18 = 0.0;
	if(ADC1Conversion.ExIin_N19 < 0.0)  ADC1Conversion.ExIin_N19 = 0.0;	
		
	if(ADC1Conversion.DAOUTA < 0.0) ADC1Conversion.DAOUTA = 0.0;
	if(ADC1Conversion.DAOUTB < 0.0) ADC1Conversion.DAOUTB = 0.0;
	if(ADC1Conversion.DAOUTC < 0.0) ADC1Conversion.DAOUTC = 0.0;	
	if(ADC1Conversion.DAOUTD < 0.0) ADC1Conversion.DAOUTD = 0.0;
	
	if(ADC1Conversion.ExVin_N16 > 10.0) ADC1Conversion.ExVin_N16 = 10.0;
	if(ADC1Conversion.ExVin_N17 > 10.0) ADC1Conversion.ExVin_N17 = 10.0;
	if(ADC1Conversion.ExIin_N18 > 20.0) ADC1Conversion.ExIin_N18 = 20.0;
	if(ADC1Conversion.ExIin_N19 > 20.0) ADC1Conversion.ExIin_N19 = 20.0;

	if(ADC1Conversion.DAOUTA > 2000.0) ADC1Conversion.DAOUTA = 2000.0;
	if(ADC1Conversion.DAOUTB > 300.0) ADC1Conversion.DAOUTB = 300.0;
	if(ADC1Conversion.DAOUTC > 2000.0) ADC1Conversion.DAOUTC = 2000.0;	
	if(ADC1Conversion.DAOUTD > 3000.0) ADC1Conversion.DAOUTD = 3000.0;	
	
	
//	while((CAN1->TSR&CAN_TSR_TME0) != CAN_TSR_TME0);
//	while((CAN1->TSR&CAN_TSR_TME1) != CAN_TSR_TME1);
//	while((CAN1->TSR&CAN_TSR_TME2) != CAN_TSR_TME2);	
//	
// 0x101
	CanTxMsgBuf[0].StdId = 0x101;
	CanTxMsgBuf[0].ExtId = 0x00;
	CanTxMsgBuf[0].RTR = CAN_RTR_DATA; 
	CanTxMsgBuf[0].IDE = CAN_ID_STD;  
	CanTxMsgBuf[0].DLC = 8;	
		
	CanTxMsgBuf[0].Data[0] = (uint16_t)(ADC1Conversion.ExVin_N16 * 1000) & 0xFF;
	CanTxMsgBuf[0].Data[1] = ((uint16_t)(ADC1Conversion.ExVin_N16 * 1000) >> 8) & 0xFF;
	CanTxMsgBuf[0].Data[2] = (uint16_t)(ADC1Conversion.ExVin_N17 * 1000) & 0xFF;
	CanTxMsgBuf[0].Data[3] = ((uint16_t)(ADC1Conversion.ExVin_N17 * 1000) >> 8) & 0xFF;
	CanTxMsgBuf[0].Data[4] = (uint16_t)(ADC1Conversion.ExIin_N18 * 500) & 0xFF;
	CanTxMsgBuf[0].Data[5] = ((uint16_t)(ADC1Conversion.ExIin_N18 * 500) >> 8) & 0xFF;
	CanTxMsgBuf[0].Data[6] = (uint16_t)(ADC1Conversion.ExIin_N19 * 500) & 0xFF;
	CanTxMsgBuf[0].Data[7] = ((uint16_t)(ADC1Conversion.ExIin_N19 * 500) >> 8) & 0xFF;
	
	CAN_Transmit(CAN1,&CanTxMsgBuf[0]);
	
	
	// 0x105
	CanTxMsgBuf[1].StdId = 0x105;
	CanTxMsgBuf[1].ExtId = 0x00;
	CanTxMsgBuf[1].RTR = CAN_RTR_DATA; 
	CanTxMsgBuf[1].IDE = CAN_ID_STD;  
	CanTxMsgBuf[1].DLC = 8;	
		
	CanTxMsgBuf[1].Data[0] = TIM4_ETR_GetDropCounts&0xFF;         // low byte
	CanTxMsgBuf[1].Data[1] = (TIM4_ETR_GetDropCounts >> 8)&0xFF;  // high byte
	CanTxMsgBuf[1].Data[2] = 0;
	CanTxMsgBuf[1].Data[3] = 0;
	CanTxMsgBuf[1].Data[4] = 0;
	CanTxMsgBuf[1].Data[5] = 0;
	CanTxMsgBuf[1].Data[6] = 0;
	CanTxMsgBuf[1].Data[7] = 0;	
	CAN_Transmit(CAN1,&CanTxMsgBuf[1]);
	
	// 0x100  输入状态
	CanTxMsgBuf[2].StdId = 0x100;
	CanTxMsgBuf[2].ExtId = 0x00;
	CanTxMsgBuf[2].RTR = CAN_RTR_DATA; 
	CanTxMsgBuf[2].IDE = CAN_ID_STD;  
	CanTxMsgBuf[2].DLC = 8;	
		
	CanTxMsgBuf[2].Data[0] = ExInput.Value & 0xFF;
	CanTxMsgBuf[2].Data[1] = (ExInput.Value>>8) & 0x0F;
	CanTxMsgBuf[2].Data[2] = 0x00;
	CanTxMsgBuf[2].Data[3] = 0x00;
	CanTxMsgBuf[2].Data[4] = 0x00;
	CanTxMsgBuf[2].Data[5] = 0x00;
	CanTxMsgBuf[2].Data[6] = 0x00;
	CanTxMsgBuf[2].Data[7] = 0x00;	
	CAN_Transmit(CAN1,&CanTxMsgBuf[2]);
	
//	DelayMS(2);
	
	while(timecount--)
	{
		if((CAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0) break;
	}
	
	timecount = timeout;
	while(timecount--)
	{
		if((CAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1) break;
	}
	
	timecount = timeout;
	while(timecount--)
	{
		if((CAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2) break;
	}	
//	while((CAN1->TSR&CAN_TSR_TME0) != CAN_TSR_TME0);
//	while((CAN1->TSR&CAN_TSR_TME1) != CAN_TSR_TME1);
//	while((CAN1->TSR&CAN_TSR_TME2) != CAN_TSR_TME2);
//	
//	DelayMS(2);
	// 0x104   
	CanTxMsgBuf[3].StdId = 0x104;
	CanTxMsgBuf[3].ExtId = 0x00;
	CanTxMsgBuf[3].RTR = CAN_RTR_DATA; 
	CanTxMsgBuf[3].IDE = CAN_ID_STD;  
	CanTxMsgBuf[3].DLC = 8;	
		
	CanTxMsgBuf[3].Data[0] = (uint16_t) (ADC1Conversion.DAOUTB*10) & 0xFF;
	CanTxMsgBuf[3].Data[1] = ((uint16_t)(ADC1Conversion.DAOUTB*10) >> 8) & 0xFF;
	
	CanTxMsgBuf[3].Data[2] = (uint16_t) (ADC1Conversion.DAOUTA*10) & 0xFF;
	CanTxMsgBuf[3].Data[3] = ((uint16_t)(ADC1Conversion.DAOUTA*10) >> 8) & 0xFF;
	
	CanTxMsgBuf[3].Data[4] = (uint16_t)(ADC1Conversion.DAOUTD*5) & 0xFF;
	CanTxMsgBuf[3].Data[5] = ((uint16_t)(ADC1Conversion.DAOUTD*5) >> 8) & 0xFF;
	
	CanTxMsgBuf[3].Data[6] = (uint16_t)(ADC1Conversion.DAOUTC*5) & 0xFF;
	CanTxMsgBuf[3].Data[7] = ((uint16_t)(ADC1Conversion.DAOUTC*5) >> 8) & 0xFF;		
	
	CAN_Transmit(CAN1,&CanTxMsgBuf[3]);
//	DelayMS(1);	
	
	// 0x106  电源电压g
	CanTxMsgBuf[4].StdId = 0x106;
	CanTxMsgBuf[4].ExtId = 0x00;
	CanTxMsgBuf[4].RTR = CAN_RTR_DATA; 
	CanTxMsgBuf[4].IDE = CAN_ID_STD;  
	CanTxMsgBuf[4].DLC = 8;	
	
	/* low byte first */
	CanTxMsgBuf[4].Data[0] = (uint16_t)(ADC1Conversion.Power * 100) & 0xFF;
	CanTxMsgBuf[4].Data[1] = ((uint16_t)(ADC1Conversion.Power * 100) >> 8) & 0xFF;
	CanTxMsgBuf[4].Data[2] = 0;
	CanTxMsgBuf[4].Data[3] = 0;
	CanTxMsgBuf[4].Data[4] = 0;
	CanTxMsgBuf[4].Data[5] = 0;
	CanTxMsgBuf[4].Data[6] = 0;
	CanTxMsgBuf[4].Data[7] = 0;		
	CAN_Transmit(CAN1,&CanTxMsgBuf[4]);

}

