#include "IO_encoder.h"


//int16_t cnt=0, dir=2;	//cnt: ��Ȧ����ֵ��dir����ת����0��ʾ��ת��1��ʾ��ת
int16_t rev = 0;	//Ȧ��

TIM_HandleTypeDef htim_enc;      //��������ʱ����� 

/*
���ܣ���������ʱ����ʼ��
���룺
		arr: auto reload value
		psc: prescaler
�������
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

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
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

//��������ʱ�����жϷ�����
void ENCODER_TIM_INT_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim_enc);
}


/*
���ܣ���������ʼ����
���룺��
�������
*/
void IO_Encoder_Start_Count(void)
{
	__HAL_TIM_CLEAR_FLAG(&htim_enc, TIM_IT_UPDATE);	//�����жϱ�־λ�ٿ����жϣ������ϵ�ͻ����һ���ж�
	HAL_TIM_Encoder_Start(&htim_enc, ENCODER_TIM_CHANNEL);//������������ʱģʽ
	HAL_TIM_Base_Start_IT(&htim_enc);	//������ʱ�����жϣ����������趨ֵ�����жϣ�����ص�����
}


/*
���ܣ�������ֹͣ����
���룺��
�������
*/
void IO_Encoder_Stop_Count(void)
{
	__HAL_TIM_CLEAR_FLAG(&htim_enc, TIM_IT_UPDATE);
	HAL_TIM_Encoder_Stop(&htim_enc, ENCODER_TIM_CHANNEL);
	HAL_TIM_Base_Stop_IT(&htim_enc);
}

/*
���ܣ���ȡ��������ǰ��Լ���ֵ
���룺��
�������������Լ���ֵ
*/
int32_t  IO_Encoder_Get_Count_Val(void)
{
	printf("IO_Encoder_Get_Count_Val:%d .\r\n", ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc) + rev * PULSE_PER_REV);
	return ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc) + rev * PULSE_PER_REV;
}


//����ʵ��count����ֵ
int32_t  IO_Encoder_Get_Only_Count_Val(void)
{
	printf("IO_Encoder_Get_Only_Count_Val:%d .\r\n", ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc));
	return ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc);
}

/*
���ܣ���ȡ��������ǰ���λ��
���룺��������Լ���ֵ
��������������λ��(��λ��degree)
*/
//double IO_Encoder_Get_Rel_Pos(int32_t enc_cnt_val)
//{
//	printf("IO_Encoder_Get_Rel_Pos:%lf .\r\n", enc_cnt_val * FACTOR);
//	return enc_cnt_val * FACTOR;
//}

/*
���ܣ���ȡ��������ǰ���λ��
���룺��������Լ���ֵ
��������������λ��(��λ��degree)
*/

double IO_Encoder_Get_Rel_Pos()
{
	int32_t enc_cnt_val =  ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc) + rev * PULSE_PER_REV;
	return (double)enc_cnt_val * FACTOR;
}	


//����ʵ�ʽǶ���ֵ
double IO_Encoder_Get_Only_Rel_Pos()
{
	int32_t enc_cnt_val =  ( int32_t )__HAL_TIM_GET_COUNTER(&htim_enc);
	
	printf("IO_Encoder_Get_Only_Rel_Pos:%lf .\r\n",(double)enc_cnt_val * FACTOR);
	return (double)enc_cnt_val * FACTOR;
}	
/*
���ܣ������������ǰ����ֵ
���룺��
�������
*/
void IO_Encoder_Clear_Count_Val(void)
{
	__HAL_TIM_SET_COUNTER(&htim_enc, 0);
	rev = 0;
}

/*
���ܣ���ȡ��������ǰ�Ѿ���ת����Ȧ��������һȦ�Ĳ��ֲ���
���룺��
�����Ȧ��
*/
int16_t IO_Encoder_Get_Rev(void)
{
	printf("IO_Encoder_Get_Rev��rev:%d .\r\n",rev);
	return rev;
}
