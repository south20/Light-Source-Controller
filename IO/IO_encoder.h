#ifndef IO_ENCODER_H
#define IO_ENCODER_H
#include "sys.h"

#define PULSE_PER_REV 4096
#define FACTOR				0.087890625		//编码器1个count对应的角度值

/*编码器的定时器相关宏定义*/
/*通用定时器5*/
/*硬件分配为TIM5，修改外设无效*/
#define ENCODER_TIM_CHANNEL								TIM_CHANNEL_1 | TIM_CHANNEL_2
#define ENCODER_TIM_CH_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0) 
#define ENCODER_TIM_CH_CLK_ENABLE()       do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)   
#define ENCODER_TIM_INT_IRQHandler        TIM5_IRQHandler


/*
功能：编码器定时器初始化
输入：
		arr: auto reload value
		psc: prescaler
输出：无
*/
void IO_Encoder_Tim_Init(uint16_t arr, uint16_t psc);

/*
功能：编码器开始计数
输入：无
输出：无
*/
void IO_Encoder_Start_Count(void);

/*
功能：编码器停止计数
输入：无
输出：无
*/
void IO_Encoder_Stop_Count(void);

/*
功能：获取编码器当前相对计数值
输入：无
输出：编码器相对计数值
*/
int32_t  IO_Encoder_Get_Count_Val(void);  

//计算实际count读数值
int32_t  IO_Encoder_Get_Only_Count_Val(void);  

/*
功能：获取编码器当前相对位置
输入：编码器相对计数值
输出：编码器相对位置(单位：degree)
*/
double IO_Encoder_Get_Rel_Pos();

//计算实际角度数值
double IO_Encoder_Get_Only_Rel_Pos();

/*
功能：清除编码器当前计数值
输入：无
输出：无
*/
void IO_Encoder_Clear_Count_Val(void);

/*
功能：获取编码器当前已经旋转过的圈数，不足一圈的部分不计
输入：无
输出：圈数
*/
int16_t IO_Encoder_Get_Rev(void);

extern TIM_HandleTypeDef  htim_enc;  //编码器定时器句柄


#endif

