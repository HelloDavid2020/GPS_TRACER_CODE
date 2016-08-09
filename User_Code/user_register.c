#include "user_registers.h"

//uint16_t *usRegHoldingBuf=(void*) g_modbusReg4;	
uint16_t *usRegHoldingBuf=NULL;	

uint16_t REG_INPUT_START = 1, REG_HOLDING_START = 1;
uint16_t REG_INPUT_NREGS =1024*10, REG_HOLDING_NREGS =1024*10;
uint16_t usRegInputStart = 1, usRegHoldingStart = 1;

// µ•Œª√Î
uint32_t GetDeveiceUploadTime(void)
{
	
}

