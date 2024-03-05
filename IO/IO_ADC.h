// IO_ADC.h

#ifndef IO_ADC_H
#define IO_ADC_H

#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"

#include <sys.h>
typedef struct {
    GPIO_TypeDef* gpio_port;  // GPIO �������ڵĶ˿�
    uint16_t gpio_pin;        // GPIO ���ű��
    uint32_t adc_channel;     // ADC ͨ�����
} IO_ADC_Channel;

typedef enum {
    IO_ADC_MIN = 0,
    IO_ADC_LP_FBK,       // ���������� 0-10Vģ������12λ ����PB1
    IO_ADC_CUR_FBK,      // ���������� 0-10vģ������12λ ����PB0
    IO_ADC_CUR_CMD,      // ��Դ���ʿ��� 0-10vģ������12λ ����PA5
    IO_ADC_MAX
} IO_ADC_ENUM;

void IO_ADC_Init();  // ADCͨ����ʼ��

u16 Get_Adc(u32 ch);  // ���ĳ��ͨ��ֵ 

uint16_t IO_ADC_Read(u32 ch, u8 times);  // �õ�ĳ��ͨ����������������ƽ��ֵ

extern IO_ADC_ENUM current_adc_type;  // ȫ�ֱ������洢��ǰʹ�õ���������

#endif /* IO_ADC_H */
