#include "IO_encoder.h"


//int16_t cnt=0, dir=2;	//cnt: 单圈计数值，dir：旋转方向，0表示正转，1表示反转
int16_t rev = 0;	//圈数

TIM_HandleTypeDef htim_enc;      //编码器定时器句柄 

/*
功能：编码器定时器初始化
输入：
		arr: auto reload value
		psc: prescaler
输出：无
*/
//arr:4095 psc:0
void IO_Encoder_Tim_Init(uint16_t arr, uint16_t psc)
	
{  
	TIM_Encoder_InitTypeDef sConfig = {0};

	htim_enc.Instance = TIM5;
	htim_enc.Init.Prescaler = psc;
	htim_enc.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_enc.Init.Period = arr;
	htim_enc.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 8;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 8;
	
	HAL_TIM_Encoder_Init(&htim_enc, &sConfig);

}

//定时器底层驱动，开启时钟，设置中断优先级
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* tim_encoderHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_encoderHandle->Instance==TIM5)
  {
    /* TIM5 clock enable */
    ENCODER_TIM_CH_GPIO_CLK_ENABLE();
		ENCODER_TIM_CH_CLK_ENABLE();
		
    /**TIM5 GPIO Configuration
    PH10     ------> TIM5_CH1
    PH11     ------> TIM5_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM5_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
  }
}

//编码器定时器的中断服务函数
void ENCODER_TIM_INT_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim_enc);
}


/*
功能：编码器开始计数
输入：无
输出：无
*/
void IO_Encoder_Start_Count(void)
{
	__HAL_TIM_CLEAR_FLAG(&htim_enc, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
	HAL_TIM_Encoder_Start(&htim_enc, ENCODER_TIM_CHANNEL);//启动编码器定时模式
	HAL_TIM_Base_Start_IT(&htim_enc);	//开启定时器终中断，计数超过设定值产生中断，进入回调函数
}


/*
功能：编码器停止计数
输入：无
输出：无
*/
void IO_Encoder_Stop_Count(void)
{
	__HAL_TIM_CLEAR_FLAG(&htim_enc, TIM_IT_UPDATE);
	HAL_TIM_Encoder_Stop(&htim_enc, ENCODER_TIM_CHANNEL);
	HAL_TIM_Base_Stop_IT(&htim_enc);
}

/*
功能：获取编码器当前相对计数值
输入：无
输出：编码器相对计数值
*/
int32_t  IO_Encoder_Get_Count_Val(void)
{
	printf("IO_Encoder_Get_Count_Val:%d .\r\n", ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc) + rev * PULSE_PER_REV);
	return ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc) + rev * PULSE_PER_REV;
}


//计算实际count读数值
int32_t  IO_Encoder_Get_Only_Count_Val(void)
{
	printf("IO_Encoder_Get_Only_Count_Val:%d .\r\n", ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc));
	return ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc);
}

/*
功能：获取编码器当前相对位置
输入：编码器相对计数值
输出：编码器相对位置(单位：degree)
*/
//double IO_Encoder_Get_Rel_Pos(int32_t enc_cnt_val)
//{
//	printf("IO_Encoder_Get_Rel_Pos:%lf .\r\n", enc_cnt_val * FACTOR);
//	return enc_cnt_val * FACTOR;
//}

/*
功能：获取编码器当前相对位置
输入：编码器相对计数值
输出：编码器相对位置(单位：degree)
*/

double IO_Encoder_Get_Rel_Pos()
{
	int32_t enc_cnt_val =  ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc) + rev * PULSE_PER_REV;
	return (double)enc_cnt_val * FACTOR;
}	


//计算实际角度数值
double IO_Encoder_Get_Only_Rel_Pos()
{
	int32_t enc_cnt_val =  ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc);
	
	printf("IO_Encoder_Get_Only_Rel_Pos:%lf .\r\n",(double)enc_cnt_val * FACTOR);
	return (double)enc_cnt_val * FACTOR;
}	
/*
功能：清除编码器当前计数值
输入：无
输出：无
*/
void IO_Encoder_Clear_Count_Val(void)
{
	__HAL_TIM_SET_COUNTER(&htim_enc, 0);
	rev = 0;
}

/*
功能：获取编码器当前已经旋转过的圈数，不足一圈的部分不计
输入：无
输出：圈数
*/
int16_t IO_Encoder_Get_Rev(void)
{
	printf("IO_Encoder_Get_Rev：rev:%d .\r\n",rev);
	return rev;
}
