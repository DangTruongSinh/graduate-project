#ifndef __led_H_
#define __led_H_
#include "stm32f10x_conf.h"


#define PFout(n)	*((volatile unsigned long *)(0x42000000+((GPIOF_ODR_Addr-0x40000000)<<5)+(n<<2)))


/* LED */
#define RCC_APB2Periph_LED	RCC_APB2Periph_GPIOD
#define LED_PORT	GPIOD
#define LED1_PIN	GPIO_Pin_8
#define LED2_PIN	GPIO_Pin_9
#define LED3_PIN	GPIO_Pin_10
#define LED4_PIN	GPIO_Pin_11

#define LED1(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED1_PIN)) : (GPIO_ResetBits(LED_PORT, LED1_PIN)));
#define LED2(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED2_PIN)) : (GPIO_ResetBits(LED_PORT, LED2_PIN)));
#define LED3(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED3_PIN)) : (GPIO_ResetBits(LED_PORT, LED3_PIN)));
#define LED4(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED4_PIN)) : (GPIO_ResetBits(LED_PORT, LED4_PIN)));


/* BUTTON */
#define RCC_APB2Periph_BUTTON	RCC_APB2Periph_GPIOA
#define GPIO_ButtonPort	GPIOA
#define BUTTON1	GPIO_Pin_0


/* º¯ÊýÉùÃ÷ */
void LED_Configuration(void);
#endif
