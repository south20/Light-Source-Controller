// uart_communication.h

#ifndef IO_UART_H
#define IO_UART_H

#include "sys.h"
#include "stdio.h"	
#include <stdint.h>

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART1_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

// ��ʼ����������
void IO_UART_Init(u32 bound);

//���ʹ�������
void IO_UART_Transmit(uint8_t* data, uint16_t length);

// ���մ�������
void IO_UART_Receive(uint8_t* buffer, uint16_t* length);

#endif  // UART_COMMUNICATION_H
