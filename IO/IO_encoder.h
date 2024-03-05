#ifndef IO_ENCODER_H
#define IO_ENCODER_H
#include "sys.h"

#define PULSE_PER_REV 4096
#define FACTOR				0.087890625		//������1��count��Ӧ�ĽǶ�ֵ

/*�������Ķ�ʱ����غ궨��*/
/*ͨ�ö�ʱ��5*/
/*Ӳ������ΪTIM5���޸�������Ч*/
#define ENCODER_TIM_CHANNEL								TIM_CHANNEL_1 | TIM_CHANNEL_2
#define ENCODER_TIM_CH_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0) 
#define ENCODER_TIM_CH_CLK_ENABLE()       do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)   
#define ENCODER_TIM_INT_IRQHandler        TIM5_IRQHandler


/*
���ܣ���������ʱ����ʼ��
���룺
		arr: auto reload value
		psc: prescaler
�������
*/
void IO_Encoder_Tim_Init(uint16_t arr, uint16_t psc);

/*
���ܣ���������ʼ����
���룺��
�������
*/
void IO_Encoder_Start_Count(void);

/*
���ܣ�������ֹͣ����
���룺��
�������
*/
void IO_Encoder_Stop_Count(void);

/*
���ܣ���ȡ��������ǰ��Լ���ֵ
���룺��
�������������Լ���ֵ
*/
int32_t  IO_Encoder_Get_Count_Val(void);  

//����ʵ��count����ֵ
int32_t  IO_Encoder_Get_Only_Count_Val(void);  

/*
���ܣ���ȡ��������ǰ���λ��
���룺��������Լ���ֵ
��������������λ��(��λ��degree)
*/
double IO_Encoder_Get_Rel_Pos();

//����ʵ�ʽǶ���ֵ
double IO_Encoder_Get_Only_Rel_Pos();

/*
���ܣ������������ǰ����ֵ
���룺��
�������
*/
void IO_Encoder_Clear_Count_Val(void);

/*
���ܣ���ȡ��������ǰ�Ѿ���ת����Ȧ��������һȦ�Ĳ��ֲ���
���룺��
�����Ȧ��
*/
int16_t IO_Encoder_Get_Rev(void);

extern TIM_HandleTypeDef  htim_enc;  //��������ʱ�����


#endif

