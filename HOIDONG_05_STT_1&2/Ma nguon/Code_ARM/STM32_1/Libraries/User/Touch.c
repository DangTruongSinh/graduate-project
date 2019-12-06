/******************************************************************
Version V1.1
********************************************************************/

#include "stm32f10x.h"
#include "Touch.h"
#include "LCD.h"
#include "stdlib.h"
#include "math.h"
#include "24c02.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

uint16_t Point_Color = BLUE,Back_Color = WHITE;            //Thiet lap mau background va mau pixel

Pen_Holder Pen_Point;	                                     //Dinh nghia Pen

EXTI_InitTypeDef EXTI_InitStructure;

extern void Delay(__IO uint32_t nTime);
/**************************************************
** Function name: u8 Get_Adjdata(void)
** Descriptions: Lay gia tri hieu chinh
** Input parameters: No
** Output parameters: Tra ve 1 lan doc thanh cong
***************************************************/
u8 Get_Adjdata(void)
{
	s32 temp_data;
	u16  temp[8],i;
	temp_data = I2C_EE_BufferRead(temp,0x20,8); 
   if(temp[0] == 0xff)
   {
		for(i=0;i<8;i++)
		{
			temp[i] = 0;
		}
	   I2C_EE_BufferRead(temp,0x00,8);
	   temp_data = (s32)((temp[3]<<24)|(temp[2]<<16)|(temp[1]<<8)|temp[0]);
	   Pen_Point.xfac = (float)temp_data /100000000;
	
	    I2C_EE_BufferRead(temp,0x08,8);
	   temp_data = (s32)((temp[3]<<24)|(temp[2]<<16)|(temp[1]<<8)|temp[0]);
	   Pen_Point.yfac = (float)temp_data /100000000;
	
	   I2C_EE_BufferRead(temp,0x10,8);
	   temp_data = (s32)((temp[1]<<8)|temp[0]);
	   Pen_Point.xoff = temp_data;
	
	    I2C_EE_BufferRead(temp,0x18,8);
	   temp_data = (s32)((temp[1]<<8)|temp[0]);
	   Pen_Point.yoff = temp_data;
	   return 1;
   }

   return 0;

}
/**************************************************
** Function name: void TOUCH_INIT()
** Descriptions: Khoi tao man hinh cam ung
** Input parameters: No
** Output parameters: No
***************************************************/
void TOUCH_INIT(void)
{
	TOUCH_CONFIGURATION();
 	TOUCH_ADS7483_READ_ADS(&Pen_Point.X,&Pen_Point.Y);                     //Khoi tao toa do doc dau tien	 

  /* Connect PEN EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);

  /* Configure PEN EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on PEN EXTI line */
  EXTI_GenerateSWInterrupt(EXTI_Line4);

	 LCD_CLEAR(WHITE);                    
	 if(Get_Adjdata())return;              //Neu da hieu chinh
	 else			                             //Neu chua hieu chinh
	 { 										    
	   TOUCH_ADS7483_ADJUST();                     //Tien hanh hieu chinh man hinh
		 Save_Adjdata();	                   //Luu thong so hieu chinh
	}			
	
  Get_Adjdata();                         //Lay thong so hieu chinh	
}

/**************************************************
** Function name: void TOUCH_CONFIGURATION()
** Descriptions: Cau hinh cac chan 
** Input parameters: No
** Output parameters: No
***************************************************/
void TOUCH_CONFIGURATION()
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	//Khoi tao cac Pin lien quan den SPI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//Khoi tao cac Pin lien quan den SPI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//Khoi tao cac Pin lien quan den SPI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//Khoi tao cac Pin lien quan den SPI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//GPIO_SetBits(GPIOC, GPIO_Pin_6);
}

/**************************************************
** Function name: void TOUCH_PEN_INT_SET(uint8_t en)
** Descriptions: 1: cho phep ngat 0: cam ngat
** Input parameters: No
** Output parameters: Co ngat 
***************************************************/ 
void TOUCH_PEN_INT_SET(uint8_t en)
{
	if(en)EXTI->IMR|=1<<4;                        	
	else EXTI->IMR&=~(1<<4);                   
}

/**********************************************************
HAM KIEM TRA CO CHAM TOUCH CO CHONG DOI
Chuc nang: Kiem tra touch neu co cham thi chong doi va tra ve ket qua la 1
***********************************************************/
uint8_t TOUCH_PRESS(void)
{		
u8 di;
		if(Pen_Point.Key_Sta==Key_Down) 
		{ 	
				for(di=0;di<3;di++)
				{
					do
					{
							TOUCH_ADS7483_CONVERT_POSITION();
							Pen_Point.Key_Sta=Key_Up;	
							Delay(2);
					}
					while(PEN==0);			
				}
					return(1);					
		}
		else return(0);
}

/**************************************************
** Function name: void TOUCH_ADS7483_WRITE_BYTE(uint8_t num)  
** Descriptions: Ghi 1 byte du lieu SPI
** Input parameters: Du lieu duoc viet
** Output parameters: No
***************************************************/
void TOUCH_ADS7483_WRITE_BYTE(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0;                                                 //Tao xung canh len	, gui lenh 8 bit, moi xung 1 bit 	 
		TCLK=1;      
	} 			    
} 		 
  
/**************************************************
** Function name: uint16_t TOUCH_ADS7483_READ_BYTE_AD(uint8_t CMD)  
** Descriptions: Doc gia tri ADC tu IC 7846/7843/XPT2046/UH7846
** Input parameters: Lenh CMD
** Output parameters: Du lieu doc ve
***************************************************/
uint16_t TOUCH_ADS7483_READ_BYTE_AD(uint8_t CMD)	  
{ 	 
	uint8_t i;
	uint8_t count=0; 	  
	uint16_t Num=0; 
	TCLK=0;                                                   //Cho chan CLK xuong muc thap, trang thai nghi
	TCS=0;                                                    //Cho chan SS xuong 0 de bat dau hoat dong	 
	TOUCH_ADS7483_WRITE_BYTE(CMD);                                      //Gui lenh
	for(i=100;i>0;i--);
	//delay_us(6);																						//Voi ADS7846 thi thoi gian gui lenh can cho 6us
	TCLK=1;                                                   //Tao 1 xung canh len, bao hieu cham dut trang thai nghi   	    
	TCLK=0; 	 
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 	 
		TCLK=0;                                                 //Tao 16 xung de nhan 16 bit du lieu 	    	   
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;                                                  //Tuy nhien ADC cua 7843 chi co do phan giai 12 bit
	TCS=1;                                                    //Quay ve trang thai nghi	 
	return(Num);   
}

#define READ_TIMES 15 //So lan doc
#define LOST_VAL 5	  //Gia tri bi loai bo
/**************************************************
** Function name: uint16_t TOUCH_ADS7483_READ_XY(uint8_t xy)
** Descriptions: Doc lien tuc 15 lan gia tri toa do XY sau do loai bo 5 gia tri lon nhat va nho nhat roi tính giá tri trung binh
** Input parameters: Toa do can doc gia tri
** Output parameters: Gia tri cua toa do can doc 
***************************************************/
uint16_t TOUCH_ADS7483_READ_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=TOUCH_ADS7483_READ_BYTE_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)                                  //Doc tuan tu
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])                                         //Sap xep tang dan
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

/**************************************************
** Function name: uint8_t TOUCH_ADS7483_READ_ADS(uint16_t *x,uint16_t *y)
** Descriptions: Phoi hop voi ham ben tren de xem co nhan hay khong
** Input parameters: Dia chi xy dau tien
** Output parameters: Tra ve 1 lan doc thanh cong    
***************************************************/
uint8_t TOUCH_ADS7483_READ_ADS(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=TOUCH_ADS7483_READ_XY(CMD_RDX);
	ytemp=TOUCH_ADS7483_READ_XY(CMD_RDY);		
  // char buf[100];
//	sprintf (buf, "%d, %d\n",xtemp,ytemp);
//				LCD_SHOW_STRING(141,150,buf, BLACK,WHITE);		
	if(xtemp<100||ytemp<100)return 0;                              //Neu doc that bai
	*x=xtemp;
	*y=ytemp;
	return 1;                                                      //Neu doc thanh cong
}

#define ERR_RANGE 50 //Pham vi loi
/**************************************************
** Function name: uint8_t TOUCH_ADS7483_READ_ADS_2(uint16_t *x,uint16_t *y) 
** Descriptions: Doc gia tri ADC cua ADS7846 2 lan lien tiep và 2 gia tri nay khong duoc lech nhau qua 50 de dam bao doc chinh xac
** Input parameters: Dia chi xy dau tien
** Output parameters: Tra ve 1 lan doc thanh cong    
***************************************************/
uint8_t TOUCH_ADS7483_READ_ADS_2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=TOUCH_ADS7483_READ_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=TOUCH_ADS7483_READ_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))			//Hai gia tri khong lech nhau qua 50
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {

        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
				//char buf[100];
				//sprintf (buf, "%d, %d\n",*x,*y);
				//LCD_SHOW_STRING(141,150,buf, BLACK,WHITE);
        return 1;
    }else return 0;	  
} 
		
/**************************************************
** Function name: uint8_t TOUCH_ADS7483_READ_TP_ONCE(void)
** Descriptions: Doc gia tri cua toa do den khi nao Pen tac dong
** Input parameters: No
** Output parameters: Tra ve 1 lan doc thanh cong
***************************************************/
uint8_t TOUCH_ADS7483_READ_TP_ONCE(void)
{
	uint8_t t=0;	    
	TOUCH_PEN_INT_SET(0);                            //Khong cho phep ngat
	Pen_Point.Key_Sta=Key_Up;
	TOUCH_ADS7483_READ_ADS_2(&Pen_Point.X,&Pen_Point.Y);
	while(PEN==0&&t<=250)
	{
		t++;
		Delay(10);
	};
	TOUCH_PEN_INT_SET(1);                             //Cho phep ngat		 
	if(t>=250)return 0;                         //Neu thoi gian nhan hon 250 thi kohng con hieu luc nua ( nhan roi )
	else return 1;	
}


/**************************************************
**Function name: void Drow_Touch_Point(uint8_t x,uint16_t y)
** Descriptions:
** Input parameters: x,y: toa do TFT
** Output parameters:No
***************************************************/
void Drow_Touch_Point(uint8_t x,uint16_t y)
{
	LCD_DRAW_LINE(x-12,y,x+13,y);              //Duong ngang
	LCD_DRAW_LINE(x,y-12,x,y+13);              //Duong thang
	LCD_DRAW_POINT(x+1,y+1);
	LCD_DRAW_POINT(x-1,y+1);
	LCD_DRAW_POINT(x+1,y-1);
	LCD_DRAW_POINT(x-1,y-1);
	LCD_DRAW_CIRCLE(x,y,6);                       //Duong tron
}	  

/**************************************************
** Function name void Draw_Big_Point(uint8_t x,uint16_t y,uint16_t color)
** Descriptions: Ve cac diem cham co kich thuoc 2*2
** Input parameters: x,y : toa do man hinh TFT   color : mau sac cac diem cua Big point
** Output parameters:No
***************************************************/
void Draw_Big_Point(uint8_t x,uint16_t y,uint16_t color)
{
	LCD_DRAW_POINT(x,y);                           //Diem trung tam ( Diem mau )
	LCD_DRAW_POINT(x+1,y);
	LCD_DRAW_POINT(x,y+1);
	LCD_DRAW_POINT(x+1,y+1);	 	  	
}

/**************************************************
** Function name: void TOUCH_ADS7483_CONVERT_POSITION(void)
** Descriptions: Goi ham doc toa do diem cham neu co thi tinh toan va dua ve gia tri toa do
** Input parameters: No
** Output parameters: No
***************************************************/
void TOUCH_ADS7483_CONVERT_POSITION(void)
{		 	  
	if(TOUCH_ADS7483_READ_ADS_2(&Pen_Point.X,&Pen_Point.Y))
	{	
		Pen_Point.xfac = 0.130634;
		Pen_Point.yfac = 0.178230;
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  
			//	char buf[100];
			//	sprintf (buf, "%d, %d\n",Pen_Point.X0,Pen_Point.Y0);
			//	LCD_SHOW_STRING(141,150,buf, BLACK,WHITE);
	}
}

/**************************************************
** Function name: void TOUCH_ADS7483_ADJUST(void)
** Descriptions: Nhan 4 thong so hieu chinh
** Input parameters: No
** Output parameters: No
***************************************************/
void TOUCH_ADS7483_ADJUST(void)
{								 
	uint16_t pos_temp[4][2];                    //Tong hop cac gia tri luu tru
	uint8_t  cnt=0;	
	uint16_t d1,d2;
	uint32_t tem1,tem2;
	float fac; 	   
	cnt=0;				
	LCD_CLEAR(WHITE);               
	Drow_Touch_Point(20,20);                    //Hien thi diem thu nhat
	Pen_Point.Key_Sta=Key_Up;                   //Loai bo cac tin heu kich hoat 
	Pen_Point.xfac=0;//xfac Duoc su dung de danh dau viec hieu chuan, no phai duoc xoa truoc khi hieu chuan
	while(1)
	{
		if(Pen_Point.Key_Sta==Key_Down)           //Neu co nhan
		{
			if(TOUCH_ADS7483_READ_TP_ONCE())                      //Lay gia tri quan trong nhat
			{  								   
				pos_temp[cnt][0]=Pen_Point.X;
				pos_temp[cnt][1]=Pen_Point.Y;
				cnt++;
			}			 
			switch(cnt)
			{			   
				case 1:
					LCD_CLEAR(WHITE);            
					Drow_Touch_Point(220,20);            //Hien diem thu 2
					break;
				case 2:
					LCD_CLEAR(WHITE);
					Drow_Touch_Point(20,300);            //Hien diem thu 3
					break;
				case 3:
					LCD_CLEAR(WHITE);                 
					Drow_Touch_Point(220,300);           //Hien diem thu 4
					break;
				case 4:	                               //Neu da nhan trung het 4 diem
					
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);          //x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);          //y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);                               //Tinh toan khoang cach giua diem 1 va 2
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);          //x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);          //y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);                               //Tinh toan khoang cach giua diem 3 va 4
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)              //Neu that bai
					{
						cnt=0;
						LCD_CLEAR(WHITE);                  
						Drow_Touch_Point(20,20);
						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);          //x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);          //y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);                               //Tinh toan khoang cach diem 1 va 3
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);          //x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);          //y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);                               //Tinh toan khoang cach diem 2 va 4
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)                            //Neu that bai
					{
						cnt=0;
						LCD_CLEAR(WHITE);                          
						Drow_Touch_Point(20,20);
						continue;
					}                                                 //Tien hanh khac phuc
								   
					//Duong cheo bang
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);          //x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);          //y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);                               //Tinh khoang cach diem 1,4
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);          //x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);          //y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);                               //Tinh khoang cach diem 2,3
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)                            //Neu that bai
					{
						cnt=0;
						LCD_CLEAR(WHITE);                            
						Drow_Touch_Point(20,20);
						continue;
					}                                                  //Tien hanh sua chua
					                                                   //Ket qua la
					Pen_Point.xfac=(float)200/(pos_temp[1][0]-pos_temp[0][0]);            //OK
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//OK
						  
					Pen_Point.yfac=(float)280/(pos_temp[2][1]-pos_temp[0][1]);            //OK
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//OK  
					
					LCD_CLEAR(WHITE);
				char buf[100];
				sprintf (buf, "%f-%f-%f-%f",Pen_Point.xfac,Pen_Point.xoff,Pen_Point.yfac,Pen_Point.yoff);
				LCD_SHOW_STRING(0,150,buf, BLACK,WHITE);		
//while(1);					
				//	LCD_SHOW_STRING(35,110,"Touch Screen Adjust OK!",RED,WHITE);           //Bao hieu chinh hoan thanh
					Delay(1000);
					LCD_CLEAR(WHITE);                                                    
					return;                                                               //Hieu chuan hoan chinh		 
			}
		}
	} 
}

/**************************************************
** Function name: void Save_Adjdata(void)
** Descriptions: Luu cac thong so hieu chuan
** Input parameters: No
** Output parameters: No
***************************************************/
void Save_Adjdata(void)
{
	s32 temp_data;
	u16  temp[8];
	temp_data=Pen_Point.xfac*100000000;    //Luu he so hieu chinh x
	temp[0]=(u8)(temp_data & 0xff);        //Luu he so hieu chinh x
	temp[1]=(u8)((temp_data >> 8) & 0xff); //Luu he so hieu chinh x
	temp[2]=(u8)((temp_data >> 16) & 0xff);//Luu he so hieu chinh x	
	temp[3]=(u8)((temp_data >> 24) & 0xff);//Luu he so hieu chinh x
  I2C_EE_BufferWrite(temp,0x00,8);
	 
	temp_data=Pen_Point.yfac*100000000;    //Luu he so hieu chinh y
	temp[0]=(u8)(temp_data & 0xff);        //Luu he so hieu chinh y
	temp[1]=(u8)((temp_data >> 8) & 0xff); //Luu he so hieu chinh y	
	temp[2]=(u8)((temp_data >> 16) & 0xff);//Luu he so hieu chinh y	
	temp[3]=(u8)((temp_data >> 24) & 0xff);//Luu he so hieu chinh y			
  I2C_EE_BufferWrite(temp,0x08,8);	
  

	temp_data=Pen_Point.xoff;
	temp[0]=(u8)(temp_data &0xff);
	temp[1]=(u8)((temp_data >>8)&0xff);
   I2C_EE_BufferWrite(temp,0x10,8);	

	temp_data=Pen_Point.yoff;
	temp[0]=(u8)(temp_data &0xff);
	temp[1]=(u8)((temp_data >>8)&0xff);
  I2C_EE_BufferWrite(temp,0x18,8);	

	temp[0]=0xff;
  I2C_EE_BufferWrite(temp,0x20,8);	//Sau khi hieu chinh
}



