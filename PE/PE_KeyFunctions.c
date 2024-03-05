#include "PE_KeyFunctions.h"
#include "PE_LevelFunctions.h"
#include "PE_Get_GPIO_Sensor.h"
#include "PE_Motor.h"
#include "PE_HaloDisplay.h"



//bitstatus设置为IO_GPIO_PIN_SET为有有效

int PE_Key_Up(IO_GPIO_Pin_State    bitstatus)
{

	int rtn = 0;


	//rtn = IO_GPIO_Write_Pin(IO_GPIO_UP, bitstatus);

	//挡位增加1，电机正转
	rtn = PE_Add_Level(1);

	return rtn;
}


int PE_Key_Down(IO_GPIO_Pin_State    bitstatus)
{

	int rtn = 0;


	//rtn = IO_GPIO_Write_Pin(IO_GPIO_DOWN, bitstatus);

	//挡位减少1，电机反转
	rtn = PE_Reduce_Level(1);
	
	return rtn;
}



int PE_Start_Stop()
{

	int rtn = 0;
	IO_GPIO_Pin_State close = IO_GPIO_PIN_RESET;
	IO_GPIO_Pin_State open = IO_GPIO_PIN_RESET;

	//START键按下
	//rtn = IO_GPIO_Read_Pin(IO_GPIO_START, bitstatus);

	rtn = PE_Get_GPIO_ShuterCloseSensor(&close);
	rtn = PE_Get_GPIO_ShuterOpenSensor(&open);


	//如果为此时为关门状态，也就是IO_GPIO_SHU_CLOSE关门传感器被触发
	if(close == IO_GPIO_PIN_RESET && open == IO_GPIO_PIN_SET)
	{

		//电机开关打开
		PE_Shutter_Open();
		rtn = PE_Get_GPIO_ShuterOpenSensor(&open);
		if(close==IO_GPIO_PIN_RESET)
		{
			//PE_Shutter_Mode();
			//SHUTTER快门验证信号写1
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_VERIFY,IO_GPIO_PIN_SET);
		}

	}
	//如果为此时为开门状态
	else if(close == IO_GPIO_PIN_SET && open == IO_GPIO_PIN_RESET)
	{
		
		//电机开关关
		PE_Shutter_Close();
		rtn = PE_Get_GPIO_ShuterCloseSensor(&close);
		if(close==IO_GPIO_PIN_RESET)
		{
			//PE_Shutter_Mode();
			//SHUTTER快门验证信号写0
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_VERIFY,IO_GPIO_PIN_RESET);
			
			rtn = PE_HaloDisplay_Colour(Blue);
		}

	}
	else
	{
 		//电机开关关
		PE_Shutter_Close();
		rtn = PE_Get_GPIO_ShuterCloseSensor(&close);
		if(close==IO_GPIO_PIN_RESET)
		{
			//PE_Shutter_Mode();
			//SHUTTER快门验证信号写0
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_VERIFY,IO_GPIO_PIN_RESET);
			rtn = PE_HaloDisplay_Colour(Green);
		}
	}

	return rtn;
}





















