
#ifndef __LCD_H
#define __LCD_H

#include "includes.h"

#define DISP_OFF 0X3E    //�ر���ʾ
#define DISP_ON 0X3F    //����ʾ
#define START_ROW 0xC0    //������ʼ��
#define FIRST_PAGE 0xB8    //����ҳ
#define START_YADD 0x40    //������

#define ADDR_PAGE 0xB8    //����ҳ
#define ADDR_COLU 0x40    //������


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
	unsigned char buf[8][192];   	   // ��ʾ���ݻ���  8ҳ 192�ֽ�	
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
** ��������: void  LCD_Ini(void)
** ��������:  
** �䡡��: 	  
** �䡡��: 
** ˵  ����	 ��ʼ�� 
********************************************************************************************************/
extern  void  LCD_Ini(void);//
/********************************************************************************
** ��������: void LCM_Reflsh(unsigned char all)	
** ��������: 
** �䡡��: 	  all==0   ÿ��ִ�к���ֻˢ��64���е�8�� ���� ȫ��ˢ�� 
** �䡡��: 
** ˵  ����	  �˺�����������ѭ��ˢ��	 
              ��ʾ���ڴ������8ҳx192�У�1536�ֽڣ���Ƶ36MHz��һ��ˢ��8�Σ�1.4ms/�Σ�;
 
  	
  ҳ	 �е�ַ
  1    1-----------------------------192x1
  2    193---------------------------192x2
  3    385---------------------------192x3
  4    577---------------------------192x4
  5    769---------------------------192x5
  6    961---------------------------192x6
  7    1153--------------------------192x7
  8    1345--------------------------192x8
  
        ��һ��                      ��192��
          D0 ........................ D0
          .  ........................ .
  ��1ҳ   .  ........................ .
          .  ........................ .
          D7 ........................ D7

  ����7ҳ������˳�����һҳ��ͬ

 *******************************************************************************/																	  
extern void LCM_Reflsh(unsigned char all);	

/*********************************************************************************************************
** ��������: unsigned char  LCD_FileWR(unsigned char posen,unsigned char x,unsigned char y,unsigned char * src,unsigned char len,unsigned char forceascii)
** ��������:  ����Ӣ��	   y��8�ı��� (y/8)��λ	 x���ж�λ src: ��Ӣ��GB
** �䡡��: 	   rev:>0����    forceascii: >0 ǿ��Ϊascii  ����>=a1 ����Ϊ������

********************************************************************************************************/
extern unsigned char  LCD_FileWR(unsigned short x,unsigned short y,unsigned char * src,unsigned char len,unsigned char rev,unsigned char forceascii);
/********************************************************************************
   ȫ��������ݣ���Ƶ36MHz��ִ��ʱ��Լ ms
 *******************************************************************************/
extern void LCD_Fill (unsigned char dat);
extern void LCD_Fill_BMP (unsigned char *dat,unsigned char len);
extern void lcd_fill_test (unsigned char page,unsigned char dat);
/*********************************************************************************************************
** ��������: void  LCD_Line(unsigned short x0,unsigned short y0,unsigned short x1,unsigned short y1)
** ��������: ��ֱ��	   ���Ͻ�Ϊ0 ���½�Ϊ��LCD_PIXELx��LCD_PIXELy��
** �䡡��: 	  
** �䡡��: 
** ˵  ����	 
** ˵  ����	 
********************************************************************************************************/
extern void  LCD_Line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char rev);

/*********************************************************************************************************
** ��������: void  LCD_8x16(unsigned short x0,unsigned short y0,unsigned char rev)
** ��������:  ��һ��16x16����  ע��y Ӧ����8�ı���
** �䡡��:   x0:0-(LCD_PIXELx-8)  y0:0-(LCD_PIXELy-16) rev:0-���� ���򷴰�
** �䡡��: 
** ˵  ����
**   d0.0-------------------d15.0
**   d0.7-------------------d15.7   .
**   d16.0------------------d31.0   .
**   d16.7------------------d31.7
********************************************************************************************************/
extern void  LCD_16x16(unsigned short x0,unsigned short y0,unsigned char * src,unsigned char rev);
/*********************************************************************************************************
** ��������: void  LCD_AreaRev(unsigned char  x,unsigned char  y,unsigned char  n)
** ��������:ȡ������ֽ�
** �䡡��: 	 
** �䡡��: 
** ˵  ����Ӧ�������趨���ƶ�����	 ���� �ϵ��� �����Զ�ͣ   
********************************************************************************************************/
extern void  LCD_AreaRev(unsigned char  x,unsigned char  y,unsigned short  n);

/*********************************************************************************************************
** ��������: void  LCD_8x16(unsigned short x0,unsigned short y0,unsigned char rev)
** ��������:  ��һ��8x16����  ע��y0 Ӧ����8�ı���
** �䡡��:   x0:0-(LCD_PIXELx-8)  y0:0-(LCD_PIXELy-16) rev:0-���� ���򷴰�
** �䡡��: 
** ˵  ����
**   d0.0---d7.0 
**      .
**      .
**   d8.7---d15.7
********************************************************************************************************/
void  LCD_8x16(unsigned short x0,unsigned short y0,unsigned char * src,unsigned char rev);
extern void  LCD_15x21(unsigned short x0,unsigned short y0,unsigned char *src,unsigned char rev);
void LCD_BackLight(unsigned char en);          //����
#endif


