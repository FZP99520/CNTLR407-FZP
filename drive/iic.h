#ifndef __I2C_H
#define __I2C_H
#include "stm32f4xx.h"
#include "systick.h"
#define IIC_GPIO GPIOB
#define IIC_SCL GPIO_Pin_8
#define IIC_SDA GPIO_Pin_9

void SDA_OUT(void);
void SDA_IN(void);
//IO操作函数	 
#define IIC_SCL_H     GPIO_SetBits(IIC_GPIO,IIC_SCL)//SCL
#define IIC_SCL_L     GPIO_ResetBits(IIC_GPIO,IIC_SCL)//SCL

#define IIC_SDA_H     GPIO_SetBits(IIC_GPIO,IIC_SDA)//SDA
#define IIC_SDA_L     GPIO_ResetBits(IIC_GPIO,IIC_SDA)//SDA

#define READ_SDA   GPIO_ReadInputDataBit(IIC_GPIO,IIC_SDA)  //输入SDA 

  //IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
void iic_delay(u8 i);
#endif
