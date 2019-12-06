#include "spi.h"

SPI_TypeDef* SPIy = SPI1;//change (5 places in this file)

void initSPI1(void)
{	
	//khai bao bien
	GPIO_InitTypeDef GPIO_InitStructure; 
	SPI_InitTypeDef   SPI_InitStructure;
	//Cap xung clock cho port A, SPI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
	
	/* GPIO Config -------------------------------------------------------------*/
	//Cau hinh pin A4 - NSS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOA, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	GPIOA->BSRR   =  GPIO_BSRR_BS4;
	
	//Cau hinh pin A5 - SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOA, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	
	//Cau hinh pin A6 - MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOA, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	GPIOA->BSRR   =  GPIO_BSRR_BS6;
	
	//Cau hinh pin A7 - MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOA, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	
	 /* SPIy Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}
void disableSPI1(void)
{
	SPI_Cmd(SPI1, DISABLE);
}

void initSPI2(void)
{	
	//khai bao bien
	GPIO_InitTypeDef GPIO_InitStructure; 
	SPI_InitTypeDef   SPI_InitStructure;
	//Cap xung clock cho port B, SPI2
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB1Periph_SPI2, ENABLE);
	
	/* GPIO Config -------------------------------------------------------------*/
	//Cau hinh pin B12 - NSS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOB, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	GPIOB->BSRR   =  GPIO_BSRR_BS12;
	
	//Cau hinh pin B13 - SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOB, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	
	//Cau hinh pin B14 - MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOB, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	GPIOB->BSRR   =  GPIO_BSRR_BS14;
	
	//Cau hinh pin B15 - MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//default:toc do mac dinh
	GPIO_Init(GPIOB, &GPIO_InitStructure);//Cau lenh Cau Hinh.
	
	 /* SPIy Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}

uint8_t SPIySendByte(uint8_t data) {
	while (!(SPIy->SR & SPI_SR_TXE));      				//Cho den khi truyen hoan thanh(STM32F103)
	SPIy->DR=data;																//Ghi du lieu duoc truyen den thanh ghi
	while (!(SPIy->SR & SPI_SR_RXNE));     				//Cho den khi nhan hoan thanh (STM32F103)
	return SPIy->DR;		         									// 
}

void SPIy_WriteReg(uint8_t address, uint8_t value) {
	SPI1_NSS_ON();										// CS_Low  //change
	SPIySendByte(address); 
	SPIySendByte(value);
	SPI1_NSS_OFF();										// CS_HIGH  //change
}

uint8_t SPIy_ReadReg(uint8_t address) {
	uint8_t	val;

	SPI1_NSS_ON();										// CS_Low	//change
	SPIySendByte(address);
	val = SPIySendByte(0x00);
	SPI1_NSS_OFF();										// CS_HIGH //change
	return val;
}
