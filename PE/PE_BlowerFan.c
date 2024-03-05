
#include "PE_BlowerFan.h"
#include "stm32f7xx_hal.h"

// 定义PWM相关参数
//#define PWM_FREQUENCY 25000
//#define PWM_PERIOD (1000000 / PWM_FREQUENCY) // 计算PWM周期（以微秒为单位）

//风扇转速的捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到高低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM9CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM9CH2_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)

//鼓风机转速
u8  TIM9CH1_CAPTURE_STA=0;			    				
u16	TIM9CH1_CAPTURE_VAL;



TIM_HandleTypeDef TIM9_Handler; 


// 声明TIM句柄和配置结构体

TIM_HandleTypeDef TIM3_Handler;         //定时器句柄 
TIM_OC_InitTypeDef TIM3CH1Handler;   


//arr：自动重装值(TIM2,TIM5是32位的!!TIM9是16位)
//psc：时钟预分频数


void PE_BlowerFan_Init()
{
	u16 arr = 39;
	u16 psc = 107;

    TIM3_Handler.Instance=TIM3;            //定时器3
    TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_Handler.Init.Period=arr;          //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
    TIM3CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM3CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3CH1Handler,TIM_CHANNEL_1);//配置TIM3通道1
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//开启PWM通道1


}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_6;           	//PA6
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;	//PB1复用为TIM3_CH4
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


//设置TIM通道4的占空比
//compare:比较值
void TIM_SetTIM3Compare1(u32 compare)
{
	TIM3->CCR1=compare; 
}

//获取TIM捕获/比较寄存器值
u32 TIM_GetTIM3Capture1(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_1);
}



// 控制函数
int PE_BlowerFan_Control(uint32_t cycle) 
{
	//cycle：510-3400
	//占空比有效范围temp_duty_cycle：10%-100%            4 40
	//cycle = k* temp_duty_cycle + b

//	（0.1，510） （1，3400）
//
//k = 2890 /0.9 = 3211.11
//510 = 321.111 + b
//b = 178.889

	double duty_cycle;
	double temp_duty_cycle; //实际占空比
	int rtn = 0;
//    rtn = IO_DS1821_ReadTemperature(&temperature); // 读取温度
//
//    // 根据温度调整鼓风机速度
//    uint32_t duty_cycle;
//    if (temperature > 100) 
//	{
//        // 设置为最大速度
//        duty_cycle = 39;
//    } 
//	else 
//	{
//        // 设置为最小速度
//        duty_cycle = 15;
//    }
	temp_duty_cycle = (cycle - 133.34)* 1000 / 3766660;
	duty_cycle = temp_duty_cycle *40;
		printf("temp_duty_cycle:%lf duty_cycle:%lf  \r\n", temp_duty_cycle, duty_cycle);
	TIM_SetTIM3Compare1(duty_cycle);

    // 更新PWM占空比以控制速度
   	// sConfigOC.Pulse = duty_cycle;
    //HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_1);
    //HAL_TIM_PWM_Start(&htimx, TIM_CHANNEL_1);
	
	return rtn;
}

void CoolingFan_TIM9_CH2_Cap_Init()
{  
    TIM_IC_InitTypeDef TIM9_CH2Config = {0};  
    
    TIM9_Handler.Instance=TIM9;                          //通用定时器5
    TIM9_Handler.Init.Prescaler=215;                     //分频
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM9_Handler.Init.Period=0xFFFF;                        //自动装载值
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM9_Handler);
    
    TIM9_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM9_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM9_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM9_CH2Config.ICFilter=8;                          //配置输入滤波器
    HAL_TIM_IC_ConfigChannel(&TIM9_Handler,&TIM9_CH2Config,TIM_CHANNEL_2);//配置TIM5通道1
    HAL_TIM_IC_Start_IT(&TIM9_Handler,TIM_CHANNEL_2);   //开始捕获TIM5的通道1
    __HAL_TIM_ENABLE_IT(&TIM9_Handler,TIM_IT_UPDATE);   //使能更新中断
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


//定时器5底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器5句柄

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{

	    GPIO_InitTypeDef GPIO_Initure;
		GPIO_InitTypeDef GPIO_Initure2;
	
		__HAL_RCC_TIM9_CLK_ENABLE();            //使能TIM9时钟
	    __HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOE时钟
		
	    GPIO_Initure.Pin=GPIO_PIN_5;            //PE5
	    GPIO_Initure.Mode=GPIO_MODE_AF_PP;    
	    GPIO_Initure.Pull=GPIO_PULLUP;        
	    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
	    GPIO_Initure.Alternate=GPIO_AF3_TIM9;   //PE5复用为TIM9通道1
	    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
	    GPIO_Initure2.Pin=GPIO_PIN_6;            //Pe6
	    GPIO_Initure2.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
	    GPIO_Initure2.Pull=GPIO_PULLUP;        //上拉
	    GPIO_Initure2.Speed=GPIO_SPEED_HIGH;     //高速
	    GPIO_Initure2.Alternate=GPIO_AF3_TIM9;   //PE6复用为TIM9通道2
	    HAL_GPIO_Init(GPIOE,&GPIO_Initure2);

	    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,2,0);    //设置中断优先级，抢占优先级2，子优先级0
	    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);          //开启ITM9中断   
	
}
	

// 中断服务函数，用于计算鼓风机转速

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
        // 捕获状态和值的处理
        if((TIM9CH2_CAPTURE_STA & 0X80) == 0) // 还未成功捕获
        {
            if(TIM9CH2_CAPTURE_STA & 0X40) // 捕获到一个下降沿
            {          
                TIM9CH2_CAPTURE_STA |= 0X80;  // 标记成功捕获到一次高电平脉宽
                TIM9CH2_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // 获取当前的捕获值

                // 重新设置捕获模式
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2);  // 清除原来的设置
                TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_ICPOLARITY_RISING); // 配置为上升沿捕获
            }
            else // 还未开始, 第一次捕获上升沿
            {
                TIM9CH2_CAPTURE_STA = 0;   // 清空
                TIM9CH2_CAPTURE_VAL = 0;
                TIM9CH2_CAPTURE_STA |= 0X40;  // 标记捕获到了上升沿

                __HAL_TIM_DISABLE(htim); // 关闭定时器9
                __HAL_TIM_SET_COUNTER(htim, 0);
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2);  // 清除原来的设置
                TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_ICPOLARITY_FALLING); // 设置为下降沿捕获
                __HAL_TIM_ENABLE(htim); // 重新使能定时器9
            }         
        }
    }
}
int PE_CoolingFan_CheckSpeed(int* fan_rpm)
{
	int rtn=0;
	long temp=0;  
	long temp_t=0;
	if(TIM9CH2_CAPTURE_STA&0X80)		//成功捕获到了一次高电平
	{
		temp=TIM9CH2_CAPTURE_STA&0X3F; 
		temp*=0XFFFF;				//溢出时间总和
		temp+=TIM9CH2_CAPTURE_VAL;		//得到总的高电平时间
		printf("HIGH:%ld us\r\n",temp);//打印总的高点平时间
		temp_t=temp*4;
		TIM9CH2_CAPTURE_STA=0;			//开启下一次捕获
	}
	*fan_rpm = 60000000 / temp_t;

	return rtn;
}

int PE_BlowerFan_CheckSpeed(int* fan_rpm)
{
	int rtn=0;
	long temp=0;  
	long temp_t=0;
	if(TIM9CH1_CAPTURE_STA&0X80)		//成功捕获到了一次高电平
	{
		temp=TIM9CH1_CAPTURE_STA&0X3F; 
		temp*=0XFFFF;				//溢出时间总和
		temp+=TIM9CH1_CAPTURE_VAL;		//得到总的高电平时间
		printf("HIGH:%ld us\r\n",temp);//打印总的高点平时间
		temp_t=temp*4;
		TIM9CH1_CAPTURE_STA=0;			//开启下一次捕获
	}
	*fan_rpm = 60000000 / temp_t;
	return rtn;

}

