#ifndef _TIMER8_H
#define _TIMER8_H
#include "sys.h"

extern TIM_HandleTypeDef TIM1_Handler;
extern TIM_HandleTypeDef TIM8_Handler;
extern uint16_t WorkingHour;

void TIM1_Configuration(void);
void TIM8_Configuration(void);

uint16_t GetWorkingHours(void);


#endif /* __TIM_H__ */


