
#ifndef __LCD_H
#define __LCD_H

#include "includes.h"

#define DISP_OFF 0X3E    //关闭显示
#define DISP_ON 0X3F    //打开显示
#define START_ROW 0xC0    //设置起始行
#define FIRST_PAGE 0xB8    //设置页
#define START_YADD 0x40    //设置列

#define ADDR_PAGE 0xB8    //设置页
#define ADDR_COLU 0x40    //设置列


#define LCD_PORT   P0
#define LCD_RESET  P2_2
#define LCD_CSA    P2_3
#define LCD_CSB    P2_4
#define LCD_RS     P2_5
#define LCD_RW     P2_6
#define LCD_E      P2_7


extern void LCD_Initialize(void);
typedef struct
{
	unsigned char buf[8][192];   	   // 显示内容缓冲  8页 192字节	
}LCD_typeDef;

extern void  LCD_On(unsigned char en);
extern const uint8_t ASCII8x8_Table[];
extern const uint8_t ASCII8x16_Table[1536];
extern uint8_t lianhua[];
extern uint8_t hz1[];
#define LCD_PIXELx  192//192
#define LCD_PIXELy  64

#define LINE_0  (0)
#define LINE_1  (8*1)
#define LINE_2  (8*2)
#define LINE_3  (8*3)
#define LINE_4  (8*4)
#define LINE_5  (8*5)
#define LINE_6  (8*6)
#define LINE_7  (8*7)
#define LINE_8  (8*8)

extern LCD_typeDef	 LCD;


/*********************************************************************************************************
** 函数名称: void  LCD_Ini(void)
** 功能描述:  
** 输　入: 	  
** 输　出: 
** 说  明：	 初始化 
********************************************************************************************************/
extern  void  LCD_Ini(void);//
/********************************************************************************
** 函数名称: void LCM_Reflsh(unsigned char all)	
** 功能描述: 
** 输　入: 	  all==0   每次执行函数只刷新64行中得8行 否则 全部刷新 
** 输　出: 
** 说  明：	  此函数放主程序循环刷新	 
              显示放内存操作（8页x192列，1536字节，主频36MHz，一屏刷新8次，1.4ms/次）;
 
  	
  页	 列地址
  1    1-----------------------------192x1
  2    193---------------------------192x2
  3    385---------------------------192x3
  4    577---------------------------192x4
  5    769---------------------------192x5
  6    961---------------------------192x6
  7    1153--------------------------192x7
  8    1345--------------------------192x8
  
        第一列                      第192列
          D0 ........................ D0
          .  ........................ .
  第1页   .  ........................ .
          .  ........................ .
          D7 ........................ D7

  以下7页的排列顺序与第一页相同

 *******************************************************************************/																	  
extern void LCM_Reflsh(unsigned char all);	

/*********************************************************************************************************
** 函数名称: unsigned char  LCD_FileWR(unsigned char posen,unsigned char x,unsigned char y,unsigned char * src,unsigned char len,unsigned char forceascii)
** 功能描述:  画中英文	   y：8的倍数 (y/8)定位	 x：行定位 src: 中英文GB
** 输　入: 	   rev:>0反向    forceascii: >0 强制为ascii  否则>=a1 则认为是中文

********************************************************************************************************/
extern unsigned char  LCD_FileWR(unsigned short x,unsigned short y,unsigned char * src,unsigned char len,unsigned char rev,unsigned char forceascii);
/********************************************************************************
   全屏填充数据，主频36MHz，执行时间约 ms
 *******************************************************************************/
extern void LCD_Fill (unsigned char dat);
extern void LCD_Fill_BMP (unsigned char *dat,unsigned char len);
extern void lcd_fill_test (unsigned char page,unsigned char dat);
/*********************************************************************************************************
** 函数名称: void  LCD_Line(unsigned short x0,unsigned short y0,unsigned short x1,unsigned short y1)
** 功能描述: 画直线	   左上角为0 右下角为（LCD_PIXELx，LCD_PIXELy）
** 输　入: 	  
** 输　出: 
** 说  明：	 
** 说  明：	 
********************************************************************************************************/
extern void  LCD_Line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char rev);

/*********************************************************************************************************
** 函数名称: void  LCD_8x16(unsigned short x0,unsigned short y0,unsigned char rev)
** 功能描述:  画一个16x16点阵  注意y 应该是8的倍数
** 输　入:   x0:0-(LCD_PIXELx-8)  y0:0-(LCD_PIXELy-16) rev:0-正常 否则反白
** 输　出: 
** 说  明：
**   d0.0-------------------d15.0
**   d0.7-------------------d15.7   .
**   d16.0------------------d31.0   .
**   d16.7------------------d31.7
********************************************************************************************************/
extern void  LCD_16x16(unsigned short x0,unsigned short y0,unsigned char * src,unsigned char rev);
/*********************************************************************************************************
** 函数名称: void  LCD_AreaRev(unsigned char  x,unsigned char  y,unsigned char  n)
** 功能描述:取反多个字节
** 输　入: 	 
** 输　出: 
** 说  明：应该事先设定好移动方向	 左到右 上到下 超限自动停   
********************************************************************************************************/
extern void  LCD_AreaRev(unsigned char  x,unsigned char  y,unsigned short  n);

/*********************************************************************************************************
** 函数名称: void  LCD_8x16(unsigned short x0,unsigned short y0,unsigned char rev)
** 功能描述:  画一个8x16点阵  注意y0 应该是8的倍数
** 输　入:   x0:0-(LCD_PIXELx-8)  y0:0-(LCD_PIXELy-16) rev:0-正常 否则反白
** 输　出: 
** 说  明：
**   d0.0---d7.0 
**      .
**      .
**   d8.7---d15.7
********************************************************************************************************/
void  LCD_8x16(unsigned short x0,unsigned short y0,unsigned char * src,unsigned char rev);
extern void  LCD_15x21(unsigned short x0,unsigned short y0,unsigned char *src,unsigned char rev);
void LCD_BackLight(unsigned char en);          //背光
#endif


