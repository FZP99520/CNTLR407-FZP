#include "beep.h"
#include "systick.h"
void Beep_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStruct; //�ṹ���������
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;  //LED
	 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT; //�������ģʽ
	 GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	
	 GPIO_Init(GPIOC,&GPIO_InitStruct);  
	 GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
void beep_delay(u16 n)
{
	for(u16 i=0;i<n;i++) __nop();
}
void Beep_ms(u16 nms)
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    Delay_ms(nms);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
void Beep_times(u8 n)
{
    u8 i;
    for(i=0;i<n;i++)
    {
     Beep_ms(100);
     Delay_ms(100);
    }
}
void Beep(u8 times,u16 time_work,u16 time_stop)
{
   u8 i;
   for(i=0;i<times;i++)
   {
     GPIO_ResetBits(GPIOC,GPIO_Pin_13);
     Delay_ms(time_work);
     GPIO_SetBits(GPIOC,GPIO_Pin_13);
    Delay_ms(time_stop);
   }
}
