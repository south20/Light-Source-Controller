#include "PE_Light.h"
#include "IO_GPIO.h"


int PE_Open_Hg_Light()
{
    int rtn = 0;
	//1.�ж�Lamp_on_off״̬

	//2.�ж�LP_FBK����������״̬
	
	//3.�ж�sys_interlock״̬
	
	//4.�жϹ⵼���Ƿ�嵽λ
	
	//5. �ж�ϵͳ�Ƿ�������ʼ��
    rtn = IO_GPIO_Write_Pin(IO_GPIO_IGN_OUT, 0);
	return rtn;
}

int PE_Close_Hg_Light()
{
    int rtn = 0;
	//1.�ж�Lamp_on_off״̬

	//2.�ж�LP_FBK����������״̬
	
	//3.�ж�sys_interlock״̬
	
	//4.�жϹ⵼���Ƿ�嵽λ
	
	//5. �ж�ϵͳ�Ƿ�������ʼ��
    rtn = IO_GPIO_Write_Pin(IO_GPIO_IGN_OUT, 1);
	return rtn;
}

