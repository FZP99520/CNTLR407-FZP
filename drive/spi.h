#ifndef _spi_H
#define _spi_H
#include "stm32f4xx.h"


void SPI1_Init(void);  //SPI��ʼ��
u8 SPI1_ReadWriteByte(u8 dat);		 //SPI2��дһ���ֽ�
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);	//����SPI2���ٶ�


#endif
