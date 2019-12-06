#include "stm32f10x.h"                  // Device header
#include "LCD.h"
#include "Touch.h"
#include "24c02.h"
#include "spi.h"
#include "rc522.h"
#include "R305.h"
#include "USART.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//ds1307
#include "ds1307.h"
//SDCard
#include "SD_driver.h"
#include "ff.h"
#include "cJSON.h"

char JSON_STRING[] = "{\"ID\":1,\"Name\":\"NGUYEN VAN A\",\"STS\":0,\"NREG\":0,\"NF\":0,\"NR\":0,\"TLATE\":0,\"TSOON\":0}" ;

extern u8 hinhnen240_320[];
extern u8 password50_50[];
extern u8 gImage_RFID50_50[];
extern u8 gImage_r305_50_44[];
extern u8 gImage_myanh[];
u8 TT_TOUCH;
u8 refresh;

//ds1307
uint8_t D, d, M, y, h, m, s;

void Delay(uint32_t Time){
    uint32_t i;
    
    i = 0;
    while (Time--) 
		{
       for (i = 0; i < 5000; i++);
    }
}
int8_t Nhan_dang_RFID		(uint8_t * str ){
		
		if(str[0]==0x63 && str[1]==0x8b && str[2]==0xa5 && str[3]==0x59 && str[4]==0x14){
				LCD_SHOW_STRING(50,100,"NGUYEN VAN A", BLACK,WHITE);
				return 1;
		}
		else if(str[0]==0x36 && str[1]==0xda && str[2]==0x3e && str[3]==0xd5 && str[4]==0x07){
				LCD_SHOW_STRING(50,100,"NGUYEN VAN B", BLACK,WHITE);
				return 2;
		}
				else if(str[0]==0x0d && str[1]==0x21 && str[2]==0xf8 && str[3]==0xd8 && str[4]==0x0c){
				LCD_SHOW_STRING(50,100,"NGUYEN VAN C", BLACK,WHITE);
				return 3;
		}
						else if(str[0]==0x13 && str[1]==0x27 && str[2]==0x3b && str[3]==0x8b && str[4]==0x84){
				LCD_SHOW_STRING(50,100,"NGUYEN VAN D", BLACK,WHITE);
				return 4;
		}							
		else{
				LCD_SHOW_STRING(50,100,"The chua co", BLACK,WHITE);
				return 0;
		}
}
int8_t Nhan_dang_vantay	(uint8_t vt )		{	
		if(vt==1){
				LCD_SHOW_STRING(50,100,"Nguyen Van A  ", BLACK,WHITE);
				return 1;
		}
		else if(vt==2){
				LCD_SHOW_STRING(50,100,"Nguyen Van B", BLACK,WHITE);
				return 2;
		}
		else if(vt==3){
				LCD_SHOW_STRING(50,100,"Nguyen Van C", BLACK,WHITE);
				return 3;
		}
		else if(vt==4){
				LCD_SHOW_STRING(50,100,"Nguyen Van D", BLACK,WHITE);
				return 4;
		}
		else if(vt==5){
				LCD_SHOW_STRING(50,100,"Nguyen Van E", BLACK,WHITE);
				return 5;
		}
		else if(vt==6){
				LCD_SHOW_STRING(50,100,"Nguyen Van F", BLACK,WHITE);
				return 6;
		}
		else if(vt==7){
				LCD_SHOW_STRING(50,100,"Nguyen Van G", BLACK,WHITE);
				return 7;
		}
		else if(vt==8){
				LCD_SHOW_STRING(50,100,"Nguyen Van H", BLACK,WHITE);
				return 8;
		}
		else if(vt==9){
				LCD_SHOW_STRING(50,100,"Nguyen Van K", BLACK,WHITE);
				return 9;
		}
		else if(vt==10){
				LCD_SHOW_STRING(50,100,"Nguyen Van L", BLACK,WHITE);
				return 10;
		}
		else{
		
				LCD_SHOW_STRING(50,100,"Van tay chua co", BLACK,WHITE);
				return 0;
		}
}

uint8_t get_rfid_id(void)
{
	uint8_t id;
	uint8_t		str[MFRC522_MAX_LEN];
  char buf [100];	
	initSPI1();		//change 
				//MFRC522_Init();															
				SPI1_NSS_OFF();	//change
	Delay(500);
if (!MFRC522_Request(PICC_REQIDL, str)) 
				if (!MFRC522_Anticoll(str)) 
				{	
					sprintf (buf, "%x %x %x %x %x %x %x\r\n",str[0],str[1],str[2],str[3],str[4],str[5],str[6]);				
					LCD_SHOW_STRING(50,130,buf, BLACK,WHITE);		
					id=Nhan_dang_RFID(str);
				}
				return id;
}

void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			      //2 cap ProprityGroup

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	        //Ngat ngoai kenh 4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //Chon thu tu uu tien 2 cho PriorityGroup cho kenh 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		    //Chon thu tu uu tien 0 cho SubPriority cho kenh 4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//Cho phep ngat
	NVIC_Init(&NVIC_InitStructure);														//Cai dat cac cau hinh tren vao NVIC
}
//ESP8266
void NVIC_Configuration_USART3(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			  //中断分组2

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 		  //USART1接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint8_t dl[30]="";
uint8_t ucheck=0,i=0;
void USART3_IRQHandler(void){

	while(!USART_GetFlagStatus(USART3, USART_FLAG_RXNE));
	dl[i] = USART_ReceiveData(USART3);
	
	if (dl[i] == '*')
	{			
		dl[i] = '\0';
		i = 0;
		ucheck = 1;
		
		LCD_SHOW_STRING(50,250,(char*)dl, BLACK,WHITE);
	}
	else
		i++;
}

void RCC_Configuration(void){

	ErrorStatus HSEStartUpStatus;                      
	RCC_DeInit();                                           //Reset RCC
	RCC_HSEConfig(RCC_HSE_ON);                              //Bat HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp();             //Waits for HSE start-up
	if(HSEStartUpStatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);                      //Cau hinh HCLK = SYSCLK =72Mhz
		RCC_PCLK2Config(RCC_HCLK_Div1);                       //Cau hinh PCLK2 = HCLK =64Mhz
		RCC_PCLK1Config(RCC_HCLK_Div2);                       //Cau hinh PCLK1 = HCLK/2 =36Mhz
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//Chon HSE la ngo vao PLL he so nhan la 9
		RCC_PLLCmd(ENABLE);                                   //Bat PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //Cho PLL khoi dong xong
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //Dat PLLCLK lam SYSCLK
		while(RCC_GetSYSCLKSource() != 0x08);                 //Cho den khi dat PLLCLK lam SYSCLK xong
	}
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
	                         | RCC_APB2Periph_GPIOB
                           |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
                           , ENABLE );										//Cho phep cap xung Clock cho cac PORT va ngoai vi UART1
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3                       
                           , ENABLE );
	//APB2 .......... APB1_SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);			//Cho phep cap xung Clock cho ngoai vi SPI1(RC522)
	//APB1 .......... APB1_SPI2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);			//Cho phep cap xung Clock cho ngoai vi AFIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);			//Cho phep cap xung Clock cho ngoai vi I2C1
}

//SDCard
FATFS fs;            // Work area (file system object) for logical drive
FIL fsrc;
UINT bw, br;
u8 buffer[1024]; // file copy buffer
FRESULT res;         // FatFs function common result code
//

int main(void){

	int change=0, mode=0;
	int8_t fCheck=-3, id=0,vt=0;
	int fid=1;
	char buf [100];
	
	// RC522
	uint8_t		str[MFRC522_MAX_LEN];												// MFRC522_MAX_LEN = 1
	
	RCC_Configuration();
	NVIC_Configuration(); 
	I2C_EE_Init();																						//Khoi tao I2C
	
	LCD_INIT();
	TOUCH_INIT();	
	//TOUCH_ADS7483_ADJUST();
	//RC522-SPI2
	initSPI1();		//change (2 place in this file)
  MFRC522_Init();															
  SPI1_NSS_OFF();	
	//
	//R305
	USARTx_Configuration(USART2,GPIOA, GPIO_Pin_2, GPIO_Pin_3,57600,DISABLE);	
	USARTx_Configuration(USART1,GPIOA, GPIO_Pin_9, GPIO_Pin_10,57600,DISABLE);	//USART1DEBUG
	//
	//ESP8266
	USARTx_Configuration(USART3,GPIOB, GPIO_Pin_10, GPIO_Pin_11,57600,ENABLE);	
	NVIC_Configuration_USART3();
	//
		//DS1307
	ds1307_init();
	// Set initial date and time 
	ds1307_set_calendar_date(DS1307_MONDAY, 20, 6, 19);
	Delay(100);
	ds1307_set_time_24(07, 30, 50);
	Delay(100);
	//
	//disableSPI1();
	TOUCH_INIT();	
	
	
	TT_TOUCH=TOUCH_PRESS();
	LCD_DRAW_PICTURE(0,0,240,320,gImage_myanh); 	//background image, slow load code
	LCD_DRAW_PICTURE(10,200,50,50,gImage_RFID50_50);				
	LCD_DRAW_PICTURE(100,200,44,50,gImage_r305_50_44);
	LCD_DRAW_PICTURE(190,200,50,50,password50_50);
	
	//SDCard
	SPI_Configuration();

	//
	
	if 			(SD_Init()!=0){
		LCD_SHOW_STRING(50,5,"Loi he thong", BLACK,WHITE);
		LCD_SHOW_STRING(50,25,"Hay kiem tra lai", BLACK,WHITE);
	}
	
	while		(SD_Init()!=1){
	//while		(1){	
	//Lay thoi gian thuc
	ds1307_get_calendar_date(&D, &d, &M, &y);
	ds1307_get_time_24(&h, &m, &s);
		/*
	sprintf (buf, "%d:%d:%d\n",h,m,s); 
	LCD_SHOW_STRING(80,255,buf, BLACK,WHITE);	
	sprintf (buf, "%d/%d/%d\n",d,M,y);
	LCD_SHOW_STRING(80,275,buf, BLACK,WHITE);	
		*/
//	Delay(50);			
		if      (change == 0) {
			if(mode==0){
				TOUCH_CONFIGURATION();	
				mode = 1;
		LCD_DRAW_PICTURE(0,0,240,320,gImage_myanh); 
		LCD_DRAW_PICTURE(10,200,50,50,gImage_RFID50_50);				
	  LCD_DRAW_PICTURE(100,200,44,50,gImage_r305_50_44);
	  LCD_DRAW_PICTURE(190,200,50,50,password50_50);
				LCD_SHOW_STRING(50,5,"Touch Mode", BLACK,WHITE);
				LCD_SHOW_STRING(50,25,"MOI CHON CHE DO", BLACK,WHITE);
				
				LCD_SHOW_STRING(10,60,"     DO AN TOT NGHIEP       ", RED,YELLOW);
				LCD_SHOW_STRING(10,80,"GVHD: DANG PHUOC HAI TRANG  ", BLACK,WHITE);
				LCD_SHOW_STRING(10,100,"SVTH1: NGUYEN THANH TINH    ", BLACK,WHITE);
				LCD_SHOW_STRING(10,120,"SVTH2: NGUYEN HUU NHIEN     ", BLACK,WHITE);
			}		
			TT_TOUCH=TOUCH_PRESS();
			if(TT_TOUCH==1)
			{		
				//sprintf (buf, "%d, %d\n",Pen_Point.X0,Pen_Point.Y0);
				//LCD_SHOW_STRING(141,190,buf, BLACK,WHITE);			
				if(Pen_Point.X0<65 && Pen_Point.X0>30&& Pen_Point.Y0>110 && Pen_Point.Y0<150) {
					mode = 0;
					change = 1; // RFID
					refresh = 1;
				}else if (Pen_Point.X0>110 && Pen_Point.X0<150 && Pen_Point.Y0>110 && Pen_Point.Y0<150) {
					mode = 0;
					change = 2; // CAm bien van tay
					refresh =1;
				}else if(Pen_Point.X0>200 && Pen_Point.X0<240 && Pen_Point.Y0>110 && Pen_Point.Y0<150) {
					mode = 0;
					change = 3;
					refresh = 1;
				}
				
			}			
		}
		else if (change == 1) { //TH dung RFID
		if(refresh != 0)	//Ve lai hinh nen neu can thiet
			{
		LCD_DRAW_PICTURE(0,0,240,320,gImage_myanh); 
		LCD_DRAW_PICTURE(10,200,50,50,gImage_RFID50_50);				
	  LCD_DRAW_PICTURE(100,200,44,50,gImage_r305_50_44);
	  LCD_DRAW_PICTURE(190,200,50,50,password50_50);
			refresh = 0;
			} //Ket thuc ve lai hinh nen + icon
			
	sprintf (buf, "%d:%d:%d\n",h,m,s); // in thoi gian
	LCD_SHOW_STRING(80,255,buf, BLACK,WHITE);	
	sprintf (buf, "%d/%d/%d\n",d,M,y);
	LCD_SHOW_STRING(80,275,buf, BLACK,WHITE);	 //ket thuc in thoi gian	
			
				initSPI1();		//khoi dong spi													
				SPI1_NSS_OFF();	
				LCD_SHOW_STRING(50,5,"    RFID     ", BLACK,WHITE);	
				LCD_SHOW_STRING(50,25,"Moi quet the   ", BLACK,WHITE);
			
			if (!MFRC522_Request(PICC_REQIDL, str)) 
				if (!MFRC522_Anticoll(str)) 
				{	
					sprintf (buf, "%x %x %x %x %x %x %x\r\n",str[0],str[1],str[2],str[3],str[4],str[5],str[6]);				
					LCD_SHOW_STRING(50,130,buf, BLACK,WHITE);		
					id=Nhan_dang_RFID(str);
						if(id!=0){
							//sd card
								f_mount(0,&fs);
								res = f_open(&fsrc,"RFID.csv",FA_OPEN_EXISTING | FA_WRITE);	
								if(res == FR_OK){
										res = f_lseek(&fsrc, f_size(&fsrc));
										if(res == FR_OK){
											if			(id==1)	f_printf(&fsrc,"NGUYEN VAN A,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (id==2)	f_printf(&fsrc,"NGUYEN VAN B,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if	(id==3)	f_printf(&fsrc,"NGUYEN VAN C,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (id==4)	f_printf(&fsrc,"NGUYEN VAN D,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
										}								
								}
								f_close(&fsrc);
								f_mount(0,NULL);		
					//ESP8266
					switch(id)
									{
									case 1: USART_SendString(USART3,"NGUYEN VAN A - RFID\r\n"); break;
									case 2: USART_SendString(USART3,"NGUYEN VAN B - RFID\r\n"); break;
									case 3: USART_SendString(USART3,"NGUYEN VAN C - RFID\r\n"); break;
									case 4: USART_SendString(USART3,"NGUYEN VAN D - RFID\r\n"); break;
									case 5: USART_SendString(USART3,"NGUYEN VAN E - RFID\r\n"); break;
									}
					Delay(1000);
					}
				}
				
			if(Pen_Point.Key_Sta==Key_Down) {	//IntTouch
				LCD_SHOW_STRING(50,150,"                    ", BLACK,WHITE);
				mode=0;
				change = 0;	
				
			}
		}
		else if (change == 2) {
		if(refresh != 0)	
			{
		LCD_DRAW_PICTURE(0,0,240,320,gImage_myanh); 
		LCD_DRAW_PICTURE(10,200,50,50,gImage_RFID50_50);				
	  LCD_DRAW_PICTURE(100,200,44,50,gImage_r305_50_44);
	  LCD_DRAW_PICTURE(190,200,50,50,password50_50);
			refresh = 0;
			}
	sprintf (buf, "%d:%d:%d\n",h,m,s); 
	LCD_SHOW_STRING(80,255,buf, BLACK,WHITE);	
	sprintf (buf, "%d/%d/%d\n",d,M,y);
	LCD_SHOW_STRING(80,275,buf, BLACK,WHITE);
				LCD_SHOW_STRING(50,5,"    R305 Mode    ", BLACK,WHITE);
				LCD_SHOW_STRING(50,25,"Moi dat tay vao", BLACK,WHITE);	
//				USART_SendString(USART1, "Founding..... ");	//USART1DEBUG
				//while(!verifyPassword()) {};			//xac minh password
				verifyPassword();
				if(verifyPassword() == 1) {
//					USART_SendString(USART1, "Found sucess! ");	//USART1DEBUG				
					fCheck = fingerIDSearch();			//tim id van tay
					if(fCheck == -1)
					LCD_SHOW_STRING(50,130,"Loi!             ", BLACK,WHITE);				
					else if (fCheck == -2)
						LCD_SHOW_STRING(50,130,"Khong tim thay", BLACK,WHITE);
					else {
						sprintf (buf, "Finger ID: %d\n       ",fCheck);
						LCD_SHOW_STRING(50,130,buf, BLACK,WHITE);
						//sd card
						vt=Nhan_dang_vantay(fCheck);
						Delay(1000);
						if(vt!=0){
							//sd card
								f_mount(0,&fs);
								res = f_open(&fsrc,"VANTAY.csv",FA_OPEN_EXISTING | FA_WRITE);	
								if(res == FR_OK){
										res = f_lseek(&fsrc, f_size(&fsrc));
										if(res == FR_OK){
											if			(vt==1)	f_printf(&fsrc,"Nguyen Van A,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==2)	f_printf(&fsrc,"Nguyen Van B,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==3)	f_printf(&fsrc,"Nguyen Van C,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==4)	f_printf(&fsrc,"Nguyen Van D,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==5)	f_printf(&fsrc,"Nguyen Van E,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==6)	f_printf(&fsrc,"Nguyen Van F,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==7)	f_printf(&fsrc,"Nguyen Van G,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==8)	f_printf(&fsrc,"Nguyen Van H,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==9)	f_printf(&fsrc,"Nguyen Van K,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
											else if (vt==10)f_printf(&fsrc,"Nguyen Van L,%d:%d:%d,%d:%d:%d\n",d,M,y,h,m,s);
										}								
								}
								f_close(&fsrc);
								f_mount(0,NULL);		
					       //GUI DU LIEU QUA ESP8266
									switch(vt)
									{
									case 1: USART_SendString(USART3,"NGUYEN VAN A - VAN TAY\r\n"); break;
									case 2: USART_SendString(USART3,"NGUYEN VAN B - VAN TAY\r\n"); break;
									case 3: USART_SendString(USART3,"NGUYEN VAN C - VAN TAY\r\n"); break;
									case 4: USART_SendString(USART3,"NGUYEN VAN D - VAN TAY\r\n"); break;
									}
					       Delay(1000);
						}
					}
				}
				else {
					LCD_SHOW_STRING(50,130,"Found wrong!     ", BLACK,WHITE);
//				USART_SendString(USART1, "Something wrong!\n");	//USART1DEBUG
					mode=0;
					change=0;
				}
				
					if(Pen_Point.Key_Sta==Key_Down) {	//IntTouch
						mode=0;
						change = 0;	
					 }			
		}
		
		else if (change == 3) {
			uint8_t id;
		if(refresh != 0)	
			{
		LCD_DRAW_PICTURE(0,0,240,320,gImage_myanh); 
		LCD_DRAW_PICTURE(10,200,50,50,gImage_RFID50_50);				
	  LCD_DRAW_PICTURE(100,200,44,50,gImage_r305_50_44);
	  LCD_DRAW_PICTURE(190,200,50,50,password50_50);
			refresh = 0;
			}
	sprintf (buf, "%d:%d:%d\n",h,m,s); 
	LCD_SHOW_STRING(80,255,buf, BLACK,WHITE);	
	sprintf (buf, "%d/%d/%d\n",d,M,y);
	LCD_SHOW_STRING(80,275,buf, BLACK,WHITE);
			LCD_SHOW_STRING(50,5,"   THEM VAN TAY  ", BLACK,WHITE);
			LCD_SHOW_STRING(10,25,"Quet RFID roi them van tay", BLACK,WHITE);	
						//	verifyPassword();
			id = 0;
			while(id ==0) id = get_rfid_id();
							if(verifyPassword() == 1) {
								if(fingerEnroll(id)==1) {	//once
									LCD_SHOW_STRING(50,130,"Da them thanh cong       ", BLACK,WHITE);
									//Delay(10000);
									LCD_SHOW_STRING(50,130,buf, BLACK,WHITE);
										//sd card
										f_mount(0,&fs);
										res = f_open(&fsrc,"MAUVANTAY.csv",FA_OPEN_EXISTING | FA_WRITE);
										if(res == FR_OK){
													res = f_lseek(&fsrc, f_size(&fsrc));
													if(res == FR_OK){
														f_printf(&fsrc,buf,"%d,",id);
														sprintf (buf, "Finger ID: %d\n ",id);
														LCD_SHOW_STRING(50,150,buf, BLACK,WHITE);
														fid++;
														LCD_SHOW_STRING(60,140,"Write...", BLACK,WHITE);
														Delay(3000);
														LCD_SHOW_STRING(60,140,"         ", BLACK,WHITE);
														LCD_SHOW_STRING(50,150,"             ", BLACK,WHITE);
													}
											}
											f_close(&fsrc);
											f_mount(0,NULL); //dong lien kettoi the nho
											switch(id)
									{
									case 1: USART_SendString(USART3,"DK VAN TAY NGUYEN VAN A\r\n"); break;
									case 2: USART_SendString(USART3,"DK VAN TAY NGUYEN VAN B\r\n"); break;
									case 3: USART_SendString(USART3,"DK VAN TAY NGUYEN VAN C\r\n"); break;
									case 4: USART_SendString(USART3,"DK VAN TAY NGUYEN VAN D\r\n"); break;
									case 5: USART_SendString(USART3,"DK VAN TAY NGUYEN VAN E\r\n"); break;
									}
					       Delay(1000);
									}
											else {LCD_SHOW_STRING(50,130,"khong thanh cong    ", BLACK,WHITE);
									}												
						mode=0;
						change = 0;											
			}
	} // change = 3 end
}
	return 0;
}
