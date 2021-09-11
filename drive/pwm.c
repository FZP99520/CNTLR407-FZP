#include "pwm.h"
void TIM1_PWM_Init(void)
{
//�˲������ֶ��޸�IO������
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//TIM1ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��PORTAʱ�� 

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);//����GPIOE_Pin9ΪTIM1_Ch1,   
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);//����GPIOE_Pin11ΪTIM1_Ch2,
          
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;           //GPIO  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //�ٶ�100MHz  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //����  
    GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��P  
      
    TIM_TimeBaseStructure.TIM_Prescaler=168-1;  //��ʱ����Ƶ  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ  
    TIM_TimeBaseStructure.TIM_Period=1000-1;   //�Զ���װ��ֵ  
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//Ĭ�Ͼ�Ϊ0  
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1  
    //��ʼ��TIM1  PWMģʽ       
    //PWM ģʽ 1�C�C �ڵ�������ģʽ�£�ֻҪ TIMx_CNT<TIMx_CCR1��ͨ�� 1 ��Ϊ��Ч״̬������Ϊ��Ч״̬���ڵݼ�����ģʽ��.   
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM1Ϊ����ռ�ձ�ģʽ��PWM2Ϊ������ģʽ  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�,��Ч��ƽΪ�͵�ƽ  
  
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//�ڿ���ʱ�����,��������ÿ��Ըı�TIM_OCPolarity ���û��䣬��1ͨ��������  
      
    TIM_OCInitStructure.TIM_Pulse = 0; //����ͨ��1 CCR1��ռ�ձ���ֵ��  
    TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1��ʼ��  
      
    TIM_OCInitStructure.TIM_Pulse = 0; 	
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);//ͨ��2
      
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���,CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�  
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ��   
    TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1  
    TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��TIM1��PWM�����TIM1��TIM8��Ч,���û�����л�����                  
}  
