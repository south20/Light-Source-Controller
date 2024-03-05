
#include "PE_BlowerFan.h"
#include "stm32f7xx_hal.h"

// ����PWM��ز���
//#define PWM_FREQUENCY 25000
//#define PWM_PERIOD (1000000 / PWM_FREQUENCY) // ����PWM���ڣ���΢��Ϊ��λ��

//����ת�ٵĲ���״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM9CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM9CH2_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

//�ķ��ת��
u8  TIM9CH1_CAPTURE_STA=0;			    				
u16	TIM9CH1_CAPTURE_VAL;



TIM_HandleTypeDef TIM9_Handler; 


// ����TIM��������ýṹ��

TIM_HandleTypeDef TIM3_Handler;         //��ʱ����� 
TIM_OC_InitTypeDef TIM3CH1Handler;   


//arr���Զ���װֵ(TIM2,TIM5��32λ��!!TIM9��16λ)
//psc��ʱ��Ԥ��Ƶ��


void PE_BlowerFan_Init()
{
	u16 arr = 39;
	u16 psc = 107;

    TIM3_Handler.Instance=TIM3;            //��ʱ��3
    TIM3_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM3_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //��ʼ��PWM
    
    TIM3CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM3CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�
                                            //Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM3CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ��
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3CH1Handler,TIM_CHANNEL_1);//����TIM3ͨ��1
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//����PWMͨ��1


}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//ʹ�ܶ�ʱ��3
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_6;           	//PA6
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;	//PB1����ΪTIM3_CH4
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


//����TIMͨ��4��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM3Compare1(u32 compare)
{
	TIM3->CCR1=compare; 
}

//��ȡTIM����/�ȽϼĴ���ֵ
u32 TIM_GetTIM3Capture1(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_1);
}



// ���ƺ���
int PE_BlowerFan_Control(uint32_t cycle) 
{
	//cycle��510-3400
	//ռ�ձ���Ч��Χtemp_duty_cycle��10%-100%            4 40
	//cycle = k* temp_duty_cycle + b

//	��0.1��510�� ��1��3400��
//
//k = 2890 /0.9 = 3211.11
//510 = 321.111 + b
//b = 178.889

	double duty_cycle;
	double temp_duty_cycle; //ʵ��ռ�ձ�
	int rtn = 0;
//    rtn = IO_DS1821_ReadTemperature(&temperature); // ��ȡ�¶�
//
//    // �����¶ȵ����ķ���ٶ�
//    uint32_t duty_cycle;
//    if (temperature > 100) 
//	{
//        // ����Ϊ����ٶ�
//        duty_cycle = 39;
//    } 
//	else 
//	{
//        // ����Ϊ��С�ٶ�
//        duty_cycle = 15;
//    }
	temp_duty_cycle = (cycle - 133.34)* 1000 / 3766660;
	duty_cycle = temp_duty_cycle *40;
		printf("temp_duty_cycle:%lf duty_cycle:%lf  \r\n", temp_duty_cycle, duty_cycle);
	TIM_SetTIM3Compare1(duty_cycle);

    // ����PWMռ�ձ��Կ����ٶ�
   	// sConfigOC.Pulse = duty_cycle;
    //HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_1);
    //HAL_TIM_PWM_Start(&htimx, TIM_CHANNEL_1);
	
	return rtn;
}

void CoolingFan_TIM9_CH2_Cap_Init()
{  
    TIM_IC_InitTypeDef TIM9_CH2Config = {0};  
    
    TIM9_Handler.Instance=TIM9;                          //ͨ�ö�ʱ��5
    TIM9_Handler.Init.Prescaler=215;                     //��Ƶ
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM9_Handler.Init.Period=0xFFFF;                        //�Զ�װ��ֵ
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM9_Handler);
    
    TIM9_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM9_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI1��
    TIM9_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM9_CH2Config.ICFilter=8;                          //���������˲���
    HAL_TIM_IC_ConfigChannel(&TIM9_Handler,&TIM9_CH2Config,TIM_CHANNEL_2);//����TIM5ͨ��1
    HAL_TIM_IC_Start_IT(&TIM9_Handler,TIM_CHANNEL_2);   //��ʼ����TIM5��ͨ��1
    __HAL_TIM_ENABLE_IT(&TIM9_Handler,TIM_IT_UPDATE);   //ʹ�ܸ����ж�
}

void BlowerFan_TIM9_CH1_Cap_Init(u16 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM9_CH1Config = {0};  
    
    TIM9_Handler.Instance=TIM9;                          //
    TIM9_Handler.Init.Prescaler=215;                     //
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //
    TIM9_Handler.Init.Period=0xFFFF;                        //
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM9_Handler);
    
    TIM9_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //
    TIM9_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//
    TIM9_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //
    TIM9_CH1Config.ICFilter=8;                          //
    HAL_TIM_IC_ConfigChannel(&TIM9_Handler,&TIM9_CH1Config,TIM_CHANNEL_1);//
    HAL_TIM_IC_Start_IT(&TIM9_Handler,TIM_CHANNEL_1);   //
    __HAL_TIM_ENABLE_IT(&TIM9_Handler,TIM_IT_UPDATE); 

}


//��ʱ��5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_IC_Init()����
//htim:��ʱ��5���

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{

	    GPIO_InitTypeDef GPIO_Initure;
		GPIO_InitTypeDef GPIO_Initure2;
	
		__HAL_RCC_TIM9_CLK_ENABLE();            //ʹ��TIM9ʱ��
	    __HAL_RCC_GPIOE_CLK_ENABLE();			//����GPIOEʱ��
		
	    GPIO_Initure.Pin=GPIO_PIN_5;            //PE5
	    GPIO_Initure.Mode=GPIO_MODE_AF_PP;    
	    GPIO_Initure.Pull=GPIO_PULLUP;        
	    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
	    GPIO_Initure.Alternate=GPIO_AF3_TIM9;   //PE5����ΪTIM9ͨ��1
	    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
	    GPIO_Initure2.Pin=GPIO_PIN_6;            //Pe6
	    GPIO_Initure2.Mode=GPIO_MODE_AF_PP;      //�����������
	    GPIO_Initure2.Pull=GPIO_PULLUP;        //����
	    GPIO_Initure2.Speed=GPIO_SPEED_HIGH;     //����
	    GPIO_Initure2.Alternate=GPIO_AF3_TIM9;   //PE6����ΪTIM9ͨ��2
	    HAL_GPIO_Init(GPIOE,&GPIO_Initure2);

	    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
	    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);          //����ITM9�ж�   
	
}
	

// �жϷ����������ڼ���ķ��ת��

void TIM1_BRK_TIM9_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM9_Handler);
}
 


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM9 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {

        if((TIM9CH1_CAPTURE_STA & 0X80) == 0) 
        {
            if(TIM9CH1_CAPTURE_STA & 0X40) 
            {          
                TIM9CH1_CAPTURE_STA |= 0X80;  
                TIM9CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); 

               
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1);  
                TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
            }
            else 
            {
                TIM9CH1_CAPTURE_STA = 0;   
                TIM9CH1_CAPTURE_VAL = 0;
                TIM9CH1_CAPTURE_STA |= 0X40;  

                __HAL_TIM_DISABLE(htim); 
                __HAL_TIM_SET_COUNTER(htim, 0);
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1);  
                TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING); 
                __HAL_TIM_ENABLE(htim); 
            }         
        }


    }

	
	if(htim->Instance == TIM9 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        // ����״̬��ֵ�Ĵ���
        if((TIM9CH2_CAPTURE_STA & 0X80) == 0) // ��δ�ɹ�����
        {
            if(TIM9CH2_CAPTURE_STA & 0X40) // ����һ���½���
            {          
                TIM9CH2_CAPTURE_STA |= 0X80;  // ��ǳɹ�����һ�θߵ�ƽ����
                TIM9CH2_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // ��ȡ��ǰ�Ĳ���ֵ

                // �������ò���ģʽ
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2);  // ���ԭ��������
                TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_ICPOLARITY_RISING); // ����Ϊ�����ز���
            }
            else // ��δ��ʼ, ��һ�β���������
            {
                TIM9CH2_CAPTURE_STA = 0;   // ���
                TIM9CH2_CAPTURE_VAL = 0;
                TIM9CH2_CAPTURE_STA |= 0X40;  // ��ǲ�����������

                __HAL_TIM_DISABLE(htim); // �رն�ʱ��9
                __HAL_TIM_SET_COUNTER(htim, 0);
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2);  // ���ԭ��������
                TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_ICPOLARITY_FALLING); // ����Ϊ�½��ز���
                __HAL_TIM_ENABLE(htim); // ����ʹ�ܶ�ʱ��9
            }         
        }
    }
}
int PE_CoolingFan_CheckSpeed(int* fan_rpm)
{
	int rtn=0;
	long temp=0;  
	long temp_t=0;
	if(TIM9CH2_CAPTURE_STA&0X80)		//�ɹ�������һ�θߵ�ƽ
	{
		temp=TIM9CH2_CAPTURE_STA&0X3F; 
		temp*=0XFFFF;				//���ʱ���ܺ�
		temp+=TIM9CH2_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
		printf("HIGH:%ld us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
		temp_t=temp*4;
		TIM9CH2_CAPTURE_STA=0;			//������һ�β���
	}
	*fan_rpm = 60000000 / temp_t;

	return rtn;
}

int PE_BlowerFan_CheckSpeed(int* fan_rpm)
{
	int rtn=0;
	long temp=0;  
	long temp_t=0;
	if(TIM9CH1_CAPTURE_STA&0X80)		//�ɹ�������һ�θߵ�ƽ
	{
		temp=TIM9CH1_CAPTURE_STA&0X3F; 
		temp*=0XFFFF;				//���ʱ���ܺ�
		temp+=TIM9CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
		printf("HIGH:%ld us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
		temp_t=temp*4;
		TIM9CH1_CAPTURE_STA=0;			//������һ�β���
	}
	*fan_rpm = 60000000 / temp_t;
	return rtn;

}

