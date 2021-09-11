#ifndef _systick_H
#define _systick_H

#include "stm32f4xx.h"

void Delay_init(u8 SYSCLK);  
void Delay_ms(u32 x);
void Delay_us(u32 x);



#endif
