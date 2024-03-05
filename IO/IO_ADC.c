#include "IO_ADC.h"
#include "delay.h"
#include "stm32f7xx_hal_adc.h"
//#include "stm32f7xx_hal_adc_ex.h"

ADC_HandleTypeDef hadc1;//ADC���
//
//// IO_ADC_Channel �ṹ�����飬�洢ÿ�� IO_ADC_ENUM ��Ӧ�����ź� ADC ͨ����Ϣ
//const IO_ADC_Channel io_adc_channels[IO_ADC_MAX] = {
//    {GPIOB, GPIO_PIN_1, ADC_CHANNEL_9},  // IO_ADC_LP_FBK
//    {GPIOB, GPIO_PIN_0, ADC_CHANNEL_1},  // IO_ADC_CUR_FBK
//    {GPIOA, GPIO_PIN_5, ADC_CHANNEL_2}   // IO_ADC_CUR_CMD
//};

IO_ADC_ENUM current_adc_type = IO_ADC_MIN;  // ��ʼ��ΪĬ����������

//��ʼ��ADC
void IO_ADC_Init()
{
    hadc1.Instance=ADC1;
    hadc1.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;  //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    hadc1.Init.Resolution=ADC_RESOLUTION_12B;            //12λģʽ
    hadc1.Init.DataAlign=ADC_DATAALIGN_RIGHT;            //�Ҷ���
    hadc1.Init.ScanConvMode=DISABLE;                     //��ɨ��ģʽ
    hadc1.Init.EOCSelection=DISABLE;                     //�ر�EOC�ж�
    hadc1.Init.ContinuousConvMode=DISABLE;               //�ر�����ת��
    hadc1.Init.NbrOfConversion=1;                        //1��ת���ڹ��������� Ҳ����ֻת����������1 
    hadc1.Init.DiscontinuousConvMode=DISABLE;            //��ֹ����������ģʽ
    hadc1.Init.NbrOfDiscConversion=0;                    //����������ͨ����Ϊ0
    hadc1.Init.ExternalTrigConv=ADC_SOFTWARE_START;      //�������
    hadc1.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    hadc1.Init.DMAContinuousRequests=DISABLE;            //�ر�DMA����
    HAL_ADC_Init(&hadc1);                                //��ʼ��  
    
    // ���� ADC ���ź�ͨ��
//    current_adc_type = IO_ADCx;  // ����ȫ�ֱ���
    //HAL_ADC_MspInit(&hadc1);
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
	
//    IO_ADC_ENUM IO_ADCx  = current_adc_type;
	
    // ʹ�� GPIO ʱ��
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // ���� ADC ����
	GPIO_Initure.Mode = GPIO_MODE_ANALOG;
	GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Pin = GPIO_PIN_1;  // PB1
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
 
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //1������
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&hadc1);                               //����ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //��ѯת��
   
	return (u16)HAL_ADC_GetValue(&hadc1);	            //�������һ��ADC1�������ת�����
}


uint16_t IO_ADC_Read(u32 ch, u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
//			printf("___________________________________________________temp_val :%ld \r\n", temp_val );
		delay_ms(5);
	}
	return temp_val/times;
}
