#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f10x.h"
//#include "stdint.h"

extern void USART1_SendByte(u16 dat);
extern void USART1Write(u8* data,u16 len);

void USART1_Configuration(void);
void USART_SendChar(USART_TypeDef* USARTx, u8 data);
void USART_SendString(USART_TypeDef* USARTx, char* str);
void USARTx_Configuration(USART_TypeDef* USARTx, GPIO_TypeDef * GPIOx, uint16_t PinTx, uint16_t Pin_Rx, uint32_t Baud, FunctionalState ItRxState);

extern void USART2_SendByte(u16 dat);
extern void USART2Write(u8* data,u16 len);

void USART2_Configuration(void);
void USART2_SendString(uint8_t *ch);
//uint16_t USART_ReceiveData(USART_TypeDef* USARTx);
#define DIR485_H  GPIOC->BSRR=1<<1
#define DIR485_L  GPIOC->BRR=1<<1 


#endif
