#include "test.h"
#include "includes.h"

int32_t time_auto_pwr_off = CNT_POWER_OFF;//60S内无操作 关机

uint8_t dis_buf[100];
uint8_t dis_line=2;
uint8_t cnt_pwr_off=0;

uint8_t cmd_tx_buf[100];
uint32_t cmd_tx_cnt=0;
BOOL flag_act=0; // 马达动作标志
BOOL flag_ver_dis=0; // 版本显示标志

BOOL flag_inf_en=0;
uint8_t infor_buf[1024]={0};
uint8_t infor_buf_len=0;

int lock_state;
uint8_t gps_state[2]={0};
uint8_t version[8]={0};

uint8_t device_id[16]={0};
uint8_t hardware_id[16]={0};
uint8_t bike_id[16]={0};
uint8_t v_bat[10]={0};
uint8_t i_current[10]={0};

void key_handle(void)
{
	static bool key_f3=0;
//		uint16_t In0_0:1;  // M
//		uint16_t In0_1:1;  //↓
//		uint16_t In0_2:1;  // PWR
	
//		uint16_t In0_3:1;  // F1
//		uint16_t In0_4:1;  // F2
//		uint16_t In0_5:1;	 // F3
	
//		uint16_t In0_6:1;  // A
//		uint16_t In0_7:1;	 // B
//		uint16_t In0_8:1;	 // C
	int32_t cnt_time_out=1000;

	if((ExInput.Value & 0x1FF) !=0) 
	{
		time_auto_pwr_off = CNT_POWER_OFF;
		ExOutput.Bit.Q0=1;//open beep
	
		if(ExInput.Bit.In0_2==1)
		{
			ExOutput.Bit.Q0=0;//open beep
			cnt_pwr_off++;
			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"---> power off %d\r\n",cnt_pwr_off);
			display_string_5x8(dis_line,1,0,(void*)"                       ");// clear line	    
			display_string_5x8(dis_line,1,0,(void*)cmd_tx_buf);
			if(cnt_pwr_off>30)
			{
				clear_screen();
				display_GB2312_string(3,5,0,"-- POWER OFF --\r\n");
				os_dly_wait(100);  // 10MS * 10=100 mS
				enter_sleep();
				while(1);
			}
		}

	// M+F1
		if(ExInput.Bit.In0_0==1 && ExInput.Bit.In0_3==1 && flag_act ==0)
		{
			flag_act =1;
			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"TX:强制开锁<%d>\r\n",++cmd_tx_cnt);
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
			printf("AT+MOTOR\r\n");
		}
		
		
	// M+C
		if(ExInput.Bit.In0_0==1 && ExInput.Bit.In0_1==1 && flag_act ==0)
		{
			flag_act =1;
			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"TX:设备重启<%d>\r\n",++cmd_tx_cnt);
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
			printf("AT+REBOOT\r\n");
		}		
		
		if(ExInput.Bit.In0_1==1 && flag_act ==0)
		{
			flag_act =1;
			EXT_INT_HIGH();
			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"TX:休眠唤醒\r\n");
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
		}
			

	
			
		if(ExInput.Bit.In0_3==1 && flag_act ==0)
		{
			flag_act =1;
			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"TX:开锁<%d>\r\n",++cmd_tx_cnt);
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
					
			printf("AT+UNLOCK\r\n");

		}
		if(ExInput.Bit.In0_4==1 && flag_act ==0)
		{
			flag_act =1;

			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"TX:指示灯+蜂鸣器<%d>\r\n",++cmd_tx_cnt);
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
				
			printf("AT+BEEP\r\n");
			os_dly_wait(1);
			printf("AT+LED\r\n");
		}
		if(ExInput.Bit.In0_5==1 && flag_act ==0) 
		{
			key_f3 ^=1;
			flag_act =1;
			if(key_f3==1)
			{
				memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
				sprintf((void*)cmd_tx_buf,"TX:GPS信息输出<%d>\r\n",++cmd_tx_cnt);
				display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
				display_GB2312_string(dis_line,1,0,cmd_tx_buf);
								
				printf("AT+GPSLOG=1\r\n");
				os_dly_wait(10);  // // 10MS * 10 = 100 mS
			}
			else
			{
				memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
				sprintf((void*)cmd_tx_buf,"TX:GPS信息关闭<%d>\r\n",++cmd_tx_cnt);
				display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
				display_GB2312_string(dis_line,1,0,cmd_tx_buf);
						
				printf("AT+GPSLOG=0\r\n");
				os_dly_wait(10);  // // 10MS * 10 = 100 mS

			}
		}

		
		if(ExInput.Bit.In0_6==1 && flag_act ==0)
		{
			flag_act =1;

			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"TX:状态上报<%d>\r\n",++cmd_tx_cnt);
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
							
			printf("at+allstate\r\n");
		}			
		if(ExInput.Bit.In0_7==1 && flag_act ==0)
		{		
			ExOutput.Value=0;
			OutputControl(ExOutput);	// 控制端口输出

			
			
//			flag_act =1;
//			en_info_rx=1;
//			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
//			sprintf((void*)cmd_tx_buf,"TX:版本信息<%d>\r\n",++cmd_tx_cnt);
//			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
//			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
//			
//			info_rx_len=0;
//			printf("AT+WHO\r\n");
//			os_dly_wait(20);// 延时200m秒
//			en_info_rx=0;    // 禁止接收
//			
//			if(info_rx_len>100) 
//				done_info_rx=1;
//			
//			ExOutput.Value=0;
//			OutputControl(ExOutput);	// 控制端口输出
			
			flag_act =1;
			en_info_rx=1;
			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);				
			sprintf((void*)cmd_tx_buf,"TX:系统信息<%d>\r\n",++cmd_tx_cnt);
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
			
			info_rx_len=0;
			printf("AT+INFO\r\n");
			os_dly_wait(20);// 延时1秒
			en_info_rx=0;    // 禁止接收
			
			if(info_rx_len>300) 
				done_info_rx=1;			

		}
			
		if(ExInput.Bit.In0_8==1 && flag_act ==0)
		{
			flag_act =1;
			++cmd_tx_cnt;
			memset(cmd_tx_buf,0x00,sizeof cmd_tx_buf);
			clear_screen();

			sprintf((void*)cmd_tx_buf,"TX:系统关机<%d>\r\n",++cmd_tx_cnt);
			display_GB2312_string(dis_line,1,0,(void*)"                       ");// clear line	    
			display_GB2312_string(dis_line,1,0,cmd_tx_buf);
			printf("AT+POWEROFF=6789\r\n");			
		}
	}
	
	
	if((ExInput.Value & 0x1FF) ==0) 
	{ 
		flag_act =0;
		EXT_INT_LOW();
		cnt_pwr_off=0;
	}
	
//-------------------------------------------------------------------------
// LCD 显示
	if(flag_usart1_rx_done==1 &&  flag_info_dis==0)
	{
		flag_usart1_rx_done=0;
		
		if(flag_csq_rx==1)
		{
			flag_csq_rx=0;
			if(getCSQ(csq_rx)!=0)
			{
				sprintf((void*)cmd_tx_buf,"GSM信号: %d \r\n",getCSQ(csq_rx));
				display_GB2312_string(dis_line+2,1,0,(void*)"                       ");// clear line	    
				display_GB2312_string(dis_line+2,1,0,cmd_tx_buf);
			}
		}
		lcd_show_rx_buf((void*)UART1RecvBuf);						
		usart1_rx_len=0;
		memset(UART1RecvBuf,0x00,sizeof UART1RecvBuf);
	}
//-------------------------------------------------------------------------

}



// AT RECV: +CSQ: 21,0
int getCSQ(uint8_t *buf)
{
	uint16_t index =0;
	int csq =0;

	index = FindStr((void *)buf,"AT RECV: +CSQ:");
	if(index>0)
	{
		index --;
		sscanf((char *)(buf+index), "AT RECV: +CSQ:%d,",&csq);
	}
	return csq;
}




void enter_sleep(void)
{

	LED_RED_OFF;
	LED_GREEN_OFF;
	LED_BLUE_OFF;

	//--------------------------------------

 lcd_cs1=0;	
 lcd_reset=0;
 lcd_rs=0;
 lcd_sid =0;
 lcd_sclk=0;
	
 Rom_CS=0;
	Rom_SCK=0;
	Rom_OUT	=0;
	Rom_IN=0;	
	
	
	
	
	LCD_PWR_OFF();
	exit_wake_up();

//PWR_EnterSTANDBYMode(); // The Standby mode allows the lowest power consumption.
	//enter stop mode 
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE); 
//	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE); 

}

void go_to_sleep(void)
{
	time_auto_pwr_off--;
	if(time_auto_pwr_off<=0)
	{
		time_auto_pwr_off=0;
		clear_screen();
		display_GB2312_string(3,5,0,"-- POWER OFF --\r\n");
		os_dly_wait(200);  // 10MS * 10=100 mS
		enter_sleep();	
	

		while(1);
	}
}
/*
<---- 20:13:42 [ TEST_INFO][20:13:38] >>>>>Input cmd = AT+INFO<<<<<
<---- 20:13:42 [ TEST_INFO][20:13:38] *******************System Info**********************
<---- 20:13:42 [ TEST_INFO][20:13:38] DeviceID    = 460001790256992
<---- 20:13:42 [ TEST_INFO][20:13:38] HardwareID  = 867273021061705
<---- 20:13:42 [ TEST_INFO][20:13:38] MoBikeID    = 5000080002
<---- 20:13:42 [ TEST_INFO][20:13:38] GPSState    = V
<---- 20:13:42 [ TEST_INFO][20:13:38] LockState   = 2
<---- 20:13:42 [ TEST_INFO][20:13:38] Reserve     = 0
<---- 20:13:42 [ TEST_INFO][20:13:38] Lon         = 0.000000
<---- 20:13:42 [ TEST_INFO][20:13:38] Lat         = 0.000000
<---- 20:13:42 [ TEST_INFO][20:13:38] Bat         = 3704 mv
<---- 20:13:42 [ TEST_INFO][20:13:38] Current     = 0 ma
<---- 20:13:42 [ TEST_INFO][20:13:38] HWVersion   = 1
<---- 20:13:42 [ TEST_INFO][20:13:38] RomVersion  = 3
<---- 20:13:42 [ TEST_INFO][20:13:38] ProtocolVer = 01
<---- 20:13:42 [ TEST_INFO][20:13:38] BikeMode    = 01
<---- 20:13:42 [ TEST_INFO][20:13:38] PushPeriod  = 30
<---- 20:13:42 [ TEST_INFO][20:13:38] BeatTick    = 0
<---- 20:13:42 [ TEST_INFO][20:13:38] GPS Work    = 1
<---- 20:13:42 [ TEST_INFO][20:13:38] ****************************************************

*/
void infor_check(void)
{
	uint8_t lcd_buf[100]={0};
	int32_t cnt_time_out=1000;
	uint16_t index =0;
	
	// 数据检索
	if(done_info_rx==1)
	{
		ExOutput.Value=0;
		OutputControl(ExOutput);	// 控制端口输出
			
		clear_screen();	
		os_dly_wait(1);  // 20mS
		//Welcome to Use our MoBike Application ( Verson 4.1.2 )
		index = FindStr((void *)info_rx,"MoBike Application ( Verson");
		if(index>0)
		{
			index --;
			sscanf((char *)(info_rx+index), "MoBike Application ( Verson%5s",version);
		}

//			sprintf((void*)lcd_buf,"固件版本: %s",version);
//			memset(version,0x00,sizeof version);
//			display_GB2312_string(3,1,0,lcd_buf);
//			display_GB2312_string(3,1,0,lcd_buf);

			//GPSState    = V
			index = FindStr((void *)info_rx,"GPSState    = ");
		if(index>0)
		{
			index --;
			sscanf((char *)(info_rx+index), "GPSState    = %1s",gps_state);
		}
			
			index = FindStr((void *)info_rx,"LockState   =");
			if(index>0)
			{
				index --;
				sscanf((char *)(info_rx+index), "LockState   =%d",&lock_state);
			}
			
			index = FindStr((void *)info_rx,"DeviceID    = ");
			if(index>0)
			{
				index --;
				sscanf((char *)(info_rx+index), "DeviceID    = %15s",device_id);
			}
			
			index = FindStr((void *)info_rx,"HardwareID  = ");
			if(index>0)
			{
				index --;
				sscanf((char *)(info_rx+index), "HardwareID  = %15s",hardware_id);
			}	
			
			index = FindStr((void *)info_rx,"MoBikeID    = ");
			if(index>0)
			{
				index --;
				sscanf((char *)(info_rx+index), "MoBikeID    = %10s",bike_id);
			}
			
			index = FindStr((void *)info_rx,"Bat         = ");
			if(index>0)
			{
				index --;		
				sscanf((char *)(info_rx+index), "Bat         = %5s",v_bat);
			}
			
			index = FindStr((void *)info_rx,"Current     = ");
			if(index>0)
			{
				index --;
				sscanf((char *)(info_rx+index), "Current     = %5s",i_current);
			}
			
			memset(info_rx,0x00,sizeof info_rx);
			info_rx_len=0;
			done_info_rx=0;
			clear_screen();	
			os_dly_wait(1);  // 20mS

			if(lock_state==2)
			{
				sprintf((void*)lcd_buf,"状态:关锁 %5s",version);
				display_GB2312_string(1,1,0,lcd_buf);
				display_GB2312_string(1,1,0,lcd_buf);				
			}
			else if(lock_state==1)
			{
				sprintf((void*)lcd_buf,"状态:开锁 %5s",version);
				display_GB2312_string(1,1,0,lcd_buf);
				display_GB2312_string(1,1,0,lcd_buf);				
			}
			else
			{
				sprintf((void*)lcd_buf,"状态:未知 %5s",version);
				display_GB2312_string(1,1,0,lcd_buf);
				display_GB2312_string(1,1,0,lcd_buf);
			}			
	
		
			display_string_5x8(3,1,0,(void*)hardware_id);// clear line	  
			display_string_5x8(3,1,0,(void*)hardware_id);// clear line	  

//		sprintf((void*)lcd_buf,"ID:  %s",hardware_id);
			display_GB2312_string(3,1,0,hardware_id);		
		
		sprintf((void*)lcd_buf,"ID:  %s",bike_id);
		display_GB2312_string(5,1,0,lcd_buf);

			sprintf((void*)lcd_buf," %s mV",v_bat);
			display_GB2312_string(7,1,0,lcd_buf);
			
			if(gps_state[0]=='A')
			display_GB2312_string(7,80,0,"已定位");
			if(gps_state[0]=='V')
			display_GB2312_string(7,80,0,"未定位");

//			sprintf((void*)lcd_buf,"%smV %smA",v_bat,i_current);
//			display_GB2312_string(7,1,0,lcd_buf);			

		// 等待按键
		DetectPortInput();
		
		while(ExInput.Bit.In0_7==0)
		{				
			DetectPortInput();
			os_dly_wait(1);  // // 10MS * 10 = 100 mS
			
			cnt_time_out--;
			if(cnt_time_out==0) break;
		}
		
		memset(info_rx,0x00,sizeof info_rx);
		info_rx_len=0;
		clear_screen();	
		display_GB2312_string(dis_line,1,0,(void*)"TX:");// clear line	    
		os_dly_wait(2);  // // 10MS * 10 = 100 mS


	}
}
	

void start_log(void)
{
	clear_screen();	
//	time_set.tm_year	= 2000+15;
//	time_set.tm_mon		= 12;
//	time_set.tm_mday	= 24;
//	time_set.tm_hour	= 15;
//	time_set.tm_min  	= 5;
//	time_set.tm_sec 	= 0;
//	Time_SetCalendarTime(time_set);	
	
//	time_now = Time_GetCalendarTime();
//	sprintf((void*)dis_buf,"%02d-%02d  %02d:%02d:%02d\r\n", time_now.tm_mon, time_now.tm_mday,time_now.tm_hour, time_now.tm_min, time_now.tm_sec);
	display_GB2312_string(1,1,0,"-----------------");
	display_GB2312_string(1,1,0,"-----------------");
	display_GB2312_string(3,1,0," Hello MoBike!");
	display_string_5x8(dis_line+3,1,0,(void*)" 2016-2-24");// clear line
	display_GB2312_string(7,1,0,(void*)" V106");// clear line
	os_dly_wait(150); // 10MS * 10=100 mS
	clear_screen();	
display_GB2312_string(dis_line,1,0,(void*)"TX:");// clear line	    

}


uint16_t FindStr(char *str,char *ptr)
{
	uint16_t index=0;
	char *STemp=NULL;
	char *PTemp=NULL;
	char *MTemp=NULL;
	if(0==str||0==ptr)
		return 0;
	for(STemp=str; *STemp!='\0'; STemp++)	 
	{
		index++;   
		MTemp=STemp;  

		for(PTemp=ptr;*PTemp!='\0';PTemp++)
		{	
			if(*PTemp!=*MTemp)
			break;
			MTemp++;
		}
		if(*PTemp=='\0')  
			break;
	}
	return index;
}


