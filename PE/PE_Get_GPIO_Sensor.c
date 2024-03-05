#include "PE_Get_GPIO_Sensor.h"
#include "IO_ADC.h"
#include "IO_DS1821.h"
#include <time.h>

#include "PE_HaloDisplay.h"

int PE_Get_GPIO_ShuterClose(IO_GPIO_Pin_State* bitstatus)
{
	int rtn = 0;
	rtn = IO_GPIO_Read_Pin(IO_GPIO_SHUTTER_ON_OFF, bitstatus);
	return rtn;

}
int PE_Get_GPIO_ShuterOpen(IO_GPIO_Pin_State* bitstatus)
{
	int rtn = 0;
	rtn = IO_GPIO_Read_Pin(IO_GPIO_SHUTTER_ON_OFF, bitstatus);
	return rtn;

}

int PE_Get_GPIO_ShuterCloseSensor(IO_GPIO_Pin_State* bitstatus)
{
	int rtn = 0;
	
	IO_GPIO_Pin_State ShuterCloseSensor = IO_GPIO_PIN_SET;
	*bitstatus = IO_GPIO_PIN_SET;
	time_t start_time = time(NULL);


	while (1) 
	{
		rtn = IO_GPIO_Read_Pin(IO_GPIO_SHU_CLOSE, bitstatus);
	
		if(*bitstatus != ShuterCloseSensor)
		{
				break;
		}
	/*
			if(count>=100)
			{
				rtn = IO_SYSTEM_ERROR;
				break; 
				
			}
			count++;
	*/
		// 判断是否超过10秒
		time_t current_time = time(NULL);
		if ((current_time - start_time) >= 10) 
		{
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_ALARM, IO_GPIO_PIN_RESET);
			PE_HaloDisplay_Colour(Red);
		}
			break;	// 当超过10秒时，退出循环
		}

	return rtn;
}

int PE_Get_GPIO_ShuterOpenSensor(IO_GPIO_Pin_State* bitstatus)
{
	int rtn = 0;
	IO_GPIO_Pin_State ShuterOpenSensor = IO_GPIO_PIN_SET;
	*bitstatus = IO_GPIO_PIN_SET;
//	int count = 0;
	time_t start_time = time(NULL);
//	while(*bitstatus == ShuterOpenSensor)
//	{
//		rtn = IO_GPIO_Read_Pin(IO_GPIO_SHU_OPEN, bitstatus);
//	}
	while (1) 
	{
		rtn = IO_GPIO_Read_Pin(IO_GPIO_SHU_OPEN, bitstatus);

		if(*bitstatus != ShuterOpenSensor)
		{
			break;
		}
/*
		if(count>=100)
		{
			rtn = IO_SYSTEM_ERROR;
			break; 
			
		}
		count++;
*/
		 // 判断是否超过10秒
        time_t current_time = time(NULL);
        if ((current_time - start_time) >= 10) 
		{
			rtn = IO_GPIO_Write_Pin(IO_GPIO_SHUTTER_ALARM, IO_GPIO_PIN_RESET);
			PE_HaloDisplay_Colour(Red);
            break;  // 当超过10秒时，退出循环
        }

	}


	return rtn;
}

int PE_Get_GPIO_TempertureSensor(uint16_t* temperature)
{
	int rtn = 0;
	//short temperature = 0;
	IO_DS1821_Init();
	*temperature = (uint16_t)DS18B21_Get_Temp();
	
	return rtn;
}

int PE_Get_GPIO_DurationRecorderSensor(uint16_t* duration)
{
	int rtn = 0;
	IO_DS1821_Init();
		
	//rtn = IO_DS1821_ReadDurationSensor(duration);
		
	return rtn;


}

int PE_Get_GPIO_LightTubeDetectionSensor(IO_GPIO_Pin_State* bitstatus)
{
	int rtn = 0;
	

	rtn = IO_GPIO_Read_Pin(IO_GPIO_LG_DTKT1, bitstatus);
	
	
	return rtn;
}



int PE_Get_GPIO_LightEnergySensor(u8 times, uint16_t* LightEnergy)
{
	int rtn = 0;
	
	
	IO_ADC_Init(IO_ADC_LP_FBK);
	*LightEnergy = IO_ADC_Read(IO_ADC_LP_FBK, times);
	
	
	return rtn;
}


int PE_Is_SMEE_Filter()
{
	//int rtn = 0;
	IO_GPIO_Pin_State FILTER_DETECT1 = IO_GPIO_PIN_RESET;
	IO_GPIO_Pin_State FILTER_DETECT2 = IO_GPIO_PIN_RESET;
	IO_GPIO_Pin_State FILTER_DETECT3 = IO_GPIO_PIN_RESET;
		
	IO_GPIO_Read_Pin(IO_GPIO_FILTER_DETECT1, &FILTER_DETECT1);
	IO_GPIO_Read_Pin(IO_GPIO_FILTER_DETECT2, &FILTER_DETECT2);
	IO_GPIO_Read_Pin(IO_GPIO_FILTER_DETECT3, &FILTER_DETECT3);

    //判断是否为SMEE滤光片，这里目前预设传感器序列为001
	if(FILTER_DETECT1 == IO_GPIO_PIN_RESET && FILTER_DETECT2 == IO_GPIO_PIN_RESET && FILTER_DETECT3 == IO_GPIO_PIN_SET)
	{

		return 1;

	}
	else
	{
	
		return 0;
	}


}







































