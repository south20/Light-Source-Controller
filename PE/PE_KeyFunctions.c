#include "PE_KeyFunctions.h"
#include "PE_LevelFunctions.h"
#include "PE_Get_GPIO_Sensor.h"
#include "PE_Motor.h"
#include "PE_HaloDisplay.h"



//bitstatus����ΪIO_GPIO_PIN_SETΪ����Ч

int PE_Key_Up(IO_GPIO_Pin_State    bitstatus)
{

	int rtn = 0;


	//rtn = IO_GPIO_Write_Pin(IO_GPIO_UP, bitstatus);

	//��λ����1�������ת
	rtn = PE_Add_Level(1);

	return rtn;
}


int PE_Key_Down(IO_GPIO_Pin_State    bitstatus)
{

	int rtn = 0;


	//rtn = IO_GPIO_Write_Pin(IO_GPIO_DOWN, bitstatus);

	//��λ����1�������ת
	rtn = PE_Reduce_Level(1);
	
	return rtn;
}



int PE_Start_Stop()
{

	int rtn = 0;
	IO_GPIO_Pin_State close = IO_GPIO_PIN_RESET;
	IO_GPIO_Pin_State open = IO_GPIO_PIN_RESET;

	//START������
	//rtn = IO_GPIO_Read_Pin(IO_GPIO_START, bitstatus);

	rtn = PE_Get_GPIO_ShuterCloseSensor(&close);
	rtn = PE_Get_GPIO_ShuterOpenSensor(&open);


	//���Ϊ��ʱΪ����״̬��Ҳ����IO_GPIO_SHU_CLOSE���Ŵ�����������
	if(close == IO_GPIO_PIN_RESET && open == IO_GPIO_PIN_SET)
	{

		//������ش�
		PE_Shutter_Open();
		rtn = PE_Get_GPIO_ShuterOpenSensor(&open);
		if(close==IO_GPIO_PIN_RESET)
		{
			//PE_Shutter_Mode();
			//SHUTTER������֤�ź�д1
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_VERIFY,IO_GPIO_PIN_SET);
		}

	}
	//���Ϊ��ʱΪ����״̬
	else if(close == IO_GPIO_PIN_SET && open == IO_GPIO_PIN_RESET)
	{
		
		//������ع�
		PE_Shutter_Close();
		rtn = PE_Get_GPIO_ShuterCloseSensor(&close);
		if(close==IO_GPIO_PIN_RESET)
		{
			//PE_Shutter_Mode();
			//SHUTTER������֤�ź�д0
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_VERIFY,IO_GPIO_PIN_RESET);
			
			rtn = PE_HaloDisplay_Colour(Blue);
		}

	}
	else
	{
 		//������ع�
		PE_Shutter_Close();
		rtn = PE_Get_GPIO_ShuterCloseSensor(&close);
		if(close==IO_GPIO_PIN_RESET)
		{
			//PE_Shutter_Mode();
			//SHUTTER������֤�ź�д0
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_VERIFY,IO_GPIO_PIN_RESET);
			rtn = PE_HaloDisplay_Colour(Green);
		}
	}

	return rtn;
}





















