#ifndef _PWM_H
#define _PWM_H
#include "stm32f4xx.h"

#define PWM1 TIM1->CCR1
#define PWM2 TIM1->CCR2
void TIM1_PWM_Init(void);

#endif
