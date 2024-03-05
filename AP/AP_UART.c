// AP_Uart.c

#include "AP_UART.h"

// ����һЩ�����ʶ
#define CMD_LED_ON  "LED_ON"
#define CMD_LED_OFF "LED_OFF"

// ������յ���ָ��
void processReceivedCommand(uint8_t* command, uint16_t length)
{
    // ��������ݾ����ҵ������������յ���ָ��
    if (strncmp((char*)command, CMD_LED_ON, strlen(CMD_LED_ON)) == 0)
    {
        // ִ��LED�򿪲���
        printf("Received command: %s\n", CMD_LED_ON);
        // ������Ե�����Ӧ�Ĺ��ܺ���ִ��LED�򿪵Ĳ���
    }
    else if (strncmp((char*)command, CMD_LED_OFF, strlen(CMD_LED_OFF)) == 0)
    {
        // ִ��LED�رղ���
        printf("Received command: %s\n", CMD_LED_OFF);
        // ������Ե�����Ӧ�Ĺ��ܺ���ִ��LED�رյĲ���
    }
    else
    {
        // δ֪����
        printf("Unknown command received\n");
    }
}

void AP_UART_Init(u32 bound)
{
    // ����IO�㺯�����д��ڳ�ʼ��
    IO_UART_Init(bound);
}

void AP_UART_SendData(uint8_t* data, uint16_t length)
{
    // ����IO�㺯�����ʹ�������
    IO_UART_Transmit(data, length);
}

void AP_UART_ReceiveData(uint8_t* buffer, uint16_t* length)
{
    // ����IO�㺯�����մ�������
    IO_UART_Receive(buffer, length);
	
    // ������յ���ָ��
    processReceivedCommand(buffer, *length);
}

