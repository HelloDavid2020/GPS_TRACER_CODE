#ifndef __USER_CAN_PROTOCAL_
#define __USER_CAN_PROTOCAL_

#include	"includes.h"


#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80



// �����������
#define CAN_TX_MSG_MAX_NUM 5


#define ADDR_CAN_DATA  1418

#define ADDR_CAN_MSG01 (0 + ADDR_CAN_DATA)
#define ADDR_CAN_MSG02 (8 + ADDR_CAN_DATA)
#define ADDR_CAN_MSG03 (16+ ADDR_CAN_DATA)
#define ADDR_CAN_MSG04 (24+ ADDR_CAN_DATA)
#define ADDR_CAN_MSG05 (32+ ADDR_CAN_DATA)
#define ADDR_CAN_MSG06 (40+ ADDR_CAN_DATA)
#define ADDR_CAN_MSG07 (48+ ADDR_CAN_DATA)
#define ADDR_CAN_MSG08 (56+ ADDR_CAN_DATA)
#define ADDR_CAN_MSG09 (64+ ADDR_CAN_DATA)
#define ADDR_CAN_MSG10 (72+ ADDR_CAN_DATA)

// transmit CAN ID
#define CAN_ID_LOCK			0x1EF
#define CAN_ID_STATUS 	0x1FB
#define CAN_ID_BEAT 		0x1FA


// receive CAN ID
#define CAN_ID_RX1		0x102
#define CAN_ID_RX2		0x103
//#define CAN_ID_RX1		0x1F1
//#define CAN_ID_RX2		0x1F2
#define CAN_ID_RX3		0x1F3
#define CAN_ID_RX4		0x1F4
#define CAN_ID_RX5		0x1F5
#define CAN_ID_RX6		0x1F6
#define CAN_ID_RX7		0x1F7
#define CAN_ID_RX8		0x1FC



// CAN_init (1, 500000);
// CAN_rx_object (1, 1,0x301, DATA_TYPE |  STANDARD_TYPE);
// CAN_rx_object (1, 1,0x302, DATA_TYPE |  STANDARD_TYPE); 
// CAN_rx_object (1, 1,0x303, DATA_TYPE |  STANDARD_TYPE); 
// CAN_rx_object (1, 1,0x304, DATA_TYPE |  STANDARD_TYPE); 
// CAN_rx_object (1, 1,0x305, DATA_TYPE |  STANDARD_TYPE); 

/*
VehicleSpeed
EngineSpeed				// ������ת��
BatteryVoltage     // ���ص�ѹ
CoolantTemperature // ˮ��
FuelLevel     // ȼ����λ
OilPressure   // ��ѹ

*/

union CONTROLLER_WARN
{
	uint8_t Vaule;
	
	struct 
	{
		uint8_t		ErrAirFilter						:1;  	// ���˶���
		uint8_t		ErrOilFilter						:1;	 	// ���˶���
		uint8_t		OilTemperatureHigh			:1;	 	// ���¸�ָʾ
		uint8_t 	CoolantTemperatureHigh	:1;	 	// ˮ�¸�ָʾ
		uint8_t 	OilPressureLow					:1;		// ��ѹ��ָʾ
		uint8_t 	Beep										:1;		// beep ����
		uint8_t 	VoltageLow							:1;		// ��ѹ��ָʾ
		uint8_t 	ErrOilWaterSeparator 		:1; 	// �ͷֱ���
	}Bit;	

};


union CONTROLLER_ALARM
{
	uint8_t Vaule;
	
	struct 
	{
		uint8_t		ErrAirFilter						:1;  	// ���˶���
		uint8_t		ErrOilFilter						:1;	 	// ���˶���
		uint8_t		OilTemperatureHigh			:1;	 	// ���¸�ָʾ
		uint8_t 	CoolantTemperatureHigh	:1;	 	// ˮ�¸�ָʾ
		uint8_t 	OilPressureLow					:1;		// ��ѹ��ָʾ
		uint8_t 	Beep										:1;		// beep ����
		uint8_t 	VoltageLow							:1;		// ��ѹ��ָʾ
		uint8_t 	ErrOilWaterSeparator 		:1; 	// �ͷֱ���
	}Bit;	

};
extern union CONTROLLER_ALARM ControllerAlarm;




typedef struct 
{
	uint8_t		CoolantTemperature;	// ˮ��
	uint8_t		FuelLevel;      		// ȼ����λ
	uint8_t		OilTemperature;    	// ȼ������
	uint8_t		OilPressure;   			// ��ѹ
	uint16_t	EngineSpeed;				// ������ת��
	uint16_t	BatteryVoltage;     // ���ص�ѹ
	
	uint32_t  WorkTime;		// ����ʱ�� Hour
	uint32_t 	Password;		// ����ֵ
	union CONTROLLER_WARN ControllerWarn ;
	
} VEHICLE;

extern VEHICLE Vehicle;
extern CanTxMsg msg_beat;
extern uint8_t Flag_SendBeatMsg;

extern uint8_t Flag_CanMsgTx;
extern uint32_t Count_CanMsgTx;

extern uint8_t CanMsgTxBuf[CAN_TX_MSG_MAX_NUM][8];
extern uint8_t CanMsgTxMaxNum; // ��Ҫ���͵�CAN������Ŀ
extern uint8_t CanMsgTxCurNum; // ��Ҫ���͵�CAN��ǰNum
extern CanTxMsg CanTxMsgBuf[CAN_TX_MSG_MAX_NUM];

extern void BeatCanMsgInit(void);
extern void User_TransmitBeatCanMsg(uint8_t data);

extern void User_CAN_RX_MSG_Handler(CanRxMsg * msg_rece);

extern void CanTxMsgHandler(void);

// can �������ݴ��
extern void CanTxMsgPackage(void);

#endif

