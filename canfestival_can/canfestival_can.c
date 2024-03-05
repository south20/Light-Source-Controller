#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include "stm32f7xx_hal_conf.h"
#include "canfestival_can.h"
#include "canfestival_master.h"
#include "canfestival.h"

//#define CAN_PORT CAN_HandleTypeDef

//CAN_HandleTypeDef CAN_InitStructure;
/* CANOPEN�ֵ� */
extern CO_Data masterObjdict_Data;

/* ����:	GPIO����
	 ����:	��
	 ����ֵ:��
 */
static void gpio_config(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;

	__HAL_RCC_CAN1_CLK_ENABLE(); // ʹ��CAN1ʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE(); // ��Ҫ����GPIOAʱ��

	GPIO_InitStructure.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;         //���ù���
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;    //100MHz  GPIO_Speed_100MHz
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //����
    GPIO_InitStructure.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);              //��ʼ��

}
static void gpio2_config(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;

	__HAL_RCC_CAN2_CLK_ENABLE(); // ʹ��CAN1ʱ��
	__HAL_RCC_GPIOB_CLK_ENABLE(); // ��Ҫ����GPIOAʱ��

	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;         //���ù���
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;    //100MHz  GPIO_Speed_100MHz
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //����
    GPIO_InitStructure.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);              //��ʼ��

}

/* ����:	�ж�Ƕ�׿���������
	 ����:	��
	 ����ֵ:��
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


/* ����:	can��������
	 ����:	��
	 ����ֵ:��
 */
 
CAN_HandleTypeDef CAN_InitStructure;
//CAN_HandleTypeDef CAN_InitStructure2;
CanTxMsgTypeDef  CAN_TxMessage;      //����?��???��
CanRxMsgTypeDef  CAN_RxMessage;      //?����????��
//CanTxMsgTypeDef  CAN2_TxMessage;      //����?��???��
//CanRxMsgTypeDef  CAN2_RxMessage;      //?����????��

void CANOpen_can_config(void)
{
	//CAN_HandleTypeDef CAN_InitStructure;
	CAN_FilterConfTypeDef  CAN_FilterInitStructure;
//	CAN_FilterConfTypeDef  CAN_FilterInitStructure2;
	HAL_StatusTypeDef rtn = HAL_OK;

	/* ����GPIO */
    //__HAL_RCC_GPIOA_CLK_ENABLE();
	gpio_config();
	//gpio2_config();

	/* �ж�Ƕ�׿��������� */
	nvic_config();
	//nvic2_config();

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* ��ʼ��CAN���� */
	CAN_InitStructure.Instance = CAN1;
    CAN_InitStructure.pTxMsg=&CAN_TxMessage;     //������Ϣ
    CAN_InitStructure.pRxMsg=&CAN_RxMessage;     //������Ϣ
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

	/* ��ʼ��CAN���� */
	if ((rtn = HAL_CAN_Init(&CAN_InitStructure)) != HAL_OK)
    {
    	printf("HAL_CAN_Init faile1,rtn = %d \r\n", rtn);
        // CAN ��ʼ��ʧ��
    }
	/* CAN���߹��������� */
	//can_filter_config();
//
//	/* ��ʼ��CAN���� */
//	CAN_InitStructure2.Instance = CAN2;
//    CAN_InitStructure2.pTxMsg=&CAN2_TxMessage;     //������Ϣ
//    CAN_InitStructure2.pRxMsg=&CAN2_RxMessage;     //������Ϣ
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
//	/* ��ʼ��CAN���� */
//	if ((rtn = HAL_CAN_Init(&CAN_InitStructure2)) != HAL_OK)
//    {
//    	printf("HAL_CAN2_Init faile1,rtn = %d \r\n", rtn);
//        // CAN ��ʼ��ʧ��
//    }


	/* ���ù�����0�飬���óɱ�׼��ʶ���ҵ�7λ��Ϊ0ʱ���� */
	CAN_FilterInitStructure.FilterNumber = 0;						/* ���ù�������0 */
	CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;		/* ����ģʽ */
	CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;	/* 32λģʽ */
	CAN_FilterInitStructure.FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.FilterIdLow = 0x0000;
	CAN_FilterInitStructure.FilterMaskIdHigh = 0x0000;					/* ��CANOpen�б�׼��ʶ���ĵ�7λ��ʾ�ڵ�ID */
	CAN_FilterInitStructure.FilterMaskIdLow = 0x0000;					/* ��CANOpen��ֻ�ñ�׼��ʶ��������֡/Զ��֡���� */
	CAN_FilterInitStructure.FilterIdHigh = 0x0000;						/* ���ڵ�IDΪ0 */
	CAN_FilterInitStructure.FilterIdLow = 0x0000;						/* ��׼֡ */
	CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* ������������FIFO0 */
	CAN_FilterInitStructure.FilterActivation = ENABLE;					/* ʹ�ܹ����� */
    CAN_FilterInitStructure.BankNumber=14;
	HAL_CAN_ConfigFilter(&CAN_InitStructure,&CAN_FilterInitStructure);
	
	HAL_CAN_Receive_IT(&CAN_InitStructure, CAN_FIFO0); 
	

	/* ���ù�����0�飬���óɱ�׼��ʶ���ҵ�7λ��Ϊ0ʱ���� */
//	CAN_FilterInitStructure2.FilterNumber = 0;						/* ���ù�������0 */
//	CAN_FilterInitStructure2.FilterMode = CAN_FILTERMODE_IDMASK;		/* ����ģʽ */
//	CAN_FilterInitStructure2.FilterScale = CAN_FILTERSCALE_32BIT;	/* 32λģʽ */
//	CAN_FilterInitStructure2.FilterIdHigh = 0x0000;
//	CAN_FilterInitStructure2.FilterIdLow = 0x0000;
//	CAN_FilterInitStructure2.FilterMaskIdHigh = 0x0000;					/* ��CANOpen�б�׼��ʶ���ĵ�7λ��ʾ�ڵ�ID */
//	CAN_FilterInitStructure2.FilterMaskIdLow = 0x0000;					/* ��CANOpen��ֻ�ñ�׼��ʶ��������֡/Զ��֡���� */
//	CAN_FilterInitStructure2.FilterIdHigh = 0x0000;						/* ���ڵ�IDΪ0 */
//	CAN_FilterInitStructure2.FilterIdLow = 0x0000;						/* ��׼֡ */
//	CAN_FilterInitStructure2.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* ������������FIFO0 */
//	CAN_FilterInitStructure2.FilterActivation = ENABLE;					/* ʹ�ܹ����� */
//    CAN_FilterInitStructure2.BankNumber=14;
//	HAL_CAN_ConfigFilter(&CAN_InitStructure2,&CAN_FilterInitStructure2);
//	
//	HAL_CAN_Receive_IT(&CAN_InitStructure2, CAN_FIFO0); 
}


void CAN1_RX0_IRQHandler(void)
{
	CanRxMsgTypeDef message;
	Message Rx_Message;

	/* ������Ϣ */
	//CAN_Receive(CAN1, CAN_FIFO0, &message);
	HAL_CAN_Receive(&CAN_InitStructure, CAN_FIFO0, 10);

	/* ��װcanopen���ݰ� */
	Rx_Message.cob_id = message.StdId;						/* ������ͽڵ�ID */
	Rx_Message.rtr = (message.RTR == CAN_RTR_DATA) ? 0 : 1;	/* ��ʶ������ */
	Rx_Message.len = message.DLC;							/* ���ݰ����� */
	memcpy(Rx_Message.data, message.Data, message.DLC);		/* ���� */

	/* canopen���ݰ����䴦���� */
	canDispatch(&masterObjdict_Data, &Rx_Message);
}

//void CAN2_RX0_IRQHandler(void)
//{
//	CanRxMsgTypeDef message;
//	Message Rx_Message;
//
//	/* ������Ϣ */
//	//CAN_Receive(CAN1, CAN_FIFO0, &message);
//	HAL_CAN_Receive(&CAN_InitStructure2, CAN_FIFO0, 10);
//
//	/* ��װcanopen���ݰ� */
//	Rx_Message.cob_id = message.StdId;						/* ������ͽڵ�ID */
//	Rx_Message.rtr = (message.RTR == CAN_RTR_DATA) ? 0 : 1;	/* ��ʶ������ */
//	Rx_Message.len = message.DLC;							/* ���ݰ����� */
//	memcpy(Rx_Message.data, message.Data, message.DLC);		/* ���� */
//
//	/* canopen���ݰ����䴦���� */
//	canDispatch(&masterObjdict_Data, &Rx_Message);
//}

/* ����:	CAN�������ݺ���
	 ����:	notused can���߶˿�
			message canopen���ݰ�
	����ֵ:	0 �ɹ�
			1 ʧ��
 */
UNS8 canSend(CAN_PORT port, Message *message)
{
	UNS8 i = 0;
	uint32_t Timeout = 50;
	/* ��	װCAN���ݰ� */
	CAN_InitStructure.pTxMsg->DLC = message->len;							/* ���ݳ��� */
	CAN_InitStructure.pTxMsg->IDE = CAN_ID_STD;								/* ��׼ID */
	CAN_InitStructure.pTxMsg->StdId = message->cob_id;						/* ��ʶ�� */
	CAN_InitStructure.pTxMsg->RTR = (message->rtr == CAN_RTR_DATA) ? 0 : 2;	/* ����֡ */              
    for(i=0; i<message->len; i++)
    {
	    CAN_InitStructure.pTxMsg->Data[i] = message->data[i];
    }
	/* �������ݰ� */
	if(HAL_CAN_Transmit(&CAN_InitStructure, Timeout) == HAL_OK)
	{	
		//printf("HAL_CAN_Transmit Successful!��%d\r\n", __FILE__ , __LINE__);
		return 0;

	}
	else
	{
		printf("HAL_CAN_Transmit Failed!��%d\r\n", __FILE__ , __LINE__);
		return 0;

	}
	/* �ȴ����ͳɹ� */
	//while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && --i);

	/* �ɹ�0 ��ʱ1 */
	//return (i != 0) ? 0 : 1;
}
