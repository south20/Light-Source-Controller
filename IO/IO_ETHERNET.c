// IO_ETHERNET.c

#include "IO_ETHERNET.h"
//#include "lwip_comm.h"
#include "lcd.h"  // ������LCD�ĳ�ʼ������
/*
void IO_Ethernet_Init(void) {
    // ��ʼ��LAN8720A
    LAN8720_Init();  // �������ľ������ʵ���������

    // �����ڴ�
    mem_init();  // �������ľ������ʵ���������

    // ����DHCP����
    dhcp_init();  // �������ľ������ʵ���������

    // ��Ӳ�������
    netif_add(...);   // �������ľ������ʵ���������
    netif_set_up(...); // �������ľ������ʵ���������

    // ��ʼ��LWIP
    lwip_comm_init();  // �������ľ������ʵ���������
}

void IO_Ethernet_Open(void) {
    // �ȴ�DHCP��ȡIP�ɹ������ʧ����ʹ�þ�̬IP
    // ������Ҫ���ݾ�������ȴ�IP��ȡ�ɹ�������ʹ��dhcp_supplied_address(&netif)�Ⱥ����ж�

    // ����Web Server����
    web_server_init();  // �������ľ������ʵ���������
}

void IO_Ethernet_Close(void) {
    // ���Ը���ʵ������ر�Ethernetģ���һЩ����
}

void IO_Ethernet_Transmit(uint8_t* data, uint16_t length) {
    // �������ݵ�Ethernetģ��
    // ʹ��lwip_comm_send�Ⱥ�����������
    lwip_comm_send(data, length);  // �������ľ������ʵ���������
}

void IO_Ethernet_Receive(uint8_t* buffer, uint16_t timeout) {
    // ��Ethernetģ��������ݣ��������ó�ʱ
    // ʹ��lwip_comm_receive�Ⱥ�����������
    lwip_comm_receive(buffer, timeout);  // �������ľ������ʵ���������
}
*/
// ���ﻹ���Ը�����Ҫ�����������ʵ��
