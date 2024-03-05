#ifndef IO_GPIO_H
#define IO_GPIO_H

#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#include "canfestival_master.h"
#include "canfestival_timer.h"
#include "canfestival_can.h"
#include "canfestival.h"
//��������붨��
 #define IO_BASE_ERROR            (0x494f0000+0x100)  //"IO"
 #define IO_SYSTEM_ERROR          (IO_BASE_ERROR+0x01) 
 #define IO_PARAMETER_ERROR       (IO_BASE_ERROR+0x02) 
 #define IO_ILLEGAL_CALL_ERROR    (IO_BASE_ERROR+0x03) 


 #define IO_INVALID_STATE_ERROR   (IO_BASE_ERROR+0x10) 
 #define IO_TIMEOUT_ERROR         (IO_BASE_ERROR+0x10) 
 
typedef enum
{
	IO_GPIO_MIN = 0,
	IO_GPIO_ZERO_POS,	       //��λ��������0��Ч	
	IO_GPIO_ENC_A,	           //VA���������	
	IO_GPIO_ENC_B,	           //VA���������	
	IO_GPIO_ENC_Z,	           //VA���������	
	IO_GPIO_ABS_EN	,          //���Ա������л�	ʼ�ձ���Ϊ1	
	IO_GPIO_CAN1_RX	,          //VA CANͨ�Žӿ�	
	IO_GPIO_CAN1_TX	,          //VA CANͨ�Žӿ�	
	IO_GPIO_UP,	               //��ť�ϼ���1��Ч	
	IO_GPIO_DOWN,              //��ť�¼���1��Ч	
	IO_GPIO_LEFT,              //��ť�¼���1��Ч	
	IO_GPIO_RIGHT,              //��ť�¼���1��Ч	
	IO_GPIO_USART1_TX	,      //RS232	
	IO_GPIO_USART1_RX	,      //RS232	
	IO_GPIO_RMII_REF_CLK,      //��̫��	
	IO_GPIO_RMII_CSR_DV	,      //��̫��	
	IO_GPIO_RMII_RXD1,	       //��̫��	
	IO_GPIO_RMII_RXDO	,      //��̫��	
	IO_GPIO_RMII_TX_EN	,      //��̫��	
	IO_GPIO_RMII_TXDO,	       //��̫��	
	IO_GPIO_RMII_TXD1	,       //��̫��	
	IO_GPIO_ETH_MDC	    ,       //��̫��	
	IO_GPIO_ETH_MDIO	,       //��̫��	
	IO_GPIO_SHUTTER_INTERLOCK,	 //shutter interlock	PLC:0��Ч	
	IO_GPIO_SHUTTER_MODE,	     //snutter mode	PLC��0��Ч	
	IO_GPIO_START	  ,          //Start/atop����:1��Ч	
	IO_GPIO_OK	  ,          //Start/atop����:1��Ч	
	IO_GPIO_SHU_OPEN,	         //���ſ���λ��0��Ч	
	IO_GPIO_SHU_CLOSE	,         //���Źص�λ��o��Ч	
	IO_GPIO_SHUTTER_ON_OFF,    	//	���Ŵ�/�ر�	
	IO_GPIO_RD,					//�ƹ�ָʾ ��:0��Ч	
	IO_GPIO_BL,					//�ƹ�ָʾ�� ��o��Ч	
	IO_GPIO_GR,					//�ƹ�ָʾ�̣���Ч	
	IO_GPIO_SHUTTER_VERIFY,			//������֤�ź�
	IO_GPIO_SHUTTER_ALARM,			//���ű����ź�
	IO_GPIO_CAN2_RX,				//SHU CANͨ�Žӿ�	
	IO_GPIO_CAN2_TX	,			//SHU CANͨ�Žӿ�	
	IO_GPIO_LAMP_ON_OFF,		//Lamp on of	PLC�ӿڣ�0��ʾoff	
	IO_GPIO_LP_FBK	,			//����������	0-10Vģ������12λ	
	IO_GPIO_SYS_INIERLOCK,  	//A_SYS INTERLOCK PLC�ӿڣ�0��Ч
	IO_GPIO_IGN_OUT ,			//���ƿ�����0��Ч
	IO_GPIO_LAMP_ON,			//�����Ѵ�	PLC�ӿڣ���0��Ч	
	IO_GPIO_LAMP_ALARM,			//���ݱ���	PLC�ӿڣ�	
	IO_GPIO_FAN_FG,				//����ת�ټ��	
	IO_GPIO_BLW_FG,				//�ķ��ת�ټ��	
	IO_GPIO_BLW_PWM,				//�ķ��ת�ٿ���	
	IO_GPIO_DQ,   			 		//�¶ȴ�����	
	IO_GPIO_LAMP_COOL,				//lampcool	PLC�ӿڣ�0��Ч	
	IO_GPIO_LG_DTKT1,				//�⵼�ܴ���������Ч	
	IO_GPIO_FILTER_DETECT1,	//�˹�Ƭ��⴫������1��Ч	
	IO_GPIO_FILTER_DETECT2,	//�˹�Ƭ��⴫������1��Ч	
	IO_GPIO_FILTER_DETECT3,	//�˹�Ƭ��⴫������1��Ч	
	IO_GPIO_CUR_FBK,			//����������	0-10vģ������12λ	
	IO_GPIO_CUR_CMD,			//��Դ���ʿ���	0-10vģ������12λ	
	IO_GPIO_ALARM_CLEAR,		//��������	PLC�ӿڣ�0��Ч	
	IO_GPIO_SP_IN1,				//��������ӿ�	PLC�ӿڣ�0��Ч	
	IO_GPIO_SP_OUT1,			//��������ӿ�	PIC¥�ڣ�o��Ч	
	IO_GPIO_SP_OUT2,			//��������ӿ�	PLC�ӿڣ�0��Ч	
	IO_GPIO_VA_ENABLE,			//PAʹ�ܣ�0��Ч	
	IO_GPIO_Shutter_ENABLE,			//PAʹ�ܣ�0��Ч	
	IO_GPIO_MAX1
} IO_GPIO_ENUM; 

typedef enum
{
 IO_GPIO_PIN_RESET = 0,
 IO_GPIO_PIN_SET 
}IO_GPIO_Pin_State;

// ��ʼ������ʹ�õ�GPIO���ŵ�ʱ��
void IO_GPIO_Init_All_Clocks(void); 
	
// ��ʼ�������õ���GPIO����
void IO_GPIO_Init_All_Pins(void); 

//��ʼ��ĳ��GPIO����
int IO_GPIO_Init_Pin(IO_GPIO_ENUM IO_GPIOx);
//��ĳ��GPIO���ŷ��ص�Ĭ��״̬
int IO_GPIO_DeInit_Pin(IO_GPIO_ENUM IO_GPIOx);
//��ȡĳ��GPI0����
int IO_GPIO_Read_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State* bitstatus);
//д��ĳ��GPI0����
int IO_GPIO_Write_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State bitstatus);

//����ĳ��GPIO���ŵ�ָ��һ��״̬��ʹ��Ҫ�ر������һ������ĳһ�����ţ���ֻ�ܸ�λ����MCU�������״̬
int IO_GPIO_Lock_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State bitstatus);	

//��תĳ��GPIO����
int IO_GPIO_Toggle_Pin(IO_GPIO_ENUM IO_GPIOx, IO_GPIO_Pin_State bitstatus);

#endif

