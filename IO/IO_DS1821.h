// IO_DS1821.h

#ifndef IO_DS1821_H
#define IO_DS1821_H

#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "sys.h"


#define DS18B21_IO_IN()  {GPIOE->MODER&=~(3<<(2*2));GPIOE->MODER|=0<<2*2;}	//PB12����ģʽ
#define DS18B21_IO_OUT() {GPIOE->MODER&=~(3<<(2*2));GPIOE->MODER|=1<<2*2;} 	//PB12���ģʽ
 
//IO��������											   
#define	DS18B21_DQ_OUT(n) (n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)) //���ݶ˿�	PB12
#define	DS18B21_DQ_IN  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) //���ݶ˿�	PB12
   
u8 IO_DS1821_Init(void);			//��ʼ��DS18B21
short DS18B21_Get_Temp(void);	//��ȡ�¶�
int  DS18B21_Get_Time(void);	//��ȡ����ʹ������
int  DS18B21_Set_Time(int time);	//���ù���ʹ������
void DS18B21_Start(void);		//��ʼ�¶�ת��
void DS18B21_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B21_Read_Byte(void);		//����һ���ֽ�
u8 DS18B21_Read_Bit(void);		//����һ��λ
u8 DS18B21_Check(void);			//����Ƿ����DS18B21
void DS18B21_Rst(void);			//��λDS18B21 
#endif
