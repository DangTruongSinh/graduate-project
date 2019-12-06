#ifndef __LCD_H_
#define __LCD_H_
#include "stm32f10x_conf.h"

/*使用位带操作，对单个IO灵活操作*/
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
/* 输入 */ 
#define PAin(n)      *((volatile unsigned long *)(0x42000000+((GPIOA_IDR_Addr-0x40000000)<<5)+(n<<2)))
#define PBin(n)      *((volatile unsigned long *)(0x42000000+((GPIOB_IDR_Addr-0x40000000)<<5)+(n<<2)))
#define PCin(n)      *((volatile unsigned long *)(0x42000000+((GPIOC_IDR_Addr-0x40000000)<<5)+(n<<2)))
#define PDin(n)      *((volatile unsigned long *)(0x42000000+((GPIOD_IDR_Addr-0x40000000)<<5)+(n<<2)))
#define PEin(n)      *((volatile unsigned long *)(0x42000000+((GPIOE_IDR_Addr-0x40000000)<<5)+(n<<2)))
#define PFin(n)      *((volatile unsigned long *)(0x42000000+((GPIOF_IDR_Addr-0x40000000)<<5)+(n<<2)))

/* 输出 */ 
#define PAout(n)   *((volatile unsigned long *)(0x42000000+((GPIOA_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PBout(n)   *((volatile unsigned long *)(0x42000000+((GPIOB_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PCout(n)   *((volatile unsigned long *)(0x42000000+((GPIOC_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PDout(n)   *((volatile unsigned long *)(0x42000000+((GPIOD_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PEout(n)   *((volatile unsigned long *)(0x42000000+((GPIOE_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define LCD_CS(n)         PDout(12)=n   
#define LCD_RS(n)         PDout(13)=n
#define LCD_WR(n)         PDout(14)=n
#define LCD_RD(n)         PDout(15)=n

#define DataToWrite(DATA)   GPIO_Write(GPIOE, DATA)  
#define DataToRead()       GPIO_ReadOutputData(GPIOE) 

/*Private define-------------------------*/
#define POINT_COLOR RED
#define BACK_COLOR   0xFFFF
/*--------16位颜色值---------------*/
#define WHITE             0xFFFF
#define BLACK             0x0000     
#define BLUE               0x001F  
#define RED               0xF800
#define MAGENTA           0xF81F
#define GREEN             0x07E0
#define CYAN              0x7FFF
#define YELLOW            0xFFE0
#define BROWN               0XBC40 //棕色
#define BRRED               0XFC07 //棕红色
#define GRAY                0X8430 //灰色
#define LGRAY               0XC618 //浅灰色



/*----------函数声明------------*/
/* 刷屏 */
void LCD_DRAW_PICTURE (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bitmap) ;
void LCD_CLEAR(uint16_t Color);
void LCD_DRAW_POINT(uint16_t xsta, uint16_t ysta);
void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour);
void LCD_DRAW_LINE(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void LCD_DRAW_CIRCLE(uint16_t x0, uint16_t y0, uint8_t r);
void LCD_DRAW_RECTANGLE(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void LCD_DRAW_TRIANGLE(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void LCD_DRAW_ELLIPSE(uint16_t xcenter,uint16_t ycenter,uint16_t xradius,uint16_t yradius);
void LCD_SHOW_NUM(u8 x,u8 y,u32 num,u8 len, u16 PenColor, u16 BackColor);
void LCD_SET_DISPLAY_WINDOW (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end);
void LCD_CLEAR_STRING(u16 x0, u16 y0, char *pcStr, u16 PenColor, u16 BackColor);
void LCD_WRITE_BITMAP_X(u8 Xpos, u16 Ypos, u8 Height, u16 Width, u8 *bitmap);
void LCD_DRAW_FLOWER(void);
void LCD_WRITE_BITMAP_Y(u8 Xpos, u16 Ypos, u8 Height, u16 Width, u8 *bitmap);
void LCD_DRAW_POINT_COLOR(uint16_t xsta, uint16_t ysta, uint16_t color);
unsigned short LCD_READ_REG (unsigned short reg);
void LCD_SET_CURSOR(u16 Xpos, u16 Ypos);
void LCD_DRAW_LINE_COLOR(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,uint16_t COLOR);
unsigned short LCD_READ_DATA (void);
void LCD_SHOW_MA_7DOAN(uint8_t X,uint16_t Y,uint8_t digit,uint8_t size,uint8_t mode);
void LCD_DRAW_CIRCLE_COLOR(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void LCD_DRAW_RECTANGLE_COLOR(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,uint16_t color);
/* 显示数字 */
//void LCD_ShowNum(u8 x, u8 y, u32 num, u8 len, u16 PenColor, u16 BackColor);
/*显示字符 包括中英文*/
void LCD_SHOW_STRING(u16 x0, u16 y0, char *pcStr, u16 PenColor, u16 BackColor);
/*显示图片 gImage取模 bmp格式*/
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u8 *pic);

void LCD_Show_hz(uint16_t x,uint16_t y,uint8_t *hz,u16 PenColor, u16 BackColor);
void LCD_HzShowString(uint16_t x,uint16_t y,uint8_t *p,u16 PenColor, u16 BackColor);

void LCD_CONFIGURATION(void);
void LCD_INIT(void);
void LCD_DisplayString(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor);
void LCD_DisplayStr(uint16_t x, 
                    uint16_t y, 
                    uint8_t * s, 
                    uint16_t PenColor, 
                    uint16_t BackColor,
                    uint8_t font);

#endif
