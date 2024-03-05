/*****


获取所有的传感器值


****/

#include "IO_GPIO.h"
#include <sys.h>


//外部给到的开关命令，中断读到此值去做处理，此函数只是读，需要在上层封装中断处理
int PE_Get_GPIO_ShuterClose(IO_GPIO_Pin_State* bitstatus);
//外部给到的开关命令
int PE_Get_GPIO_ShuterOpen(IO_GPIO_Pin_State * bitstatus);


//读取判断是否关到位，对应快门开关传感器
int PE_Get_GPIO_ShuterCloseSensor(IO_GPIO_Pin_State* bitstatus);
//判断是否开到位
int PE_Get_GPIO_ShuterOpenSensor(IO_GPIO_Pin_State * bitstatus);


//获取温度,输出值为temperature
int PE_Get_GPIO_TempertureSensor(uint16_t* temperature);

//时长记录，输出值为duration
int PE_Get_GPIO_DurationRecorderSensor(uint16_t* duration);

//光导管传感器是否存在
int PE_Get_GPIO_LightTubeDetectionSensor(IO_GPIO_Pin_State* bitstatus);

//获取光能量传感器，传入参数为采样次数，输出值为光能量
 int PE_Get_GPIO_LightEnergySensor(u8 times,uint16_t* LightEnergy);


//判断滤光片是否为SMEE预定种类，是返回1，否返回0
int PE_Is_SMEE_Filter();

















