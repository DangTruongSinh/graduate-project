#include "stm32f10x.h"
/********************************************************************************************
*函数名称：void LED_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：LED初始化配置
*******************************************************************************************/
void LED_Configuration(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //翻转频率50MHz
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
