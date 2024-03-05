#include "canfestival_timer.h"
#include "stm32f7xx_hal_tim.h"
#include "stm32f7xx.h"
#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"
#include "IO_encoder.h"
static TIM_HandleTypeDef TIM_BaseStruct;


TIM_HandleTypeDef TIM14_Handler;      //定时器句柄 

//通用定时器14中断初始化
//TIMx:定时器型号
//TIM_Period：自动重装值。
//TIM_Prescaler：时钟预分频数
//这里使用的是定时器14!
void TIMConfig(TIM_TypeDef* TIMx, uint16_t TIM_Period, uint16_t TIM_Prescaler)
{
	  //定时器TIM14初始化,配置ARR,PSC
    TIM_BaseStruct.Instance = TIMx;
    TIM_BaseStruct.Init.Prescaler = TIM_Period - 1;     //设置用来作为TIMx时钟频率除数的预分频值
    TIM_BaseStruct.Init.CounterMode = TIM_COUNTERMODE_UP;//TIM向上计数模式
    TIM_BaseStruct.Init.Period = TIM_Prescaler;        //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
    TIM_BaseStruct.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&TIM_BaseStruct);

	__HAL_TIM_CLEAR_FLAG(&TIM_BaseStruct, TIM_FLAG_UPDATE);
    //开启定时器/中断
    HAL_TIM_Base_Start_IT(&TIM_BaseStruct);
	
}



void NVIC_Configuration(void)
{
   
	__HAL_RCC_TIM14_CLK_ENABLE();            //ê1?üTIM3ê±?ó
    NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0); // 璁剧疆涓柇浼樺厛绾т负0锛屾姠鍗犱紭鍏堢骇0锛屽瓙浼樺厛绾?
    NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);      // 浣胯兘涓柇

}


//定时器14中断服务程序
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{

    HAL_TIM_IRQHandler(&TIM_BaseStruct);

    if (__HAL_TIM_GET_FLAG(&TIM_BaseStruct, TIM_FLAG_UPDATE) != RESET) {
        __HAL_TIM_CLEAR_FLAG(&TIM_BaseStruct, TIM_FLAG_UPDATE); // // canfestival鐨勫簱
        TimeDispatch(); // canfestivalμ??a  
    }
}

void setTimer(TIMEVAL value)
{
	TIM14->ARR = TIM14->CNT + value;
}


TIMEVAL getElapsedTime(void)
{
	return TIM14->CNT;
} 
