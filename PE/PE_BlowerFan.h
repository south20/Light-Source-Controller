
#ifndef PE_BLOWER_FAN_H
#define PE_BLOWER_FAN_H

#include "IO_GPIO.h" // ����GPIO��ʼ��
#include "IO_DS1821.h" // �����¶ȼ�⺯��


//����tim9��ͨ��2ת�ټ���ʼ��
void CoolingFan_TIM9_CH2_Cap_Init(); 

int PE_CoolingFan_CheckSpeed(int* fan_rpm);


// �ķ��������ų�ʼ��
void PE_BlowerFan_Init();

//�ķ��ת�ٿ���
int PE_BlowerFan_Control(uint32_t cycle);

//�ķ��ת�ټ���ʼ��
void BlowerFan_TIM9_CH1_Cap_Init();

//�ķ��ת�ټ��
int PE_BlowerFan_CheckSpeed(int* fan_rpm);


#endif // PE_BLOWER_FAN_H
