#include "iic.h"
#include "systick.h"
/*******************************/
void IIC_Init(void)
{			
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PB端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	//端口配置
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //推挽输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
	IIC_SDA_H;
	IIC_SCL_H;
}
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//端口配置
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //推挽输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//端口配置
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD; //开漏输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_H;	
	IIC_SCL_H;
	iic_delay(10);//Delay_us(1);
 	IIC_SDA_L;//START:when CLK is high,DATA change form high to low 
	iic_delay(10);//Delay_us(1);
	IIC_SCL_L;//钳住I2C总线，准备发送或接收数据 
} 
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_L;
	IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
 	iic_delay(10);//Delay_us(1);
	IIC_SCL_H; 
	iic_delay(10);
	IIC_SDA_H;  //发送I2C总线结束信号
	//iic_delay(10);//Delay_us(1);							   	
}
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_H;	   
	IIC_SCL_H;
	iic_delay(10);//Delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>200)
		{
			IIC_Stop();
			return 0;
		}
	  iic_delay(10);//Delay_us(1);
	}
	IIC_SCL_L;//时钟输出0 	   
	return 1;  
} 
void IIC_Ack(void)
{
	IIC_SCL_L;
	SDA_OUT();
	IIC_SDA_L;
	iic_delay(10);//Delay_us(1);
	IIC_SCL_H;
	iic_delay(10);//Delay_us(1);
	IIC_SCL_L;
}   
void IIC_NAck(void)
{
	IIC_SCL_L;
	SDA_OUT();
	IIC_SDA_H;
	iic_delay(10);//Delay_us(1);
	IIC_SCL_H;
	iic_delay(10);//Delay_us(1);
	IIC_SCL_L;
} 
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			if(txd&0x80)IIC_SDA_H;
			else IIC_SDA_L;
      txd<<=1; 	  
		  iic_delay(20);
		  IIC_SCL_H;
		  iic_delay(20);
		  IIC_SCL_L;	
		  iic_delay(10);
    }	 
} 	 

u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
     IIC_SCL_L; 
	   iic_delay(20);
		 IIC_SCL_H;
     receive<<=1;
     if(READ_SDA)receive++; 
		 iic_delay(20);
   }					 
    if (ack)
        IIC_Ack(); //发送ACK 
    else
        IIC_NAck();//发送nACK  
    return receive;
}
void iic_delay(u8 i)
{
	u8 a;
	for(a=0;a<i;a++)
	{
		__nop();
	}
}

