#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"
extern char printf_buff[256];

#define USART1_Buff_Len 256
#define USART2_Buff_Len 256
#define USART3_Buff_Len 256
#define USART6_Buff_Len 256

__packed typedef struct
{
	u8 tx_ok;
	u8 rx_ok;
	u8 rx_len;
	u8 tx_len;
	u8 busy;
}USART_STA;
extern USART_STA USART1_STA;
extern USART_STA USART2_STA;
extern USART_STA USART3_STA;
extern USART_STA USART6_STA;


extern u8 USART1_TX_Buff[USART1_Buff_Len];
extern u8 USART1_RX_Buff[USART1_Buff_Len];

extern u8 USART2_TX_Buff[USART2_Buff_Len];
extern u8 USART2_RX_Buff[USART2_Buff_Len];

extern u8 USART3_TX_Buff[USART3_Buff_Len];
extern u8 USART3_RX_Buff[USART3_Buff_Len];

extern u8 USART6_TX_Buff[USART6_Buff_Len];
extern u8 USART6_RX_Buff[USART6_Buff_Len];

void USART1_Init(u32 baudrate);
void USART2_Init(u32 baudrate);
void USART3_Init(u32 baudrate);
void USART6_Init(u32 baudrate);

u8 USART1_SendData(u8 *data,u8 len);
u8 USART2_SendData(u8 *data,u8 len);
u8 USART3_SendData(u8 *data,u8 len);
u8 USART6_SendData(u8 *data,u8 len);

void USART_CLR_BUFF(u8 ch);
void USART_CLR_ALL_BUFF(void);



#endif
