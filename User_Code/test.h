
#ifndef __TEST_H
#define __TEST_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "includes.h"
#include "LCD.h"
#include "config.h"
#include "G_086_PC.h"
	 
	 
extern int dat;

#define CNT_POWER_OFF  3000*5  // 3000对应1min
extern int32_t time_auto_pwr_off;//60S内无操作 关机

extern uint8_t dis_buf[100];
extern uint8_t dis_line;
extern uint8_t cnt_pwr_off;

extern uint8_t cmd_tx_buf[100];
extern uint32_t cmd_tx_cnt;
extern BOOL flag_act; // 马达动作标志	 
extern BOOL flag_ver_dis;
extern int lock_state;
extern uint8_t gps_state[2];
extern uint8_t device_id[16];
extern uint8_t version[8];

extern uint8_t hardware_id[16];
extern uint8_t bike_id[16];
extern uint8_t v_bat[10];
extern uint8_t i_current[10];
extern BOOL flag_inf_en;
extern uint8_t infor_buf[1024];
extern uint8_t infor_buf_len;

extern void go_to_sleep(void);
extern void key_handle(void);
extern void enter_sleep(void);
extern void infor_check(void);
extern void start_log(void);
extern int getCSQ(uint8_t *buf);

#endif