// AP_Uart.h

#ifndef AP_UART_H
#define AP_UART_H

#include "IO_UART.h"

// ��ʼ����������
void AP_UART_Init(u32 bound);

// ���ʹ�������
void AP_UART_SendData(uint8_t* data, uint16_t length);

// ���մ�������
void AP_UART_ReceiveData(uint8_t* buffer, uint16_t* length);

#endif  // AP_UART_H

