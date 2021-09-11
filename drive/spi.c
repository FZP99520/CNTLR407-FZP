#include "spi.h"
void SPI1_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		SPI_InitTypeDef SPI_InitStructure;
		//����SPIʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		//��ʼ��SPI IO 
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		          
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�����������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);//����ΪSPI1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	
	  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);
		SPI_Cmd(SPI1, DISABLE);
		//SPIģʽ����
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//8Mbps
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI1, &SPI_InitStructure);
		//ʹ��SPI
		SPI_Cmd(SPI1, ENABLE);
		
		SPI1_ReadWriteByte(0xff);
}

u8 SPI1_ReadWriteByte(u8 dat)		 //SPI1��дһ���ֽ�
{
  u8 t,data;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)	//�ȴ����ͻ�������
	{
		t++;
		if(t>=200)return 0;	//��ʱ���ش����־	
	}
	SPI_I2S_SendData(SPI1,dat); //��������
	t=0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)	//�ȴ�����
	{
		t++;
		if(t>=200)return 0;	//��ʱ���ش����־	
	}
	data=SPI_I2S_ReceiveData(SPI1);
	return data; //�������SPI1���յ�����			
}

void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)	//����SPI2���ٶ�
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	
	SPI1->CR1&=0XFFC7;		 //�޸�BR[2:0]ֵ
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI1,ENABLE);			
}