/******************************************************************
**	  火牛开发板（V1.0）
**	  USART配置文件
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.1
**	  作    者：openmcu
**	  完成日期:	2014.6.6
********************************************************************/
#include "stm32f10x.h"
#include "USART.h"
#include "stdio.h"

void USART1_SendByte(u16 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);	 
   
}
/*********************************************
*函数名称：void USART1_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART1初始化配置 包括GPIO初始化 TX必须配置为复用输出
**********************************************/
void USART1_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  	/* Configure USART1 Rx (PA.10) as input floating */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = 115200  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8个数据位
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;    //1个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;    //无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁用硬件流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;//使能发送/接收
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);     

}

/*********************************************
*函数名称：void USART2_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART2初始化配置 包括GPIO初始化 TX必须配置为复用输出
**********************************************/
void USART2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  	/* Configure USART2 Rx (PA.03) as input floating */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = 57600  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  //8个数据位
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;     //1个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;     //无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁用硬件流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;//使能发送/接收
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);    
}

void USARTx_Configuration(USART_TypeDef* USARTx, GPIO_TypeDef * GPIOx, uint16_t PinTx, uint16_t Pin_Rx, uint32_t Baud, FunctionalState ItRxState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* Configure USARTx Tx (PA.02) as alternate function push-pull */
  	GPIO_InitStructure.GPIO_Pin = PinTx;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOx, &GPIO_InitStructure);
    
  	/* Configure USARTx Rx (PA.03) as input floating */
  	GPIO_InitStructure.GPIO_Pin = Pin_Rx;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
  	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = Baud  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  //8个数据位
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;     //1个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;     //无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁用硬件流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;//使能发送/接收
	USART_Init(USARTx, &USART_InitStructure);
	USART_ITConfig(USARTx, USART_IT_RXNE, ItRxState);
	USART_Cmd(USARTx, ENABLE);    
}

/*********************************************
*函数名称：void USART2_SendByte(u16 Data)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART2 发送一个字节数据
**********************************************/
void USART2_SendByte(u16 Data)
{ 
   while (!(USART2->SR & USART_FLAG_TXE));
   USART2->DR = (Data & (uint16_t)0x01FF);	   
}


void USART_SendChar(USART_TypeDef* USARTx, unsigned char data)
{
		while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));
		USART_SendData(USARTx,data);
}

/*********************************************
*函数名称：void USART2_SendString(uint8_t *ch)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART2 发送字符串
**********************************************/
void USART2_SendString(uint8_t *ch)
{
	while(*ch!=0)
	{		
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, *ch);
		ch++;
	}   	
}

void USART_SendString(USART_TypeDef* USARTx, char* str)
{
	   while (*str)
   { 
		 USART_SendChar(USARTx,*str); 
     str++;
   }
}

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/*------------------------------------------------------*/
//uint16_t USART_ReceiveData(USART_TypeDef* USARTx)
//{
//  /* Check the parameters */
//  assert_param(IS_USART_ALL_PERIPH(USARTx));
//  
//  /* Receive Data */
//  return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
//}

//void USART_ReceiveData_String(USART_TypeDef* USARTx)
//{
//	for (uint8_t i=0; i<3; i++)
//	{
//		while(!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE));
//		dl[i] = USART_ReceiveData(USARTx);
//	}
//}

