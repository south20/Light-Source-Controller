/******



******/
#ifndef PE_KEY_FUNCTION_H
#define PE_KEY_FUNCTION_H

#include "IO_GPIO.h"

int PE_Key_Up(IO_GPIO_Pin_State    bitstatus);

int PE_Key_Down(IO_GPIO_Pin_State bitstatus);

//�������£��˴���1��Ч����������IO_GPIO_PIN_SET��Ϊ��������
//�����ִ򿪺͹رգ��򿪺͹رջ��Զ���״̬�жϣ�
int PE_Start_Stop(void);


#endif //PE_KEY_FUNCTION_H

























