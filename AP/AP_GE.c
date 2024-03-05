// AP_GE.c 文件

#include "AP_GE.h"
#include "IO_GPIO.h"  // 包含IO层的头文件
#include "IO_ADC.h"  // 包含IO层的头文件
#include "IO_DS1821.h"  // 包含IO层的头文件
#include "timer8.h"  // 包含IO层的头文件
#include <stdio.h>
#include <math.h>
//
////系统状态显示：
//	a)：光导未正确插入 （红色）
//	b): 光源预热中 （紫色）
//	c): 光源已打开   （绿色）
//	d): 快门已打开   （蓝色）
//	e): 光源即将需要更换 （黄色）
#include "24cxx.h"
// 全局变量，表示系统当前状态
static OperationMode currentOperationMode = TIMER_MODE;
static int cur_level = 0;

//要写入到24c02的字符串数组               举例说明，后面可以删除 20240110
const u8 TEXT_Buffer[]={"a123456789b123456789c123456789d123456789"};
#define SIZE sizeof(TEXT_Buffer) 

int AP_Initialize() {
    uint16_t rtn = 0;
	IO_GPIO_Pin_State Hg_State = 0;
	uint16_t cur_temperature = 0;
	uint32_t fan_rpm ;
	uint32_t blowerfan_rpm ;
	int i = 0;

    u16 adcx_close;
    u16 adcx_open;
	float temp_dark_value;//光能量监测传感器 关灯
	float temp_light_value;//光能量监测传感器 开灯
    // 执行初始化流程
    //初始化IO硬件
    IO_GPIO_Init_All_Clocks();
    IO_GPIO_Init_All_Pins();
	IO_DS1821_Init();
	
	AT24CXX_Init();				    //初始化24C02
    // 检测当前汞灯引脚是否打开.
	//IO_GPIO_IGN_OUT 无法读取 只能写，初始化刚开始给该值写1
//	IO_GPIO_Read_Pin(IO_GPIO_IGN_OUT, &Hg_State);
	IO_GPIO_Write_Pin(IO_GPIO_VA_ENABLE, IO_GPIO_PIN_RESET);
	IO_GPIO_Write_Pin(IO_GPIO_Shutter_ENABLE, IO_GPIO_PIN_RESET);
	IO_GPIO_Write_Pin(IO_GPIO_LAMP_ON, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_SP_OUT1, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_LAMP_ALARM, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_LAMP_COOL, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_SP_OUT2, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_ABS_EN, IO_GPIO_PIN_SET);
	IO_GPIO_Write_Pin(IO_GPIO_IGN_OUT, 1);
	
	PE_BlowerFan_Init();
	BlowerFan_TIM9_CH1_Cap_Init();
	CoolingFan_TIM9_CH2_Cap_Init();
	PE_BlowerFan_Control(3400);
	

	TIM1_Configuration();
	TIM8_Configuration();		
	
		DS18B21_Set_Time(60);
	//1.检测当前汞灯使用时长。
	if(0 == rtn)
	{
		short time_hg = 0;
		time_hg = DS18B21_Get_Time();
		if(time_hg > 1800 && time_hg < 2000)
		{
			rtn = PE_HaloDisplay_Yellow(0);
			printf("The light source has been in use for more than 2000 hours. Please replace it as soon as possible.\r\n");
			
			printf("The AAA source has been in use for %d hours\r\n", (int)time_hg);
//			return rtn;
		}
		else if(time_hg >= 2000)
		{

			rtn = PE_HaloDisplay_Red(0);
			rtn  =  ERROR_Initialized;
			printf("The light source has been in use for more than 4000 hours. Please replace it as soon as possible.\r\n");
			return rtn;
		}
	}
	//2.检测当前温度是否满足打开电源条件。
	cur_temperature = DS18B21_Get_Temp();
	 if(cur_temperature > 40)
	{
		rtn = PE_HaloDisplay_Red(0);
		while(cur_temperature > 40)
		{
			cur_temperature = DS18B21_Get_Temp();
		printf("DS1821 Temperature is %d, too high \r\n", cur_temperature);

		}
	}
	else
	{
		//预热阶段显示紫色光环
		rtn = PE_HaloDisplay_Yellow(0);
	}
	
	
	//8.风扇鼓风机测试
	if(0 == rtn)
	{
		
		rtn = PE_HaloDisplay_Yellow(0);

		for(int i = 0 ; i <5 ; ++i)
		{
			PE_CoolingFan_CheckSpeed(&fan_rpm);
			printf("fan_rpm ：%d! \r\n", fan_rpm);
			delay_ms(500);	
		}
		
//		IO_GPIO_Write_Pin(IO_GPIO_BLW_PWM, IO_GPIO_PIN_SET);
		for(int i = 0 ; i <5 ; ++i)
		{
			PE_BlowerFan_CheckSpeed(&blowerfan_rpm);
			printf(" blowerfan_rpm: %d! \r\n",  blowerfan_rpm);
			delay_ms(500);	
		}
		
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
	

	}
	//3.光阑电机回零。零位对应notch检测传感器位置，此时当做0档，逆时针旋转到300度时当做100档。相当于：1档=3度
	if(0 == rtn)
	{
		PE_Motor_Init();
		PE_VA_Home();
		
	
	}
	//4.SHutter回零档位置
	if(0 == rtn)
	{
		PE_Shutter_Home();
	}
	//5.判断是否为SMEE光源
	if(0 == rtn)
	{
		//rtn = PE_Is_SMEE_Light();
	}
	//6.正式运行的时候需要把光阑调到最大档位（或设定档位），调试阶段目前先调到0档。
	//TODO:20231207
	if(0 == rtn)
	{
		rtn = AP_Set_Led_Level(0);
	}
	//7.打开汞灯前需要检测光导管 插入才有效
	
	if(0 == rtn)
	{
		IO_GPIO_Pin_State LG_DTKT1_sensor = 0;
		IO_GPIO_Read_Pin(IO_GPIO_LG_DTKT1, &LG_DTKT1_sensor);
		printf("LG_DTKT1_sensor ：%d! \r\n", LG_DTKT1_sensor);
		if(1 == LG_DTKT1_sensor)
		{
			
			PE_Close_Hg_Light();
			rtn = ERROR_LG_Not_Insert;
			printf("ERROR_LG_Not_Insert, LG_DTKT1_sensor ：%d! \r\n", LG_DTKT1_sensor);
		}
	}

	//9.光强监测 关灯状态
	if(0 == rtn)
	{
		IO_ADC_Init(IO_ADC_LP_FBK);
		for(i = 0 ; i <5 ; ++i)
		{
			adcx_close = IO_ADC_Read(ADC_CHANNEL_9, 20);
			printf("adcx_close :%ld \r\n", adcx_close );
			delay_ms(100);	

		}
	}

	
	//10.打开汞灯
	if(0 == rtn)
	{
//		rtn = PE_Open_Hg_Light();
//			delay_ms(100);	
//		for(int i = 0; i <300 ;++i )
		for(int i = 0; i <50 ;++i )
		 { 
			//预热阶段延时4分钟
			
			int temperature = DS18B21_Get_Temp();
			printf("The temperature:%d.\r\n", temperature);
//			delay_ms(300);
			delay_ms(300);
		}
		rtn = PE_Open_Hg_Light();
		for(int i = 0; i <50 ;++i )
		 { 
			//预热阶段延时4分钟
			
			int temperature = DS18B21_Get_Temp();
			printf("The temperature2:%d.\r\n", temperature);
//			delay_ms(300);
			delay_ms(300);
		}
			delay_ms(100);	
//			AP_Set_Led_Level(100);
			
		//11.光强监测      开灯状态
		IO_ADC_Init(IO_ADC_LP_FBK);
		for(i = 0 ; i <5 ; ++i)
		{
			adcx_open = IO_ADC_Read(ADC_CHANNEL_9, 20);
			printf("adcx_open :%ld \r\n", adcx_open );
			delay_ms(100);	

		}
		//12. 灯点亮状态对比
//		if((adcx_open - adcx_close) >= 50)
//		{
////			for(int i = 0; i <500 ;++i )
//			for(int i = 0; i <5 ;++i )
//			 { 
//				//预热阶段延时4分钟
//				
//				int temperature = DS18B21_Get_Temp();
//				printf("The light temperature :%d.\r\n", temperature);
//				delay_ms(300);
//			}
//		}
//		else
//		{
//			rtn = ERROR_OPEN_LIGHT;
//			printf("ERROR_OPEN_LIGHT ! \r\n" );
//		}
	}
	

	
	//13. 改变光环显示状态
	if(0 == rtn)
	{
		rtn = PE_HaloDisplay_Green(0);
	}
	else
	{
		//如果报错，红灯亮
		
		PE_Close_Hg_Light();
		PE_HaloDisplay_Red(0);
		printf("ERROR_Initialized HaloDisplay is Red! \r\n");
		rtn = ERROR_Initialized;
		return rtn;
	}
    return rtn;  // 成功返回0，错误返回其他错误码
}

int AP4A_Set_System_Info(char *info) {
    uint16_t rtn = 0;
    // 设置系统信息
    // 需要根据实际需求和硬件接口进行具体实现
	    //char* send_Buffer={"Apollo STM32F7"};
	//1.检测EEPROM是否存在    
	printf("Check EEPROM  ：! \r\n");
	while(AT24CXX_Check())//检测不到24c02
	{
			printf("24C02 Check Failed! \r\n");
		delay_ms(500);
			printf("Please Check! \r\n");
		delay_ms(500);
	}
	
	printf("24C02 Ready! \r\n");  

	printf("Start Write 24C02.... \r\n"); 
	AT24CXX_WritePage(32,SIZE,(u8*)TEXT_Buffer);
	printf("24C02 Write Finished! %s\r\n",TEXT_Buffer); 


    return rtn;
}

int AP_Get_System_Info(char *info) {
    uint16_t rtn = 0;
    // 获取系统信息
    // 需要根据实际需求和硬件接口进行具体实现

	u8 datatemp[SIZE];

	//1.检测EEPROM是否存在    
	printf("Check EEPROM  ：! \r\n");
	while(AT24CXX_Check())//检测不到24c02
	{
			printf("24C02 Check Failed! \r\n");
		delay_ms(500);
			printf("Please Check! \r\n");
		delay_ms(500);
	}
	//2.读取系统信息	
	while(1)
	{
		
 		datatemp[0] = '\0'; 
		printf("Start Read 24C02.... \r\n"); 
		printf("SIZE:%d \r\n", SIZE);
		AT24CXX_ReadPage(32,SIZE,datatemp);
		printf("The Data Readed AAAAAIs: %s\r\n",datatemp);
//		 aa= AT24CXX_ReadOneByte(0);
//		 bb= AT24CXX_ReadOneByte(1);		
//		 cc= AT24CXX_ReadOneByte(2);
//		 ee= AT24CXX_ReadOneByte(3);		
//		 ff= AT24CXX_ReadOneByte(4);	
//			printf("The Data Readed Is: %c\r\n",aa); 
//			printf("The Data Readed Is: %c\r\n",bb);
//			printf("The Data Readed Is: %c\r\n",cc);
//			printf("The Data Readed Is: %c\r\n",ee);		
//			printf("The Data Readed Is: %c\r\n",ff);
		delay_ms(300);
				   
	} 


	
		uint32_t data = AT24CXX_ReadLenByte(0,32);
	  for(int i = 0; i < 32; i++)
    {
        info[i] = (data >> (i*8)) & 0xFF;
    }
	printf("info[i] = %s \r\n", info);
    return rtn;
}

int AP4A_Set_System_NameID(char *nameID) {
    uint16_t rtn = 0;
    // 设置系统名称ID
    // 需要根据实际需求和硬件接口进行具体实现
	    char* send_Buffer={"Apollo STM32F7"};
			AT24CXX_WriteLenByte(32,(u32)send_Buffer,32*32);
    return rtn;
}

int AP_Get_System_NameID(char *nameID) {
    uint16_t rtn = 0;
    // 获取系统名称ID
    // 需要根据实际需求和硬件接口进行具体实现
		uint32_t data = AT24CXX_ReadLenByte(32,32);
	  for(int i = 0; i < 32; i++)
    {
        nameID[i] = (data >> (i*8)) & 0xFF;
    }
    return rtn;
}

int AP4A_Set_Device_Software_Version(char *version) {
    uint16_t rtn = 0;
    // 设置设备软件版本号
    // 需要根据实际需求和硬件接口进行具体实现
	    char* send_Buffer={"Apollo STM32F7"};
			AT24CXX_WriteLenByte(64,(u32)send_Buffer,32);
    return rtn;
}

int AP_Get_Device_Software_Version(char *version) {
    uint16_t rtn = 0;
    // 获取设备软件版本号
    // 需要根据实际需求和硬件接口进行具体实现
		uint32_t data = AT24CXX_ReadLenByte(64,32);
	  for(int i = 0; i < 32; i++)
    {
        version[i] = (data >> (i*8)) & 0xFF;
    }
    return rtn;
}

int AP_Set_Device_Running_Time(int runTime) {
    uint16_t rtn = 0;
    // 设置设备运行时间
    // 需要根据实际需求和硬件接口进行具体实现
			AT24CXX_WriteLenByte(96,(u32)runTime,32 );
    return rtn;
}

int AP_Get_Device_Running_Time(int *runTime) {
    uint16_t rtn = 0;
    // 获取设备运行时间
    // 需要根据实际需求和硬件接口进行具体实现
		*runTime = AT24CXX_ReadLenByte(96,32);
    return rtn;
}

int AP_Set_Operation_Mode(OperationMode mode) {
    uint16_t rtn = 0;
    // 设置操作模式
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
    currentOperationMode = mode;  // 更新全局变量
    return rtn;
}

int AP_Get_Operation_Mode(OperationMode *mode) {
    uint16_t rtn = 0;
    // 获取操作模式
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
    *mode = currentOperationMode;  // 返回全局变量
    return rtn;
}

int AP_Set_Led_Level(int level) {
    uint16_t rtn = 0;
	int value = 0;
    // 设置光源档位
    // 需要根据实际需求和硬件接口进行具体实现
    // ...	
    cur_level = level;
	
	printf("**************************************************cur_level: %d \r\n", cur_level);
	switch(level)//方向转换
	{
		case 0:level=0;break;
		case 1:level=23;break;
		case 2:level=33;break;
		case 3:level=39;break;
		case 4:level=49;break;
		case 5:level=70;break;
		case 6:level=83;break;
		case 7:level=98;break;
		case 8:level=107;break;	
		case 9:level=118;break;	
		case 10:level=126;break;
		case 11:level=136;break;
		case 12:level=140;break;
		case 13:level=147;break;
		case 14:level=154;break;
		case 15:level=158;break;
		case 16:level=166;break;
		case 17:level=170;break;
		case 18:level=174;break;	
		case 19:level=178;break;	
		case 20:level=183;break;	
		case 21:level=187;break;
		case 22:level=190;break;
		case 23:level=195;break;
		case 24:level=198;break;
		case 25:level=203;break;
		case 26:level=205;break;
		case 27:level=207;break;
		case 28:level=211;break;	
		case 29:level=213;break;
		case 30:level=216;break;	
		case 31:level=220;break;
		case 32:level=222;break;
		case 33:level=224;break;
		case 34:level=227;break;
		case 35:level=231;break;
		case 36:level=233;break;
		case 37:level=234;break;
		case 38:level=236;break;	
		case 39:level=238;break;	
		case 40:level=240;break;	
		case 41:level=242;break;
		case 42:level=244;break;
		case 43:level=245;break;
		case 44:level=247;break;
		case 45:level=250;break;
		case 46:level=252;break;
		case 47:level=254;break;
		case 48:level=255;break;	
		case 49:level=257;break;		
		case 50:level=258;break;	
		case 51:level=260;break;
		case 52:level=262;break;
		case 53:level=264;break;
		case 54:level=266;break;
		case 55:level=267;break;
		case 56:level=269;break;
		case 57:level=270;break;
		case 58:level=271;break;	
		case 59:level=272;break;		
		case 60:level=274;break;	
		case 61:level=275;break;
		case 62:level=277;break;
		case 63:level=278;break;
		case 64:level=279;break;
		case 65:level=281;break;
		case 66:level=282;break;
		case 67:level=284;break;
		case 68:level=285;break;	
		case 69:level=286;break;		
		case 70:level=288;break;	
		case 71:level=289;break;
		case 72:level=290;break;
		case 73:level=291;break;
		case 74:level=292;break;
		case 75:level=294;break;
		case 76:level=295;break;
		case 77:level=296;break;
		case 78:level=298;break;	
		case 79:level=299;break;		
		case 80:level=301;break;	
		case 81:level=303;break;
		case 82:level=304;break;
		case 83:level=305;break;
		case 84:level=307;break;
		case 85:level=308;break;
		case 86:level=309;break;
		case 87:level=310;break;
		case 88:level=311;break;	
		case 89:level=312;break;		
		case 90:level=313;break;	
		case 91:level=315;break;
		case 92:level=316;break;
		case 93:level=318;break;
		case 94:level=319;break;
		case 95:level=321;break;
		case 96:level=324;break;
		case 97:level=326;break;
		case 98:level=328;break;	
		case 99:level=331;break;		
	   case 100:level=333;break;	
		default:
//			printf("ERROR_Initialized HaloDisplay is Red! \r\n");
			rtn = ERROR_VA_LEVEL;
			break;
	}
		
    PE_VA_Move_Abs(level);
    return rtn;
}

int AP_Get_Led_Level(int *level) {
    uint16_t rtn = 0;
    // 获取光源档位
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
    *level = cur_level;
    return rtn;
}

int AP_Up_Level(int level) {
    uint16_t rtn = 0;
	int value = 0;
    // 设置光源档位
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
//     value = level *3 /360 * 400;

	if((level + cur_level) <= 100)
	{
		 AP_Set_Led_Level(level + cur_level);
		cur_level = level + cur_level;
	}
	else
	{
	
		AP_Set_Led_Level(100);
		cur_level = 100;

	}
	
//     value = (int)floor(level * 3.333) ;
//		printf("VA Level value is %d ! \r\n", value);
//    PE_VA_Move_Rel(value);
    return rtn;
}
int AP_Down_Level(int level) {
    uint16_t rtn = 0;
	int value = 0;
    // 设置光源档位
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
//     value = level *3 /360 * 400;
	if((cur_level - level) >= 0)
	{
		 AP_Set_Led_Level(cur_level - level);
		cur_level = cur_level - level;
    }
	else
	{
	
		AP_Set_Led_Level(0);
		cur_level = 0;

	}
//	value = (int)floor(level * 3.333) ;
//		printf("VA Level value is %d ! \r\n", value);
//    PE_VA_Move_Rel(-value);
    return rtn;
}
int AP_Switch_On_Led() {
    // 首先判断灯室温度，是否满足点灯条件
    uint16_t rtn = 0;
		//TODO 需要补充
	
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
    // TODO:需要判断温度是否大于阈值，如果大于阈值，方可点灯。 该阈值暂时未定。调试时候给出 ---20231206
    if(0 == rtn)
    {
    	rtn = PE_Open_Hg_Light();
	}
    return rtn;
}

int AP_Switch_Off_Led() {
    uint16_t rtn = 0;
    // 关闭光源
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
    
 
	
    // 需要根据实际需求和硬件接口进行具体实现
    if(0 == rtn)
    {
    	rtn = PE_Close_Hg_Light();
	}
    return rtn;
}

int AP_Change_Led_Power(int power) {
    uint16_t rtn = 0;
    // 改变光源强度
    // 需要根据实际需求和硬件接口进行具体实现
    //20231206 等待思雨代码好了 同设置光强档位
    return rtn;
}

int AP_Switch_On_Led_With_Power(int power) {
    uint16_t rtn = 0;
    // 打开光源并设置强度
    // 需要根据实际需求和硬件接口进行具体实现
    rtn = AP_Change_Led_Power( power);
	if(0 == rtn)
	{
		AP_Switch_On_Led();
	}
    return rtn;
}

int AP_Get_Led_State(char *ledState) {
    uint16_t rtn = 0;
    // 获取LED状态
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
    return rtn;
}

int AP_Get_Exposure_Runtime(int *runTime) {
    uint16_t rtn = 0;
    // 获取曝光运行时间
    // 需要根据实际需求和硬件接口进行具体实现
    //rtn = IO_DS1821_ReadDurationSensor((uint16_t *)runTime);
    // ...
    return rtn;
}

int AP4A_Set_Exposure_Runtime(int runTime) {
    uint16_t rtn = 0;
    // 设置曝光运行时间
    // 需要根据实际需求和硬件接口进行具体实现
    // ...
    //rtn = IO_DS1821_WriteDurationSensor((uint16_t)runTime);
    return rtn;
}

int AP_Get_Event_Text(char *eventText) {
    uint16_t rtn = 0;
    // 获取事件文本
    // 需要根据实际需求和硬件接口进行具体实现
// ...
return rtn;
}
//TODO 错误信息暂时存在EEPROM,由于空间有限，最好存放错误码，后续可存放到NANDFlash里面。
//by 20231218 zhaob
int AP_Set_Error_Text(char *errorText) {
    uint16_t rtn = 0;
// 获取错误文本
// 需要根据实际需求和硬件接口进行具体实现
	    char* send_Buffer={"Apollo STM32F7"};
			AT24CXX_WriteLenByte(128,(u32)send_Buffer,32 );
return rtn;
}
int AP_Get_Error_Text(char *errorText) {
    uint16_t rtn = 0;
// 获取错误文本
// 需要根据实际需求和硬件接口进行具体实现
		uint32_t data = AT24CXX_ReadLenByte(128,32);
	  for(int i = 0; i < 32; i++)
    {
        errorText[i] = (data >> (i*8)) & 0xFF;
    }
return rtn;
}
int AP_Open_Shutter() {
    uint16_t rtn = 0;
	rtn = PE_Start_Stop();
	return rtn;
}

int AP_Close_Shutter() 
{
    uint16_t rtn = 0;
	rtn = PE_Start_Stop();
return rtn;
}
