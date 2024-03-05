#include "canfestival_timer.h"
#include "stm32f7xx_hal_tim.h"
#include "stm32f7xx.h"
#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"
#include "IO_encoder.h"
static TIM_HandleTypeDef TIM_BaseStruct;


TIM_HandleTypeDef TIM14_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��14�жϳ�ʼ��
//TIMx:��ʱ���ͺ�
//TIM_Period���Զ���װֵ��
//TIM_Prescaler��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��14!
void TIMConfig(TIM_TypeDef* TIMx, uint16_t TIM_Period, uint16_t TIM_Prescaler)
{
	  //��ʱ��TIM14��ʼ��,����ARR,PSC
    TIM_BaseStruct.Instance = TIMx;
    TIM_BaseStruct.Init.Prescaler = TIM_Period - 1;     //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_BaseStruct.Init.CounterMode = TIM_COUNTERMODE_UP;//TIM���ϼ���ģʽ
    TIM_BaseStruct.Init.Period = TIM_Prescaler;        //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
    TIM_BaseStruct.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&TIM_BaseStruct);

	__HAL_TIM_CLEAR_FLAG(&TIM_BaseStruct, TIM_FLAG_UPDATE);
    //������ʱ��/�ж�
    HAL_TIM_Base_Start_IT(&TIM_BaseStruct);
	
}



void NVIC_Configuration(void)
{
   
	__HAL_RCC_TIM14_CLK_ENABLE();            //��1?��TIM3����?��
    NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0); // 设置中断优先级为0，抢占优先级0，子优先�?
    NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);      // 使能中断

}


//��ʱ��14�жϷ������
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{

    HAL_TIM_IRQHandler(&TIM_BaseStruct);

    if (__HAL_TIM_GET_FLAG(&TIM_BaseStruct, TIM_FLAG_UPDATE) != RESET) {
        __HAL_TIM_CLEAR_FLAG(&TIM_BaseStruct, TIM_FLAG_UPDATE); // // canfestival的库
        TimeDispatch(); // canfestival��??a  
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
