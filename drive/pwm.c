#include "pwm.h"
void TIM1_PWM_Init(void)
{
//此部分需手动修改IO口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//TIM1时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能PORTA时钟 

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);//复用GPIOE_Pin9为TIM1_Ch1,   
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);//复用GPIOE_Pin11为TIM1_Ch2,
          
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;           //GPIO  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //速度100MHz  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //上拉  
    GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化P  
      
    TIM_TimeBaseStructure.TIM_Prescaler=168-1;  //定时器分频  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式  
    TIM_TimeBaseStructure.TIM_Period=1000-1;   //自动重装载值  
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//默认就为0  
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1  
    //初始化TIM1  PWM模式       
    //PWM 模式 1–– 在递增计数模式下，只要 TIMx_CNT<TIMx_CCR1，通道 1 便为有效状态，否则为无效状态。在递减计数模式下.   
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM1为正常占空比模式，PWM2为反极性模式  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低,有效电平为低电平  
  
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//在空闲时输出低,这里的设置可以改变TIM_OCPolarity 如果没这句，第1通道有问题  
      
    TIM_OCInitStructure.TIM_Pulse = 0; //输入通道1 CCR1（占空比数值）  
    TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1初始化  
      
    TIM_OCInitStructure.TIM_Pulse = 0; 	
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);//通道2
      
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器,CCR自动装载默认也是打开的  
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能   
    TIM_Cmd(TIM1, ENABLE);  //使能TIM1  
    TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能TIM1的PWM输出，TIM1与TIM8有效,如果没有这行会问题                  
}  
