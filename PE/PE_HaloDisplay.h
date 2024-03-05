/********

 
显示环的灯光显示
*******/

#include "IO_GPIO.h"

//bitstatus设置IO_GPIO_PIN_REST灯打开
//bitstatus设置IO_GPIO_PIN_SET灯关

typedef enum
{
	Green, //绿
	Red,   //红
	Blue,  //蓝
	Purple,//紫
	Yellow, //黄
	Close //黄
} HaloDisplay_Colour; 

int PE_HaloDisplay_Green(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Red(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Blue(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Purple(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Yellow(IO_GPIO_Pin_State bitstatus);

//此处封装了所有光环灯颜色，直接调用既可
int PE_HaloDisplay_Colour(HaloDisplay_Colour colour);

int PE_HaloDisplay_Close();











