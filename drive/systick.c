#include "systick.h"
void Delay_ms(u32 nms)  
{                   
  u32 temp;  
  SysTick->CTRL=0x01 ;	
  SysTick->LOAD=(u32)nms*21000;  
  SysTick->VAL =0x00;    
  do  
  {  
    temp=SysTick->CTRL;  
  }while((temp&0x01)&&!(temp&(1<<16)));  
  SysTick->CTRL=0x00;  
  SysTick->VAL =0X00;              
}   
void Delay_us(u32 nus)  
{  
  u32 temp;  
  SysTick->CTRL=0x01; 
  SysTick->LOAD=nus*21;            
  SysTick->VAL=0x00;       
  do  
  {  
    temp=SysTick->CTRL;  
  }while((temp&0x01)&&!(temp&(1<<16)));  
  SysTick->CTRL=0x00;  
  SysTick->VAL =0X00;    
}
