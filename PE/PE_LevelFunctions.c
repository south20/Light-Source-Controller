#include "PE_LevelFunctions.h"

//ȫ�ֱ�������λֵ 0-100
int gear_value = 0;

int PE_Get_Level(int* level)
{
	int rtn;
	
	*level = gear_value;


	return rtn;


}

int PE_Add_Level(int level)
{

	int rtn;
	int cur_leve;
	//��ȡ��ǰ��λ�����С��100
	PE_Get_Level(&cur_leve);
	
    if(cur_leve < 100)
    {
		//rtn = PE_move_position(cur_leve,level);
    }


	//����������������Ƿ�仯40
	//rtn = IO_GPIO_Read_Pin(IO_GPIO_ENC_A,);

	gear_value += level;

	return rtn;


}
int PE_Reduce_Level(int level)
{

	int rtn;
	int cur_leve;
	//��ȡ��ǰ��λ�����С��100
	PE_Get_Level(&cur_leve);
	
    if(cur_leve > 0)
    {
		//rtn = PE_move_position(cur_leve,level);
    }


	//����������������Ƿ�仯40
	//rtn = IO_GPIO_Read_Pin(IO_GPIO_ENC_A,);

	gear_value -= level;

	return rtn;


}










