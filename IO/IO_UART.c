// uart_communication.c

#include "IO_UART.h"
#include "stm32f7xx_hal.h"  


void IO_UART_Init(u32 bound)
{ 
	// 使能GPIOA和USART1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    // GPIO配置为USART1
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FAST;
    GPIO_Initure.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
}


void IO_UART_Transmit(uint8_t* data, uint16_t length) 
{
    // 调用HAL库函数发送数据，等待发送完成
    HAL_UART_Transmit(&UART1_Handler, data, length, HAL_MAX_DELAY);
}

void IO_UART_Receive(uint8_t* buffer, uint16_t* length) {
    // 调用HAL库函数接收数据，等待接收完成
    HAL_UART_Receive(&UART1_Handler, buffer, *length, HAL_MAX_DELAY);
}
