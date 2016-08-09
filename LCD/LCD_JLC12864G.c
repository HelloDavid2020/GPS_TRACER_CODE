//液晶屏型号：JLX12864G-086-PC-3S
//接口：串行
//中文字库：带中文字库IC:JLX-GB2312,竖置横排
//驱动IC:UC1701X(与ST7565R兼容)
//主板：89C516-001老主板
//单片机：51系列
//版权所有：深圳市晶联讯电子有限公司：网址  http://www.jlxlcd.cn;
//编程：叶建人，2013年10月11日   
#include "includes.h"


//#include <reg52.H>
//#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long







//sbit lcd_sclk=P3^2;  //接口定义:lcd_sclk就是LCD的SCLK
//sbit lcd_sid=P3^1;   //接口定义:lcd_sid就是LCD的SDA
//sbit lcd_rs=P3^0;    //接口定义:lcd_rs就是LCD的RS,也叫“CD”
//sbit lcd_reset=P1^0; //接口定义:lcd_reset就是LCD的RESET
//sbit lcd_cs1=P1^1;   //接口定义:lcd_cs1就是LCD的CS1

//sbit Rom_IN=P1^2;    //字库IC接口定义:Rom_IN就是字库IC的SI
//sbit Rom_OUT=P1^3;   //字库IC接口定义:Rom_OUT就是字库IC的SO
//sbit Rom_SCK=P1^4;   //字库IC接口定义:Rom_SCK就是字库IC的SCK
//sbit Rom_CS=P1^5;    //字库IC接口定义Rom_CS就是字库IC的CS#

//sbit key=P2^0;		//定义一个按键


uchar lcd_sclk=2;  //接口定义:lcd_sclk就是LCD的SCLK
uchar lcd_sid=1;   //接口定义:lcd_sid就是LCD的SDA
uchar lcd_rs=0;    //接口定义:lcd_rs就是LCD的RS,也叫“CD”
uchar lcd_reset=0; //接口定义:lcd_reset就是LCD的RESET
uchar lcd_cs1=1;   //接口定义:lcd_cs1就是LCD的CS1

uchar Rom_IN=2;    //字库IC接口定义:Rom_IN就是字库IC的SI
uchar Rom_OUT=3;   //字库IC接口定义:Rom_OUT就是字库IC的SO
uchar Rom_SCK=4;   //字库IC接口定义:Rom_SCK就是字库IC的SCK
uchar Rom_CS=5;    //字库IC接口定义Rom_CS就是字库IC的CS#

uchar key=0;		//定义一个按键

void lcd_int_jxl12864g(void)
{
				
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE)  ; 
	//RS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//RW
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//CS1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	//D0-D7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

uchar  bmp1[];

uchar  jiong1[]={//--  文字:  囧  --
//--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --
0x00,0xFE,0x82,0x42,0xA2,0x9E,0x8A,0x82,0x86,0x8A,0xB2,0x62,0x02,0xFE,0x00,0x00,
0x00,0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x7F,0x00,0x00};

uchar  lei1[]={//--  文字:  畾  --
//--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --
0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00,
0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00};


//延时
void delay(int n_ms)               
{
	n_ms = n_ms > 10000UL ? 10000UL : n_ms;
	
	n_ms =4000*n_ms;
	while(n_ms--)
	{
			for(;n_ms>0;n_ms--)
			__nop();	
	}
}
//短延时
void delay_us(int n_us)               
{
	n_us =400*n_us;
	while(n_us--)
	{
			for(;n_us>0;n_us--)
			__nop();	
	}
}

//等待一个按键，我的主板是用P2.0与GND之间接一个按键
void waitkey()
{
repeat:
  	if (key==1) goto repeat;
	else delay(600);
}



//写指令到LCD模块
void transfer_command_lcd(int data1)   
{
	char i;
	lcd_cs1=0;
	lcd_rs=0;
	for(i=0;i<8;i++)
   {
		lcd_sclk=0; 
		//delay_us(10);	 //加少量延时
		if(data1&0x80) lcd_sid=1;
		else lcd_sid=0;
		lcd_sclk=1;
		//delay_us(10);	 //加少量延时
	 	data1=data1<<=1;
   }
	lcd_cs1=1;
}

//写数据到LCD模块
void transfer_data_lcd(int data1)
{
	char i;
	lcd_cs1=0;
	lcd_rs=1;
	for(i=0;i<8;i++)
	{
		lcd_sclk=0;
		if(data1&0x80) lcd_sid=1;
		else lcd_sid=0;
		lcd_sclk=1;
		data1=data1<<=1;
	}
	lcd_cs1=1;
}




//LCD模块初始化
void initial_lcd()
{
	lcd_reset=0;        		//低电平复位
	delay(100);
	lcd_reset=1;		    	//复位完毕
	delay(100);        
	transfer_command_lcd(0xe2);	 //软复位
	delay(5);
	transfer_command_lcd(0x2c);  //升压步聚1
	delay(50);	
	transfer_command_lcd(0x2e);  //升压步聚2
	delay(50);
	transfer_command_lcd(0x2f);  //升压步聚3
	delay(5);
	transfer_command_lcd(0x23);  //粗调对比度，可设置范围0x20～0x27
	transfer_command_lcd(0x81);  //微调对比度
	transfer_command_lcd(0x28);  //微调对比度的值，可设置范围0x00～0x3f
	transfer_command_lcd(0xa2);  //1/9偏压比（bias）
	transfer_command_lcd(0xc8);  //行扫描顺序：从上到下
	transfer_command_lcd(0xa0);  //列扫描顺序：从左到右
	transfer_command_lcd(0x40);  //起始行：第一行开始
	transfer_command_lcd(0xaf);  //开显示
}

void lcd_address(uint page,uint column)
{
	column=column-0x01;
	transfer_command_lcd(0xb0+page-1);   //设置页地址，每8行为一页，全屏共64行，被分成8页
	transfer_command_lcd(0x10+(column>>4&0x0f));	//设置列地址的高4位
	transfer_command_lcd(column&0x0f);	//设置列地址的低4位	
}

//全屏清屏
void clear_screen()
{
	unsigned char i,j;
	
		
	for(i=0;i<9;i++)
	{
		transfer_command_lcd(0xb0+i);
		transfer_command_lcd(0x10);
		transfer_command_lcd(0x00);
		for(j=0;j<132;j++)
		{
		  	transfer_data_lcd(0x00);
		}
	}
}

//显示128x64点阵图像
void display_128x64(uchar *dp)
{
	uint i,j;
	
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,1);
		for (i=0;i<128;i++)
		{	
			transfer_data_lcd(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}


//显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
void display_graphic_16x16(uchar page,uchar column,uchar *dp)
{
	uint i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			transfer_data_lcd(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}


//显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
void display_graphic_8x16(uchar page,uchar column,uchar *dp)
{
	uint i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<8;i++)
		{	
			transfer_data_lcd(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}


//显示5X8点阵图像、ASCII, 或5x8点阵的自造字符、其他图标
void display_graphic_5x8(uchar page,uchar column,uchar *dp)
{
	uint i;
	lcd_address(page,column);
	for (i=0;i<6;i++)
	{	
		transfer_data_lcd(*dp);
		dp++;
	}
	
}

//送指令到晶联讯字库IC
void send_command_to_ROM( uchar datu )
{
	uchar i;
	for(i=0;i<8;i++ )
	{
		Rom_SCK=0;
		delay_us(10);
		if(datu&0x80)Rom_IN = 1;
		else Rom_IN = 0;
		datu = datu<<1;
		Rom_SCK=1;
		delay_us(10);
	}
}

//从晶联讯字库IC中取汉字或字符数据（1个字节）
static uchar get_data_from_ROM( )
{
	uchar i;
	uchar ret_data=0;
	for(i=0;i<8;i++)
	{
		Rom_OUT=1;
		Rom_SCK=0;
		//delay_us(1);
		ret_data=ret_data<<1;
		if( Rom_OUT )
			ret_data=ret_data+1;
		else
			ret_data=ret_data+0;
		Rom_SCK=1;
		//delay_us(1);
	}
	return(ret_data);
}


//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_16x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	Rom_CS = 0;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for(i=0; i<16; i++ )
	    {
			disp_data=get_data_from_ROM();
			transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	Rom_CS=1;
}

//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_8x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	Rom_CS = 0;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for(i=0; i<8; i++ )
	    {
			disp_data=get_data_from_ROM();
			transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	Rom_CS=1;
}

//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_5x8(ulong fontaddr,uchar page,uchar column) 
{
	uchar i,disp_data;
	Rom_CS = 0;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
	lcd_address(page,column);
	for(i=0; i<5; i++ )
	{
		disp_data=get_data_from_ROM();
		transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
	}
	Rom_CS=1;
}


//****************************************************************


ulong  fontaddr=0;
void display_GB2312_string(uchar page,uchar column,uchar *text)
{
	uchar i= 0;			
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);

			get_and_write_16x16(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
	
			get_and_write_16x16(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{									
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			

			get_and_write_8x16(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=1;
			column+=8;
		}
		else
			i++;	
	}
	
}

void display_string_5x8(uchar page,uchar column,uchar *text)
{
	unsigned char i= 0;
	while((text[i]>0x00))
	{
		
		if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{						
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*8);
			fontaddr = (unsigned long)(fontaddr+0x3bfc0);			


			get_and_write_5x8(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中

			i+=1;
			column+=6;
		}
		else
		i++;	
	}
}


////===============main program===================
//void main(void)
//{     
//	while(1)
//	{
//		initial_lcd();	
//		clear_screen();    //clear all dots
//		display_128x64(bmp1);
//		delay(2000);		
//		display_GB2312_string(1,1,"12864,带中文字库");	//在第1页，第1列，显示一串16x16点阵汉字或8x16的ASCII字
//		display_GB2312_string(3,1,"16X16简体汉字库,");  //显示一串16x16点阵汉字或8x16的ASCII字.以下雷同
//		display_GB2312_string(5,1,"或8X16点阵ASCII,");
//		display_GB2312_string(7,1,"或5x8点阵ASCII码");		        
//		delay(4000);
//		clear_screen();	
//		display_GB2312_string(1,1,"晶联讯成立于二零");	
//		display_GB2312_string(3,1,"零四年十一月七日");	
//		display_GB2312_string(5,1,"主要生产液晶模块");	
//		display_GB2312_string(7,1,"品质至上真诚服务");	
//		delay(4000);			
//		display_GB2312_string(1,1,"GB2312简体字库及");	
//		display_GB2312_string(3,1,"有图型功能，可自");	
//		display_GB2312_string(5,1,"编大字或图像或生");	
//		display_GB2312_string(7,1,"僻字，例如：    ");
//		display_graphic_16x16(7,97,jiong1);					//在第7页，第81列显示单个自编生僻汉字“囧”
//		display_graphic_16x16(7,113,lei1);					//显示单个自编生僻汉字"畾“
//		delay(4000);		
//		clear_screen();			
//		display_GB2312_string(1,1,"<!@#$%^&*()_-+]/");	//在第1页，第1列，显示一串16x16点阵汉字或8*16的ASCII字
//		display_string_5x8(3,1,"<!@#$%^&*()_-+]/;.,?[");//在第3页，第1列，显示一串5x8点阵的ASCII字
//		display_string_5x8(4,1,"JLX electronics Co., ");//显示一串5x8点阵的ASCII字
//		display_string_5x8(5,1,"Ltd. established at  ");//显示一串5x8点阵的ASCII字	
//		display_string_5x8(6,1,"year 2004.Focus LCM. ");//显示一串5x8点阵的ASCII字
//		display_string_5x8(7,1,"TEL:0755-29784961    ");//显示一串5x8点阵的ASCII字
//		display_string_5x8(8,1,"FAX:0755-29784964    ");//显示一串5x8点阵的ASCII字
//		delay(4000);
//		display_GB2312_string(1,1,"啊阿埃挨哎唉哀皑");	//在第1页，第1列，显示一串16x16点阵汉字或8x16的ASCII字
//		display_GB2312_string(3,1,"癌蔼矮艾碍爱隘鞍");  //显示一串16x16点阵汉字或8x16的ASCII字.以下雷同
//		display_GB2312_string(5,1,"氨安俺按暗岸胺案");
//		display_GB2312_string(7,1,"肮昂盎凹敖熬翱袄");		        
//		delay(4000);	
//		display_GB2312_string(1,1,"鬟鬣麽麾縻麂麇麈");
//		display_GB2312_string(3,1,"麋麒鏖麝麟黛黜黝");
//		display_GB2312_string(5,1,"黠黟黢黩黧黥黪黯");
//		display_GB2312_string(7,1,"鼢鼬鼯鼹鼷鼽鼾齄");		        
//		delay(4000);
//	}
//}

uchar  bmp1[]={
//--  调入了一幅图像：D:\我的文档\My Pictures\12864-555.bmp  --
//--  宽度x高度=128x64  --
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xE0,0x60,0xE0,0xE0,0xE0,0xE0,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x18,0x1C,0x0C,
0x0E,0x07,0x03,0x03,0x01,0x81,0xE0,0x78,0x1C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x1C,0x79,0xE1,0x83,
0x03,0x07,0x0E,0x0C,0x1C,0x18,0x38,0x70,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,
0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0xF0,0x78,0x1E,0x07,0x03,0x81,0x80,0xC0,0xC0,0xC0,0xE0,0x60,
0x60,0x60,0x70,0xF0,0xFE,0x3F,0x19,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x38,0x31,0x3F,
0xFE,0xF0,0x60,0x60,0xE0,0xC0,0xC0,0xC0,0x80,0x81,0x03,0x07,0x1E,0x78,0xF0,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,0xFF,0x7F,0x07,0x0F,0x1F,0x3D,
0x79,0x71,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
0x80,0xC0,0xE0,0xF0,0x71,0x79,0x3D,0x1F,0x0F,0xFF,0xFE,0xFC,0x00,0x00,0x00,0x00,
0x00,0x80,0xFE,0xFF,0xF9,0x1C,0x0E,0x07,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x0E,0x1C,0xF9,0xFF,
0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x0E,0x9E,0xFC,0xF8,0xF0,0xE0,0xC0,0xC0,0x80,
0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xE0,0xF0,0xF8,0xF8,0xFC,0x9E,0x0F,0x07,
0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x3F,0xFF,0xDF,0x38,0x70,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0x70,0x38,0xDF,0xFF,
0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x80,0xC0,0xE0,0xF0,0x78,0x3C,0x1E,0x0F,0x07,0x03,0x01,0x00,0x01,0x01,0x03,0x07,
0x0F,0x1E,0x1E,0x0E,0x0F,0x07,0x03,0x01,0x01,0x00,0x01,0x01,0x03,0x07,0x0F,0x1E,
0x3C,0x78,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x07,0x0F,0x3C,0x70,0xE0,0xC1,0x81,0x03,0x03,0x03,0x07,0x06,
0x06,0x06,0x0E,0x0F,0x7F,0xFC,0x98,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x0C,0x8C,0xFC,
0x7F,0x0F,0x06,0x06,0x07,0x03,0x03,0x03,0x81,0xC1,0xE0,0x70,0x3C,0x0F,0x07,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,0x3F,0x7C,0xFC,0xFE,0xEF,
0xE7,0xE3,0xE1,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE1,0xE3,0xE7,0xEF,0xFE,0x7C,0x3F,0x1F,0x0F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x0C,0x1C,0x18,
0x38,0x70,0x60,0xE0,0xC0,0xC1,0x87,0x9E,0xB8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xB8,0xDE,0xC7,0xE1,
0x60,0x70,0x38,0x18,0x1C,0x0C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x07,0x07,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x01,0x01,0x01,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};
