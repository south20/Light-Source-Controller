#include "IO_ADC.h"
#include "delay.h"
#include "stm32f7xx_hal_adc.h"
//#include "stm32f7xx_hal_adc_ex.h"

ADC_HandleTypeDef hadc1;//ADC句柄
//
//// IO_ADC_Channel 结构体数组，存储每个 IO_ADC_ENUM 对应的引脚和 ADC 通道信息
//const IO_ADC_Channel io_adc_channels[IO_ADC_MAX] = {
//    {GPIOB, GPIO_PIN_1, ADC_CHANNEL_9},  // IO_ADC_LP_FBK
//    {GPIOB, GPIO_PIN_0, ADC_CHANNEL_1},  // IO_ADC_CUR_FBK
//    {GPIOA, GPIO_PIN_5, ADC_CHANNEL_2}   // IO_ADC_CUR_CMD
//};

IO_ADC_ENUM current_adc_type = IO_ADC_MIN;  // 初始化为默认引脚类型

//初始化ADC
void IO_ADC_Init()
{
    hadc1.Instance=ADC1;
    hadc1.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;  //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    hadc1.Init.Resolution=ADC_RESOLUTION_12B;            //12位模式
    hadc1.Init.DataAlign=ADC_DATAALIGN_RIGHT;            //右对齐
    hadc1.Init.ScanConvMode=DISABLE;                     //非扫描模式
    hadc1.Init.EOCSelection=DISABLE;                     //关闭EOC中断
    hadc1.Init.ContinuousConvMode=DISABLE;               //关闭连续转换
    hadc1.Init.NbrOfConversion=1;                        //1个转换在规则序列中 也就是只转换规则序列1 
    hadc1.Init.DiscontinuousConvMode=DISABLE;            //禁止不连续采样模式
    hadc1.Init.NbrOfDiscConversion=0;                    //不连续采样通道数为0
    hadc1.Init.ExternalTrigConv=ADC_SOFTWARE_START;      //软件触发
    hadc1.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    hadc1.Init.DMAContinuousRequests=DISABLE;            //关闭DMA请求
    HAL_ADC_Init(&hadc1);                                //初始化  
    
    // 配置 ADC 引脚和通道
//    current_adc_type = IO_ADCx;  // 更新全局变量
    //HAL_ADC_MspInit(&hadc1);
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
	
//    IO_ADC_ENUM IO_ADCx  = current_adc_type;
	
    // 使能 GPIO 时钟
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置 ADC 引脚
	GPIO_Initure.Mode = GPIO_MODE_ANALOG;
	GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Pin = GPIO_PIN_1;  // PB1
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
 
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //1个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&hadc1);                               //开启ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
   
	return (u16)HAL_ADC_GetValue(&hadc1);	            //返回最近一次ADC1规则组的转换结果
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
