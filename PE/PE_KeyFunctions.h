/******



******/
#ifndef PE_KEY_FUNCTION_H
#define PE_KEY_FUNCTION_H

#include "IO_GPIO.h"

int PE_Key_Up(IO_GPIO_Pin_State    bitstatus);

int PE_Key_Down(IO_GPIO_Pin_State bitstatus);

//按键按下，此处是1有效，参数设置IO_GPIO_PIN_SET则为按键按下
//不区分打开和关闭（打开和关闭会自动读状态判断）
int PE_Start_Stop(void);


#endif //PE_KEY_FUNCTION_H

























