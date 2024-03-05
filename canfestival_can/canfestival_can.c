#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include "stm32f7xx_hal_conf.h"
#include "canfestival_can.h"
#include "canfestival_master.h"
#include "canfestival.h"

//#define CAN_PORT CAN_HandleTypeDef

//CAN_HandleTypeDef CAN_InitStructure;
/* CANOPEN字典 */
extern CO_Data masterObjdict_Data;

/* 功能:	GPIO配置
	 参数:	无
	 返回值:无
 */
static void gpio_config(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;

	__HAL_RCC_CAN1_CLK_ENABLE(); // 使能CAN1时钟
	__HAL_RCC_GPIOA_CLK_ENABLE(); // 需要配置GPIOA时钟

	GPIO_InitStructure.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;         //复用功能
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;    //100MHz  GPIO_Speed_100MHz
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //上拉
    GPIO_InitStructure.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);              //初始化

}
static void gpio2_config(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;

	__HAL_RCC_CAN2_CLK_ENABLE(); // 使能CAN1时钟
	__HAL_RCC_GPIOB_CLK_ENABLE(); // 需要配置GPIOA时钟

	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;         //复用功能
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;    //100MHz  GPIO_Speed_100MHz
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //上拉
    GPIO_InitStructure.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);              //初始化

}

/* 功能:	中断嵌套控制器配置
	 参数:	无
	 返回值:无
 */
static void nvic_config(void)
{
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 2, 2);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

}

static void nvic2_config(void)
{
	HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 2, 2);
	HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);

}


/* 功能:	can总线配置
	 参数:	无
	 返回值:无
 */
 
CAN_HandleTypeDef CAN_InitStructure;
//CAN_HandleTypeDef CAN_InitStructure2;
CanTxMsgTypeDef  CAN_TxMessage;      //·￠?í???￠
CanRxMsgTypeDef  CAN_RxMessage;      //?óê????￠
//CanTxMsgTypeDef  CAN2_TxMessage;      //·￠?í???￠
//CanRxMsgTypeDef  CAN2_RxMessage;      //?óê????￠

void CANOpen_can_config(void)
{
	//CAN_HandleTypeDef CAN_InitStructure;
	CAN_FilterConfTypeDef  CAN_FilterInitStructure;
//	CAN_FilterConfTypeDef  CAN_FilterInitStructure2;
	HAL_StatusTypeDef rtn = HAL_OK;

	/* 配置GPIO */
    //__HAL_RCC_GPIOA_CLK_ENABLE();
	gpio_config();
	//gpio2_config();

	/* 中断嵌套控制器配置 */
	nvic_config();
	//nvic2_config();

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* 初始化CAN参数 */
	CAN_InitStructure.Instance = CAN1;
    CAN_InitStructure.pTxMsg=&CAN_TxMessage;     //发送消息
    CAN_InitStructure.pRxMsg=&CAN_RxMessage;     //接收消息
	CAN_InitStructure.Init.TTCM = DISABLE;
	CAN_InitStructure.Init.ABOM = DISABLE;
	CAN_InitStructure.Init.AWUM = DISABLE;
	CAN_InitStructure.Init.NART = ENABLE;
	CAN_InitStructure.Init.RFLM = DISABLE;
	CAN_InitStructure.Init.TXFP = DISABLE;
	CAN_InitStructure.Init.Mode = CAN_MODE_NORMAL;
	CAN_InitStructure.Init.SJW = CAN_SJW_1TQ;
	CAN_InitStructure.Init.BS1 = CAN_BS1_11TQ;
	CAN_InitStructure.Init.BS2 = CAN_BS2_6TQ;
	CAN_InitStructure.Init.Prescaler = 6;

	/* 初始化CAN总线 */
	if ((rtn = HAL_CAN_Init(&CAN_InitStructure)) != HAL_OK)
    {
    	printf("HAL_CAN_Init faile1,rtn = %d \r\n", rtn);
        // CAN 初始化失败
    }
	/* CAN总线过滤器配置 */
	//can_filter_config();
//
//	/* 初始化CAN参数 */
//	CAN_InitStructure2.Instance = CAN2;
//    CAN_InitStructure2.pTxMsg=&CAN2_TxMessage;     //发送消息
//    CAN_InitStructure2.pRxMsg=&CAN2_RxMessage;     //接收消息
//	CAN_InitStructure2.Init.TTCM = DISABLE;
//	CAN_InitStructure2.Init.ABOM = DISABLE;
//	CAN_InitStructure2.Init.AWUM = DISABLE;
//	CAN_InitStructure2.Init.NART = ENABLE;
//	CAN_InitStructure2.Init.RFLM = DISABLE;
//	CAN_InitStructure2.Init.TXFP = DISABLE;
//	CAN_InitStructure2.Init.Mode = CAN_MODE_NORMAL;
//	CAN_InitStructure2.Init.SJW = CAN_SJW_1TQ;
//	CAN_InitStructure2.Init.BS1 = CAN_BS1_11TQ;
//	CAN_InitStructure2.Init.BS2 = CAN_BS2_6TQ;
//	CAN_InitStructure2.Init.Prescaler = 6;
//
//	/* 初始化CAN总线 */
//	if ((rtn = HAL_CAN_Init(&CAN_InitStructure2)) != HAL_OK)
//    {
//    	printf("HAL_CAN2_Init faile1,rtn = %d \r\n", rtn);
//        // CAN 初始化失败
//    }


	/* 配置过滤器0组，配置成标准标识符且低7位都为0时接受 */
	CAN_FilterInitStructure.FilterNumber = 0;						/* 设置过滤器组0 */
	CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;		/* 屏蔽模式 */
	CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;	/* 32位模式 */
	CAN_FilterInitStructure.FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.FilterIdLow = 0x0000;
	CAN_FilterInitStructure.FilterMaskIdHigh = 0x0000;					/* 在CANOpen中标准标识符的低7位表示节点ID */
	CAN_FilterInitStructure.FilterMaskIdLow = 0x0000;					/* 在CANOpen中只用标准标识符，数据帧/远程帧都有 */
	CAN_FilterInitStructure.FilterIdHigh = 0x0000;						/* 主节点ID为0 */
	CAN_FilterInitStructure.FilterIdLow = 0x0000;						/* 标准帧 */
	CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* 过滤器关联到FIFO0 */
	CAN_FilterInitStructure.FilterActivation = ENABLE;					/* 使能过滤器 */
    CAN_FilterInitStructure.BankNumber=14;
	HAL_CAN_ConfigFilter(&CAN_InitStructure,&CAN_FilterInitStructure);
	
	HAL_CAN_Receive_IT(&CAN_InitStructure, CAN_FIFO0); 
	

	/* 配置过滤器0组，配置成标准标识符且低7位都为0时接受 */
//	CAN_FilterInitStructure2.FilterNumber = 0;						/* 设置过滤器组0 */
//	CAN_FilterInitStructure2.FilterMode = CAN_FILTERMODE_IDMASK;		/* 屏蔽模式 */
//	CAN_FilterInitStructure2.FilterScale = CAN_FILTERSCALE_32BIT;	/* 32位模式 */
//	CAN_FilterInitStructure2.FilterIdHigh = 0x0000;
//	CAN_FilterInitStructure2.FilterIdLow = 0x0000;
//	CAN_FilterInitStructure2.FilterMaskIdHigh = 0x0000;					/* 在CANOpen中标准标识符的低7位表示节点ID */
//	CAN_FilterInitStructure2.FilterMaskIdLow = 0x0000;					/* 在CANOpen中只用标准标识符，数据帧/远程帧都有 */
//	CAN_FilterInitStructure2.FilterIdHigh = 0x0000;						/* 主节点ID为0 */
//	CAN_FilterInitStructure2.FilterIdLow = 0x0000;						/* 标准帧 */
//	CAN_FilterInitStructure2.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* 过滤器关联到FIFO0 */
//	CAN_FilterInitStructure2.FilterActivation = ENABLE;					/* 使能过滤器 */
//    CAN_FilterInitStructure2.BankNumber=14;
//	HAL_CAN_ConfigFilter(&CAN_InitStructure2,&CAN_FilterInitStructure2);
//	
//	HAL_CAN_Receive_IT(&CAN_InitStructure2, CAN_FIFO0); 
}


void CAN1_RX0_IRQHandler(void)
{
	CanRxMsgTypeDef message;
	Message Rx_Message;

	/* 接收消息 */
	//CAN_Receive(CAN1, CAN_FIFO0, &message);
	HAL_CAN_Receive(&CAN_InitStructure, CAN_FIFO0, 10);

	/* 组装canopen数据包 */
	Rx_Message.cob_id = message.StdId;						/* 功能码和节点ID */
	Rx_Message.rtr = (message.RTR == CAN_RTR_DATA) ? 0 : 1;	/* 标识符类型 */
	Rx_Message.len = message.DLC;							/* 数据包长度 */
	memcpy(Rx_Message.data, message.Data, message.DLC);		/* 数据 */

	/* canopen数据包分配处理函数 */
	canDispatch(&masterObjdict_Data, &Rx_Message);
}

//void CAN2_RX0_IRQHandler(void)
//{
//	CanRxMsgTypeDef message;
//	Message Rx_Message;
//
//	/* 接收消息 */
//	//CAN_Receive(CAN1, CAN_FIFO0, &message);
//	HAL_CAN_Receive(&CAN_InitStructure2, CAN_FIFO0, 10);
//
//	/* 组装canopen数据包 */
//	Rx_Message.cob_id = message.StdId;						/* 功能码和节点ID */
//	Rx_Message.rtr = (message.RTR == CAN_RTR_DATA) ? 0 : 1;	/* 标识符类型 */
//	Rx_Message.len = message.DLC;							/* 数据包长度 */
//	memcpy(Rx_Message.data, message.Data, message.DLC);		/* 数据 */
//
//	/* canopen数据包分配处理函数 */
//	canDispatch(&masterObjdict_Data, &Rx_Message);
//}

/* 功能:	CAN发送数据函数
	 参数:	notused can总线端口
			message canopen数据包
	返回值:	0 成功
			1 失败
 */
UNS8 canSend(CAN_PORT port, Message *message)
{
	UNS8 i = 0;
	uint32_t Timeout = 50;
	/* 组	装CAN数据包 */
	CAN_InitStructure.pTxMsg->DLC = message->len;							/* 数据长度 */
	CAN_InitStructure.pTxMsg->IDE = CAN_ID_STD;								/* 标准ID */
	CAN_InitStructure.pTxMsg->StdId = message->cob_id;						/* 标识符 */
	CAN_InitStructure.pTxMsg->RTR = (message->rtr == CAN_RTR_DATA) ? 0 : 2;	/* 数据帧 */              
    for(i=0; i<message->len; i++)
    {
	    CAN_InitStructure.pTxMsg->Data[i] = message->data[i];
    }
	/* 发送数据包 */
	if(HAL_CAN_Transmit(&CAN_InitStructure, Timeout) == HAL_OK)
	{	
		//printf("HAL_CAN_Transmit Successful!：%d\r\n", __FILE__ , __LINE__);
		return 0;

	}
	else
	{
		printf("HAL_CAN_Transmit Failed!：%d\r\n", __FILE__ , __LINE__);
		return 0;

	}
	/* 等待发送成功 */
	//while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && --i);

	/* 成功0 超时1 */
	//return (i != 0) ? 0 : 1;
}
