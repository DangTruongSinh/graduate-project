#ifndef __TOUCH_H
#define __TOUCH_H	
#include "stm32f10x_conf.h"
/* ����״̬	*/ 
#define Key_Down 0x01
#define Key_Up   0x00 
/* �ʸ˽ṹ�� */
typedef struct 
{
	u16 X0;//ԭʼ����
	u16 Y0;
	u16 X; //����/�ݴ�����
	u16 Y;						   	    
	u8  Key_Sta;//�ʵ�״̬			  
	//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;
extern Pen_Holder Pen_Point;

/*������оƬ������������	*/   
#define PEN  PCin(4)   //PC4 INT
#define DOUT PAin(6)   //PA6  MISO
#define TDIN PAout(7)  //PA7  MOSI
#define TCLK PAout(5)  //PA5  SCLK
#define TCS  PCout(6)  //PC6  CS 

/* ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ� */
#define CMD_RDY 0X90  //0B10010000���ò�ַ�ʽ��X����
#define CMD_RDX	0XD0  //0B11010000���ò�ַ�ʽ��Y����  
 
void TOUCH_INIT(void);
void TOUCH_ADS7483_ADJUST(void);
void TOUCH_ADS7483_CONVERT_POSITION(void);
void TOUCH_PEN_INT_SET(uint8_t en);
void TOUCH_CONFIGURATION(void);
void TOUCH_ADS7483_WRITE_BYTE(uint8_t num);
uint16_t TOUCH_ADS7483_READ_BYTE_AD(uint8_t CMD);
uint16_t TOUCH_ADS7483_READ_XY(uint8_t xy);
uint8_t TOUCH_ADS7483_READ_TP_ONCE(void);
uint8_t TOUCH_ADS7483_READ_ADS_2(uint16_t *x,uint16_t *y);
uint8_t TOUCH_ADS7483_READ_ADS(uint16_t *x,uint16_t *y);
uint8_t TOUCH_PRESS(void);
void Drow_Touch_Point(uint8_t x,uint16_t y);
void Draw_Big_Point(uint8_t x,uint16_t y,uint16_t color);   //���
void Save_Adjdata(void);                                    //����У׼����
#endif
