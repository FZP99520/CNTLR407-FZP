#include "main.h"
#include "adc.h"
#include "mpu9250.h"
#include "ili9488.h"
#include "key.h"
#include "show.h"
#include "os.h"
#include "task.h"
#include "input_ctrl.h"
u8 buff[10]={0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA};
static u16 row=0,col=0;
int main()
{
	unsigned int i=0;
	u8 delay=0;
	SystemClock_Init();//设置时钟
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组
	Led_Init();
	Beep_Init();
	LCD_Init();
	LCD_DisplayString(110,120,"Designed by Feng Zipeng");
	LCD_DisplayString(110,140,"Waiting To Start Up......");
  Delay_ms(500);//延迟3秒启动
	Delay_ms(500);
	LCD_Clear();
	LCD_DisplayString(0,0,"ADC Initialzing......");
	Adc_Init();  //ADC初始化
	LCD_DisplayString(280,0,"OK");
	NRF24L01_Init();//射频模块初始化
	LCD_DisplayString(0,20,"NRF24L01 Initialzing......");
	if( NRF24L01_Check()==1 )
	{
		EXTI0_Init();
		NRF24L01_TX_Mode();
		LCD_DisplayString(280,20,"OK");
		LED1_ON;
	}
	else 
		LCD_DisplayString(280,20,"ERROR!");
	#if 0
	USART1_Init(115200);
	DMA_USART1_Init();
  USART2_Init(115200);
	DMA_USART2_Init();
	USART3_Init(115200);
 	DMA_USART3_Init();
	USART6_Init(115200);
 	DMA_USART6_Init();
	USART_CLR_ALL_BUFF();
	#endif
	LCD_DisplayString(0,40,"Acquiring Throttle Offset Voltage......");
	for(i=0;i<500;i++)
	{
		GetThro_Data();//保证读取的电压值在正常范围
		Delay_ms(4);
	}
	while(GetThro_Offset()!=1)//利用2秒时间获取偏离值
	{
		Delay_ms(5);
	}
	Beep_times(2);
	
	
	LCD_DisplayString(0,120,"All Modules is initialzed !");
	LCD_DisplayString(0,140,"Ready to start up......");
	Delay_ms(500);
	Delay_ms(500);
  Page_Next=1;
//	TIM2_Int_Init();//主要事件处理 5ms中断
  TIM3_Int_Init();//T=5ms Only Use TIM3 
//	TIM4_Int_Init();
	
	while(1)
	{
     
	}
}


