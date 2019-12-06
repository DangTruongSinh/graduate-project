#include "stm32f10x.h"
#include "hardware_conf.h"
#include "TV_PORT_CONF.h"
#include "TV_GLCD_ILI932x.h"
#include "TV_TOUCH.h"
#include "TV_DELAY.h"
#include "TV_LED.h" 
#include	"TV_NVIC.h"

extern u8 gImage_ICON_01_XANH_DUONG_47_58[];

uint8_t  TT_LED=0,RONG_X; 
uint16_t TT_TOUCH,DAI_Y; 

uint8_t VTX0_1=0,VTX0_2;
uint16_t VTY0_1=0,VTY0_2;
uint8_t VTX1_1=0,VTX1_2;
uint16_t VTY1_1=0,VTY1_2;

/**********************************************************
HAM XOA HINH BITMAP TRUOC KHI DI CHUYEN
***********************************************************/
void XOA_BITMAP()
{		
		uint8_t XC;
		uint16_t YC;
		for(YC=0;YC<VTY0_1+DAI_Y;YC++)
						{
							for(XC=VTX0_1;XC<VTX0_1+RONG_X;XC++)
							{
								LCD_DRAW_POINT(XC,YC);
							}
						}	
}
						
/**********************************************************
HAM KIEM TRA CO CHAM TOUCH 
***********************************************************/
uint8_t TOUCH_PRESS()
{		
		if(Pen_Point.Key_Sta==Key_Down) 
		{ 			
					do
					{
							TOUCH_ADS7483_CONVERT_POSITION();
							Pen_Point.Key_Sta=Key_Up;
							DELAY_MS(30);
					}while(PEN==0);						
					return(1);					
		}
		else return(0);
}
/**********************************************************
CHUONG TRINH CHINH
***********************************************************/
int main(void)
{
			SystemInit();	PORT_CONF();
			NVIC_CONFIGURATION_TOUCH();						
			LCD_INIT();		TOUCH_INIT();		LED_INIT_2();
			
			LCD_CLEAR(YELLOW);					
			BACK_COLOR=YELLOW;
			POINT_COLOR=YELLOW;	
			
			RONG_X=47; DAI_Y=58; 
			VTX0_2=VTX0_1+RONG_X;			
			VTY0_2=VTY0_1+DAI_Y;

		
			LCD_WRITE_BITMAP_Y(VTX0_1,VTY0_1,RONG_X,DAI_Y, gImage_ICON_01_XANH_DUONG_47_58);
			while(1)
			{	
						TT_TOUCH=TOUCH_PRESS();
						if (TT_TOUCH==1)
						{
															
								if (( Pen_Point.X0<200) &&(Pen_Point.Y0<280))									
									{
										XOA_BITMAP();
										VTX0_1=Pen_Point.X0;	
										VTY0_1=Pen_Point.Y0;
										LCD_WRITE_BITMAP_Y(VTX0_1,VTY0_1,RONG_X,DAI_Y, gImage_ICON_01_XANH_DUONG_47_58);										
									}
						}
			}
}
