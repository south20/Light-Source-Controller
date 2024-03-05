// uart_communication.h

#ifndef IO_UART_H
#define IO_UART_H

#include "sys.h"
#include "stdio.h"	
#include <stdint.h>

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer

// 初始化串口配置
void IO_UART_Init(u32 bound);

//发送串口配置
void IO_UART_Transmit(uint8_t* data, uint16_t length);

// 接收串口配置
void IO_UART_Receive(uint8_t* buffer, uint16_t* length);

#endif  // UART_COMMUNICATION_H
