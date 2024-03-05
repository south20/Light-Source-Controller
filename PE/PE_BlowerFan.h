
#ifndef PE_BLOWER_FAN_H
#define PE_BLOWER_FAN_H

#include "IO_GPIO.h" // 引入GPIO初始化
#include "IO_DS1821.h" // 引入温度检测函数


//风扇tim9的通道2转速检测初始化
void CoolingFan_TIM9_CH2_Cap_Init(); 

int PE_CoolingFan_CheckSpeed(int* fan_rpm);


// 鼓风机相关引脚初始化
void PE_BlowerFan_Init();

//鼓风机转速控制
int PE_BlowerFan_Control(uint32_t cycle);

//鼓风机转速检测初始化
void BlowerFan_TIM9_CH1_Cap_Init();

//鼓风机转速检测
int PE_BlowerFan_CheckSpeed(int* fan_rpm);


#endif // PE_BLOWER_FAN_H
