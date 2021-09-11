#include "usart.h"
#include "dma.h"
#include "systick.h"
#include "stdio.h"
#include "string.h"
#include "gps.h"
#include "stdarg.h"
#include "gps.h"
USART_STA USART1_STA;
USART_STA USART2_STA;
USART_STA USART3_STA;
USART_STA USART6_STA;

u8 USART1_TX_Buff[USART1_Buff_Len];
u8 USART1_RX_Buff[USART1_Buff_Len];
u8 USART2_TX_Buff[USART2_Buff_Len];
u8 USART2_RX_Buff[USART2_Buff_Len];
u8 USART3_TX_Buff[USART3_Buff_Len];
u8 USART3_RX_Buff[USART3_Buff_Len];
u8 USART6_TX_Buff[USART6_Buff_Len];
u8 USART6_RX_Buff[USART6_Buff_Len];
void USART_CLR_ALL_BUFF(void)
{
	u8 i;
	for(i=0;i<6;i++) USART_CLR_BUFF(i);
}
void USART_CLR_BUFF(u8 x)
{
	switch (x)
	{
		case 1:memset(USART1_TX_Buff,0,USART1_Buff_Len);
	         memset(USART1_RX_Buff,0,USART1_Buff_Len);break;
		case 2:memset(USART2_TX_Buff,0,USART2_Buff_Len);
	         memset(USART2_RX_Buff,0,USART2_Buff_Len);break;
		case 3:memset(USART3_TX_Buff,0,USART3_Buff_Len);
	         memset(USART3_RX_Buff,0,USART3_Buff_Len);break;
		case 6:memset(USART6_TX_Buff,0,USART6_Buff_Len);
	         memset(USART6_RX_Buff,0,USART6_Buff_Len);break;
		default:break;
	}
}
/*********************USART1*******************************/
u8 USART1_SendData(u8 *data,u8 len)//Use DMA to Send Data
{
	while(USART1_STA.busy){}//wait for transmission complete
	USART1_STA.busy = 1;
	memcpy(USART1_TX_Buff,data,len);//Copy Data to buff
	//USART_ClearFlag(USART1,USART_IT_TC);
	DMA_SetCurrDataCounter(DMA2_Stream7,len);//
 	DMA_Cmd(DMA2_Stream7,ENABLE);//
	return 1;
}
void USART1_Init(u32 baudrate)
{
	 USART_InitTypeDef USART_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);   
   /* Enable USART clock */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
   /* Enable the DMA clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  /* USARTx GPIO configuration -----------------------------------------------*/ 
  /* Connect USART pins to AF7 */
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
   /* Configure USART Tx and Rx as alternate function push-pull */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   /* USARTx configuration ----------------------------------------------------*/
   /* Enable the USART OverSampling by 8 */
   USART_OverSampling8Cmd(USART1, ENABLE); 
	 
   USART_InitStructure.USART_BaudRate = baudrate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   /* When using Parity the word length must be configured to 9 bits */
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART1, &USART_InitStructure);
	 
	 USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//接收非空中断 使用DMA接收要失能RXNE
	 USART_ITConfig(USART1,USART_IT_TXE,DISABLE); //发送缓存器空中断
	 USART_ITConfig(USART1,USART_IT_TC,DISABLE);  //发送完成中断
	 
	 USART_ITConfig(USART1,USART_IT_IDLE,ENABLE); //空闲中断
	 
	 USART_ClearFlag(USART1,USART_FLAG_TC);
	 USART_ClearFlag(USART1,USART_FLAG_RXNE);
	 USART_ClearFlag(USART1,USART_FLAG_IDLE);
	 USART_ClearITPendingBit(USART1,USART_IT_IDLE);
	 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //子优先级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
   NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
	 USART_Cmd(USART1,DISABLE);
}
void USART1_IRQHandler(void)//USART1 IRQ Handler,USED DMA
{
	u8 receive;
	static u8 temp=0;
	if(USART_GetITStatus(USART1,USART_IT_TC)!= RESET)  //发送完成中断
 {
	 USART_ClearFlag(USART1,USART_FLAG_TC);
	 USART1_STA.tx_ok=1;
	 USART1_STA.busy=0;//释放传输通道
	 USART_ITConfig(USART1,USART_IT_TC,DISABLE);  //发送完成中断
 }
 #if 1
 if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)
 {
	  USART1->SR;
	  USART1->DR;
	 
	  DMA_Cmd(DMA2_Stream5,DISABLE);
		DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5);
		USART1_STA.rx_len = USART1_Buff_Len - DMA_GetCurrDataCounter(DMA2_Stream5);
		//memcpy(USART1_RX_Buff,USART1_RX_Buff,USART1_STA.rx_len);
	  USART1_STA.rx_ok=1;//flag receive finished
		DMA_SetCurrDataCounter(DMA2_Stream5,USART1_Buff_Len);
		DMA_Cmd(DMA2_Stream5,ENABLE);
 }
 #endif
 
 #if 0   //When do not use DMA,Open this to receive
 if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
 {
	 USART_ClearFlag(USART1,USART_FLAG_RXNE);
	 receive= USART_ReceiveData(USART1);
	 if(receive==0xF7 && (temp>20) ) 
	 {
		 USART1_STA.rx_ok=1
		 temp=0;
	 }
	 else USART1_RX_Buff[temp++]=receive;
 }
 #endif
}
/************************USART2*********************************/
u8 USART2_SendData(u8 *data,u8 len)//Use DMA to Send Data
{
	while(USART2_STA.busy){}//wait for transmission complete
	USART2_STA.busy = 1;
	memcpy(USART2_TX_Buff,data,len);//Copy Data to buff
	DMA_SetCurrDataCounter(DMA1_Stream6,len);
 	DMA_Cmd(DMA1_Stream6,ENABLE);
	return 1;
}
void USART2_Init(u32 baudrate)
{
	 USART_InitTypeDef USART_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);   
   /* Enable USART clock */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  /* USARTx GPIO configuration -----------------------------------------------*/ 
  /* Connect USART pins to AF7 */
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
   /* Configure USART Tx and Rx as alternate function push-pull */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   /* USARTx configuration ----------------------------------------------------*/
   /* Enable the USART OverSampling by 8 */
   //USART_OverSampling8Cmd(USART2, ENABLE); 
	 
   USART_InitStructure.USART_BaudRate = baudrate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   /* When using Parity the word length must be configured to 9 bits */
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART2, &USART_InitStructure);
	 
	 USART_ITConfig(USART2,USART_IT_TC,DISABLE);//发送完成中断
	 USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//接收非空中断
	 USART_ITConfig(USART2,USART_IT_TXE,DISABLE);//发送缓存器空中断
	 
	 USART_ITConfig(USART2,USART_IT_IDLE,ENABLE); //空闲中断
	 
	 USART_ClearFlag(USART2,USART_FLAG_TC);
	 USART_ClearFlag(USART2,USART_FLAG_RXNE);	 
	 USART_ClearFlag(USART2,USART_FLAG_IDLE);

	 NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //子优先级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
   NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
	 USART_Cmd(USART2,ENABLE);
}
void USART2_IRQHandler(void)
{
	u8 flag;
	if(USART_GetITStatus(USART2,USART_IT_TC)!=RESET)
	{
	 USART_ClearFlag(USART2,USART_FLAG_TC);
	 USART2_STA.busy=0;//释放传输通道
	 USART_ITConfig(USART2,USART_IT_TC,DISABLE);  //关闭发送完成中断
		
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)
	{
	  USART2->SR;
	  USART2->DR;
	 
	  DMA_Cmd(DMA1_Stream5,DISABLE);
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5);
		USART2_STA.rx_len = USART2_Buff_Len - DMA_GetCurrDataCounter(DMA1_Stream5);
		//memcpy(USART1_RX_Buff,USART1_RX_Buff,USART1_RX_Cnt);
	  USART2_STA.rx_ok=1;//Set High Bit to flag receive finished
		DMA_SetCurrDataCounter(DMA1_Stream5,USART2_Buff_Len);
		DMA_Cmd(DMA1_Stream5,ENABLE);
		
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);
	}
}
/***************USART3************************/
u8 USART3_SendData(u8 *data,u8 len)//Use DMA to Send Data
{
	while(USART3_STA.busy){}//wait for transmission complete
	USART3_STA.busy = 1;
	memcpy(USART3_TX_Buff,data,len);//Copy Data to buff
	USART_ClearFlag(USART3,USART_IT_TC);
	DMA_SetCurrDataCounter(DMA1_Stream3,len);//
 	DMA_Cmd(DMA1_Stream3,ENABLE);//
	return 1;
}
void USART3_Init(u32 baudrate)
{
	 USART_InitTypeDef USART_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);   
   /* Enable USART clock */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  /* USARTx GPIO configuration -----------------------------------------------*/ 
  /* Connect USART pins to AF7 */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
   /* Configure USART Tx and Rx as alternate function push-pull */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   /* USARTx configuration ----------------------------------------------------*/
   /* Enable the USART OverSampling by 8 */
  // USART_OverSampling8Cmd(USART3, ENABLE); 
	 
   USART_InitStructure.USART_BaudRate = baudrate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   /* When using Parity the word length must be configured to 9 bits */
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART3, &USART_InitStructure);
	 
	 USART_ITConfig(USART3,USART_IT_TC,DISABLE);//发送完成中断
	 USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//接收非空中断
	 USART_ITConfig(USART3,USART_IT_TXE,DISABLE);//发送缓存器空中断
	 
	 USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); //空闲中断
   USART_ClearFlag(USART3,USART_FLAG_TC);
	 USART_ClearITPendingBit(USART3,USART_IT_TC);
	 
	 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //子优先级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
   NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
	 USART_Cmd(USART3,ENABLE);
	 //_CLR_BUFF_(USART3_RX_Buff,USART3_Buff_Len);
}
void USART3_IRQHandler(void)
{
	u8 receive;
	static u8 temp=0;
	if(USART_GetITStatus(USART3,USART_IT_TC)!= RESET)  //发送完成中断
 {
	 USART_ClearFlag(USART3,USART_FLAG_TC);
	 USART3_STA.busy=0;//释放传输通道
	 USART_ITConfig(USART3,USART_IT_TC,DISABLE);  //发送完成中断
 }
 if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET)
 {
	  USART3->SR;
	  USART3->DR;
	  DMA_Cmd(DMA1_Stream1,DISABLE);
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);
		USART3_STA.rx_len = USART3_Buff_Len - DMA_GetCurrDataCounter(DMA1_Stream1);
		memcpy(gps_buff,USART3_RX_Buff,USART3_STA.rx_len);//copy data to gps buff
	 	USART3_STA.rx_ok=1;
		DMA_SetCurrDataCounter(DMA1_Stream1,USART3_Buff_Len);
		DMA_Cmd(DMA1_Stream1,ENABLE);
	  
	 /**处理GPS数据**/
	 NMEA_GPGGA_Analysis(&gps_data,gps_buff);
	 NMEA_GPVTG_Analysis(&gps_data,gps_buff);
	 /***************/
	 
 }
  if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
    (void)USART_ReceiveData(USART3);
  }
}
/***************USART6*******************/
u8 USART6_SendData(u8 *data,u8 len)//Use DMA to Send Data
{
	while(USART6_STA.busy){}//wait for transmission complete
	USART6_STA.busy = 1;
	memcpy(USART6_TX_Buff,data,len);//Copy Data to buff
	USART_ClearFlag(USART6,USART_IT_TC);
	DMA_SetCurrDataCounter(DMA2_Stream6,len);//
 	DMA_Cmd(DMA2_Stream6,ENABLE);//
	return 1;
}
void USART6_Init(u32 baudrate)
{
	 USART_InitTypeDef USART_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);   
   /* Enable USART clock */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
  /* USARTx GPIO configuration -----------------------------------------------*/ 
  /* Connect USART pins to AF8 */
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
   /* Configure USART Tx and Rx as alternate function push-pull */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   /* USARTx configuration ----------------------------------------------------*/
   /* Enable the USART OverSampling by 8 */
   //USART_OverSampling8Cmd(USART2, ENABLE); 
	 
   USART_InitStructure.USART_BaudRate = baudrate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   /* When using Parity the word length must be configured to 9 bits */
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART6, &USART_InitStructure);
	 
	 USART_ITConfig(USART6,USART_IT_TC,DISABLE);//发送完成中断
	 USART_ITConfig(USART6,USART_IT_RXNE,DISABLE);//接收非空中断
	 USART_ITConfig(USART6,USART_IT_TXE,DISABLE);//发送缓存器空中断
	 USART_ITConfig(USART6,USART_IT_IDLE,ENABLE); //空闲中断
	 
	 USART_ClearFlag(USART6,USART_FLAG_IDLE);

	 NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //子优先级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
   NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
	 USART_Cmd(USART6,ENABLE);
}
void USART6_IRQHandler(void)
{
	u8 receive;
	static u8 temp=0;
	if(USART_GetITStatus(USART6,USART_IT_TC)!= RESET)  //发送完成中断
 {
	 USART_ClearFlag(USART6,USART_FLAG_TC);
	 USART6_STA.busy=0;//释放传输通道
	 USART_ITConfig(USART6,USART_IT_TC,DISABLE);  //发送完成中断
 }
 if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET)
 {
	  USART6->SR;
	  USART6->DR;
	 
	  DMA_Cmd(DMA2_Stream1,DISABLE);
		DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);
		USART6_STA.rx_len = USART6_Buff_Len - DMA_GetCurrDataCounter(DMA2_Stream1);
		//memcpy(gps_buff,USART6_RX_Buff,USART6_STA.rx_len);//copy data to gps buff
	 	USART6_STA.rx_ok=1;
		DMA_SetCurrDataCounter(DMA2_Stream1,USART6_Buff_Len);
		DMA_Cmd(DMA2_Stream1,ENABLE);
 }

}
