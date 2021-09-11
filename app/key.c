
#include  "key.h"

//������ʼ������
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5; //KEY ��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//�Ѿ������������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE
	
} 

void KEY_Scan(Input_TypeDef* k)
{
	if(LEFT_Left_STA==0) 
	{
		k->LEFT_Left=1;//1 ��ʾ�������£�0 ��ʾû�а���
		k->LEFT_Left_cnt++;//��¼�����ӳ�
	}
	else 
	{
		k->LEFT_Left=0;
		k->LEFT_Left_cnt=0;
	}
	if(LEFT_Right_STA==0) 
	{
		k->LEFT_Right=1;
		k->LEFT_Right_cnt++;
	}
	else 
	{
		k->LEFT_Right=0;
		k->LEFT_Right_cnt=0;
	}
	if(LEFT_Up_STA==0) 
	{
		k->LEFT_Up=1;
		k->LEFT_Up_cnt++;
	}
	else 
	{
		k->LEFT_Up=0;
		k->LEFT_Up_cnt=0;
	}
	if(LEFT_Down_STA==0)
	{		
		k->LEFT_Down=1;
		k->LEFT_Down_cnt++;
	}
	else 
	{
		k->LEFT_Down=0;
		k->LEFT_Down_cnt=0;
	}
	if(LEFT_Upper_STA==0) 
	{
		k->LEFT_Upper=1;
		k->LEFT_Upper_cnt++;
	}
	else 
	{
		k->LEFT_Upper=0;
		k->LEFT_Upper_cnt=0;
	}
	if(RIGHT_Left_STA==0)
	{
		k->RIGHT_Left=1;
		k->RIGHT_Left_cnt++;
	}
	else 
	{
		k->RIGHT_Left=0;
		k->RIGHT_Left_cnt=0;
	}
	if(RIGHT_Right_STA==0)
	{
		k->RIGHT_Right=1;
		k->RIGHT_Right_cnt++;
	}
	else
	{
		k->RIGHT_Right=0;
		k->RIGHT_Right_cnt=0;
	}
	if(RIGHT_Up_STA==0) 
	{
		k->RIGHT_Up=1;
		k->RIGHT_Up_cnt++;
	}
	else 
	{
		k->RIGHT_Up=0;
		k->RIGHT_Up_cnt=0;
	}
	if(RIGHT_Down_STA==0) 
	{
		k->RIGHT_Down=1;
		k->RIGHT_Down_cnt++;
	}
	else 
	{
		k->RIGHT_Down=0;
		k->RIGHT_Down_cnt=0;
	}
	if(RIGHT_Upper_STA==0) 
	{
		k->RIGHT_Upper=1;
		k->RIGHT_Upper_cnt++;
	}
	else 
	{
		k->RIGHT_Upper=0;
		k->RIGHT_Upper_cnt=0;
	}
}
