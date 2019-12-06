#include "stm32f10x.h"
/********************************************************************************************
*�������ƣ�void LED_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����LED��ʼ������
*******************************************************************************************/
void LED_Configuration(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //��תƵ��50MHz
   GPIO_Init(GPIOD, &GPIO_InitStructure);
	 
	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

}
