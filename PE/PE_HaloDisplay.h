/********

 
��ʾ���ĵƹ���ʾ
*******/

#include "IO_GPIO.h"

//bitstatus����IO_GPIO_PIN_REST�ƴ�
//bitstatus����IO_GPIO_PIN_SET�ƹ�

typedef enum
{
	Green, //��
	Red,   //��
	Blue,  //��
	Purple,//��
	Yellow, //��
	Close //��
} HaloDisplay_Colour; 

int PE_HaloDisplay_Green(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Red(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Blue(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Purple(IO_GPIO_Pin_State bitstatus);

int PE_HaloDisplay_Yellow(IO_GPIO_Pin_State bitstatus);

//�˴���װ�����й⻷����ɫ��ֱ�ӵ��üȿ�
int PE_HaloDisplay_Colour(HaloDisplay_Colour colour);

int PE_HaloDisplay_Close();











