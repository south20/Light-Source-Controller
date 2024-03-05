#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//�ⲿ�ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
//	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
    
    GPIO_Initure.Pin=GPIO_PIN_10;                //PB7
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;               //PG11
//    GPIO_Initure.Pin=GPIO_PIN_11;               //PG11
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_3;     //PH8,3  �����ش���������
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_4;               //PG11
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_9;               //PG11
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;      //�����ش���
    GPIO_Initure.Pull=GPIO_NOPULL;			//
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

	
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;     //PI0,1,2  �����ش���������
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;			//����
//    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
	
    GPIO_Initure.Pin=GPIO_PIN_14|GPIO_PIN_13;     //PH8,3  �����ش���������
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;			//����
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
    GPIO_Initure.Pin= GPIO_PIN_15;     //PH8,3  �����ش���������
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;      //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;			//����
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
    GPIO_Initure.Pin=GPIO_PIN_5;     //PH8,3  �����ش���������
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;			//����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //�ж���0
    HAL_NVIC_SetPriority(EXTI0_IRQn,1,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI0_IRQn); 
	
    //�ж���1
    HAL_NVIC_SetPriority(EXTI1_IRQn,1,3);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	//ʹ���ж���2
    //�ж���2
    HAL_NVIC_SetPriority(EXTI2_IRQn,1,4);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //ʹ���ж���2
    
    //�ж���3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //ʹ���ж���2
    
    //�ж���4
    HAL_NVIC_SetPriority(EXTI4_IRQn,2,6);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);             //ʹ���ж���2
    
    //�ж���8,9
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,4);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //ʹ���ж���2
    
    //�ж��� 10 11 12
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,5);   //��ռ���ȼ�Ϊ3�������ȼ�Ϊ3
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //ʹ���ж���13  
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //�����жϴ����ú���
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);   //�����жϴ����ú���
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //�����жϴ����ú���
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);   //�����жϴ����ú���
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   //�����жϴ����ú���
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);  //�����жϴ����ú���
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);  //�����жϴ����ú���
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);  //�����жϴ����ú���
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);  //�����жϴ����ú���
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);  //�����жϴ����ú���
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);  //�����жϴ����ú���
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);  //�����жϴ����ú���
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);  //�����жϴ����ú���
}

