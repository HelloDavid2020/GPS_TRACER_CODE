#ifndef __INCLUDES_H
#define __INCLUDES_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <RTL.h>
	 
#include  "user_typedef.h"
#include  "user_queue.h"
	 
#include "mbcrc.h"
#include "modbus.h"
#include "gprs.h"
// #include "command.h"
#include "common.h"
#include "gprs.h"
// #include "GPS.h"
	 
	 

#include "user_gpio.h"
#include "user_can.h"
#include "user_monitor.h"
#include "user_debugMsg.h"
#include "user_acc.h"
#include "user_rt8025x.h"
	 
#include "user_timedelay.h"	 
#include "user_nvic.h"
#include "user_i2c_fram.h"	 
#include "user_uart.h"	 
	 
#include "user_system.h"	 
#include "user_gps.h"	 
#include "user_gprs.h"
#include "user_timer.h"
#include "user_tcpip.h"
#include "user_adc.h"

#include "user_pwm.h"
#include "user_bsp.h"
#include "user_interrupt.h"
#include "user_can_protocol.h"
//#include "user_dma.h"
#include "user_registers.h"

#include "mb.h"
#include "mbport.h"

#include "user_tcpip.h"
#include "user_blindzone.h"
#include "user_distance.h"
#include "rtc.h"
#include "iap.h"
#include "user_clock.h"

#include "user_dac.h"

#include "user_spi.h"
#include "spi_fram.h"
#include "spi_flash.h"
#include "spi_eeprom.h"

#include "user_wdg.h"
#include "user_swdg.h"


#include  "FM25CL64.h"
#include "Lcd.h"
#include "config.h"
#include "G_086_PC.h"

#include "test.h"

#include "sys.h"
#include "LSM330DLC.h"
#include "DS18B20.h"
#include "file.h"
#include  "user_spi.h"

#ifdef __cplusplus
}
#endif 

#endif

