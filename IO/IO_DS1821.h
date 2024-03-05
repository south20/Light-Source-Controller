// IO_DS1821.h

#ifndef IO_DS1821_H
#define IO_DS1821_H

#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "sys.h"


#define DS18B21_IO_IN()  {GPIOE->MODER&=~(3<<(2*2));GPIOE->MODER|=0<<2*2;}	//PB12输入模式
#define DS18B21_IO_OUT() {GPIOE->MODER&=~(3<<(2*2));GPIOE->MODER|=1<<2*2;} 	//PB12输出模式
 
//IO操作函数											   
#define	DS18B21_DQ_OUT(n) (n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)) //数据端口	PB12
#define	DS18B21_DQ_IN  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) //数据端口	PB12
   
u8 IO_DS1821_Init(void);			//初始化DS18B21
short DS18B21_Get_Temp(void);	//获取温度
int  DS18B21_Get_Time(void);	//获取汞灯使用寿命
int  DS18B21_Set_Time(int time);	//设置汞灯使用寿命
void DS18B21_Start(void);		//开始温度转换
void DS18B21_Write_Byte(u8 dat);//写入一个字节
u8 DS18B21_Read_Byte(void);		//读出一个字节
u8 DS18B21_Read_Bit(void);		//读出一个位
u8 DS18B21_Check(void);			//检测是否存在DS18B21
void DS18B21_Rst(void);			//复位DS18B21 
#endif
