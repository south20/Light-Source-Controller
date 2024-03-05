// IO_ADC.h

#ifndef IO_ADC_H
#define IO_ADC_H

#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"

#include <sys.h>
typedef struct {
    GPIO_TypeDef* gpio_port;  // GPIO 引脚所在的端口
    uint16_t gpio_pin;        // GPIO 引脚编号
    uint32_t adc_channel;     // ADC 通道编号
} IO_ADC_Channel;

typedef enum {
    IO_ADC_MIN = 0,
    IO_ADC_LP_FBK,       // 光能量反馈 0-10V模拟量，12位 引脚PB1
    IO_ADC_CUR_FBK,      // 光能量反馈 0-10v模拟量，12位 引脚PB0
    IO_ADC_CUR_CMD,      // 电源功率控制 0-10v模拟量，12位 引脚PA5
    IO_ADC_MAX
} IO_ADC_ENUM;

void IO_ADC_Init();  // ADC通道初始化

u16 Get_Adc(u32 ch);  // 获得某个通道值 

uint16_t IO_ADC_Read(u32 ch, u8 times);  // 得到某个通道给定次数采样的平均值

extern IO_ADC_ENUM current_adc_type;  // 全局变量，存储当前使用的引脚类型

#endif /* IO_ADC_H */
