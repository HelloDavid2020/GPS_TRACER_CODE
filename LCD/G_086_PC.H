#ifndef __G_086_PC_H_
#define __G_086_PC_H_



#include "includes.h"

#define lcd_cs1 		PDout(3)		// PA0对应液晶屏接口"cs1"（片选）
#define lcd_reset 	PDout(4)		// PA1对应液晶屏接口"reset"	(复位）
#define lcd_rs			PDout(5)		// PA2对应液晶屏接口"rs"（Register Select:指令或数据寄存器选择）	
#define lcd_sid 		PDout(6)		// PA3对应液晶屏接口"SDA"（串行数据）	
#define lcd_sclk 		PDout(7)		// PA4对应液晶屏接口"SCLK"（串行时钟）	

#define Rom_CS			PBout(8)  	// PA5对应字库IC接口"Rom_CS"(也即：字库IC的CS#）
#define Rom_SCK			PBout(9)  	// PA6对应字库IC接口"Rom_SCK"（也即：字库IC的SCK）
#define Rom_OUT			PEin(0)  	// PA7对应字库IC接口"Rom_OUT"（也即：字库IC的SO。SPI接口的"MISO" ）
#define Rom_IN			PEout(1)  	// PC4对应字库IC接口"Rom_IN"（也即：字库IC的SI。SPI接口的"MOSI"）

#define LCD_PWR_OFF() 	GPIO_SetBits(GPIOB, GPIO_Pin_7);
#define LCD_PWR_ON()    GPIO_ResetBits(GPIOB, GPIO_Pin_7);

//--------------------------
#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long
//--------------------------
extern u32  fontaddr;
extern uchar bmp_12864_1[];
extern uchar bmp_12864_2[];


extern void waitkey(void);
//初始化PA口为输出口.并使能这个IO口的时钟		    
//IO初始化
extern void lcd_port_init(void);



//写指令到LCD模块
extern void transfer_command_lcd(int data1)   ;
//写数据到LCD模块
extern void transfer_data_lcd(int data1);


//LCD模块初始化
extern void initial_lcd();
//设置液晶屏的显示地址（page，column）
extern void lcd_address(uchar page,uchar column);

//全屏清屏
extern void clear_screen();
//一些出厂电测用的画面，比如全显，隔行显示，隔列显示等等
extern void full_screen_test(uchar data1,uchar data2);

//一些出厂电测用的画面，比如全显，隔行显示，隔列显示等等
extern void test( void);

//显示128x64点阵图像
extern void display_graphic_128x64(uchar page,uchar column,uchar *dp);

//***送指令到晶联讯字库IC**
extern void send_command_to_ROM( uchar datu );


//***从晶联讯字库IC中取汉字或字符数据（1个字节）**
static uchar get_data_from_ROM( );


//从指定地址读出数据写到液晶屏指定（page,column)座标中
extern void get_and_write_16x16(ulong fontaddr,uchar reverse,uchar page,uchar column);


//从指定地址读出数据写到液晶屏指定（page,column)座标中
extern void get_and_write_8x16(u32 fontaddr,uchar reverse,uchar page,uchar column);


//从指定地址读出数据写到液晶屏指定（page,column)座标中
extern void get_and_write_5x8(u32 fontaddr,uchar reverse,uchar page,uchar column); 


//****************************************************************

//显示16x16点阵的汉字/全角符号/全角标点，或8x16点阵的数字/英文/半角标点/ASCII码符号，
//当“reverse=1”时选择反显，当“reverse=0”时选择正显
extern void display_GB2312_string(uchar page,uchar column,uchar reverse,uchar *text);


//显示5x8点阵数字/半角符号/半角标点
//当“reverse=1”时选择反显，当“reverse=0”时选择正显
extern void display_string_5x8(uchar page,uchar column,uchar reverse,uchar *text);
extern void display_char_5x8(uchar page,uchar column,uchar reverse,uchar text);
void  lcd_show_rx_buf(char *str);
#endif

