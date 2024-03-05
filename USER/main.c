#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "exti.h"
#include "usmart.h"
#include "AP_GE.h"
#include "can.h"
#include "canfestival_master.h"
#include "canfestival_timer.h"
#include "canfestival_can.h"
#include "stm32f7xx_hal_tim.h"
#include "canfestival.h"
#include "IO_encoder.h"
#include "IO_DS1821.h"
#include "timer8.h"

#include "PE_HaloDisplay.h"
#include "PE_BlowerFan.h"

#include "IO_GPIO.h"
#include "IO_ADC.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 实验30
 CAN实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


extern uint32_t capture_index;// 捕获完成标志

extern uint32_t fan_rpm ;
extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值 

extern u8  TIM9CH1_CAPTURE_STA;			    				
extern u16	TIM9CH1_CAPTURE_VAL;


extern u8  TIM9CH2_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM9CH2_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
/**/
static void CheckSDOAndContinue(CO_Data* d, UNS8 nodeId)
{
	UNS32 abortCode;	
	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
		printf("Master : Failed in initializing slave %2.2x, AbortCode :%4.4x \n", nodeId, abortCode);

	/* Finalise last SDO transfer with this node */
	closeSDOtransfer(&masterObjdict_Data, nodeId, SDO_CLIENT);

	//ConfigureSlaveNode(d, nodeId);
}


extern int16_t cnt=0, dir=2, tim2_irq_cnt=0;
//dir 1:减小 0：增加
extern int16_t rev;



//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	IO_GPIO_Pin_State Shutter_Open_sensor = 0;
	IO_GPIO_Pin_State Shutter_Close_sensor = 0;
	IO_GPIO_Pin_State Real_Shutter_Open_sensor = 0;
	IO_GPIO_Pin_State IO_GPIO_IGN_OUT_sensor = 0;
    delay_ms(10);      //消抖
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
			
            break;
		//报警清除
        case GPIO_PIN_1:
			PE_HaloDisplay_Colour(Green);
			IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_ALARM, 0);
			IO_GPIO_Write_Pin(IO_GPIO_LAMP_ALARM, 0);
            break;
        case GPIO_PIN_2:
			PE_HaloDisplay_Colour(Red);
			IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_ALARM, 1);
			IO_GPIO_Write_Pin(IO_GPIO_LAMP_ALARM, 1);
            break;
        case GPIO_PIN_3:
			if( HAL_GPIO_ReadPin(GPIOH,GPIO_Pin) == GPIO_PIN_SET )
			{
				AP_Up_Level(1);
			}
			else
			{
			}
            break;
        case GPIO_PIN_4:
			if( HAL_GPIO_ReadPin(GPIOB,GPIO_Pin) == GPIO_PIN_SET )
			  {
				AP_Down_Level(1);
			  }
            break;
        case GPIO_PIN_8:
			
			IO_GPIO_Read_Pin(IO_GPIO_SHU_OPEN, &Shutter_Open_sensor);
			IO_GPIO_Read_Pin(IO_GPIO_SHU_CLOSE, &Shutter_Close_sensor);
			Real_Shutter_Open_sensor = (1 - Shutter_Close_sensor) ;
			if(Shutter_Close_sensor)
			{
				PE_Shutter_Move_Abs(-1);
			}
			else if(Real_Shutter_Open_sensor)
			{
				PE_Shutter_Move_Abs(35);
			}
			else 
			{
				PE_Shutter_Move_Abs(35);
			}
			
            break;
        case GPIO_PIN_9:
			
			IO_GPIO_Read_Pin(IO_GPIO_IGN_OUT, &IO_GPIO_IGN_OUT_sensor);
			//关闭定时器
			if(IO_GPIO_IGN_OUT_sensor)
			{
				__HAL_TIM_CLEAR_FLAG(&TIM8_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
				__HAL_TIM_CLEAR_FLAG(&TIM1_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
				HAL_TIM_Base_Stop(&TIM8_Handler);
				HAL_TIM_Base_Stop(&TIM1_Handler);
			
			}
			//打开定时器
			else 
			{
				PE_HaloDisplay_Colour(Red);
				__HAL_TIM_CLEAR_FLAG(&TIM8_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
				__HAL_TIM_CLEAR_FLAG(&TIM1_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
				HAL_TIM_Base_Start(&TIM8_Handler);
				HAL_TIM_Base_Start(&TIM1_Handler);
			
			}
			
            break;

        case GPIO_PIN_10:
            break;
			
        case GPIO_PIN_11:
			AP_Set_Led_Level(100);
            break;
        case GPIO_PIN_12:
			AP_Set_Led_Level(0);
            break;
        case GPIO_PIN_13:
			PE_HaloDisplay_Colour(Red);
			IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_ALARM, 0);
//			IO_GPIO_Write_Pin(IO_GPIO_LAMP_ALARM, 0);
            break;
        case GPIO_PIN_5:
			IO_GPIO_Read_Pin(IO_GPIO_SHU_OPEN, &Shutter_Open_sensor);
			IO_GPIO_Read_Pin(IO_GPIO_SHU_CLOSE, &Shutter_Close_sensor);
			Real_Shutter_Open_sensor = (1 - Shutter_Close_sensor) ;
			if(Shutter_Close_sensor)
			{
				PE_Shutter_Move_Abs(-1);
			}
			else if(Real_Shutter_Open_sensor)
			{
				PE_Shutter_Move_Abs(35);
			}
			else 
			{
				PE_Shutter_Move_Abs(35);
			}
			
            break;
		default:
			break;
    }
}

//回调函数，所有定时器公用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	int time_a = 0;
	if(htim->Instance==TIM5)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim_enc))   
	    {
	        rev--;
	    }
	    else
	    {
	        rev++;
	    }
	}
  
	if(htim->Instance==TIM9 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if((TIM9CH1_CAPTURE_STA&0X80)==0)
		{
				if(TIM9CH1_CAPTURE_STA&0X40)
				{
					if((TIM9CH1_CAPTURE_STA&0X3F)==0X3F)
					{
						TIM9CH1_CAPTURE_STA|=0X80;		
						TIM9CH1_CAPTURE_VAL=0XFFFF;
					}
					else 
					{
						TIM9CH1_CAPTURE_STA++;
					}
				}	 
		}
	}
	if(htim->Instance==TIM9 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if((TIM9CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
		{
				if(TIM9CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM9CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
					{
						TIM9CH2_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM9CH2_CAPTURE_VAL=0XFFFF;
					}
					else 
					{
						TIM9CH2_CAPTURE_STA++;
					}
				}	 
		}	
	}
//	if(htim->Instance==TIM8)
//	{	
//		time_a = DS18B21_Get_Time();
//	
//		DS18B21_Set_Time(time_a+1);
//	}
    if(htim==(&TIM8_Handler))
    {
        WorkingHour++;
		time_a = DS18B21_Get_Time();
	
		DS18B21_Set_Time(time_a+1);
    }
}

int main(void)
{
	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 res;
	u8 mode=1;
  int slave_num = 0;
  int rtn = 0;
	printf("Ready in Cache_Enable() ...\r\n");

	Cache_Enable();                 //打开L1-Cache
	printf("Ready in HAL_Init() ...\r\n");
	HAL_Init();				        //初始化HAL库
	printf("Ready in Stm32_Clock_Init() ...\r\n");
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz
	printf("Ready in delay_init() ...\r\n"); 
	delay_init(216);                //延时初始化
	printf("Ready in uart_init() ...\r\n");
	uart_init(115200);		        //串口初始化
	printf("Ready in TIMConfig() ...\r\n");
	TIMConfig(TIM14, 10000, 107);     //1us计数一次
	printf("Ready in CANOpen_can_config() ...\r\n");
	CANOpen_can_config();
	
//		HAL_TIM_Base_Start_IT(&TIM14);	//开启定时器终中断，计数超过设定值产生中断，进入回调函数
//	  HAL_TIM_Encoder_Start(&TIM14, TIM_CHANNEL_ALL);//启动编码器定时模式
//	printf("Ready in usmart_dev.init() ...\r\n");
//	usmart_dev.init(108); 		    //初始化USMART
	printf("Ready in LED_Init() ...\r\n");

	EXTI_Init();					//外部中断初始化

//	LED_Init();                     //初始化LED
	KEY_Init();                     //初始化按键
	SDRAM_Init();                   //初始化SDRAM
	LCD_Init();                     //LCD初始化
	BACK_COLOR = WHITE;
	 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"CAN TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/7/12");
	
	

	Test_Process();
	BackGroundProcess();

	
	return 0;

}

int Test_Process()
{

    u16 adcx;
	float temp;
//
//	IO_ADC_Init(IO_ADC_LP_FBK);
//	while(1)
//	{
//		adcx = IO_ADC_Read(IO_ADC_LP_FBK, 20);
//		printf("adcx :%d \r\n", adcx );
//		temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
//		adcx=temp;                            //赋值整数部分给adcx变量，因为adcx为u16整形
//		temp-=adcx;                           //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
//		temp*=1000;                           //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
//		printf("temp :%ld \r\n", temp );
//		delay_ms(250);	
//
//	}
    u8 len;
	u16 times=0; 
	int fan_rpm = 0;
	int blowerfan_rpm = 0;

		IO_GPIO_Pin_State Zero_sensor = 0;
		IO_GPIO_Pin_State bitstatus = 0;
	
//		IO_DS1821_Init();
		//温度监测传感器
	//	while(1)
	//	{
	//			IO_DS1821_ReadTemperature(&time);
	//			printf("time:%d \r\n", time);
	//			delay_ms(300);	 
	//
	//	}
	
	
	AP_Initialize();
//    TIM8_Init(50000-1,10800-1);      //定时器3初始化，定时器时钟为108M，分频系数为10800-1，
         
		DS18B21_Set_Time(5);                           //所以定时器3的频率为108M/10800=10K，自动重装载为5000-1，那么定时器周期就是500ms

	delay_ms(1000);	
//	AP_Set_Led_Level(0);
	PE_Shutter_Close();

	//IO_GPIO_Init_All_Clocks();
	//IO_GPIO_Init_All_Pins(); 
	//初始化时需要给默认信号的引脚
//	IO_GPIO_Write_Pin(IO_GPIO_VA_ENABLE, IO_GPIO_PIN_RESET);
//	IO_GPIO_Write_Pin(IO_GPIO_Shutter_ENABLE, IO_GPIO_PIN_RESET);
//	IO_GPIO_Write_Pin(IO_GPIO_LAMP_ON, IO_GPIO_PIN_SET);
//	IO_GPIO_Write_Pin(IO_GPIO_SP_OUT1, IO_GPIO_PIN_SET);
//	IO_GPIO_Write_Pin(IO_GPIO_LAMP_ALARM, IO_GPIO_PIN_SET);
//	IO_GPIO_Write_Pin(IO_GPIO_LAMP_COOL, IO_GPIO_PIN_SET);
//	IO_GPIO_Write_Pin(IO_GPIO_SP_OUT2, IO_GPIO_PIN_SET);
//	IO_GPIO_Write_Pin(IO_GPIO_ABS_EN, IO_GPIO_PIN_SET);
//	IO_GPIO_Write_Pin(IO_GPIO_IGN_OUT, IO_GPIO_PIN_SET);
	//VA电机测试
	//PE_Motor_Init();
	
    char ATT[] = "AAA";
    char BTT[] = "BBB!";
	uint16_t time = 0;
	short temperature= 0;
		IO_GPIO_Pin_State IO_GPIO_IGN_OUT_sensor = 0;
	
//	PE_VA_Home();
//	PE_Shutter_Home();
//	
//	IO_GPIO_Read_Pin(IO_GPIO_ZERO_POS, &Zero_sensor);
//	printf("Zero_sensor:%d \r\n", Zero_sensor);
//			delay_ms(3000);
	int cnt = 0; 
	int rtn = 0;
	
	float temp_light_value = 0.0;
	
	//PE_BlowerFan_Init();
	//BlowerFan_TIM9_CH1_Cap_Init();
	//CoolingFan_TIM9_CH2_Cap_Init();


	
	IO_Encoder_Tim_Init(4095, 0);
	IO_Encoder_Start_Count();
	
	IO_ADC_Init(IO_ADC_LP_FBK);
	
//			IO_GPIO_Read_Pin(IO_GPIO_IGN_OUT, &IO_GPIO_IGN_OUT_sensor);
//			//关闭定时器
//			if(IO_GPIO_IGN_OUT_sensor)
//			{
//				__HAL_TIM_CLEAR_FLAG(&TIM8_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
//				__HAL_TIM_CLEAR_FLAG(&TIM1_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
//				HAL_TIM_Base_Stop(&TIM8_Handler);
//				HAL_TIM_Base_Stop(&TIM1_Handler);
//			
//			}
//			//打开定时器
//			else 
//			{
//				PE_HaloDisplay_Colour(Red);
//				__HAL_TIM_CLEAR_FLAG(&TIM8_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
//				__HAL_TIM_CLEAR_FLAG(&TIM1_Handler, TIM_IT_UPDATE);	//先清中断标志位再开启中断，否则上电就会进入一次中断
//				HAL_TIM_Base_Start(&TIM8_Handler);
//				HAL_TIM_Base_Start(&TIM1_Handler);
//			
//			}
			
//	while(1)
	for(int j = 0; j < 1000; ++j)
	{
		printf("-------------->   j = %d \r\n", j);
		if(j == 30)
		{
			PE_Close_Hg_Light();
		}
		//PE_BlowerFan_Control(3400);
		IO_GPIO_Pin_State stateUP = 0;
		IO_GPIO_Pin_State stateDown = 0;
		IO_GPIO_Pin_State stateOK = 0;
		IO_GPIO_Pin_State stateStart = 0;
		IO_GPIO_Pin_State stateLeft = 0;
		IO_GPIO_Pin_State stateRight = 0;
		IO_GPIO_Pin_State Zero_sensor = 0;
		IO_GPIO_Pin_State Shutter_Open_sensor = 0;
		IO_GPIO_Pin_State Real_Shutter_Open_sensor = 0;
		IO_GPIO_Pin_State Shutter_Close_sensor = 0;
		IO_GPIO_Pin_State FILTER_DETECT1_sensor = 0;
		IO_GPIO_Pin_State FILTER_DETECT2_sensor = 0;
		IO_GPIO_Pin_State SHUTTER_ON_OFF_sensor = 0;
		IO_GPIO_Pin_State FILTER_DETECT3_sensor = 0;
		IO_GPIO_Pin_State Alarm_Clear_sensor = 0;
		IO_GPIO_Pin_State SYS_interlock_sensor = 0;
		IO_GPIO_Pin_State shutter_interlock_sensor = 0;
		IO_GPIO_Pin_State lamp_on_off_sensor = 0;
		IO_GPIO_Pin_State LG_DTKT1_sensor = 0;
		IO_GPIO_Read_Pin(IO_GPIO_LG_DTKT1, &LG_DTKT1_sensor);
//		printf("LG_DTKT1_sensor ：%d! \r\n", LG_DTKT1_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_UP, &stateUP);
		IO_GPIO_Read_Pin(IO_GPIO_DOWN, &stateDown);
		IO_GPIO_Read_Pin(IO_GPIO_START, &stateStart);
		IO_GPIO_Read_Pin(IO_GPIO_OK, &stateOK);
		IO_GPIO_Read_Pin(IO_GPIO_LEFT, &stateLeft);
		IO_GPIO_Read_Pin(IO_GPIO_RIGHT, &stateRight);
		IO_GPIO_Read_Pin(IO_GPIO_SHU_OPEN, &Shutter_Open_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_SHU_CLOSE, &Shutter_Close_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_FILTER_DETECT1, &FILTER_DETECT1_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_FILTER_DETECT2, &FILTER_DETECT2_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_FILTER_DETECT3, &FILTER_DETECT3_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_SHUTTER_ON_OFF, &SHUTTER_ON_OFF_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_ALARM_CLEAR, &Alarm_Clear_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_SYS_INIERLOCK, &SYS_interlock_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_SHUTTER_INTERLOCK, &shutter_interlock_sensor);
		IO_GPIO_Read_Pin(IO_GPIO_LAMP_ON_OFF, &lamp_on_off_sensor);
		Real_Shutter_Open_sensor = (1 - Shutter_Close_sensor) ;
//		printf("FILTER_DETECT1_sensor:%d,FILTER_DETECT2_sensor:%d,FILTER_DETECT3_sensor:%d\r\n",FILTER_DETECT1_sensor, FILTER_DETECT2_sensor, FILTER_DETECT3_sensor);//打印总的高点平时间
//		printf("stateLeft:%d,stateRight:%d\r\n",stateLeft, stateRight);//打印总的高点平时间
		printf("Shutter_Open_sensor:%d, Shutter_Close_sensor = :%d , SHUTTER_ON_OFF_sensor = :%d,  Alarm_Clear_sensor =: %d,  SYS_interlock_sensor =: %d\r\n", Shutter_Open_sensor, Shutter_Close_sensor, SHUTTER_ON_OFF_sensor, Alarm_Clear_sensor, SYS_interlock_sensor);
		printf("shutter_interlock_sensor:%d, lamp_on_off_sensor =: %d \r\n", shutter_interlock_sensor, lamp_on_off_sensor);
		printf("stateUP:%d, stateDown:%d,stateStart:%d,stateOK:%d,stateLeft:%d,stateRight:%d\r\n",stateUP, stateDown, stateStart, stateOK, stateLeft, stateRight);//打印总的高点平时间

		
//		IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_ALARM, 1);
//		IO_GPIO_Write_Pin(IO_GPIO_LAMP_ALARM, 1);
//		IO_GPIO_Write_Pin(IO_GPIO_LAMP_COOL, 1);
//		IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_VERIFY, 1);
//		IO_GPIO_Write_Pin(IO_GPIO_LAMP_ON, 1);
//		DS18B21_Set_Time(300);

		time = DS18B21_Get_Time();
		printf("=================================================================time:%d \r\n", time);
		temperature = DS18B21_Get_Temp();
		printf("temperature:%d \r\n", temperature);
		if(temperature > 130)
		{
//			printf("temperature is too big,over range!:%d \r\n", temperature);
		}
//		delay_ms(300);	 		//判断按键是否触发，通过按键控制电机运动
		//TODO:20231222 需要实现通过按键调节光阑档位功能
		//PH3
		//20240116暂时屏蔽while1中循环控制档位功能
//		if(1 == stateUP)
//		{
//			AP_Up_Level(5);
////			PE_VA_Move_Rel(1);
//
//			
//		}
//		//PA3
//		if(1 == stateDown)
//		{
//			AP_Down_Level(5);
////			PE_VA_Move_Rel(-1);
//		}
//		
//		//PG11
//		if(1 == stateLeft)
//		{
//			AP_Set_Led_Level(100);
////			PE_Shutter_Move_Rel(20);
////			delay_ms(100);
//			
//		}
//		//PG12
//		if(1 == stateRight)
//		{
//			AP_Set_Led_Level(0);
////			PE_Shutter_Move_Abs(35);
////			delay_ms(100);
//		}
//		
//		//PH8
//		if(1 == stateStart)
//		{
//			if(Shutter_Close_sensor)
//			{
//				PE_Shutter_Move_Abs(-1);
//			}
//			else if(Real_Shutter_Open_sensor)
//			{
//				PE_Shutter_Move_Abs(35);
//			}
//			else 
//			{
//				PE_Shutter_Move_Abs(35);
//			}
//			
//		}
//		

		for(int i = 0 ; i <5 ; ++i)
		{
			adcx = IO_ADC_Read(ADC_CHANNEL_9, 20);
			printf("adcx :%d \r\n", adcx );
			temp_light_value=(float)adcx*(2.5/4096);		  //获取计算后的带小数的实际电压值，比如3.1111
			adcx=temp_light_value;							  //赋值整数部分给adcx变量，因为adcx为u16整形
			temp_light_value-=adcx; 						  //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
			temp_light_value*=1000; 						  //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
			printf("temp_light_value :%ld \r\n", temp_light_value );
			delay_ms(100);	

		}

		IO_Encoder_Get_Only_Count_Val();
		IO_Encoder_Get_Only_Rel_Pos();
		IO_Encoder_Get_Rev();
		
		PE_CoolingFan_CheckSpeed(&fan_rpm);
//		delay_ms(200);	

		PE_BlowerFan_CheckSpeed(&blowerfan_rpm);
//		delay_ms(200);	
		
		if(blowerfan_rpm > 3200)
			{}
		else
		{
			PE_Close_Hg_Light();
			rtn = ERROR_FAN_or_BlowerFAN;
		}
		if(fan_rpm > 5999)
			{}
		else
		{
			PE_Close_Hg_Light();
			rtn = ERROR_FAN_or_BlowerFAN;
		}
		printf(" blowerfan_rpm: %d!, fan_rpm ：%d!\r\n",  blowerfan_rpm, fan_rpm);
			delay_ms(500);
//		IO_GPIO_Read_Pin(IO_GPIO_ZERO_POS, &Zero_sensor);
//		printf("Zero_sensor:%d \r\n", Zero_sensor);
//		PE_HaloDisplay_Colour(Blue);
//		delay_ms(200);
		//PE_HaloDisplay_Colour(Green);
		//delay_ms(1000);
//		PE_HaloDisplay_Colour(Red);
//		  delay_ms(1000);
//		PE_HaloDisplay_Colour(Close);
//		  delay_ms(1000);
//		PE_HaloDisplay_Colour(Purple);
//		  delay_ms(1000);
//		PE_HaloDisplay_Colour(Yellow);
//		  delay_ms(1000);

//	温度检测功能和汞灯使用时长读取功能
//		short temperature = 0;
//		int time_value = 0;
//		int i_a = 0;
//		while(1)
//		{		
//			temperature=DS18B21_Get_Temp(); 
//			printf("temperature = %hd \n", temperature);
//			time_value = DS18B21_Get_Time();
//			printf("time_value = %hd \n", time_value);
//			DS18B21_Set_Time(5);
//			//i_a++;
//		}
	}

	
		for(int i  = 0; i < 5; ++i)
		{
			PE_VA_Get_Zero_Pos_Sensor(&bitstatus);
			printf("Start bitstatus:%d \r\n", bitstatus);
			delay_ms(300);   
		}
	//VA电机测试
	PE_Motor_Init();
//	PE_VA_Move_Rel(-200); 
		for(int i  = 0; i < 5; ++i)
		{
			PE_VA_Get_Zero_Pos_Sensor(&bitstatus);
			printf("before home bitstatus:%d \r\n", bitstatus);
			delay_ms(300);   
		}
	PE_VA_Home();
		for(int i  = 0; i < 5; ++i)
		{
			PE_VA_Get_Zero_Pos_Sensor(&bitstatus);
			printf("Aftere home bitstatus:%d \r\n", bitstatus);
			delay_ms(300);   
		}
	PE_VA_Move_Rel(-50); 
	while(1)
	{
		PE_VA_Get_Zero_Pos_Sensor(&bitstatus);
		printf("bitstatus:%d \r\n", bitstatus);
			delay_ms(300);   
	}

	//光导管监测传感器
//	while(1)
//	{
//		PE_Get_GPIO_LightTubeDetectionSensor(&bitstatus);
//			printf("bitstatus:%d \r\n", bitstatus);
//			delay_ms(300);   
//
//	}
	//零位监测传感器
//	while(1)
//	{
//		IO_GPIO_Read_Pin(IO_GPIO_ZERO_POS, &Zero_sensor);
//		printf("Zero_sensor:%d \r\n", Zero_sensor);
//        delay_ms(200); 
//
//	}

//风扇鼓风机测试
//	PE_BlowerFan_Init();
//	BlowerFan_TIM9_CH1_Cap_Init();
////	CoolingFan_TIM9_CH2_Cap_Init();
//	PE_BlowerFan_Control(2000);
//
//	while(1)
//	{
////		PE_CoolingFan_CheckSpeed(&fan_rpm);
//		PE_BlowerFan_CheckSpeed(&fan_rpm);
//		printf("fan_rpm:%d \r\n", fan_rpm);
//			delay_ms(300);   
//	}
	//IO_DS1821_Init();
	//温度监测传感器
//	while(1)
//	{
//			IO_DS1821_ReadTemperature(&time);
//			printf("time:%d \r\n", time);
//			delay_ms(300);   
//
//	}

//上位机通讯串口指令切换
//	while(1)
//	{
//			//A();
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的消息为:\r\n");
//			
//			
//		
//			// 检查接收缓冲区中的数据是否与比较字符串一致
//			if (strcmp((char*)USART_RX_BUF, ATT) == 0) 
//				{
//				// 调用A()函数
//				A();
//			}
//			else if(strcmp((char*)USART_RX_BUF, BTT) == 0)
//			{
//				B();
//			}
//		
//			//A();
//			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
//			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
//			printf("\r\n\r\n");//插入换行
//			USART_RX_STA=0;
//		}
//		else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\r\nALIENTEK STM32F7开发板 串口实验\r\n");
//				printf("正点原子@ALIENTEK\r\n\r\n\r\n");
//			}
//			if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
//			if(times%30==0)LED0_Toggle;//闪烁LED,提示系统正在运行.
//			delay_ms(10);	
//		}
//
//	}



	return 0;
}

int A()
{
	printf("This is A! \r\n");
}

int B()
{
	printf("This is B! \r\n");
}

int BackGroundProcess()
{
	
	AT24CXX_Init(); 
	while(AT24CXX_Check())//检测不到24c02
	{
		//LCD_ShowString(30,150,200,16,16,"24C02 Check Failed!");
		printf("24C02 Check Failed!\r\n");
		delay_ms(500);
		//LCD_ShowString(30,150,200,16,16,"Please Check!      ");
		printf("Please Check!      !\r\n");
		delay_ms(500);
		LED0_Toggle;//DS0闪烁
	}
	char* send_Buffer={"123456"};
	AP4A_Set_System_Info(send_Buffer);
	char info[32] = {0};
	while(1)
	{
		AP_Get_System_Info(info);
	}

	//delay_ms(1000);
	//printf("Ready in NMT_Start_Node ...\r\n");
	//masterSendNMTstateChange(&masterObjdict_Data, 0x01, NMT_Start_Node);

	//delay_ms(300);


	// printf("Ready in start SYNC ...\r\n");

   // startSYNC(&masterObjdict_Data); 
   
	PE_Motor_Init();
//	delay_ms(2000);

	IO_GPIO_Pin_State m_state = 0;
	IO_GPIO_Pin_State m_state2 = 0;
	
	IO_GPIO_Write_Pin(IO_GPIO_VA_ENABLE, IO_GPIO_PIN_RESET);
	IO_GPIO_Write_Pin(IO_GPIO_Shutter_ENABLE, IO_GPIO_PIN_RESET);
	IO_GPIO_Write_Pin(IO_GPIO_LAMP_ON, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_SP_OUT1, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_LAMP_ALARM, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_LAMP_COOL, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_SP_OUT2, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_ABS_EN, IO_GPIO_PIN_SET);
	
	KEY_Init();

//	IO_Encoder_Tim_Init(4095, 0);
//	IO_Encoder_Start_Count();
	int32_t Count_Val = 0;
	
	IO_GPIO_Pin_State stateUP = 1;
	IO_GPIO_Pin_State stateDown = 1;


	//PE_Motor_Init();
	
//功能：编码器15s内进行计数，然后停止计数并清零延时20s后继续开始计数
//	while(1)
//	{
//		printf("IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count IO_Encoder_Start_Count \r\n");
//		IO_Encoder_Start_Count();
//		for(i = 0; i < 15; ++i)
//		{
//		
//			IO_Encoder_Get_Only_Count_Val();
//			IO_Encoder_Get_Only_Rel_Pos();
//			IO_Encoder_Get_Rev();
//			
//			
//
//
//			    delay_ms(1000);
//		}
//
//		
//		printf("IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count IO_Encoder_Stop_Count \r\n");
//		IO_Encoder_Stop_Count();
//		IO_Encoder_Clear_Count_Val();
//			
//		for(i = 0; i < 20; ++i)
//		{
//		
//			IO_Encoder_Get_Only_Count_Val();
//			IO_Encoder_Get_Only_Rel_Pos();
//			IO_Encoder_Get_Rev();
//			    delay_ms(1000);
//
//		}
//	}
	
	//IO_Encoder_Get_Only_Rel_Pos();
	delay_ms(1000);
//	PE_VA_Home();
//	PE_VA_Move_Rel(400);
//	PE_Shutter_Move_Rel(40);
	IO_Encoder_Get_Only_Rel_Pos();
	

	CoolingFan_TIM9_CH2_Cap_Init(0XFFFF,216-1);
	int fan_rpm = 0;
	long long temp=0;  
	while(1)
	{
		PE_CoolingFan_CheckSpeed(&fan_rpm);
		printf("fan_rpm0:%d \r\n", fan_rpm);
		delay_ms(300);
	}
//	while(1)
//	{
//		printf("fan_rpm1:%d \r\n", fan_rpm);
//
//		if(TIM9CH2_CAPTURE_STA&0X80)		//成功捕获到了一次高电平
//		{
//			temp=TIM9CH2_CAPTURE_STA&0X3F; 
//			temp*=0XFFFF;				//溢出时间总和
//			temp+=TIM9CH2_CAPTURE_VAL;		//得到总的高电平时间
//			printf("HIGH:%lld us\r\n",temp);//打印总的高点平时间
//			
//			TIM9CH2_CAPTURE_STA=0;			//开启下一次捕获
//		}
//		
//		printf("fan_rpm2:%d \r\n", fan_rpm);
//
//	}

//电机编码器功能
	while(1)
	{
		IO_GPIO_Pin_State stateUP = 0;
		IO_GPIO_Pin_State stateDown = 0;
		IO_GPIO_Pin_State stateOK = 0;
		IO_GPIO_Pin_State stateStart = 0;
		IO_GPIO_Pin_State stateLeft = 0;
		IO_GPIO_Pin_State stateRight = 0;
		IO_GPIO_Read_Pin(IO_GPIO_UP, &stateUP);
		IO_GPIO_Read_Pin(IO_GPIO_DOWN, &stateDown);
		IO_GPIO_Read_Pin(IO_GPIO_START, &stateStart);
		IO_GPIO_Read_Pin(IO_GPIO_OK, &stateOK);
		IO_GPIO_Read_Pin(IO_GPIO_LEFT, &stateLeft);
		IO_GPIO_Read_Pin(IO_GPIO_RIGHT, &stateRight);
		printf("stateUP:%d, stateDown:%d,stateStart:%d,stateOK:%d,stateLeft:%d,stateRight:%d\r\n",stateUP, stateDown, stateStart, stateOK, stateLeft, stateRight);//打印总的高点平时间
		if(1 == stateUP)
		{
			PE_Shutter_Move_Rel(40);
		    delay_ms(1000);
			
		}
		if(1 == stateDown)
		{
			PE_Shutter_Move_Rel(-40);
		    delay_ms(1000);
		}
		IO_Encoder_Get_Only_Rel_Pos();
		IO_Encoder_Get_Rel_Pos();
		IO_Encoder_Get_Count_Val();
		IO_Encoder_Get_Only_Count_Val();
		IO_Encoder_Get_Rev();
//		PE_HaloDisplay_Colour(Blue);
        delay_ms(10);
		//PE_HaloDisplay_Colour(Green);
        //delay_ms(1000);
		PE_HaloDisplay_Colour(Red);
        delay_ms(1000);
		PE_HaloDisplay_Colour(Close);
        delay_ms(1000);
		PE_HaloDisplay_Colour(Purple);
        delay_ms(1000);
		PE_HaloDisplay_Colour(Yellow);
        delay_ms(1000);
	}
		//PE_CoolingFan_Init();
    //PWM_Init();  // 初始化PWM
     
	//TIM5_CH1_Cap_Init(0XFFFFFFFF,108-1);// 初始化TIM5输入捕获
//	 long long temp=0; 
//		int fan_rpm = 0;
//		TIM9_CH1_Cap_Init(0XFFFF,108-1);
//	 while(1)
//    {
//        delay_ms(10);
//		 
//		        
//        if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
//		{
//			temp=TIM5CH1_CAPTURE_STA&0X3F; 
//			temp*=0XFFFF;		 	    //溢出时间总和
//			temp+=TIM5CH1_CAPTURE_VAL;      //得到总的高电平时间
//			printf("HIGH:%lld us\r\n",temp);//打印总的高点平时间
//			TIM5CH1_CAPTURE_STA=0;          //开启下一次捕获
//		}
//    } 
}

//
//
//void IO_Init() {
//    // 硬件初始化逻辑
//    // ...
//}
//
//void IO_LightSourceControl(bool enable) {
//    // 控制光源的开关
//    // ...
//}
//
//void IO_TemperatureRead(float *temperature) {
//    // 读取温度传感器的值
//    // ...
//}
//
//void PE_LightSourceManagement() {
//    // 处理光源供电和状态检测
//    // ...
//}
//
//void PE_TemperatureManagement() {
//    // 处理温度检测和散热
//    // ...
//}
//
//void PE_FilterManagement() {
//    // 处理滤光片检测
//    // ...
//}
//
//void AP_Init() {
//    // 初始化任务
//    // ...
//}
//
//void AP_MainLoop() {
//    while (1) {
//        // 前台任务处理
//        AP_ForegroundTasks();
//
//        // 后台任务处理
//        AP_BackgroundTasks();
//    }
//}
//
//void AP_ForegroundTasks() {
//    // 中断服务程序（ISRs）会直接调用这个函数
//    AP_HandleInterrupts();
//
//    // 处理实时事件
//    AP_ProcessRealTimeEvents();
//
//    // 其他前台任务逻辑...
//}
//
//void AP_BackgroundTasks() {
//    // 主循环中的周期性任务
//    AP_PeriodicTasks();
//
//    // 数据处理
//    AP_ProcessData();
//
//    // 通信任务
//    AP_CommunicationTask();
//}
//
//void AP_HandleInterrupts() {
//    // 根据实际情况处理中断
//    // ...
//}
//
//void AP_PeriodicTasks() {
//    // 周期性任务逻辑
//    // ...
//}
//
//void AP_ProcessData() {
//    // 数据处理逻辑
//    // ...
//}
//
//void AP_CommunicationTask() {
//    // 通信任务逻辑
//    // ...
//}
//
//void AP_ProcessRealTimeEvents() {
//    // 实时事件处理逻辑
//    // ...
//}
//


