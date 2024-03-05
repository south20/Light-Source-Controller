#include "PE_Light.h"
#include "IO_GPIO.h"


int PE_Open_Hg_Light()
{
    int rtn = 0;
	//1.判断Lamp_on_off状态

	//2.判断LP_FBK光能量反馈状态
	
	//3.判断sys_interlock状态
	
	//4.判断光导管是否插到位
	
	//5. 判断系统是否正常初始化
    rtn = IO_GPIO_Write_Pin(IO_GPIO_IGN_OUT, 0);
	return rtn;
}

int PE_Close_Hg_Light()
{
    int rtn = 0;
	//1.判断Lamp_on_off状态

	//2.判断LP_FBK光能量反馈状态
	
	//3.判断sys_interlock状态
	
	//4.判断光导管是否插到位
	
	//5. 判断系统是否正常初始化
    rtn = IO_GPIO_Write_Pin(IO_GPIO_IGN_OUT, 1);
	return rtn;
}

