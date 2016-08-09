#include "rtc.h"

/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
//__IO uint32_t TimeDisplay;	
u32 rtc_counter = 0;
u8 SetTimeFlag =0;
u8 SetAlarmTimeFlag = 1 ;

time_t current_time;
struct tm time_now;
struct tm time_set;
__IO uint32_t TimeDisplay;	


void dis_rtc(void)
{
	uint8_t dis_buf[100];

	time_now = Time_GetCalendarTime();
	sprintf((void*)dis_buf,"%02d-%02d  %02d:%02d:%02d\r\n", time_now.tm_mon, time_now.tm_mday,time_now.tm_hour, time_now.tm_min, time_now.tm_sec);
	display_GB2312_string(1,1,0,dis_buf);

}



/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

//  /* Reset Backup Domain */
//  BKP_DeInit();

	#ifdef RTC_LSE
  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	#else
  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);
  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	#endif

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

		
	#ifdef RTC_LSE
  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	#else
  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(40000-1); /* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1 +1) */
	#endif
/* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}


/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar(time_t t)
* Description    : ת��UNIXʱ���Ϊ����ʱ��
* Input 		 : u32 t  ��ǰʱ���UNIXʱ���
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtimeת�������tm_year�����ֵ����Ҫת�ɾ���ֵ
	return *t_tm;
}


/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : д��RTCʱ�ӵ�ǰʱ��
* Input 		 : struct tm t
* Output		 : None
* Return		 : time_t
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  // �ⲿtm�ṹ��洢�����Ϊ2008��ʽ
											// ��time.h�ж������ݸ�ʽΪ1900�꿪ʼ�����
											// ���ԣ�������ת��ʱҪ���ǵ�������ء�
	
	t.tm_mon -= 1; // unix time �·�0��Ӧ1��
	return mktime(&t);
}

/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Description    : ��RTCȡ��ǰʱ���Unixʱ���ֵ
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime()
* Description    : ��RTCȡ��ǰʱ�������ʱ�䣨struct tm��
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;
	struct tm t_tm;

	t_t = (time_t)RTC_GetCounter();
	t_tm = Time_ConvUnixToCalendar(t_t);
	
	t_tm.tm_mon		+= 1;
	t_tm.tm_yday 	+= 1;
	return t_tm;
}

/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Description    : ��������Unixʱ���д��RTC
* Input 		 : time_t t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Description    : ��������Calendar��ʽʱ��ת����UNIXʱ���д��RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
}

void Time_Show(void)
{

	if (TimeDisplay == 1)
	{
		printf("> Date: %d-%d-%d WeekDay:%d %02d:%02d:%02d (YearDay: %03d)\r\n", time_now.tm_year,\
																				time_now.tm_mon, time_now.tm_mday, time_now.tm_wday,\
																				time_now.tm_hour, time_now.tm_min, time_now.tm_sec,\
																				time_now.tm_yday);
		TimeDisplay = 0;
	}
}

/*
 * ��������Time_Regulate
 * ����  �������û��ڳ����ն��������ʱ��ֵ������ֵ������
 *         RTC �����Ĵ����С�
 * ����  ����
 * ���  ���û��ڳ����ն��������ʱ��ֵ����λΪ s
 * ����  ���ڲ�����
 */
uint32_t Time_Regulate(void)
{
  uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

  printf("\r\n==============Time Settings=====================================");
  printf("\r\n  Please Set Hours");

  while (Tmp_HH == 0xFF)
  {
    Tmp_HH = USART_Scanf(23);
  }
  printf(":  %d", Tmp_HH);
  printf("\r\n  Please Set Minutes");
  while (Tmp_MM == 0xFF)
  {
    Tmp_MM = USART_Scanf(59);
  }
  printf(":  %d", Tmp_MM);
  printf("\r\n  Please Set Seconds");
  while (Tmp_SS == 0xFF)
  {
    Tmp_SS = USART_Scanf(59);
  }
  printf(":  %d", Tmp_SS);

  /* Return the value to store in RTC counter register */
  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}


void SetRtcTime(void)
{
		/* Enable PWR and BKP clocks */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

		/* Allow access to BKP Domain */
		PWR_BackupAccessCmd(ENABLE);

		/* Reset Backup Domain */
		BKP_DeInit();

		#ifdef RTC_LSE
		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{}

		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		#else
		/* Enable the LSI OSC */
		RCC_LSICmd(ENABLE);
		/* Wait till LSI is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{}
		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		#endif

		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();


		printf("\r\n RTC configured....");

		/* Adjust time by values entred by the user on the hyperterminal */
//		Time_Adjust();
		Time_SetCalendarTime(time_set);

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
}

void RtcInit(void)
{

	RTC_Configuration();
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
		/* Backup data register value is not correct or not yet programmed (when
			 the first time the program is executed) */
		printf("\r\n> RTC not yet configured....");
		SetRtcTime();
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n> Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n> External Reset occurred....");
    }

    printf("\r\n> No need to configure RTC....");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }

#ifdef RTCClockOutput_Enable
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */

  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif

  /* Clear reset flags */
  RCC_ClearFlag();
	
	// Rx8025 ��ʱ
	time_set.tm_year	= 2000+ExRtcTime.year;
	time_set.tm_mon		= ExRtcTime.mon;
	time_set.tm_mday	= ExRtcTime.day;
	time_set.tm_hour	= ExRtcTime.hour;
	time_set.tm_min 	= ExRtcTime.min;
	time_set.tm_sec 	= ExRtcTime.sec;

	Time_SetCalendarTime(time_set);	
	
	
	
	
	
}

/*
 * ��������USART_Scanf
 * ����  �����ڴӳ����ն��л�ȡ��ֵ
 * ����  ��- value �û��ڳ����ն����������ֵ
 * ���  ����
 * ����  ���ڲ�����
 */ 
uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(USART1));
		// �Ӵ����ն��������ȥ������ASCII��ֵ
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\rPlease enter valid number between 0 and 9");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
  return index;
}

// �趨����ʱ��
void SetAlarmTime(void)
{
	// 23:59:59
	struct tm time_current;
	uint32_t DayTotalSeconds; // һ���ۻ�������
	uint32_t CurrentTotalSeconds; // ��ǰʱ���ۻ�������
	uint32_t AlarmValue;  // ���
	
	DayTotalSeconds = 23*3600 + 59*60 +59;
	
	time_current = Time_GetCalendarTime();
	CurrentTotalSeconds = time_current.tm_hour*3600 + time_current.tm_min*60 + time_current.tm_sec;
	
	AlarmValue = DayTotalSeconds - CurrentTotalSeconds;
	
	/* Wait till RTC Second event occurs */
	RTC_ClearFlag(RTC_FLAG_SEC);
	while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);

	/* Set the RTC Alarm after 3s */
	RTC_SetAlarm(RTC_GetCounter()+AlarmValue);
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/******************* (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� *****END OF FILE****/
