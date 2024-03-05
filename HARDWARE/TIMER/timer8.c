#include "timer8.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//
//TIM_HandleTypeDef TIM8_Handler;      //��ʱ����� 
//TIM_HandleTypeDef TIM1_Handler;      //��ʱ����� 
//
////ͨ�ö�ʱ��3�жϳ�ʼ��
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=��ʱ������Ƶ��,��λ:Mhz
////����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
//void TIM8_TIM1_Init(u16 arr,u16 psc)
//{  
//    TIM11_Handler.Instance=TIM8;                          //ͨ�ö�ʱ��3
//    TIM1_Handler.Ini.Prescaler=psc;                     //��Ƶ
//    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
//    TIM1_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
//    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
//    TIM8_Handler.Instance=TIM8;                          //ͨ�ö�ʱ��3
//    TIM8_Handler.Init.Prescaler=psc;                     //��Ƶ
//    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
//    TIM8_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
//    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
//    TIM8_SelectInputTrigger(TIM8, TIM_TS_ITR1);//TIM3�Ĵ�������
//    HAL_TIM_Base_Init(&TIM8_Handler);
//    
//    HAL_TIM_Base_Start_IT(&TIM8_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
//    
//	HAL_TIM_Base_Stop(&TIM8_Handler);
//}
//
////��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
////�˺����ᱻHAL_TIM_Base_Init()��������
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
//{
//    if(htim->Instance==TIM8)
//	{
//		__HAL_RCC_TIM8_CLK_ENABLE();            //ʹ��TIM3ʱ��
//		HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
//		HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);          //����ITM8�ж�   
//	}  
//}
//
////��ʱ��3�жϷ�����
//void TIM8_UP_TIM13_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&TIM8_Handler);
//}



/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/* USER CODE END 1 */

TIM_HandleTypeDef TIM1_Handler;      //��ʱ��1��� 
TIM_HandleTypeDef TIM8_Handler;			//��ʱ��8��� 

uint16_t WorkingHour = 0;

void TIM1_Configuration(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
 
  __HAL_RCC_TIM1_CLK_ENABLE();            //ʹ��TIM1ʱ��
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
  __HAL_TIM_CLEAR_FLAG(&TIM1_Handler, TIM_IT_UPDATE);	//�����жϱ�־λ�ٿ����жϣ������ϵ�ͻ����һ���ж�
  HAL_TIM_Base_Start(&TIM1_Handler); 
  HAL_TIM_Base_Stop(&TIM1_Handler);
}
 
void TIM8_Configuration(void)
{
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
 
  __HAL_RCC_TIM8_CLK_ENABLE();            //ʹ��TIM8ʱ��                                                          
 
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
  __HAL_TIM_CLEAR_FLAG(&TIM8_Handler, TIM_IT_UPDATE);	//�����жϱ�־λ�ٿ����жϣ������ϵ�ͻ����һ���ж�
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

