#include "24cxx.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//24CXX��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
 	#define PAGE_NUM			128						//??
	#define PAGE_SIZE			32						//????(??)
	#define CAPACITY_SIZE		(PAGE_NUM * PAGE_SIZE)	//???(??)
	#define ADDR_BYTE_NUM		2	

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	IIC_Init();//IIC��ʼ��
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
void  AT24CXX_ReadPage(u16 u16Addr, uint8_t u8Len, uint8_t *pBuff)
{
	uint8_t i;	
	unsigned char ack;
	IIC_Start();//��ʼ�ź�	
	IIC_Send_Byte(DEV_ADDR | WRITE_CMD);//����Ѱַ+��/дѡ��
	IIC_Wait_Ack();//�ȴ�Ӧ��
	IIC_Send_Byte((uint8_t)((u16Addr >> 8) & 0xFF));
	IIC_Wait_Ack();//�ȴ�Ӧ��
	IIC_Send_Byte((uint8_t)(u16Addr & 0xFF));
	IIC_Wait_Ack();//�ȴ�Ӧ��
	IIC_Start();//��ʼ�ź�
	IIC_Send_Byte(DEV_ADDR | READ_CMD);//����Ѱַ+��
	IIC_Wait_Ack();//�ȴ�Ӧ��
	if (u8Len > PAGE_SIZE)//���ȴ���ҳ�ĳ���
	{
		u8Len = PAGE_SIZE;
	}
	if ((u16Addr + (uint16_t)u8Len) > CAPACITY_SIZE)//��������
	{
		u8Len = (uint8_t)(CAPACITY_SIZE - u16Addr);
	}
	if (((u16Addr % PAGE_SIZE) + (uint16_t)u8Len) > PAGE_SIZE)//�ж��Ƿ��ҳ
	{
		u8Len -= (uint8_t)((u16Addr + (uint16_t)u8Len) % PAGE_SIZE);//��ҳ���ص���ҳ�Ĳ���
	}
		printf("u8Len:%d \r\n", u8Len);
	AT24CXX_ReadOneByte(0 + u16Addr);
		IIC_Ack();//����Ӧ��
	for (i = 0; i < (u8Len - 1); i++)
	{
		*(pBuff + i) = AT24CXX_ReadOneByte(i + u16Addr);
			printf("*(pBuff + i): %c\r\n",*(pBuff + i));
		IIC_Ack();//����Ӧ��
	}
	*(pBuff + u8Len - 1) = '\0';
	IIC_NAck();//���һ����Ӧ��
	IIC_Stop();//ֹͣ�ź�
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
void AT24CXX_WritePage(u16 WriteAddr,u8 DataToWrite, u8 *pData)
{		
    u8 i;	
    IIC_Start();  
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte((uint8_t)((WriteAddr >> 8) & 0xFF));//���͸ߵ�ַ	  
	IIC_Wait_Ack();	   
    IIC_Send_Byte((uint8_t)(WriteAddr & 0xFF));   //���͵͵�ַ
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
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
