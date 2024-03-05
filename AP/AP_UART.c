// AP_Uart.c

#include "AP_UART.h"

// 定义一些命令标识
#define CMD_LED_ON  "LED_ON"
#define CMD_LED_OFF "LED_OFF"

// 处理接收到的指令
void processReceivedCommand(uint8_t* command, uint16_t length)
{
    // 在这里根据具体的业务需求解析接收到的指令
    if (strncmp((char*)command, CMD_LED_ON, strlen(CMD_LED_ON)) == 0)
    {
        // 执行LED打开操作
        printf("Received command: %s\n", CMD_LED_ON);
        // 这里可以调用相应的功能函数执行LED打开的操作
    }
    else if (strncmp((char*)command, CMD_LED_OFF, strlen(CMD_LED_OFF)) == 0)
    {
        // 执行LED关闭操作
        printf("Received command: %s\n", CMD_LED_OFF);
        // 这里可以调用相应的功能函数执行LED关闭的操作
    }
    else
    {
        // 未知命令
        printf("Unknown command received\n");
    }
}

void AP_UART_Init(u32 bound)
{
    // 调用IO层函数进行串口初始化
    IO_UART_Init(bound);
}

void AP_UART_SendData(uint8_t* data, uint16_t length)
{
    // 调用IO层函数发送串口数据
    IO_UART_Transmit(data, length);
}

void AP_UART_ReceiveData(uint8_t* buffer, uint16_t* length)
{
    // 调用IO层函数接收串口数据
    IO_UART_Receive(buffer, length);
	
    // 处理接收到的指令
    processReceivedCommand(buffer, *length);
}

