// AP_Uart.h

#ifndef AP_UART_H
#define AP_UART_H

#include "IO_UART.h"

// 初始化串口配置
void AP_UART_Init(u32 bound);

// 发送串口数据
void AP_UART_SendData(uint8_t* data, uint16_t length);

// 接收串口数据
void AP_UART_ReceiveData(uint8_t* buffer, uint16_t* length);

#endif  // AP_UART_H

