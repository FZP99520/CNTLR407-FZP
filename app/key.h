#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h"
#include "input_ctrl.h"
#define  KEY_MAX_CNT 100 //按下确认时长

#define LEFT_Left_STA    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)	
#define LEFT_Down_STA    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)		
#define LEFT_Right_STA   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)
#define LEFT_Up_STA      GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)

#define RIGHT_Left_STA   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)	
#define RIGHT_Down_STA   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)		
#define RIGHT_Right_STA  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define RIGHT_Up_STA     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)

#define LEFT_Upper_STA   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)
#define RIGHT_Upper_STA  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)

void KEY_Init(void);//IO初始化
void KEY_Scan(Input_TypeDef* input);


#endif

