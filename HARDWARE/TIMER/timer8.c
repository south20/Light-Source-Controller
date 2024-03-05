#include "timer8.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//
//TIM_HandleTypeDef TIM8_Handler;      //定时器句柄 
//TIM_HandleTypeDef TIM1_Handler;      //定时器句柄 
//
////通用定时器3中断初始化
////arr：自动重装值。
////psc：时钟预分频数
////定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=定时器工作频率,单位:Mhz
////这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
//void TIM8_TIM1_Init(u16 arr,u16 psc)
//{  
//    TIM11_Handler.Instance=TIM8;                          //通用定时器3
//    TIM1_Handler.Ini.Prescaler=psc;                     //分频
//    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
//    TIM1_Handler.Init.Period=arr;                        //自动装载值
//    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
//    TIM8_Handler.Instance=TIM8;                          //通用定时器3
//    TIM8_Handler.Init.Prescaler=psc;                     //分频
//    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
//    TIM8_Handler.Init.Period=arr;                        //自动装载值
//    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
//    TIM8_SelectInputTrigger(TIM8, TIM_TS_ITR1);//TIM3的触发输入
//    HAL_TIM_Base_Init(&TIM8_Handler);
//    
//    HAL_TIM_Base_Start_IT(&TIM8_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
//    
//	HAL_TIM_Base_Stop(&TIM8_Handler);
//}
//
////定时器底册驱动，开启时钟，设置中断优先级
////此函数会被HAL_TIM_Base_Init()函数调用
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
//{
//    if(htim->Instance==TIM8)
//	{
//		__HAL_RCC_TIM8_CLK_ENABLE();            //使能TIM3时钟
//		HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
//		HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);          //开启ITM8中断   
//	}  
//}
//
////定时器3中断服务函数
//void TIM8_UP_TIM13_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&TIM8_Handler);
//}



/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/* USER CODE END 1 */

TIM_HandleTypeDef TIM1_Handler;      //定时器1句柄 
TIM_HandleTypeDef TIM8_Handler;			//定时器8句柄 

uint16_t WorkingHour = 0;

void TIM1_Configuration(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
 
  __HAL_RCC_TIM1_CLK_ENABLE();            //使能TIM1时钟
  TIM1_Handler.Instance = TIM1;                                                          
  TIM1_Handler.Init.Prescaler = 21600 - 1;              //f = 216MHz/10800 = 20kHz, T = 0.05ms                                    
  TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                                    
  TIM1_Handler.Init.Period = 10000 - 1;                                                
  TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                              
            
  if (HAL_TIM_Base_Init(&TIM1_Handler) != HAL_OK)                                        
  {
    // Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;                      
  if (HAL_TIM_ConfigClockSource(&TIM1_Handler, &sClockSourceConfig) != HAL_OK)           
  {
    // Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;                            
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;                     
  if (HAL_TIMEx_MasterConfigSynchronization(&TIM1_Handler, &sMasterConfig) != HAL_OK)    
  {
    // Error_Handler();
  }
  __HAL_TIM_CLEAR_FLAG(&TIM1_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
  HAL_TIM_Base_Start(&TIM1_Handler); 
  HAL_TIM_Base_Stop(&TIM1_Handler);
}
 
void TIM8_Configuration(void)
{
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
 
  __HAL_RCC_TIM8_CLK_ENABLE();            //使能TIM8时钟                                                          
 
  TIM8_Handler.Instance = TIM8;                                                          
  TIM8_Handler.Init.Prescaler = 1 - 1;                                                   
  TIM8_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                                    
  TIM8_Handler.Init.Period = 10 - 1;                                             
  TIM8_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                              

  if (HAL_TIM_Base_Init(&TIM8_Handler) != HAL_OK)                                        
  {
    // Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;                               
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;                                        
  if (HAL_TIM_SlaveConfigSynchronization(&TIM8_Handler, &sSlaveConfig) != HAL_OK)                
  {
    // Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;                             
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;                    
  if (HAL_TIMEx_MasterConfigSynchronization(&TIM8_Handler, &sMasterConfig) != HAL_OK)    
  {
    // Error_Handler();
  }
  __HAL_TIM_CLEAR_FLAG(&TIM8_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
  HAL_TIM_Base_Start(&TIM8_Handler); 
	HAL_TIM_Base_Start_IT(&TIM8_Handler);
	
	HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
				HAL_TIM_Base_Stop(&TIM8_Handler);
}
 

void TIM8_UP_TIM13_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM8_Handler);
}

uint16_t GetWorkingHours(void)
{
    return WorkingHour;
}

