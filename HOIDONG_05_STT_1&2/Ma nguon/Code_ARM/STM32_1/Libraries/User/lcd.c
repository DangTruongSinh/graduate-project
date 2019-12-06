//***********************************
#include "LCD.h"
#include "font.h"
#include "stdio.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "TV_FONT_MA7DOAN.h"
//#include "Flash.h"
uint16_t DeviceCode;                                           
#define Font_Size 16                 		                         //Font chu mac dinh 16*8
#define XY 1												               //Chon chieu hien thi: 1 la doc, 0 la ngang
/*********************************************************
** Function name:       void LCD_Delay (u32 nCount)
** Descriptions:        Delay 10ms
** input parameters:    nCount
** output parameters:   No
** Returned value:      No
*********************************************************/
void LCD_Delay(u32 nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*100;i++);
}

/*********************************************************
** Function name:       void LCD_WRITE_CMD(uint16_t LCD_Reg)
** Descriptions:        Thiet lap ghi lenh ( Gui lenh)
** input parameters:    Reg
** output parameters:   No
** Returned value:      No
*********************************************************/
void LCD_WRITE_CMD(uint16_t LCD_Reg)
{
	LCD_RD(1);					                      //Khi ghi thi chan RD luon o muc 1
	LCD_CS(0);					                      //Lua chon chip¡
	LCD_RS(0);					                      //Tin hieu D/C trong datasheet. Cho RS bang 0 ( Gui lenh )
	DataToWrite(LCD_Reg);											
	LCD_WR(0);				
	LCD_WR(1);					                      //Ghi lenh ( tao xung 0 len 1 )
	LCD_CS(1);		  
}

/*********************************************************
** Function name:       void LCD_WRITE_DATA(uint16_t LCD_Data)	
** Descriptions:        Thiet lap ghi du lieu ( Gui du lieu )
** input parameters:    Data
** output parameters:   No
** Returned value:      No
*********************************************************/
void LCD_WRITE_DATA(uint16_t LCD_Data)	
{
	LCD_RD(1);					                    //Khi ghi thi chan RD luon o muc 1
	LCD_CS(0);
	LCD_RS(1);					                    //Cho RS bang 1 ( Gui du lieu )
	DataToWrite(LCD_Data);
	LCD_WR(0);
	LCD_WR(1);					                    //Ghi lenh ( tao xung 0 len 1 )
	LCD_CS(1);	
} 

/*********************************************************
** Function name:       unsigned short LCD_READ_DATA (void)	
** Descriptions:        Doc du lieu
** input parameters:    No
** output parameters:   No
** Returned value:      Tra ve gia tri cua val
*********************************************************/
__inline unsigned short LCD_READ_DATA (void)
{				
  unsigned short val = 0;
  LCD_RS(1);
  LCD_WR(1);		                           //Khi doc thi chan WR luon o muc 1
  LCD_RD(0);		                           //Cho chan RD xuong 0 de doc
  GPIOE->CRH = 0x44444444;                 //Cac byte ghi du lieu doc ve
	GPIOE->CRL = 0x44444444;
	val = GPIOE->IDR;                        //Doc du lieu 8 byte cao
	val = GPIOE->IDR;                        //Doc du lieu 8 byte thap
	GPIOE->CRH = 0x33333333;                 //Thiet lap dau ra
	GPIOE->CRL = 0x33333333;
	LCD_RD(1);															 ////Cho chan RD len 1 de dung doc
	return val;
}

/*********************************************************
** Function name:       void wr_cmd (unsigned char c) 
** Descriptions:        Ghi lenh
** input parameters:    Lenh
** output parameters:   No
** Returned value:      No
*********************************************************/
__inline void wr_cmd (unsigned char c) 
{
	LCD_RS(0);                                 
	LCD_RD(1);                                 
	GPIOE->ODR = c;                             
	LCD_WR(0);                                  
	LCD_WR(1);                           
}

/*********************************************************
** Function name:       unsigned short LCD_READ_REG (unsigned short reg) 
** Descriptions:        Doc thanh ghi
** input parameters:    Thanh ghi
** output parameters:   No
** Returned value:      Tra ve gia tri cua thanh ghi
*********************************************************/
unsigned short LCD_READ_REG (unsigned short reg)
{ 
	LCD_CS(0);                    
	wr_cmd(reg);                            //Chon thanh ghi can doc
	reg = LCD_READ_DATA();      	                  //Doc gia tri thanh ghi
	LCD_CS(1);
	return reg;                             //Tra ve gia tri thanh ghi
}

/*********************************************************************************
** Function name:     void LCD_WRITE_REG(uint16_t LCD_Reg ,uint16_t LCD_RegValue)
** Descriptions:      Cau hinh thanh ghi
** input parameters:  Thanh ghi:LCD_Reg   Du lieu: RegValue
** output parameters: ÎÞ
** Returned value:    ÎÞ
**********************************************************************************/
void LCD_WRITE_REG(uint16_t LCD_Reg ,uint16_t LCD_RegValue)
{
	LCD_WRITE_CMD(LCD_Reg);                    //Chon thanh ghi
  LCD_WRITE_DATA(LCD_RegValue);	            //Ghi du lieu len thanh ghi
			    
}

/*********************************************************
** Function name:       void LCD_WRITE_RAM_PREPARE(void)
** Descriptions:        Ghi len GRAM
** input parameters:    No
** output parameters:   No
** Returned value:      No
*********************************************************/
void LCD_WRITE_RAM_PREPARE(void)
{
	LCD_WRITE_CMD(0x22);
}


/*********************************************************
** Function name:       void LCD_WriteRAM(u16 RGB_Code)
** Descriptions:        Ghi len GRAM LCD
** input parameters:    Gia tri cua mau sac
** output parameters:   No
** Returned value:      No
*********************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WRITE_DATA(RGB_Code);           
}

/**********************************************************************************
** Function name:       void LCD_CONFIGURATION(void)
** Descriptions:        Cau hinh cac chan LCD
** input parameters:  	No
** output parameters:   No
** Returned value:      No
************************************************************************************/
void LCD_CONFIGURATION(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
							  
	/*Cau hinh cac chan ket noi LCD o trang thai keo-day*/
	/*PORT E (all)????DB00~DB16*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* LCD_CS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* LCD_RS  LCD_WR  LCD_RD*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**********************************************************************************
** Function name:       void LCD_INIT(void)
** Descriptions:        Khoi tao LCD
** input parameters:  	ÎÞ
** output parameters:   ÎÞ
** Returned value:      ÎÞ
************************************************************************************/
void LCD_INIT(void)
{
	LCD_CONFIGURATION();							 //Cau hinh cac chan

	LCD_Delay(5);                     // delay 50 ms 
	LCD_Delay(5);                     // delay 50 ms 
	DeviceCode = LCD_READ_REG(0x0000);       // Doc dia chi cua chip dieu khien
	LCD_Delay(5); 
	DeviceCode=0x8999;
	// delay 50 ms
	if(DeviceCode==0x8999)	           // Neu chip dieu khien la SSD1289
	{
		/*
		///-----   Start Initial Sequence ------
		LCD_WRITE_REG(0x00, 0x0001); //Start internal OSC 
		LCD_WRITE_REG(0x01, 0x3B3F); //Driver output control 
		LCD_WRITE_REG(0x02, 0x0600); // set 1 line inversion	
		///-------- Power control setup --------
		LCD_WRITE_REG(0x0C, 0x0007); // Adjust VCIX2 output voltage 
		LCD_WRITE_REG(0x0D, 0x0006); // Set amplitude magnification of VLCD63 
		LCD_WRITE_REG(0x0E, 0x3200); // Set alternating amplitude of VCOM 
		LCD_WRITE_REG(0x1E, 0x00BB); // Set VcomH voltage 
		LCD_WRITE_REG(0x03, 0x6A64); // Step-up factor/cycle setting  
		///-------- RAM position control --------
		LCD_WRITE_REG(0x0F, 0x0000); // Gate scan position start at G0 
		LCD_WRITE_REG(0x44, 0xEF00); // Horizontal RAM address position 
		LCD_WRITE_REG(0x45, 0x0000); // Vertical RAM address start position
		LCD_WRITE_REG(0x46, 0x013F); // Vertical RAM address end position 
		/// ------ Adjust the Gamma Curve -------
		LCD_WRITE_REG(0x30, 0x0000);
		LCD_WRITE_REG(0x31, 0x0706);
		LCD_WRITE_REG(0x32, 0x0206);
		LCD_WRITE_REG(0x33, 0x0300);
		LCD_WRITE_REG(0x34, 0x0002);
		LCD_WRITE_REG(0x35, 0x0000);
		LCD_WRITE_REG(0x36, 0x0707);
		LCD_WRITE_REG(0x37, 0x0200);
		LCD_WRITE_REG(0x3A, 0x0908);
		LCD_WRITE_REG(0x3B, 0x0F0D);
		///--------- Special command -----------
		LCD_WRITE_REG(0x28, 0x0006); /// Enable test command	
		LCD_WRITE_REG(0x2F, 0x12EB); /// RAM speed tuning	 
		LCD_WRITE_REG(0x26, 0x7000); /// Internal Bandgap strength 
		LCD_WRITE_REG(0x20, 0xB0E3); /// Internal Vcom strength 
		LCD_WRITE_REG(0x27, 0x0044); // Internal Vcomh/VcomL timing 
		LCD_WRITE_REG(0x2E, 0x7E45); // VCOM charge sharing time 
		//--------- Turn On display ------------
		LCD_WRITE_REG(0x10, 0x0000); // Sleep mode off 
		LCD_Delay(3);              // Wait 30mS 
		LCD_WRITE_REG(0x11, 0x6870); // Entry mode setup. 262K type B, take care on the data bus with 16it only
		LCD_WRITE_REG(0x07, 0x0033); // Display ON	
		*/
		//=============
		
		/*
			// power supply setting
    // set R07h at 0021h (GON=1,DTE=0,D[1:0]=01)
    LCD_WRITE_REG(0x0007,0x0021);
    // set R00h at 0001h (OSCEN=1)
    LCD_WRITE_REG(0x0000,0x0001);
    // set R07h at 0023h (GON=1,DTE=0,D[1:0]=11)
    LCD_WRITE_REG(0x0007,0x0023);
    // set R10h at 0000h (Exit sleep mode)
    LCD_WRITE_REG(0x0010,0x0000);
    // Wait 30ms
    LCD_Delay(3);
    // set R07h at 0033h (GON=1,DTE=1,D[1:0]=11)
    LCD_WRITE_REG(0x0007,0x0033);
    // Entry mode setting (R11h)
    // R11H Entry mode
    // vsmode DFM1 DFM0 TRANS OEDef WMode DMode1 DMode0 TY1 TY0 ID1 ID0 AM LG2 LG2 LG0
    //   0     1    1     0     0     0     0      0     0   1   1   1  *   0   0   0
    // 14..13.DFM[1,0] = (11 => 65k Color; 10 => 256k Color)
    // 05..04.ID[1,0] = (00 = hdec,vdec; 01 = hinc,vdec; 10 = hdec,vinc; 11 = hinc,vinc
    // 03.AM = 0 for horizontal; AM = 1 for vertical
    LCD_WRITE_REG(0x0011,(1<<14)|(1<<13)|(1<<6)|(1<<5)|(1<<4)|(1<<3));
    // LCD driver AC setting (R02h)
    LCD_WRITE_REG(0x0002,0x0600);
    // power control 1
    // DCT3 DCT2 DCT1 DCT0 BT2 BT1 BT0 0 DC3 DC2 DC1 DC0 AP2 AP1 AP0 0
    // 1     0    1    0    1   0   0  0  1   0   1   0   0   1   0  0
    // DCT[3:0] fosc/4 BT[2:0]  DC{3:0] fosc/4
    LCD_WRITE_REG(0x0003,0x0804);//0xA8A4
    LCD_WRITE_REG(0x000C,0x0000);//
    LCD_WRITE_REG(0x000D,0x0808);// 0x080C --> 0x0808
    // power control 4
    // 0 0 VCOMG VDV4 VDV3 VDV2 VDV1 VDV0 0 0 0 0 0 0 0 0
    // 0 0   1    0    1    0    1    1   0 0 0 0 0 0 0 0
    LCD_WRITE_REG(0x000E,0x2900);
    LCD_WRITE_REG(0x001E,0x00B8);
    // Driver output control
    // 0 RL REV CAD BGR SM TB MUX8 MUX7 MUX6 MUX5 MUX4 MUX3 MUX2 MUX1 MUX0
    // 0 0  1    0   1  0  1  1     0    0    1    1    1    1    1    1
    // 14.RL  = Output shift direction of Source driver (1 = S0->S719; 0 = S719->S0)
    // 13.REV = Grayscale mode (1 = normal; 0 = inverted)
    // 11.BGR = Components order(1 = BGR; 0 = RGB)
    // 09.TB  = Output shift direction of Gate driver (1 = G0->G319; 0 = G319->G0)
    // 08..00.MUX = Number of lines (0..319)
    LCD_WRITE_REG(0x0001,(0<<14)|(1<<13)|(1<<11)|(0<<9)|(1<<8)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
    LCD_WRITE_REG(0x0010,0x0000);
    LCD_WRITE_REG(0x0005,0x0000);
    LCD_WRITE_REG(0x0006,0x0000);
    LCD_WRITE_REG(0x0016,0xEF1C);
    LCD_WRITE_REG(0x0017,0x0003);
    LCD_WRITE_REG(0x0007,0x0233);
    LCD_WRITE_REG(0x000B,0x0000|(3<<6));
    LCD_WRITE_REG(0x000F,0x0000);
    LCD_WRITE_REG(0x0041,0x0000);
    LCD_WRITE_REG(0x0042,0x0000);
    LCD_WRITE_REG(0x0048,0x0000);
    LCD_WRITE_REG(0x0049,0x013F);
    LCD_WRITE_REG(0x004A,0x0000);
    LCD_WRITE_REG(0x004B,0x0000);
    LCD_WRITE_REG(0x0044,0xEF00);
    LCD_WRITE_REG(0x0045,0x0000);
    LCD_WRITE_REG(0x0046,0x013F);
    // Gamma control
    LCD_WRITE_REG(0x0030,0x0707);
    LCD_WRITE_REG(0x0031,0x0204);
    LCD_WRITE_REG(0x0032,0x0204);
    LCD_WRITE_REG(0x0033,0x0502);
    LCD_WRITE_REG(0x0034,0x0507);
    LCD_WRITE_REG(0x0035,0x0204);
    LCD_WRITE_REG(0x0036,0x0204);
    LCD_WRITE_REG(0x0037,0x0502);
    LCD_WRITE_REG(0x003A,0x0302);
    LCD_WRITE_REG(0x003B,0x0302);
    // Gamma control end
    LCD_WRITE_REG(0x0023,0x0000);
    LCD_WRITE_REG(0x0024,0x0000);
    LCD_WRITE_REG(0x0025,0x8000);   
    LCD_WRITE_REG(0x004f,0);
    LCD_WRITE_REG(0x004e,0);
		*/
		LCD_WRITE_REG(0x0000,0x0001);      LCD_Delay(5);  //´ò¿ª¾§Õñ
    	LCD_WRITE_REG(0x0003,0xA8A4);    LCD_Delay(5);   //0xA8A4
    	LCD_WRITE_REG(0x000C,0x0000);    LCD_Delay(5);   
    	LCD_WRITE_REG(0x000D,0x080C);    LCD_Delay(5);   
    	LCD_WRITE_REG(0x000E,0x2B00);    LCD_Delay(5);   
    	LCD_WRITE_REG(0x001E,0x00B0);    LCD_Delay(5);   
    	LCD_WRITE_REG(0x0001,0x2B3F);    LCD_Delay(5);   //Çý¶¯Êä³ö¿ØÖÆ320*240  0x6B3F
    	LCD_WRITE_REG(0x0002,0x0600);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0010,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0011,0x6070);    LCD_Delay(5);        //0x4030           //¶¨ÒåÊý¾Ý¸ñÊ½  16Î»É« 		ºáÆÁ 0x6058
    	LCD_WRITE_REG(0x0005,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0006,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0016,0xEF1C);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0017,0x0003);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0007,0x0233);    LCD_Delay(5);        //0x0233       
    	LCD_WRITE_REG(0x000B,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x000F,0x0000);    LCD_Delay(5);        //É¨Ãè¿ªÊ¼µØÖ·
    	LCD_WRITE_REG(0x0041,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0042,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0048,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0049,0x013F);    LCD_Delay(5);
    	LCD_WRITE_REG(0x004A,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x004B,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0044,0xEF00);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0045,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0046,0x013F);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0030,0x0707);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0031,0x0204);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0032,0x0204);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0033,0x0502);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0034,0x0507);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0035,0x0204);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0036,0x0204);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0037,0x0502);    LCD_Delay(5);
    	LCD_WRITE_REG(0x003A,0x0302);    LCD_Delay(5);
    	LCD_WRITE_REG(0x003B,0x0302);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0023,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0024,0x0000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x0025,0x8000);    LCD_Delay(5);
    	LCD_WRITE_REG(0x004f,0);        //ÐÐÊ×Ö·0
    	LCD_WRITE_REG(0x004e,0);        //ÁÐÊ×Ö·0
		
	}		

  	if(DeviceCode==0x9325||DeviceCode==0x9328)//Neu chip dieu khien la ILI9325
	{
 		LCD_WRITE_REG(0x00e5,0x78F0);      
//        LCD_WRITE_REG(0x0000,0x0001);
        LCD_WRITE_REG(0x0001,0x0100);     
        LCD_WRITE_REG(0x0002,0x0700);                   
        LCD_WRITE_REG(0x0003,0x1030);    
        LCD_WRITE_REG(0x0004,0x0000);                                   
        LCD_WRITE_REG(0x0008,0x0202);	           
        LCD_WRITE_REG(0x0009,0x0000);         
        LCD_WRITE_REG(0x000a,0x0000);         
        LCD_WRITE_REG(0x000c,0x0001);         
        LCD_WRITE_REG(0x000d,0x0000);          
        LCD_WRITE_REG(0x000f,0x0000);
//Power On sequence //
        LCD_WRITE_REG(0x0010,0x0000);   
        LCD_WRITE_REG(0x0011,0x0007);
        LCD_WRITE_REG(0x0012,0x0000);                                                                 
        LCD_WRITE_REG(0x0013,0x0000);
        LCD_WRITE_REG(0x0007,0x0001);          
     
		    LCD_Delay(5);
        LCD_WRITE_REG(0x0010,0x1690);   
        LCD_WRITE_REG(0x0011,0x0227);
        LCD_Delay(5);
        LCD_WRITE_REG(0x0012,0x009d);                   
        LCD_Delay(5);
        LCD_WRITE_REG(0x0013,0x1900);   
        LCD_WRITE_REG(0x0029,0x0025);
        LCD_WRITE_REG(0x002b,0x000d);
        LCD_Delay(5); 
        LCD_WRITE_REG(0x0020,0x0000);                                                            
        LCD_WRITE_REG(0x0021,0x0000);           
		    LCD_Delay(5);
		//Ù¤ÂíÐ£Õý
        LCD_WRITE_REG(0x0030,0x0007); 
        LCD_WRITE_REG(0x0031,0x0303);   
        LCD_WRITE_REG(0x0032,0x0003);
        LCD_WRITE_REG(0x0035,0x0206);
        LCD_WRITE_REG(0x0036,0x0008); 
        LCD_WRITE_REG(0x0037,0x0406);
        LCD_WRITE_REG(0x0038,0x0304);        
        LCD_WRITE_REG(0x0039,0x0007);     
        LCD_WRITE_REG(0x003c,0x0602);
        LCD_WRITE_REG(0x003d,0x0008);
        LCD_Delay(5);
        LCD_WRITE_REG(0x0050,0x0000);
        LCD_WRITE_REG(0x0051,0x00ef);                   
        LCD_WRITE_REG(0x0052,0x0000);                   
        LCD_WRITE_REG(0x0053,0x013f); 
        
        LCD_WRITE_REG(0x0060,0xa700);        
        LCD_WRITE_REG(0x0061,0x0001); 
        LCD_WRITE_REG(0x006a,0x0000);
        LCD_WRITE_REG(0x0080,0x0000);
        LCD_WRITE_REG(0x0081,0x0000);
        LCD_WRITE_REG(0x0082,0x0000);
        LCD_WRITE_REG(0x0083,0x0000);
        LCD_WRITE_REG(0x0084,0x0000);
        LCD_WRITE_REG(0x0085,0x0000);
      
        LCD_WRITE_REG(0x0090,0x0010);     
        LCD_WRITE_REG(0x0092,0x0600);  
		   
        LCD_WRITE_REG(0x0007,0x0133);
	}	
	LCD_Delay(5);	                       //Delay 50ms
	LCD_CLEAR(BLACK);
}
/************************************************************************
** Function name:       void LCD_SET_CURSOR(u16 Xpos, u16 Ypos)
** Descriptions:        Thiet lap vi tri con tro
** input parameters:    Toc do truc X uint8_t;  Toa do truc Y uint16_t
** output parameters:   No
** Returned value:      No
**************************************************************************/
void LCD_SET_CURSOR(u16 Xpos, u16 Ypos)
{
	LCD_WRITE_REG(0x004E, Xpos);
	LCD_WRITE_REG(0X004F, Ypos);
	
	if(DeviceCode == 0X8999)
 	{
 		LCD_WRITE_REG(0X4E, Xpos);
 		LCD_WRITE_REG(0X4F, Ypos);
  }
	else 
 	{
		LCD_WRITE_REG(0X20, Xpos);
		LCD_WRITE_REG(0X21, Ypos);
	}
}


/***********************************************************
** Function name:       void LCD_CLEAR(uint16_t Color)
** Descriptions:        Xoa LCD
** input parameters:    Gia tri mau
** output parameters:   No
** Returned value:      No
************************************************************/
void LCD_CLEAR(uint16_t Color)
{
	uint32_t index=0;      
	LCD_SET_CURSOR(0x00,0x0000);          //Thiet lap vi tri con tro
	LCD_WRITE_RAM_PREPARE();              //Bat dau viet GRAM
	for(index=0;index<76800;index++)
	{
		LCD_WRITE_DATA(Color);	
	}
}

/****************************************************************************
* Ãû    ³Æ£ºu16 LCD_GetPoint(u16 x,u16 y)
* ¹¦    ÄÜ£º»ñÈ¡Ö¸¶¨×ù±êµÄÑÕÉ«Öµ
* Èë¿Ú²ÎÊý£ºx      ÐÐ×ù±ê
*           y      ÁÐ×ù±ê
* ³ö¿Ú²ÎÊý£ºµ±Ç°×ù±êÑÕÉ«Öµ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºCurrentColor = LCD_GetPoint(10,10);
* ±¸×¢£º´Ëº¯ÊýÓÐ´ýÑéÖ¤£¬´Ë´¦ÆÁ±Î
****************************************************************************/
//u16 LCD_GetPoint(u16 x,u16 y)
//{
//  LCD_SET_CURSOR(x,y);
//  if(DeviceCode=0X8999)
//    return (LCD_ReadRAM());
//  else
//    return (LCD_BGRtoRGB(LCD_ReadRAM()));
//}


/***********************************************************************
** Function name:       void LCD_DRAW_POINT(uint16_t xsta, uint16_t ysta)
** Descriptions:        ´òµã
** input parameters:    Toa do bat dau truc X 0~239; Toa do bat dau truc Y 0~319
** output parameters:   No
** Returned value:      No
***********************************************************************/
void LCD_DRAW_POINT(uint16_t xsta, uint16_t ysta)
{
	LCD_SET_CURSOR(xsta,ysta);           //Thiet lap vi tri con tro 
	LCD_WRITE_CMD(0x22);                   //Viet len GRAM
	LCD_WRITE_DATA(POINT_COLOR); 
}

void LCD_DRAW_POINT_COLOR(uint16_t xsta, uint16_t ysta, uint16_t color)
{
	LCD_SET_CURSOR(xsta,ysta);           //Thiet lap vi tri con tro 
	LCD_WRITE_CMD(0x22);                   //Viet len GRAM
	LCD_WRITE_DATA(color); 
}

/*******************************************************************************************************
** Function name:       void LCD_SET_DISPLAY_WINDOW (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
** Descriptions:        Cai dat duoc toi da 240*320
** input parameters:    Toa do bat dau truc X 0~239; Toa do bat dau truc Y 0~319
** output parameters:   No
** Returned value:      No
******************************************************************************************************/
void LCD_SET_DISPLAY_WINDOW(unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
{
	if(DeviceCode==0x8999)
		{
	    LCD_WRITE_REG(0x44,x|((x_end-1)<<8));
	    LCD_WRITE_REG(0x45,y);
	    LCD_WRITE_REG(0x46,y_end-1);
		}
		else
		{
		  LCD_WRITE_REG(0x50, x);	   	       //Dia chi bat dau GRAM truc X
  	  LCD_WRITE_REG(0x51, x+x_end); 	     //Dia chi ket thuc GRAM truc X 
  	  LCD_WRITE_REG(0x52, y);		  	     //Dia chi bat dau GRAM truc Y
  	  LCD_WRITE_REG(0x53, y+y_end);       //Dia chi ket thuc GRAM truc y 
  	  LCD_SET_CURSOR(x, y);			         //Thiet lap con tro
		}
}


/*************************************************************************************************************
** Function name:       void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
** Descriptions:        Hien thi mau len mot khoang nao do
** input parameters:    Toa do bat dau truc X
		                    Toa do bat dau truc Y
		                    Toa do ket thuc truc X
		                    Toa do ket thuc truc Y
		                    Gia tri mau sac
** output parameters:   No
** Returned value:      No
**************************************************************************************************************/
void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
{                    
  u32 n;	
	LCD_SET_DISPLAY_WINDOW (xsta, ysta, xend, yend);   //Kich thuoc cua khoang can Fill	
	LCD_SET_CURSOR(xsta,ysta);                 //Thiet lap con tro
	LCD_WRITE_RAM_PREPARE();                   //Viet len GRAM*6 	   	   
	n=(u32)(yend-ysta+1)*(xend-xsta+1);    
	while(n--)																 ////Cho den khi full fill
	{
		LCD_WRITE_DATA(colour);
	}          
	 
	LCD_SET_DISPLAY_WINDOW (0, 0, 240, 320);      	    //Hien thi lai man hinh
}  


/****************************************************************************************************
** Function name:       void LCD_DRAW_LINE(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
** Descriptions:        Ve duong thang
** input parameters:    Toa do bat dau truc X
		                    Toa do bat dau truc Y
		                    Toa do ket thuc truc X
		                    Toa do ket thuc truc Y
** output parameters:   No
** Returned value:      No
****************************************************************************************************/
void LCD_DRAW_LINE(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
{
    u16 x, y, t;
	if((xsta==xend)&&(ysta==yend))LCD_DRAW_POINT(xsta, ysta);
	else if(abs(yend-ysta)>abs(xend-xsta))                    //Do doc lon hon 1
	{
		if(ysta>yend) 
		{
			t=ysta;
			ysta=yend;
			yend=t; 
			t=xsta;
			xsta=xend;
			xend=t; 
		}
		for(y=ysta;y<yend;y++)                                   //Tham chieu len truc Y
		{
			x=(u32)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
			LCD_DRAW_POINT(x, y);  
		}
	}
	else                                                       //Do doc nho hon hoac bang 1
	{
		if(xsta>xend)
		{
			t=ysta;
			ysta=yend;
			yend=t;
			t=xsta;
			xsta=xend;
			xend=t;
		}   
		for(x=xsta;x<=xend;x++)                                   //Tham chieu len truc X
		{
			y =(u32)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
			LCD_DRAW_POINT(x,y); 
		}
	} 
} 

void LCD_DRAW_LINE_COLOR(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,uint16_t COLOR)
{
    u16 x, y, t;
	if((xsta==xend)&&(ysta==yend))LCD_DRAW_POINT(xsta, ysta);
	else if(abs(yend-ysta)>abs(xend-xsta))                    //Do doc lon hon 1
	{
		if(ysta>yend) 
		{
			t=ysta;
			ysta=yend;
			yend=t; 
			t=xsta;
			xsta=xend;
			xend=t; 
		}
		for(y=ysta;y<yend;y++)                                   //Tham chieu len truc Y
		{
			x=(u32)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
			LCD_DRAW_POINT_COLOR(x, y,COLOR);  
		}
	}
	else                                                       //Do doc nho hon hoac bang 1
	{
		if(xsta>xend)
		{
			t=ysta;
			ysta=yend;
			yend=t;
			t=xsta;
			xsta=xend;
			xend=t;
		}   
		for(x=xsta;x<=xend;x++)                                   //Tham chieu len truc X
		{
			y =(u32)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
			LCD_DRAW_POINT_COLOR(x,y,COLOR); 
		}
	} 
} 


/*****************************************************************************
** Function name:       void LCD_DRAW_CIRCLE(uint16_t x0, uint16_t y0, uint8_t r)
** Descriptions:        Ve vong tron
** input parameters:    Tam truc X
	                      Tam truc Y
		                    Ban kinh
** output parameters:   No
** Returned value:      No
******************************************************************************/
void LCD_DRAW_CIRCLE(uint16_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                                       
	while(a<=b)
	{
		LCD_DRAW_POINT(x0-b,y0-a);                         //3           
		LCD_DRAW_POINT(x0+b,y0-a);                         //0           
		LCD_DRAW_POINT(x0-a,y0+b);                         //1       
		LCD_DRAW_POINT(x0-b,y0-a);                         //7           
		LCD_DRAW_POINT(x0-a,y0-b);                         //2             
		LCD_DRAW_POINT(x0+b,y0+a);                         //4               
		LCD_DRAW_POINT(x0+a,y0-b);                         //5
		LCD_DRAW_POINT(x0+a,y0+b);                         //6 
		LCD_DRAW_POINT(x0-b,y0+a);             
		a++;

		/*Thuat toan Bresenham*/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DRAW_POINT(x0+a,y0+b);
	}
} 


/*****************************************************************************
** Function name:       void LCD_DRAW_RECTANGLE(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
** Descriptions:        Ve hinh chu nhat
** input parameters:    xsta XÆðÊ¼×ø±ê
	                      ysta YÆðÊ¼×ø±ê
		                    xend X½áÊø×ø±ê
		                    yend Y½áÊø×ø±ê
** output parameters:   ÎÞ
** Returned value:      ÎÞ
******************************************************************************/
void LCD_DRAW_RECTANGLE(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
{
	LCD_DRAW_LINE(xsta,ysta,xend,ysta);
	LCD_DRAW_LINE(xsta,ysta,xsta,yend);
	LCD_DRAW_LINE(xsta,yend,xend,yend);
	LCD_DRAW_LINE(xend,ysta,xend,yend);
}

/*****************************************************************************
** Function name:       void LCD_DRAW_TRIANGLE(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
** Descriptions:        Ve hinh tam giac
** input parameters:    xsta XÆðÊ¼×ø±ê
	                      ysta YÆðÊ¼×ø±ê
		                    xend X½áÊø×ø±ê
		                    yend Y½áÊø×ø±ê
** output parameters:   ÎÞ
** Returned value:      ÎÞ
******************************************************************************/
void LCD_DRAW_TRIANGLE(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
	LCD_DRAW_LINE(x1,y1,x2,y2);
	LCD_DRAW_LINE(x1,y1,x3,y3);
	LCD_DRAW_LINE(x2,y2,x3,y3);
} 

/*****************************************************************************
** Function name:       void LCD_DRAW_ELLIPSE(uint16_t xcenter,uint16_t ycenter,uint16_t xradius,uint16_t yradius)
** Descriptions:        Ve hinh elipse
** input parameters:    
** output parameters:   ÎÞ
** Returned value:      ÎÞ
******************************************************************************/
void LCD_DRAW_ELLIPSE(uint16_t xcenter,uint16_t ycenter,uint16_t xradius,uint16_t yradius)
{
  int x,y; 
	float c,p;
  x=0; y=yradius;
  c=(float)yradius/xradius;
  c=c*c; 
	p=2*c-2*yradius+1;
  while (c*x<=y)
  {
    LCD_DRAW_POINT(xcenter+x,ycenter+y);
    LCD_DRAW_POINT(xcenter-x,ycenter+y);
    LCD_DRAW_POINT(xcenter+x,ycenter-y);
    LCD_DRAW_POINT(xcenter-x,ycenter-y);
    if (p<0) 
			p += 2*c*(2*x+3);
    else
    {
      p +=4*(1-y)+2*c*(2*x+3);
      y--;
    }
    x++;
  }
   
  y=0;x=xradius;
  c= (float)xradius/yradius;
  c=c*c; p=2*c-2*xradius+1;
  while (c*y<=x)
  {
    LCD_DRAW_POINT(xcenter+x,ycenter+y);
    LCD_DRAW_POINT(xcenter-x,ycenter+y);
		LCD_DRAW_POINT(xcenter+x,ycenter-y);
    LCD_DRAW_POINT(xcenter-x,ycenter-y);
    if (p<0) 
			p +=2*c*(2*y+3);
		else
    {
      p +=4*(1-x)+2*c*(2*y+3);
      x--;
    }
    y++;
  }
}

/***************************************************************************************************
***************************************************************************************************/
void LCD_DRAW_FLOWER(void)
{
   //int midx, midy;
   //initwindow(800,600);
   //srand(time(0));
    // r=100, x1=200,y1=100 , set=0 ;
//	volatile uint16_t i;
	uint16_t i;
	i=0;
   for(i=0;i<1000;i++)
   {
		 float x=0,set=0,clr;
		 float b=+90*cos(i*10);
     //float b=+110*cos(i*50);
		 //float b=+120*cos(i*200);
     int a1=120+(b)*sin(i);
		int b1=160+(b)*cos(i);
		//int clr;
		set=set+1;if(set==4000){set=0;}  
			//clr=rand()%16;
		clr=rand()%200*(i*10);
		/*if(i<1000)
			clr=RED;
		if(1000<=i<2000)
			clr=GREEN;
		if(i>2000)
			clr=YELLOW;*/
     //LCD_DRAW_POINT(a1,b1-100);
     //LCD_DRAW_POINT(a1-200,b1-100);
			//LCD_DRAW_POINT(a1+200,b1-100);
     //LCD_DRAW_POINT(a1,b1+150);
     //LCD_DRAW_POINT(a1-200,b1+150);
     //LCD_DRAW_POINT(a1+200,b1+150);
		
			LCD_SET_CURSOR(a1,b1);           //Thiet lap vi tri con tro 
			LCD_WRITE_CMD(0x22);                   //Viet len GRAM
			LCD_WRITE_DATA(clr); 
			x=x+0.5;
     //LCD_Delay(100);
    }
	 //i=0;
 }
/***************************************************************************************************
** Function name:       void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
** Descriptions:        Hien thi ki tu
** input parameters:    Toa do bat dau x:0~234 y:0~308
		                    num       Ma ASCII cua ki tu
		                    size      Kich thuoc chu, mac dinh la 8*16
		                    PenColor  Mau chu
		                    BackColor Mau nen
** output parameters:   No
** Returned value:      No
****************************************************************************************************/
void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
{       
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 304 

    u8 temp;
    u8 pos,t;
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;		 
	if(DeviceCode == 0X8999)                   
	{
    if(XY)                                       						//Hien thi theo chieu doc
 	   {
 		  LCD_SET_DISPLAY_WINDOW(x,y,x+size/2,y+size);	       	//Thiet lap cua so
 		  LCD_WRITE_REG(0x11, 0x6870);
 		  LCD_SET_CURSOR(x, y); 
 	  }
   else                                          						//Neu hien thi theo chieu ngang
 	  {
 		  LCD_SET_DISPLAY_WINDOW(x,y-size/2,x+size,y);	       	//Thiet lap cua so
 		  LCD_WRITE_REG(0x11, 0x6858);
 		  LCD_SET_CURSOR(x, y-1);                     					//Thiet lap vi tri con tro
 	  }  
	}
  else if(DeviceCode == 0X9325 | DeviceCode == 0X9328) 
	{ 
		LCD_SET_DISPLAY_WINDOW(x,y,(size/2-1),size-1);        //ÉèÖÃ´°¿Ú	Çý¶¯ICÎªILI9325/9328
	  LCD_SET_CURSOR(x, y);                         //ÉèÖÃ¹â±êÎ»ÖÃ 
	}
	LCD_WRITE_RAM_PREPARE();                        //¿ªÊ¼Ð´ÈëGRAM   
	num=num-' ';                                   //µÃµ½Æ«ÒÆºóµÄÖµ 
	for(pos=0;pos<size;pos++)
	{
		if(size==12)
			temp=asc2_1206[num][pos];                  //µ÷ÓÃ1206×ÖÌå
		else 
			temp=asc2_1608[num][pos];		               //µ÷ÓÃ1608×ÖÌå
		for(t=0;t<size/2;t++)
	    {                 
	      if(temp&0x01)			                       //´ÓµÍÎ»¿ªÊ¼
			   {
				   LCD_WRITE_DATA(PenColor);                //»­×ÖÌåÑÕÉ« Ò»¸öµã
			   }
			 else 
				  LCD_WRITE_DATA(BackColor);	               //»­±³¾°ÑÕÉ« Ò»¸öµã      
	        temp>>=1; 
	    }
	}			
	LCD_SET_DISPLAY_WINDOW(0x0000,0x0000,240,320);	         //Khoi phuc lai kich thuoc cua so
	
} 
/**********************************************************************************
** Function name:       unsigned int NUM_POWER(unsigned int m,unsigned int n)
** Descriptions:        mµÄn´Î·½ ÓÃÓÚÈ¡³öÊýÖµÃ¿Ò»Î»
** input parameters:  	Ma hex cua m unsigned int m
**											unsigned int n						
** output parameters:   m^n
** Returned value:      No
************************************************************************************/
u32 NUM_POWER(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
 
/*********************************************************************************************
** Function name:       void LCD_SHOW_NUM(u8 x,u8 y,u32 num,u8 len, u16 PenColor, u16 BackColor)
** Descriptions:        Hien thi so(0~4294967295), co xoa so 0 vo nghia
** input parameters:  	Toa do bat dau     u8 x
  											Toa do bat dau     u8 y
  											So:       num
    										Chieu dai:  len		
                        Mau so:   PenColor
                        Mau nen:  BackColor
** output parameters:   No
** Returned value:      No
************************************************************************************************/
void LCD_SHOW_NUM(u8 x,u8 y,u32 num,u8 len, u16 PenColor, u16 BackColor)
{    
	u8 size = 16;                                         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/NUM_POWER(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size, PenColor, BackColor);
				LCD_ShowChar(x+(size/2)*t,y,'0',size, PenColor, BackColor);
				continue;
			}
			else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size, PenColor, BackColor); 
	}
} 
  
/**********************************************************
ham hien thi so tai toa do, so con so va kich thuoc thuong la 15 
KHONG XOA SO 0 VO NGHIA
***********************************************************/ 
void LCD_SHOW_NUM_FULL(u8 x,u8 y,u32 num,u8 len, u16 PenColor, u16 BackColor)
{         	
	u8 size = 16;                                         	
	u8 t,temp;
	//u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/NUM_POWER(10,len-t-1))%10;	
		LCD_ShowChar(x+(size/2)*t,y,temp+'0',size, PenColor, BackColor); 
	}
}

/*********************************************************************************************
** Function name:       void LCD_ShowCharString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t PenColor, uint16_t BackColor)
** Descriptions:        Hien thi chuoi ki tu ( khong the hien thi rieng tung chu )
** input parameters:  	Toa do bat dau 					x,y
	                      Dia chi bat dau chuoi 	p
		                    PenColor  Mau chu
		                    BackColor Mau nen
** output parameters:   No
** Returned value:      No
************************************************************************************************/
void LCD_ShowCharString(uint16_t x, uint16_t y, const char *p, uint16_t PenColor, uint16_t BackColor)
{   
	uint8_t size = 16;                              
  if(x>MAX_CHAR_POSX){x=0;y+=size;}			              //Neu chuoi vuot qua truc X
	if(y>MAX_CHAR_POSY){y=x=0;LCD_CLEAR(WHITE);}	      //Neu Chuoi vuot qua truc Y thi man hinh se bi xoa trang
  LCD_ShowChar(x, y, *p, size, PenColor, BackColor);	//0±íÊ¾·Çµþ¼Ó·½Ê½
 
}

/**********************************************************************************
** Function name:       u16 findHzIndex(u8 *hz) 
** Descriptions:        Ë÷Òýºº×Ö´æ´¢µÄÄÚ´æµØÖ·
** input parameters:  	hz				
** output parameters:   ÎÞ
** Returned value:      ÎÞ
************************************************************************************/
u16 findHzIndex(char *hz)                               // ÔÚ×Ô¶¨Òåºº×Ö¿âÔÚ²éÕÒËùÒªÏÔÊ¾                                                   // µÄºº×ÖµÄÎ»ÖÃ 
{
	u16 i=0;
	FNT_GB16 *ptGb16 = (FNT_GB16 *)GBHZ_16;		          //ptGb16Ö¸ÏòGBHZ_16
	while(ptGb16[i].Index[0] > 0x80)
	{
	    if ((*hz == ptGb16[i].Index[0]) && (*(hz+1) == ptGb16[i].Index[1])) //ºº×ÖÓÃÁ½Î»À´±íÊ¾µØÖ·Âë
		{
	        return i;
	    }
	    i++;
	    if(i > (sizeof((FNT_GB16 *)GBHZ_16) / sizeof(FNT_GB16) - 1))  //ËÑË÷ÏÂ±êÔ¼Êø 
	    {
		    break;
	    }
	}
	return 0;
}

	
/************************************************************************************************
** Function name:       void WriteOneHz(u16 x0, u16 y0, u8 *pucMsk, u16 PenColor, u16 BackColor)
** Descriptions:        Hien thi 1 ki tu TQ
** input parameters:  	x0,y0      Toa do bat dau
		                    *pucMsk    Con tro dia chi cua chuoi
		                    PenColor	 Mau chu
		                    BackColor  Mau nen			
** output parameters:   No
** Returned value:      No
************************************************************************************************/
void WriteOneHz(u16 x0, u16 y0, u8 *pucMsk, u16 PenColor, u16 BackColor)
{
    u16 i,j;
    u16 mod[16];                                      //µ±Ç°×ÖÄ£ 16*16 
    u16 *pusMsk;                                      //µ±Ç°×Ö¿âµØÖ·  
    u16 y;

	u16 size = 16;                                      //ºº×ÖÄ¬ÈÏ´óÐ¡16*16

    pusMsk = (u16 *)pucMsk;


    for(i=0; i<16; i++)                                //±£´æµ±Ç°ºº×ÖµãÕóÊ½×ÖÄ£ 
    {
        mod[i] = *pusMsk;                              //È¡µÃµ±Ç°×ÖÄ££¬°ë×Ö¶ÔÆë·ÃÎÊ   
        mod[i] = ((mod[i] & 0xff00) >> 8) | ((mod[i] & 0x00ff) << 8);//×ÖÄ£½»»»¸ßµÍ×Ö½Ú
		pusMsk = pusMsk+1;
    }
    y = y0;
	LCD_SET_DISPLAY_WINDOW(x0,y0,x0+size,y0+size);	 	             //ÉèÖÃ´°¿Ú
	LCD_SET_CURSOR(x0,y0);                                //ÉèÖÃ¹â±êÎ»ÖÃ
	LCD_WRITE_RAM_PREPARE();                              //¿ªÊ¼Ð´ÈëGRAM  
    for(i=0; i<16; i++)                                //16ÐÐ 
    {                                              
      for(j=0; j<16; j++)                              //16ÁÐ  
       {
		    if((mod[i] << j) & 0x8000)                     //ÏÔÊ¾µÚiÐÐ ¹²16¸öµã 
          { 
			      LCD_WriteRAM(PenColor);
          } 
			  else 
			    {
            LCD_WriteRAM(BackColor);                   //ÓÃ¶Á·½Ê½Ìø¹ýÐ´¿Õ°×µãµÄÏñËØ
			    } 
       }
        y++;
    }
	LCD_SET_DISPLAY_WINDOW(0x0000,0x0000,240,320);  	             //»Ö¸´´°Ìå´óÐ¡
}

/**********************************************************************************
** Function name:       void LCD_ShowHzString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
** Descriptions:        ÏÔÊ¾ºº×Ö×Ö·û´®(²»ÄÜµ¥¶Àµ÷ÓÃ)
** input parameters:  	x0£¬y0    ÆðÊ¼×ø±ê
		                    pcStr     Ö¸Ïò×Ö·û´®µØÖ·
		                    PenColor  ×ÖÌåÑÕÉ«
		                    BackColor ×ÖÌå±³¾°				
** output parameters:   ÎÞ
** Returned value:      ÎÞ
************************************************************************************/
void LCD_ShowHzString(u16 x0, u16 y0, char *pcStr, u16 PenColor, u16 BackColor)
{
#define MAX_HZ_POSX 224
#define MAX_HZ_POSY 304 
	u16 usIndex;
	u8 size = 16; 
	FNT_GB16 *ptGb16 = 0;    
    ptGb16 = (FNT_GB16 *)GBHZ_16; 

	if(x0>MAX_HZ_POSX){x0=0;y0+=size;}			            //³¬³öXÖá×ÖÌå×îÐ¡µ¥Î»£¬»»ÐÐ
    if(y0>MAX_HZ_POSY){y0=x0=0;LCD_CLEAR(WHITE);}	    //³¬³öYÖá×ÖÌå×îÐ¡µ¥Î»£¬»Øµ½Ô­µã£¬²¢ÇÒÇåÆÁ

	usIndex = findHzIndex(pcStr);
	WriteOneHz(x0, y0, (u8 *)&(ptGb16[usIndex].Msk[0]),  PenColor, BackColor); //ÏÔÊ¾×Ö·û 
}

/**********************************************************************************
** Function name:       void LCD_SHOW_STRING(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
** Descriptions:        Goi tung ki tu va cho hien thi de dat duoc chuoi mong muon
** input parameters:  	x0, y0    Toa do bat dau
		                    *pcStr    Con tro dia chi cua 1 chuoi
		                    PenColor  Mau chu
		                    BackColor Mau nen				
** output parameters:   No
** Returned value:      No
************************************************************************************/
void LCD_SHOW_STRING(u16 x0, u16 y0, char *pcStr, u16 PenColor, u16 BackColor)
{
	while(*pcStr!='\0')			//Ki tu rong NULL
	{
	 	if(*pcStr>0x80)       //Hien thi cac ki tu TQ
		{
			LCD_ShowHzString(x0, y0, pcStr, PenColor, BackColor);
			pcStr += 2;
			x0 += 16;	
		}
		else                 //Hien thi ki tu
		{
			LCD_ShowCharString(x0, y0, pcStr, PenColor, BackColor);	
			pcStr +=1;
			x0+= 8;
		}
	}	
}

void LCD_CLEAR_STRING(u16 x0, u16 y0, char *pcStr, u16 PenColor, u16 BackColor)
{
	while(*pcStr!='\0')			//Ki tu rong NULL
	{
	 	if(*pcStr>0x80)       //Hien thi cac ki tu TQ
		{
			LCD_ShowHzString(x0, y0, pcStr, PenColor, BackColor);
			pcStr += 2;
			x0 += 16;	
		}
		else                 //Hien thi ki tu
		{
			LCD_ShowCharString(x0, y0, pcStr, PenColor, BackColor);	
			pcStr +=1;
			x0+= 8;
		}
	}	
}

/**********************************************************
HAM XOA CHUOI KY TU CO THEO TRUC X  
THONG SO VAO: TOA DO HIEN THI (X,Y), KICH CO CHU: 12/16, CHUOI
***********************************************************/
/**********************************************************************************
** Function name:       void LCD_DisplayString(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor)
** Descriptions:       	Khi muon hien thi chong cheo
** input parameters:  	x,y    Toa do bat dau
		                    *s     Tro vao 1 chuoi dia chi
		                    PenColor  ×ÖÌåÑÕÉ«
		                    BackColor ×ÖÌå±³¾°				
** output parameters:   ÎÞ
** Returned value:      ÎÞ
************************************************************************************/
void LCD_DisplayString(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor) 
{
	
  while (*s) 
	{  
  			
		if(XY)
		{
		LCD_ShowChar(x, 320-y, *s++, 16, PenColor, BackColor);
			x+=Font_Size/2;
			if(x>=232)
			{
				x=0;
				y+=Font_Size;
				if(y>=304)y=0;
			}
		}
		else
		{
		  LCD_ShowChar(x, 320-y, *s++, 16, PenColor, BackColor);
			y+=Font_Size/2;
			if(y>=312)
			{
				y=0;
				x+=Font_Size;
				if(x>=224)y=0;
			}
		}
  }
 
}

/**********************************************************************************
** Function name:       u16 LCD_RGBtoBGR(u16 Color)
** Descriptions:        Chuyen dinh dang RRRRRGGGGGGBBBBB sang BBBBBGGGGGGRRRRR
** input parameters:  	Color      Gia tri mau RGB		
** output parameters:   Gia tri mau BGR
** Returned value:      No
************************************************************************************/
u16 LCD_RGBtoBGR(u16 Color)
{						   
  u16  r, g, b, bgr;

  b = (Color>>0)  & 0x1f;	                    //Dich cac mau B
  g = (Color>>5)  & 0x3f;	                    //Dich cac mau G
  r = (Color>>11) & 0x1f;	                    //Dich cac mau R
  
  bgr =  (b<<11) + (g<<5) + (r<<0);

  return( bgr );
}


/**********************************************************************************
** Function name:       void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,u8 *pic)
** Descriptions:        Dinh dang anh mau quet ngang, che do 16 bit
** input parameters:  	StartX     Toa do bat dau X
                        StartY     Toa do bat dau Y
                        EndX       To do ket thuc X
                        EndY       To do ket thc Y
                        pic        Con tro dau cua buc hinh
** output parameters:   No
** Returned value:      No
************************************************************************************/
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u8 *pic)
{
	static	u16 i=0,j=0;
	u16 *bitmap = (u16 *)pic;
	
	LCD_SET_DISPLAY_WINDOW(StartX, StartY, EndX, EndY);	//Set kich thuoc anh can hien thi
	LCD_SET_CURSOR(StartX,StartY);
	LCD_WRITE_RAM_PREPARE();
	for(j=0; j<EndY-StartY; j++)
	{
		for(i=0; i<EndX-StartX; i++) 
			LCD_WriteRAM(*bitmap++); 	
	}
	LCD_SET_DISPLAY_WINDOW(0, 0, 240, 320);              //Refresh lai man hinh
}

void LCD_WRITE_BITMAP_X(u8 Xpos, u16 Ypos, u8 Height, u16 Width, u8 *bitmap)
{
  vu32 index;
  vu32 size = Height * Width;
  u16 *bitmap_ptr = (u16 *)bitmap;
   LCD_WRITE_REG(0x11, 0x6878);
   
  LCD_SET_DISPLAY_WINDOW(Xpos, Ypos, Xpos+Height, Ypos+Width);
  LCD_SET_CURSOR(Xpos,Ypos);
   
  LCD_WRITE_REG(0x03, 0x1038);
	LCD_CS(0);
  LCD_WRITE_CMD(0x22);
  for(index = 0; index < size; index++)
  {
    LCD_WRITE_DATA(*bitmap_ptr++);
  }
	LCD_CS(1);
  LCD_WRITE_REG(0x11, 0x6870);
}

void LCD_WRITE_BITMAP_Y(u8 Xpos, u16 Ypos, u8 Height, u16 Width, u8 *bitmap)
{
  vu32 index;
  vu32 size = Height * Width;
  u16 *bitmap_ptr = (u16 *)bitmap;
   LCD_SET_DISPLAY_WINDOW(Xpos, Ypos, Xpos+Height, Ypos+Width);
  LCD_SET_CURSOR(Xpos,Ypos);
   
  LCD_WRITE_REG(0x03, 0x1038);
	LCD_CS(0);
  LCD_WRITE_CMD(0x22);
  for(index = 0; index < size; index++)
  {
    LCD_WRITE_DATA(*bitmap_ptr++);
  }
	LCD_CS(1);
	LCD_SET_DISPLAY_WINDOW(0, 0, 240, 320);

}

void LCD_DRAW_PICTURE (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bitmap) 
{
  unsigned int   i;
  unsigned int   len = w*h;
  unsigned short *bitmap_ptr = (unsigned short *)bitmap;
/*
  LCD_WRITE_REG(0x50, y);                      // Horizontal GRAM Start Address      
  LCD_WRITE_REG(0x51, y+h-1);                  // Horizontal GRAM End   Address (-1) 
  LCD_WRITE_REG(0x52, x);                      // Vertical   GRAM Start Address      
  LCD_WRITE_REG(0x53, x+w-1);                  // Vertical   GRAM End   Address (-1) 

  LCD_WRITE_REG(0x20, y);
  LCD_WRITE_REG(0x21, x);
	*/
	/*
  LCD_WRITE_REG(0x50, x);                      // Horizontal GRAM Start Address      
  LCD_WRITE_REG(0x51, x+w-1);                  // Horizontal GRAM End   Address (-1) 
  LCD_WRITE_REG(0x52, y);                      // Vertical   GRAM Start Address      
  LCD_WRITE_REG(0x53, y+h-1);                  // Vertical   GRAM End   Address (-1) 

  LCD_WRITE_REG(0x20, y);
  LCD_WRITE_REG(0x21, x);
	*/
	 		  LCD_SET_DISPLAY_WINDOW(x,y,x+w,y+h);	       	//Thiet lap cua so
 		  LCD_WRITE_REG(0x11, 0x6870);
 		  LCD_SET_CURSOR(x, y); 

  LCD_CS(0);
  LCD_WRITE_CMD(0x22);
  for (i = 0; i < len; i++) {
    LCD_WRITE_DATA(*bitmap_ptr++);
  }
  LCD_CS(1);
}

void LCD_SHOW_MA_7DOAN(uint8_t X,uint16_t Y,uint8_t digit,uint8_t size,uint8_t mode)
{
      uint16_t temp;
    uint8_t pos,t,i,sb_cot;  
      if(digit<10)
      {
         if(X>MAX_CHAR_POSX||Y>MAX_CHAR_POSY) return;       
         LCD_SET_DISPLAY_WINDOW(X,Y,((size-1)),(size-1));  
         LCD_SET_CURSOR(X,Y); 
         LCD_WRITE_RAM_PREPARE();              
         sb_cot=size/8;
         for(pos=0;pos<size;pos++)   // CHIEU DAI
         {            
            for (i=0;i<sb_cot;i++)   // CHIEU RONG
            {            
               temp=MA_7DOAN[digit][pos*sb_cot+i];      
               for(t=0;t<8;t++)         // 1 BYTE
               {                     
              if((temp<<t)&0x80)         LCD_WRITE_DATA(POINT_COLOR);
                  else                               LCD_WRITE_DATA(BACK_COLOR);                                  
               }
            }                        
         }   
//   LCD_WRITE_REG(0x43, 0x0000); 
      LCD_WRITE_REG(0x44, 0xEF00); 
      LCD_WRITE_REG(0x45, 0x0000); 
      LCD_WRITE_REG(0x46, 0x013F);    
      }
}

void LCD_DRAW_CIRCLE_COLOR(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                                       
	while(a<=b)
	{
		LCD_DRAW_POINT_COLOR(x0-b,y0-a,color);                         //3           
		LCD_DRAW_POINT_COLOR(x0+b,y0-a,color);                         //0           
		LCD_DRAW_POINT_COLOR(x0-a,y0+b,color);                         //1       
		LCD_DRAW_POINT_COLOR(x0-b,y0-a,color);                         //7           
		LCD_DRAW_POINT_COLOR(x0-a,y0-b,color);                         //2             
		LCD_DRAW_POINT_COLOR(x0+b,y0+a,color);                         //4               
		LCD_DRAW_POINT_COLOR(x0+a,y0-b,color);                         //5
		LCD_DRAW_POINT_COLOR(x0+a,y0+b,color);                         //6 
		LCD_DRAW_POINT_COLOR(x0-b,y0+a,color);             
		a++;

		/*Thuat toan Bresenham*/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DRAW_POINT_COLOR(x0+a,y0+b,color);
	}
} 

//////////////////////////////////////////////////////////////////

////////////////////////////////////////////
void LCD_DRAW_RECTANGLE_COLOR(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,uint16_t color)
{
	LCD_DRAW_LINE_COLOR(xsta,ysta,xend,ysta,color);
	LCD_DRAW_LINE_COLOR(xsta,ysta,xsta,yend,color);
	LCD_DRAW_LINE_COLOR(xsta,yend,xend,yend,color);
	LCD_DRAW_LINE_COLOR(xend,ysta,xend,yend,color);
}
