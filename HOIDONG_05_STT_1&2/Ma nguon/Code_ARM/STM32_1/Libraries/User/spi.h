#ifndef __SPI_H
#define __SPI_H

// STM32F103 SPI1

// 3.3V -  RST
// PA4  - (OUT)	SPI1_NSS(SDA)
// PA5  - (OUT)	SPI1_SCK
// PA6  - (IN)	SPI1_MISO (Master In)
// PA7  - (OUT)	SPI1_MOSI (Master Out)
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//#include "DEF_STM32.h"	//me
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


#define 	SPI_I2S_FLAG_BSY	((uint16_t)0x0080)
#define 	SPI1_NSS_ON()		GPIOA->BSRR = GPIO_BSRR_BR4
#define 	SPI1_NSS_OFF()		GPIOA->BSRR = GPIO_BSRR_BS4
#define 	SPI2_NSS_ON()		GPIOB->BSRR = GPIO_BSRR_BR12
#define 	SPI2_NSS_OFF()		GPIOB->BSRR = GPIO_BSRR_BS12

extern void initSPI1(void);
void disableSPI1(void);
extern void initSPI2(void);
extern uint8_t SPIySendByte(uint8_t data);
extern void SPIy_WriteReg(uint8_t address, uint8_t value);
extern uint8_t SPIy_ReadReg(uint8_t address);

#endif
