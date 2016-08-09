/**
  ******************************************************************************


  *          ===================================================================
  *
  *          +-------------------------------------------------------+
  *          |                     Pin assignment                    |
  *          +-------------------------+---------------+-------------+
  *          |  STM32 SPI Pins         |     SD        |    Pin      |
  *          +-------------------------+---------------+-------------+
  *          | SD_SPI_CS_PIN           |   ChipSelect  |    1        |
  *          | SD_SPI_MOSI_PIN / MOSI  |   DataIn      |    2        |
  *          |                         |   GND         |    3 (0 V)  |
  *          |                         |   VDD         |    4 (3.3 V)|
  *          | SD_SPI_SCK_PIN / SCLK   |   Clock       |    5        |
  *          |                         |   GND         |    6 (0 V)  |
  *          | SD_SPI_MISO_PIN / MISO  |   DataOut     |    7        |
  *          +-------------------------+---------------+-------------+
  ******************************************************************************
  ******************************************************************************  
  */
	
	
#include "stm32f10x.h"
#include "sys_io.h"
#include "sd.h"
#include "spi.h"
#include  "user_spi.h"

SD_CARD_INFOR sd_card;
uint8_t  SD_Type=0;//SD卡的类型 


/**
 * @brief  Detect if SD card is correctly plugged in the memory slot.
 * @param  None
 * @retval Return if SD is detected or not
 */
SD_PRESENT_STATE SD_Detect(void)
{
  __IO uint8_t status = SD_PRESENT;

  /*!< Check GPIO to detect SD */
  if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10))
  {
    status = SD_NOT_PRESENT;
  }
  return status;
}




////////////////////////////////////移植修改区///////////////////////////////////
//移植时候的接口
//data:要写入的数据
//返回值:读到的数据
uint8_t SD_SPI_ReadWriteByte(uint8_t data)
{
	return SPI3_ReadWriteByte(data);
}	  
//SD卡初始化的时候,需要低速
void SD_SPI_SpeedLow(void)
{
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_256);//设置到低速模式	
}
//SD卡正常工作的时候,可以高速了
void SD_SPI_SpeedHigh(void)
{
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_4);//设置到高速模式	72M/4=18M
}
//SPI硬件层初始化
void SD_SPI_Init(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PB8 推挽
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
			 
	SPI3_Init();
	SD_CS_HIGH();
}
///////////////////////////////////////////////////////////////////////////////////
//取消选择,释放SPI总线
void SD_DisSelect(void)
{
	SPI3_CS=1;
 	SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
}
//选择sd卡,并且等待卡准备OK
//返回值:0,成功;1,失败;
uint8_t SD_Select(void)
{
	SPI3_CS=0;
	if(SD_WaitReady()==0)return 0;//等待成功
	SD_DisSelect();
	return 1;//等待失败
}
//等待卡准备好
//返回值:0,准备好了;其他,错误代码
uint8_t SD_WaitReady(void)
{
  /*!< Check if response is got or a timeout is happen */
	uint32_t t=0;
	do
	{
		if(SD_SPI_ReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;		  	
	}while(t<0xFFFF);//等待 
	return 1;
}
//等待SD卡回应
//Response:要得到的回应值
//返回值:0,成功得到了该回应值
//    其他,得到回应值失败
uint8_t SD_GetResponse(uint8_t Response)
{
	uint16_t Count=0xFFF;//等待次数	   						  
	while ((SD_SPI_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败   
	else return MSD_RESPONSE_NO_ERROR;//正确回应
}
//从sd卡读取一个数据包的内容
//buf:数据缓存区
//len:要读取的数据长度.
//返回值:0,成功;其他,失败;	
uint8_t SD_RecvData(uint8_t*buf,uint16_t len)
{			  	  
	if(SD_GetResponse(0xFE))return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf=SPI3_ReadWriteByte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);									  					    
    return 0;//读取成功
}
//向sd卡写入一个数据包的内容 512字节
//buf:数据缓存区
//cmd:指令
//返回值:0,成功;其他,失败;	
uint8_t SD_SendBlock(uint8_t*buf,uint8_t cmd)
{	
	uint16_t t;		  	  
	if(SD_WaitReady())return 1;//等待准备失效
	SD_SPI_ReadWriteByte(cmd);
	if(cmd!=0XFD)//不是结束指令
	{
		for(t=0;t<512;t++)SPI3_ReadWriteByte(buf[t]);//提高速度,减少函数传参时间
	    SD_SPI_ReadWriteByte(0xFF);//忽略crc
	    SD_SPI_ReadWriteByte(0xFF);
		t=SD_SPI_ReadWriteByte(0xFF);//接收响应
		if((t&0x1F)!=0x05)return 2;//响应错误									  					    
	}						 									  					    
    return 0;//写入成功
}

//向SD卡发送一个命令
//输入: uint8_t cmd   命令 
//      uint32_t arg  命令参数
//      uint8_t crc   crc校验值	   
//返回值:SD卡返回的响应															  
uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;	
	uint32_t Retry=0; 
	SD_DisSelect();//取消上次片选
	if(SD_Select())return 0XFF;//片选失效 
	//发送
    SD_SPI_ReadWriteByte(cmd | 0x40);//分别写入命令
    SD_SPI_ReadWriteByte(arg >> 24);
    SD_SPI_ReadWriteByte(arg >> 16);
    SD_SPI_ReadWriteByte(arg >> 8);
    SD_SPI_ReadWriteByte(arg);	  
    SD_SPI_ReadWriteByte(crc); 
	if(cmd==CMD12)SD_SPI_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //等待响应，或超时退出
	Retry=0XFF;
	do
	{
		r1=SD_SPI_ReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);	 
	//返回状态值
    return r1;
}		    																			  
//获取SD卡的CID信息，包括制造商信息
//输入: uint8_t *cid_data(存放CID的内存，至少16Byte）	  
//返回值:0：NO_ERR
//		 1：错误														   
uint8_t SD_GetCID(uint8_t *cid_data)
{
    uint8_t r1;	   
    //发CMD10命令，读CID
    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
	{
		r1=SD_RecvData(cid_data,16);//接收16个字节的数据	 
    }
	SD_DisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}																				  
//获取SD卡的CSD信息，包括容量和速度信息
//输入:uint8_t *cid_data(存放CID的内存，至少16Byte）	    
//返回值:0：NO_ERR
//		 1：错误														   
uint8_t SD_GetCSD(uint8_t *csd_data)
{
    uint8_t r1;	 
    r1=SD_SendCmd(CMD9,0,0x01);//发CMD9命令，读CSD
    if(r1==0)
	{
    	r1=SD_RecvData(csd_data, 16);//接收16个字节的数据 
    }
	SD_DisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}  
//获取SD卡的总扇区数（扇区数）   
//返回值:0： 取容量出错 
//       其他:SD卡的容量(扇区数/512字节)
//每扇区的字节数必为512，因为如果不是512，则初始化不能通过.														  
uint32_t SD_GetSectorCount(void)
{
    uint8_t csd[16];
    uint32_t Capacity;  
    uint8_t n;
	uint16_t csize;  					    
	//取CSD信息，如果期间出错，返回0
    if(SD_GetCSD(csd)!=0) return 0;	    
    //如果为SDHC卡，按照下面方式计算
    if((csd[0]&0xC0)==0x40)	 //V2.00的卡
    {	
		csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
		Capacity = (uint32_t)csize << 10;//得到扇区数	 		   
    }else//V1.XX的卡
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
		Capacity= (uint32_t)csize << (n - 9);//得到扇区数   
    }
    return Capacity;
}
//初始化SD卡
uint8_t sd_spi_init(void)
{
	uint8_t r1;      // 存放SD卡的返回值
	uint16_t retry;  // 用来进行超时计数
	uint8_t buf[4];  
	uint16_t i;
	uint8_t init_flag = FAILED;
	
  /*!< Initialize SD_SPI 初始化SPI相应管脚与处理器的SPI接口 */
	printf("> ---------------------------\r\n");
  printf("> SD Card Init in SPI mode.\r\n");
	
	SD_SPI_Init();		//初始化IO
 	SD_SPI_SpeedLow();	//设置到低速模式 
	retry=20;
	do
	{
    /*!< SD chip select high */
    SD_CS_HIGH();
  
    /*!< Send dummy byte 0xFF, 10 times with CS high */
    /*!< Rise CS and MOSI for 80 clocks cycles */		
		for(i=0;i<10;i++)
		{
			/*!< Send dummy byte 0xFF */
		SD_SPI_ReadWriteByte(0XFF);//发送最少74个脉冲
		}
		
		r1=SD_SendCmd(CMD0,0,0x95);//进入IDLE状态
		
		
	}while((r1!=0X01) && retry--);  /*!< Wait for In Idle State Response (R1 Format) equal to 0x01 */

	
 	sd_card.type =0;//默认无卡
	
	/*check version*/
	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)
			buf[i]=SD_SPI_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
			{
				retry=0XFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1=SD_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
				{
					for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);//得到OCR值
					if(buf[0]&0x40) sd_card.type =SD_TYPE_V2HC;    //检查CCS
					else sd_card.type =SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);		//发送CMD55
			r1=SD_SendCmd(CMD41,0,0X01);	//发送CMD41
			if(r1<=1)
			{		
				sd_card.type =SD_TYPE_V1;
				retry=0XFF;
				do //等待退出IDLE模式
				{
					SD_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1=SD_SendCmd(CMD41,0,0X01);//发送CMD41
				}while(r1&&retry--);
			}else
			{
				sd_card.type =SD_TYPE_MMC;//MMC V3
				retry=0XFF;
				do //等待退出IDLE模式
				{											    
					r1=SD_SendCmd(CMD1,0,0X01);//发送CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0) sd_card.type=SD_TYPE_ERR;//错误的卡
		}
	}
	SD_DisSelect();//取消片选
	SD_SPI_SpeedHigh();//高速
	
	switch(sd_card.type)
	{
		case SD_TYPE_MMC:
			printf("> SD type: MMC\r\n");
			break;
		case SD_TYPE_V1:
			printf("> SD type: V1\r\n");
			break;
		case SD_TYPE_V2:
			printf("> SD type: V2\r\n");
			break;
		case SD_TYPE_V2HC:
			printf("> SD type: V2HC \r\n");
			break;
		default: 
			printf("> * unkown sd card! *\r\n");
			break;
	}	

	if(sd_card.type)
	{
		init_flag = PASSED;
		printf("\r\nSD Card OK.\r\n");		
		return PASSED;
	}
	else
	{
		init_flag = FAILED;
		printf("\r\nSD Card Error.\r\n");		
		return FAILED;	
	}
	
}



//读SD卡
//buf:数据缓存区
//sector:扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
uint8_t SD_ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt)
{
	uint8_t r1;
	if(sd_card.type !=SD_TYPE_V2HC)sector <<= 9;//转换为字节地址
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD17,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1=SD_RecvData(buf,512);//接收512个字节	   
		}
	}else
	{
		r1=SD_SendCmd(CMD18,sector,0X01);//连续读命令
		do
		{
			r1=SD_RecvData(buf,512);//接收512个字节	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_SendCmd(CMD12,0,0X01);	//发送停止命令
	}   
	SD_DisSelect();//取消片选
	return r1;//
}
//写SD卡
//buf:数据缓存区
//sector:起始扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
uint8_t SD_WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt)
{
	uint8_t r1;
	if(sd_card.type !=SD_TYPE_V2HC)sector *= 512;//转换为字节地址
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD24,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1=SD_SendBlock(buf,0xFE);//写512个字节	   
		}
	}else
	{
		if(sd_card.type !=SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);//发送指令	
		}
 		r1=SD_SendCmd(CMD25,sector,0X01);//连续读命令
		if(r1==0)
		{
			do
			{
				r1=SD_SendBlock(buf,0xFC);//接收512个字节	 
				buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);//接收512个字节 
		}
	}   
	SD_DisSelect();//取消片选
	return r1;//
}	
