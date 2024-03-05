#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//外部中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//外部中断初始化
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
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//下拉
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;               //PG11
//    GPIO_Initure.Pin=GPIO_PIN_11;               //PG11
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_3;     //PH8,3  上升沿触发，下拉
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_4;               //PG11
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_9;               //PG11
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_NOPULL;			//
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

	
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;     //PI0,1,2  上升沿触发，下拉
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
//    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
	
    GPIO_Initure.Pin=GPIO_PIN_14|GPIO_PIN_13;     //PH8,3  上升沿触发，下拉
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
    GPIO_Initure.Pin= GPIO_PIN_15;     //PH8,3  上升沿触发，下拉
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;      //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
    GPIO_Initure.Pin=GPIO_PIN_5;     //PH8,3  上升沿触发，下拉
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //中断线0
    HAL_NVIC_SetPriority(EXTI0_IRQn,1,2);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI0_IRQn); 
	
    //中断线1
    HAL_NVIC_SetPriority(EXTI1_IRQn,1,3);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	//使能中断线2
    //中断线2
    HAL_NVIC_SetPriority(EXTI2_IRQn,1,4);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线2
    
    //中断线3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //使能中断线2
    
    //中断线4
    HAL_NVIC_SetPriority(EXTI4_IRQn,2,6);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);             //使能中断线2
    
    //中断线8,9
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,4);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //使能中断线2
    
    //中断线 10 11 12
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,5);   //抢占优先级为3，子优先级为3
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //使能中断线13  
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //调用中断处理公用函数
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);   //调用中断处理公用函数
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //调用中断处理公用函数
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);   //调用中断处理公用函数
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   //调用中断处理公用函数
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);  //调用中断处理公用函数
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);  //调用中断处理公用函数
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);  //调用中断处理公用函数
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);  //调用中断处理公用函数
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);  //调用中断处理公用函数
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);  //调用中断处理公用函数
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);  //调用中断处理公用函数
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);  //调用中断处理公用函数
}

