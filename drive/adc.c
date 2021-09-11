#include "adc.h"
#include "systick.h"
#include "RC.h"
#include "show.h"
MoveAvarageFilter_TypeDef F_ADC_LEFT_X={10,0,0,{0}};
MoveAvarageFilter_TypeDef F_ADC_LEFT_Y={10,0,0,{0}};
MoveAvarageFilter_TypeDef F_ADC_RIGHT_X={10,0,0,{0}};
MoveAvarageFilter_TypeDef F_ADC_RIGHT_Y={10,0,0,{0}};
static float MoveAvarageFilter(MoveAvarageFilter_TypeDef* filter,float data);
void Adc_Init(void)
{
  GPIO_InitTypeDef      GPIO_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure;
               
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
 
  //先初始化ADC1通道1 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PA1 通道1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
 
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);                 //ADC1复位
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);               //复位结束
 
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay =ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
 
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描模式        
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge =ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;//右对齐   
  ADC_InitStructure.ADC_NbrOfConversion = 4;//1个转换在规则序列中 也就是只转换规则序列1
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
 
  ADC_Cmd(ADC1, ENABLE);//开启AD转换器 
}
 
//获得ADC值
//ch: @ref ADC_channels
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
static u16 GET_ADC(u8 ch)  
{
  ADC_RegularChannelConfig(ADC1,ch,1, ADC_SampleTime_480Cycles );//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度
  ADC_SoftwareStartConv(ADC1);  //使能指定的ADC1的软件转换启动功能        
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
  return ADC_GetConversionValue(ADC1);//返回最近一次ADC1规则组的转换结果
}
#define Thro_MAX 1999u
#define Thro_MIN 1001u
void GetThro_Data(void)
{
	INPUT.ADC_LEFT_X=GET_ADC(4);
	INPUT.ADC_LEFT_Y=GET_ADC(5);
	INPUT.ADC_RIGHT_Y=GET_ADC(6);
	INPUT.ADC_RIGHT_X=GET_ADC(7);
	//对读取到的ADC值做滑动平均滤波
	INPUT.ADC_LEFT_X=MoveAvarageFilter(&F_ADC_LEFT_X,INPUT.ADC_LEFT_X);
	INPUT.ADC_LEFT_Y=MoveAvarageFilter(&F_ADC_LEFT_Y,INPUT.ADC_LEFT_Y);
	INPUT.ADC_RIGHT_X=MoveAvarageFilter(&F_ADC_RIGHT_X,INPUT.ADC_RIGHT_X);
	INPUT.ADC_RIGHT_Y=MoveAvarageFilter(&F_ADC_RIGHT_Y,INPUT.ADC_RIGHT_Y);
	
	INPUT.Volt_LEFT_X=(float)(INPUT.ADC_LEFT_X)*3.3f/4096;
	INPUT.Volt_LEFT_Y=(float)(INPUT.ADC_LEFT_Y)*3.3f/4096;
	INPUT.Volt_RIGHT_X=(float)(INPUT.ADC_RIGHT_X)*3.3f/4096;
	INPUT.Volt_RIGHT_Y=(float)(INPUT.ADC_RIGHT_Y)*3.3f/4096;
	
	if(INPUT.Volt_Offset_sta==1)//校准完成再计算油门量
	{
		INPUT.Thro_Yaw=(INPUT.Volt_LEFT_X-INPUT.Volt_LEFT_X_Offset)*300.0f + 1500u;
		if(LEFT_Up_STA==0) INPUT.Thro_High=1;
		else if(LEFT_Down_STA==0) INPUT.Thro_High=-1;
		else INPUT.Thro_High=0;
		INPUT.Thro_High=(INPUT.Volt_LEFT_Y-INPUT.Volt_LEFT_Y_Offset)*300.0f + 1500u;
		INPUT.Thro_Pitch=(INPUT.Volt_RIGHT_Y-INPUT.Volt_RIGHT_Y_Offset)*300.0f + 1500u;
		INPUT.Thro_Roll=(INPUT.Volt_RIGHT_X-INPUT.Volt_RIGHT_X_Offset)*300.0f + 1500u;
		
		INPUT.Thro_High = INPUT.Thro_High>Thro_MAX ? Thro_MAX:INPUT.Thro_High;
		INPUT.Thro_High = INPUT.Thro_High<Thro_MIN ? Thro_MIN:INPUT.Thro_High;
		INPUT.Thro_Yaw = INPUT.Thro_Yaw>Thro_MAX ? Thro_MAX:INPUT.Thro_Yaw;
		INPUT.Thro_Yaw = INPUT.Thro_Yaw<Thro_MIN ? Thro_MIN:INPUT.Thro_Yaw;
		INPUT.Thro_Roll = INPUT.Thro_Roll>Thro_MAX ? Thro_MAX:INPUT.Thro_Roll;
		INPUT.Thro_Roll = INPUT.Thro_Roll<Thro_MIN ? Thro_MIN:INPUT.Thro_Roll;
		INPUT.Thro_Pitch = INPUT.Thro_Pitch>Thro_MAX ? Thro_MAX:INPUT.Thro_Pitch;
		INPUT.Thro_Pitch = INPUT.Thro_Pitch<Thro_MIN ? Thro_MIN:INPUT.Thro_Pitch;
		if(Page_Now==1)Show_Throttle_Update(INPUT);      //更新油门控制数据
	}
	
}
u8 GetThro_Offset(void)
{
	static u16 cnt=0;
	static float vol[4]={0,0,0,0};
	GetThro_Data();
	cnt++;
	  if(cnt>=OffsetNum)
		{
			INPUT.Volt_LEFT_X_Offset =  vol[0]/cnt;
			//INPUT.Volt_LEFT_Y_Offset =  vol[1]/cnt;
			INPUT.Volt_LEFT_Y_Offset =  vol[1]/cnt;
			INPUT.Volt_RIGHT_X_Offset = vol[2]/cnt;
			INPUT.Volt_RIGHT_Y_Offset = vol[3]/cnt;
			vol[0] = 0;
		  vol[1] = 0;
		  vol[2] = 0;
		  vol[3] = 0;
			INPUT.Volt_Offset_sta=1;
			return 1;//校准完成
		}
		else
		{
		 vol[0] += INPUT.Volt_LEFT_X;
		 vol[1] += INPUT.Volt_LEFT_Y;
		 vol[2] += INPUT.Volt_RIGHT_X;
		 vol[3] += INPUT.Volt_RIGHT_Y;
			return 0;
		}
}
float MoveAvarageFilter(MoveAvarageFilter_TypeDef* filter,float data)
{
    if((filter->Num_Wind>Max_Size_Wind)||(0==filter->Num_Wind))
    {
        filter->Num_Wind=Max_Size_Wind;
    }
    if(filter->Index>=filter->Num_Wind)
    {
        filter->Index=0;
    }   
	    filter->Sum-=filter->Wind[filter->Index];
	    filter->Wind[filter->Index]=data;
	    filter->Sum+=filter->Wind[filter->Index];
	    filter->Index++;

    return filter->Sum/filter->Num_Wind;  
}
