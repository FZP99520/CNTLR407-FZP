#ifndef _ADC_H
#define _ADC_H
#define OffsetNum 100
#include "stm32f4xx.h"
#include "input_ctrl.h"
#define Max_Size_Wind 100
typedef struct
{
    float Num_Wind;
    uint8_t Index;
    float Sum;
    float Wind[Max_Size_Wind];
}MoveAvarageFilter_TypeDef;//»¬¶¯Æ½¾ùÂË²¨

void Adc_Init(void);
void GetThro_Data(void);
u8 GetThro_Offset(void);


#endif
