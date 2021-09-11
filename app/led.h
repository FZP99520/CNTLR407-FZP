#ifndef _led_H
#define _led_H

#include "stm32f4xx.h"

#define LED1_ON GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define LED1_OFF GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define LED1_Status GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)


#define LED2_ON GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define LED2_OFF GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define LED2_Status GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_4)
void LED_Flash(u8 times);
void Led_Init(void);
#endif
