#ifndef IO_GPIO_H
#define IO_GPIO_H

#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#include "canfestival_master.h"
#include "canfestival_timer.h"
#include "canfestival_can.h"
#include "canfestival.h"
//对外错误码定义
 #define IO_BASE_ERROR            (0x494f0000+0x100)  //"IO"
 #define IO_SYSTEM_ERROR          (IO_BASE_ERROR+0x01) 
 #define IO_PARAMETER_ERROR       (IO_BASE_ERROR+0x02) 
 #define IO_ILLEGAL_CALL_ERROR    (IO_BASE_ERROR+0x03) 


 #define IO_INVALID_STATE_ERROR   (IO_BASE_ERROR+0x10) 
 #define IO_TIMEOUT_ERROR         (IO_BASE_ERROR+0x10) 
 
typedef enum
{
	IO_GPIO_MIN = 0,
	IO_GPIO_ZERO_POS,	       //零位传感器，0有效	
	IO_GPIO_ENC_A,	           //VA电机编码器	
	IO_GPIO_ENC_B,	           //VA电机编码器	
	IO_GPIO_ENC_Z,	           //VA电机编码器	
	IO_GPIO_ABS_EN	,          //绝对编码器切换	始终保持为1	
	IO_GPIO_CAN1_RX	,          //VA CAN通信接口	
	IO_GPIO_CAN1_TX	,          //VA CAN通信接口	
	IO_GPIO_UP,	               //按钮上键，1有效	
	IO_GPIO_DOWN,              //按钮下键，1有效	
	IO_GPIO_LEFT,              //按钮下键，1有效	
	IO_GPIO_RIGHT,              //按钮下键，1有效	
	IO_GPIO_USART1_TX	,      //RS232	
	IO_GPIO_USART1_RX	,      //RS232	
	IO_GPIO_RMII_REF_CLK,      //以太网	
	IO_GPIO_RMII_CSR_DV	,      //以太网	
	IO_GPIO_RMII_RXD1,	       //以太网	
	IO_GPIO_RMII_RXDO	,      //以太网	
	IO_GPIO_RMII_TX_EN	,      //以太网	
	IO_GPIO_RMII_TXDO,	       //以太网	
	IO_GPIO_RMII_TXD1	,       //以太网	
	IO_GPIO_ETH_MDC	    ,       //以太网	
	IO_GPIO_ETH_MDIO	,       //以太网	
	IO_GPIO_SHUTTER_INTERLOCK,	 //shutter interlock	PLC:0有效	
	IO_GPIO_SHUTTER_MODE,	     //snutter mode	PLC，0有效	
	IO_GPIO_START	  ,          //Start/atop按键:1有效	
	IO_GPIO_OK	  ,          //Start/atop按键:1有效	
	IO_GPIO_SHU_OPEN,	         //快门开到位，0有效	
	IO_GPIO_SHU_CLOSE	,         //快门关到位，o有效	
	IO_GPIO_SHUTTER_ON_OFF,    	//	快门打开/关闭	
	IO_GPIO_RD,					//灯光指示 红:0有效	
	IO_GPIO_BL,					//灯光指示蓝 ，o有效	
	IO_GPIO_GR,					//灯光指示绿，有效	
	IO_GPIO_SHUTTER_VERIFY,			//快门验证信号
	IO_GPIO_SHUTTER_ALARM,			//快门报警信号
	IO_GPIO_CAN2_RX,				//SHU CAN通信接口	
	IO_GPIO_CAN2_TX	,			//SHU CAN通信接口	
	IO_GPIO_LAMP_ON_OFF,		//Lamp on of	PLC接口，0表示off	
	IO_GPIO_LP_FBK	,			//光能量反馈	0-10V模拟量，12位	
	IO_GPIO_SYS_INIERLOCK,  	//A_SYS INTERLOCK PLC接口，0有效
	IO_GPIO_IGN_OUT ,			//汞灯开启，0有效
	IO_GPIO_LAMP_ON,			//灯泡已打开	PLC接口，，0有效	
	IO_GPIO_LAMP_ALARM,			//灯泡报警	PLC接口，	
	IO_GPIO_FAN_FG,				//风扇转速检测	
	IO_GPIO_BLW_FG,				//鼓风机转速检测	
	IO_GPIO_BLW_PWM,				//鼓风机转速控制	
	IO_GPIO_DQ,   			 		//温度传感器	
	IO_GPIO_LAMP_COOL,				//lampcool	PLC接口，0有效	
	IO_GPIO_LG_DTKT1,				//光导管传感器，有效	
	IO_GPIO_FILTER_DETECT1,	//滤光片检测传感器，1有效	
	IO_GPIO_FILTER_DETECT2,	//滤光片检测传感器，1有效	
	IO_GPIO_FILTER_DETECT3,	//滤光片检测传感器，1有效	
	IO_GPIO_CUR_FBK,			//光能量反馈	0-10v模拟量，12位	
	IO_GPIO_CUR_CMD,			//电源功率控制	0-10v模拟量，12位	
	IO_GPIO_ALARM_CLEAR,		//消除报警	PLC接口，0有效	
	IO_GPIO_SP_IN1,				//备用输入接口	PLC接口，0有效	
	IO_GPIO_SP_OUT1,			//备用输出接口	PIC楼口，o有效	
	IO_GPIO_SP_OUT2,			//备用输出接口	PLC接口，0有效	
	IO_GPIO_VA_ENABLE,			//PA使能，0有效	
	IO_GPIO_Shutter_ENABLE,			//PA使能，0有效	
	IO_GPIO_MAX1
} IO_GPIO_ENUM; 

typedef enum
{
 IO_GPIO_PIN_RESET = 0,
 IO_GPIO_PIN_SET 
}IO_GPIO_Pin_State;

// 初始化所有使用到GPIO引脚的时钟
void IO_GPIO_Init_All_Clocks(void); 
	
// 初始化所有用到的GPIO引脚
void IO_GPIO_Init_All_Pins(void); 

//初始化某个GPIO引脚
int IO_GPIO_Init_Pin(IO_GPIO_ENUM IO_GPIOx);
//将某个GPIO引脚返回到默认状态
int IO_GPIO_DeInit_Pin(IO_GPIO_ENUM IO_GPIOx);
//读取某个GPI0引聊
int IO_GPIO_Read_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State* bitstatus);
//写入某个GPI0引脚
int IO_GPIO_Write_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State bitstatus);

//锁定某个GPIO引脚到指定一个状态，使用要特别谨慎，一旦锁定某一个引脚，就只能复位整个MCU清除锁定状态
int IO_GPIO_Lock_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State bitstatus);	

//反转某个GPIO引脚
int IO_GPIO_Toggle_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State bitstatus);

#endif

