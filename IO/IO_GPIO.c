#include "IO_GPIO.h"

void IO_GPIO_Init_All_Clocks(void) 
{
    // ��ʼ�����п��ܵ�ʱ�����ŵ�ʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
    
}

void IO_GPIO_Init_All_Pins(void) 
{
	for(int i = IO_GPIO_ZERO_POS; i < IO_GPIO_MAX1; ++i)
	{
		IO_GPIO_Init_Pin(i);
	}
    
}


int IO_GPIO_Init_Pin(IO_GPIO_ENUM IO_GPIOx)
{

    GPIO_InitTypeDef GPIO_Initure;
    

    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin = 0;

    // ��ʼ�� GPIO ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ��Ϊ����
    GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    // ���� IO_GPIOx ö��ѡ����Ӧ�� GPIOx ������
    switch (IO_GPIOx)
    {
    case IO_GPIO_ZERO_POS:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_ENC_A:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_B:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_Z:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ABS_EN:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_UP:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_DOWN:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_4;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_LEFT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_RIGHT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_12;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_INTERLOCK:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_13;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SHUTTER_MODE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_0;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_START:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_8;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_OK:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_OPEN:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_CLOSE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_6;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_ON_OFF:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_RD:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_BL:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_4;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_GR:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_VERIFY:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_15;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_ALARM:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON_OFF:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_15;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SYS_INIERLOCK:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_IGN_OUT:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_14;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ALARM:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_COOL:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LG_DTKT1:
        GPIOx = GPIOF;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_FILTER_DETECT1:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_9;
        break;
    case IO_GPIO_FILTER_DETECT2:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_11;
        break;
    case IO_GPIO_FILTER_DETECT3:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_13;
        break;
    case IO_GPIO_ALARM_CLEAR:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_IN1:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_14;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_OUT1:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SP_OUT2:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_FAN_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_6;
        break;
    case IO_GPIO_BLW_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_5;
        break;
    case IO_GPIO_BLW_PWM:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_VA_ENABLE:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_Shutter_ENABLE:
        GPIOx = GPIOD;
        GPIO_Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    // ��Ӹ���� case��������Ҫ��ȡ��������
    default:
        // ����δ֪�� IO_GPIOx
        return IO_PARAMETER_ERROR; // ����Ĭ��״̬
    }

    GPIO_InitStruct.Pin = GPIO_Pin;

    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	return 0;
    // ���ݴ����״̬�������ŵĳ�ʼ��ƽ
    //HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (GPIO_PinState)state);
}

int IO_GPIO_DeInit_Pin(IO_GPIO_ENUM IO_GPIOx)
{
   // GPIO_InitTypeDef GPIO_Initure;
    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin = 0;

    // ��ʼ�� GPIO ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ��Ϊ����
    GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    // ���� IO_GPIOx ö��ѡ����Ӧ�� GPIOx ������
    switch (IO_GPIOx)
    {
    case IO_GPIO_ZERO_POS:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_ENC_A:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_B:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_Z:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ABS_EN:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_UP:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_DOWN:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_4;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_LEFT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_RIGHT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_12;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_INTERLOCK:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_13;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SHUTTER_MODE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_0;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_START:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_8;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_OK:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_OPEN:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_CLOSE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_6;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_ON_OFF:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_RD:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_BL:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_4;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_GR:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_VERIFY:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_15;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_ALARM:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON_OFF:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_15;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SYS_INIERLOCK:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_IGN_OUT:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_14;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ALARM:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_COOL:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LG_DTKT1:
        GPIOx = GPIOF;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_FILTER_DETECT1:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_9;
        break;
    case IO_GPIO_FILTER_DETECT2:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_11;
        break;
    case IO_GPIO_FILTER_DETECT3:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_13;
        break;
    case IO_GPIO_ALARM_CLEAR:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_IN1:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_14;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_OUT1:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SP_OUT2:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_FAN_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_6;
        break;
    case IO_GPIO_BLW_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_5;
        break;
    case IO_GPIO_BLW_PWM:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_VA_ENABLE:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_Shutter_ENABLE:
        GPIOx = GPIOD;
        GPIO_Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    // ��Ӹ���� case��������Ҫ��ȡ��������
    default:
        // ����δ֪�� IO_GPIOx
        return IO_PARAMETER_ERROR; // ����Ĭ��״̬
    }

    // Deinitialize GPIO ����
    HAL_GPIO_DeInit(GPIOx, GPIO_Pin);
	return 0;
}

int IO_GPIO_Read_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State * state)
{
    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin = 0;

    // ��ʼ�� GPIO ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ��Ϊ����
    GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    // ���� IO_GPIOx ö��ѡ����Ӧ�� GPIOx ������
    switch (IO_GPIOx)
    {
    case IO_GPIO_ZERO_POS:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_ENC_A:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_B:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_Z:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ABS_EN:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_UP:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_DOWN:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_4;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_LEFT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_RIGHT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_12;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_INTERLOCK:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_13;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SHUTTER_MODE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_0;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_START:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_8;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_OK:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_OPEN:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_CLOSE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_6;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_ON_OFF:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_RD:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_BL:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_4;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_GR:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_VERIFY:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_15;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_ALARM:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON_OFF:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_15;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SYS_INIERLOCK:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_IGN_OUT:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_14;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ALARM:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_COOL:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LG_DTKT1:
        GPIOx = GPIOF;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_FILTER_DETECT1:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_9;
        break;
    case IO_GPIO_FILTER_DETECT2:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_11;
        break;
    case IO_GPIO_FILTER_DETECT3:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_13;
        break;
    case IO_GPIO_ALARM_CLEAR:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_IN1:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_14;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_OUT1:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SP_OUT2:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_FAN_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_6;
        break;
    case IO_GPIO_BLW_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_5;
        break;
    case IO_GPIO_BLW_PWM:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_VA_ENABLE:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_Shutter_ENABLE:
        GPIOx = GPIOD;
        GPIO_Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    // ��Ӹ���� case��������Ҫ��ȡ��������
    default:
        // ����δ֪�� IO_GPIOx
        return IO_PARAMETER_ERROR; // ����Ĭ��״̬
    }
	
	*state = (IO_GPIO_Pin_State)HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

    // ��ȡ GPIO ����״̬
    return 0;
}

int IO_GPIO_Write_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State state)
{
    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin = 0;

    // ��ʼ�� GPIO ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ��Ϊ����
    GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    // ���� IO_GPIOx ö��ѡ����Ӧ�� GPIOx ������
    switch (IO_GPIOx)
    {
    case IO_GPIO_ZERO_POS:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_ENC_A:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_B:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_Z:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ABS_EN:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_UP:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_DOWN:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_4;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_LEFT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_RIGHT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_12;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_INTERLOCK:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_13;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SHUTTER_MODE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_0;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_START:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_8;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_OK:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_OPEN:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_CLOSE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_6;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_ON_OFF:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_RD:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_BL:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_4;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_GR:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_VERIFY:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_15;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_ALARM:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON_OFF:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_15;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SYS_INIERLOCK:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_IGN_OUT:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_14;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ALARM:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_COOL:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LG_DTKT1:
        GPIOx = GPIOF;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_FILTER_DETECT1:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_9;
        break;
    case IO_GPIO_FILTER_DETECT2:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_11;
        break;
    case IO_GPIO_FILTER_DETECT3:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_13;
        break;
    case IO_GPIO_ALARM_CLEAR:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_IN1:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_14;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_OUT1:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SP_OUT2:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_FAN_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_6;
        break;
    case IO_GPIO_BLW_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_5;
        break;
    case IO_GPIO_BLW_PWM:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_VA_ENABLE:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_Shutter_ENABLE:
        GPIOx = GPIOD;
        GPIO_Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    // ��Ӹ���� case��������Ҫ��ȡ��������
    default:
        // ����δ֪�� IO_GPIOx
        return IO_PARAMETER_ERROR; // ����Ĭ��״̬
    }


    // д�� GPIO ����״̬
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (GPIO_PinState)state);
	
	return 0;
}

int IO_GPIO_Lock_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State state)
{
    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin = 0;

    // ��ʼ�� GPIO ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ��Ϊ����
    GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    // ���� IO_GPIOx ö��ѡ����Ӧ�� GPIOx ������
    switch (IO_GPIOx)
    {
    case IO_GPIO_ZERO_POS:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_ENC_A:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_B:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_Z:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ABS_EN:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_UP:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_DOWN:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_4;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_LEFT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_RIGHT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_12;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_INTERLOCK:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_13;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SHUTTER_MODE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_0;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_START:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_8;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_OK:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_OPEN:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_CLOSE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_6;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_ON_OFF:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_RD:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_BL:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_4;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_GR:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_VERIFY:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_15;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_ALARM:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON_OFF:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_15;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SYS_INIERLOCK:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_IGN_OUT:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_14;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ALARM:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_COOL:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LG_DTKT1:
        GPIOx = GPIOF;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_FILTER_DETECT1:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_9;
        break;
    case IO_GPIO_FILTER_DETECT2:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_11;
        break;
    case IO_GPIO_FILTER_DETECT3:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_13;
        break;
    case IO_GPIO_ALARM_CLEAR:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_IN1:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_14;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_OUT1:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SP_OUT2:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_FAN_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_6;
        break;
    case IO_GPIO_BLW_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_5;
        break;
    case IO_GPIO_BLW_PWM:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_VA_ENABLE:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_Shutter_ENABLE:
        GPIOx = GPIOD;
        GPIO_Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    // ��Ӹ���� case��������Ҫ��ȡ��������
    default:
        // ����δ֪�� IO_GPIOx
        return IO_PARAMETER_ERROR; // ����Ĭ��״̬
    }


    //���� GPIO ����״̬��������Ͳ��ܽ��г�ʼ��������
    HAL_GPIO_LockPin(GPIOx, GPIO_Pin);
	return 0;
}


int IO_GPIO_Toggle_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State bitstatus)
{
    GPIO_TypeDef* GPIOx = NULL;
    uint16_t GPIO_Pin = 0;

    // ��ʼ�� GPIO ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ��Ϊ����
    GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    // ���� IO_GPIOx ö��ѡ����Ӧ�� GPIOx ������
    switch (IO_GPIOx)
    {
    case IO_GPIO_ZERO_POS:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_ENC_A:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_B:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ENC_Z:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_ABS_EN:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_UP:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_3;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_DOWN:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_4;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_LEFT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_11;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_RIGHT:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_12;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_INTERLOCK:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_13;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SHUTTER_MODE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_0;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_START:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_8;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_OK:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_OPEN:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHU_CLOSE:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_6;
    	GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // ����
        break;
    case IO_GPIO_SHUTTER_ON_OFF:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_5;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_RD:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_BL:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_4;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_GR:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_VERIFY:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_15;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SHUTTER_ALARM:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON_OFF:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_15;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SYS_INIERLOCK:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_IGN_OUT:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ON:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_14;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_ALARM:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LAMP_COOL:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_LG_DTKT1:
        GPIOx = GPIOF;
        GPIO_Pin = GPIO_PIN_10;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_FILTER_DETECT1:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_9;
        break;
    case IO_GPIO_FILTER_DETECT2:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_11;
        break;
    case IO_GPIO_FILTER_DETECT3:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_13;
        break;
    case IO_GPIO_ALARM_CLEAR:
        GPIOx = GPIOI;
        GPIO_Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_IN1:
        GPIOx = GPIOH;
        GPIO_Pin = GPIO_PIN_14;
    	GPIO_InitStruct.Pull = GPIO_PULLUP;     // ����
        break;
    case IO_GPIO_SP_OUT1:
        GPIOx = GPIOG;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_SP_OUT2:
        GPIOx = GPIOC;
        GPIO_Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_FAN_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_6;
        break;
    case IO_GPIO_BLW_FG:
        GPIOx = GPIOE;
        GPIO_Pin = GPIO_PIN_5;
        break;
    case IO_GPIO_BLW_PWM:
        GPIOx = GPIOA;
        GPIO_Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_VA_ENABLE:
        GPIOx = GPIOB;
        GPIO_Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    case IO_GPIO_Shutter_ENABLE:
        GPIOx = GPIOD;
        GPIO_Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��Ϊ�������
        break;
    // ��Ӹ���� case��������Ҫ��ȡ��������
    default:
        // ����δ֪�� IO_GPIOx
        return IO_PARAMETER_ERROR; // ����Ĭ��״̬
    }

    // ��ת GPIO ����״̬
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	return 0;
}
