#include "24cxx.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//24CXX驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
 	#define PAGE_NUM			128						//??
	#define PAGE_SIZE			32						//????(??)
	#define CAPACITY_SIZE		(PAGE_NUM * PAGE_SIZE)	//???(??)
	#define ADDR_BYTE_NUM		2	

//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();//IIC初始化
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
void  AT24CXX_ReadPage(u16 u16Addr, uint8_t u8Len, uint8_t *pBuff)
{
	uint8_t i;	
	unsigned char ack;
	IIC_Start();//起始信号	
	IIC_Send_Byte(DEV_ADDR | WRITE_CMD);//器件寻址+读/写选择
	IIC_Wait_Ack();//等待应答
	IIC_Send_Byte((uint8_t)((u16Addr >> 8) & 0xFF));
	IIC_Wait_Ack();//等待应答
	IIC_Send_Byte((uint8_t)(u16Addr & 0xFF));
	IIC_Wait_Ack();//等待应答
	IIC_Start();//起始信号
	IIC_Send_Byte(DEV_ADDR | READ_CMD);//器件寻址+读
	IIC_Wait_Ack();//等待应答
	if (u8Len > PAGE_SIZE)//长度大于页的长度
	{
		u8Len = PAGE_SIZE;
	}
	if ((u16Addr + (uint16_t)u8Len) > CAPACITY_SIZE)//超过容量
	{
		u8Len = (uint8_t)(CAPACITY_SIZE - u16Addr);
	}
	if (((u16Addr % PAGE_SIZE) + (uint16_t)u8Len) > PAGE_SIZE)//判断是否跨页
	{
		u8Len -= (uint8_t)((u16Addr + (uint16_t)u8Len) % PAGE_SIZE);//跨页，截掉跨页的部分
	}
		printf("u8Len:%d \r\n", u8Len);
	AT24CXX_ReadOneByte(0 + u16Addr);
		IIC_Ack();//主机应答
	for (i = 0; i < (u8Len - 1); i++)
	{
		*(pBuff + i) = AT24CXX_ReadOneByte(i + u16Addr);
			printf("*(pBuff + i): %c\r\n",*(pBuff + i));
		IIC_Ack();//主机应答
	}
	*(pBuff + u8Len - 1) = '\0';
	IIC_NAck();//最后一个不应答
	IIC_Stop();//停止信号
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
void AT24CXX_WritePage(u16 WriteAddr,u8 DataToWrite, u8 *pData)
{		
    u8 i;	
    IIC_Start();  
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte((uint8_t)((WriteAddr >> 8) & 0xFF));//发送高地址	  
	IIC_Wait_Ack();	   
    IIC_Send_Byte((uint8_t)(WriteAddr & 0xFF));   //发送低地址
	IIC_Wait_Ack(); 	 										  		    		
  if (DataToWrite > PAGE_SIZE)//????????
	{
		DataToWrite = PAGE_SIZE;
	}
	if ((WriteAddr + (uint16_t)DataToWrite) > CAPACITY_SIZE)//????
	{
		DataToWrite = (uint8_t)(CAPACITY_SIZE - WriteAddr);
	}
	if (((WriteAddr % PAGE_SIZE) + (uint16_t)DataToWrite) > PAGE_SIZE)//??????
	{
		DataToWrite -= (uint8_t)((WriteAddr + (uint16_t)DataToWrite) % PAGE_SIZE);//??,???????
	}
	for (i = 0; i < DataToWrite; i++)
	{
		IIC_Send_Byte(*(pData + i));
		IIC_Wait_Ack();//????
	}
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
