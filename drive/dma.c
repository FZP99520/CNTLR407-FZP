#include "dma.h"
#include "usart.h"
#include "stm32f4xx_dma.h"
#include "string.h"
#include "printf.h"
void DMA_USART1_Init(void)
{
	 	NVIC_InitTypeDef NVIC_InitStructure;
	 DMA_InitTypeDef  DMA_InitStructure;
	   /* Enable the DMA clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	 DMA_InitStructure.DMA_BufferSize =(u32)USART1_Buff_Len;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART1->DR)) ;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   /* Configure TX DMA */
   DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)USART1_TX_Buff ;
   DMA_Init(DMA2_Stream7,&DMA_InitStructure);
	 DMA_Cmd(DMA2_Stream7, DISABLE);
 /* Configure RX DMA */
	 DMA_DeInit(DMA2_Stream5);
   DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(u32)USART1_RX_Buff; 
   DMA_Init(DMA2_Stream5,&DMA_InitStructure);
	 DMA_Cmd(DMA2_Stream5,ENABLE);
	 /*  interrupt  */
	  DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);//传输完成中断  TX
		
	 NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	 
	 /*Enable USART to DMA Request */
   USART_DMACmd(USART1, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);   
   USART_Cmd(USART1,ENABLE);   
}
void DMA2_Stream7_IRQHandler(void)//USART1_DMA IRQ_Handler
{
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) != RESET) //USART1_TX
	{
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
		USART_ITConfig(USART1,USART_IT_TC,ENABLE);  //发送完成中断
		DMA_Cmd(DMA2_Stream7,DISABLE); //关闭传输
	}
}
/*************************************************************************/
void DMA_USART2_Init(void)
{
	 DMA_InitTypeDef  DMA_InitStructure;
 	 NVIC_InitTypeDef NVIC_InitStructure;
	   /* Enable the DMA clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	 DMA_DeInit(DMA1_Stream6);//TX
	 DMA_InitStructure.DMA_Memory0BaseAddr=(u32)USART2_TX_Buff;//发送地址
	 DMA_InitStructure.DMA_BufferSize = USART2_Buff_Len;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull ;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART2->DR)) ;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   /* Configure TX DMA */
   DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(u32)USART2_TX_Buff ;
   DMA_Init(DMA1_Stream6,&DMA_InitStructure);
	 
   /* Configure RX DMA */
	 DMA_DeInit(DMA1_Stream5);
   DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(u32)USART2_RX_Buff; 
   DMA_Init(DMA1_Stream5,&DMA_InitStructure);
	 DMA_Cmd(DMA1_Stream5,ENABLE);
	 
	 NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 
	 NVIC_Init(&NVIC_InitStructure);
	 
	 DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);//开启中断
	 /*Enable USART to DMA Request */
   USART_DMACmd(USART2, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);      
   /* Enable USART */
   USART_Cmd(USART2, ENABLE);
}
/************************/
void DMA1_Stream6_IRQHandler(void)//USART2_TX DMA
{
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) != RESET) 
	{
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
		DMA_Cmd(DMA1_Stream6,DISABLE);
		USART_ITConfig(USART2,USART_IT_TC,ENABLE);
	}
}

/******************USART3 DMA**************************/
void DMA_USART3_Init(void)
{
	 	NVIC_InitTypeDef NVIC_InitStructure;
	 DMA_InitTypeDef  DMA_InitStructure;
	   /* Enable the DMA clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	 DMA_InitStructure.DMA_BufferSize =USART3_Buff_Len;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART3->DR)) ;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   /* Configure TX DMA */
   DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)USART3_TX_Buff;
   DMA_Init(DMA1_Stream3,&DMA_InitStructure);
	 DMA_Cmd(DMA1_Stream3, DISABLE);
 /* Configure RX DMA */
	 DMA_DeInit(DMA1_Stream1);
   DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(u32)USART3_RX_Buff; 
   DMA_Init(DMA1_Stream1,&DMA_InitStructure);
	 DMA_Cmd(DMA1_Stream1,ENABLE);
	 /*  interrupt  */
	  DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);//传输完成中断  TX
		
	 NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	 
	 /*Enable USART to DMA Request */
   USART_DMACmd(USART3, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);   
   USART_Cmd(USART3,ENABLE);   
}
void DMA1_Stream3_IRQHandler(void)//USART3_TX DMA
{
	if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3) != RESET) //USART3_TX
	{
		DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
		USART_ITConfig(USART3,USART_IT_TC,ENABLE);  //发送完成中断
		DMA_Cmd(DMA1_Stream3,DISABLE); //关闭传输
	}
}
/**************************/
void DMA_USART6_Init(void)
{
	 	NVIC_InitTypeDef NVIC_InitStructure;
	 DMA_InitTypeDef  DMA_InitStructure;
	   /* Enable the DMA clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	 DMA_InitStructure.DMA_BufferSize =USART6_Buff_Len;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART6->DR)) ;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   /* Configure TX DMA */
   DMA_InitStructure.DMA_Channel = DMA_Channel_5 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)USART6_TX_Buff;
   DMA_Init(DMA2_Stream6,&DMA_InitStructure);
	 DMA_Cmd(DMA2_Stream6, DISABLE);
 /* Configure RX DMA */
	 DMA_DeInit(DMA2_Stream1);
   DMA_InitStructure.DMA_Channel = DMA_Channel_5 ;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
   DMA_InitStructure.DMA_Memory0BaseAddr =(u32)USART6_RX_Buff; 
   DMA_Init(DMA2_Stream1,&DMA_InitStructure);
	 DMA_Cmd(DMA2_Stream1,ENABLE);
	 /*  interrupt  */
	  DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);//传输完成中断  TX
		
	 NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	 
	 /*Enable USART to DMA Request */
   USART_DMACmd(USART6, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);   
   USART_Cmd(USART6,ENABLE);   
}
void DMA2_Stream6_IRQHandler(void)//USART6_TX DMA
{
	if(DMA_GetITStatus(DMA2_Stream6,DMA_IT_TCIF6) != RESET) //USART3_TX
	{
		DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
		USART_ITConfig(USART6,USART_IT_TC,ENABLE);  //发送完成中断
		DMA_Cmd(DMA2_Stream6,DISABLE); //关闭传输
	}
}
