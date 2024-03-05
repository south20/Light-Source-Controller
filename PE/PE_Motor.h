// PE_VA_Motor.h

#ifndef PE_MOTOR_H
#define PE_MOTOR_H

#include "IO_GPIO.h"  // ��������һ��GPIOģ�����ڴ���IO
#include "IO_Encoder.h"
#include "canfestival_master.h"

// ����������
typedef enum {
    MOTOR_DIRECTION_FORWARD,   // �����ת
    MOTOR_DIRECTION_REVERSE    // �����ת
} MotorDirection;

// �������˶�ģʽ
typedef enum {
    MOTOR_MODE_POSITION,   // λ������ģʽ
    MOTOR_MODE_VELOCITY    // �ٶ�����ģʽ
} MotorMode;

//VA���
int PE_Motor_Init();   // ��ʼ�����   ,VA�����Shutter���ͬʱ��ʼ��
int PE_VA_Motor_SetMode(MotorMode mode);   // ���õ���˶�ģʽ
int PE_VA_Motor_SetSpeed(int speed);   // ���õ���ٶ�
int PE_VA_Move_Position(int cur_position, int end_position); // ����ƶ�����
int PE_VA_Get_Zero_Pos_Sensor(IO_GPIO_Pin_State* bitstatus); // �����λ�������Ƿ񴥷�
int PE_VA_Move_Abs(int distance); // ��������˶�ĳ��λ��
int PE_VA_Move_Rel(int distance); // �������˶���ĳ������
double PE_VA_Get_Position(double     *       distance); // ��ȡ�����ǰλ��
int PE_VA_Home(); // VA����ص�Homeλ�� ����λ����������λ��
int PE_VA_Stop(); // VA�����ͣ����

// �������˶�ģʽ
typedef enum {
    Shutter_MOTOR_MODE_POSITION,   // λ������ģʽ
    Shutter_MOTOR_MODE_VELOCITY    // �ٶ�����ģʽ
} Shutter_MotorMode;

//Shutte���
int PE_Shutter_Motor_SetMode(MotorMode mode);   // ���õ���˶�ģʽ
int PE_Shutter_Motor_SetSpeed(int speed);   // ���õ���ٶ�
int PE_Shutter_Move_Position(int cur_position, int end_position); // ����ƶ�����
int PE_Shutter_Move_Abs(int distance); // ��������˶�ĳ��λ��
int PE_Shutter_Move_Rel(int distance); // �������˶���ĳ������
int PE_Shutter_Open();  //shutter��
int PE_Shutter_Close();  //shutter�ر�
int PE_Shutter_Home();  //Ĭ��Ϊ�ص�shutter�ص�λ����������λ��
int PE_Shutter_Stop(); // VA�����ͣ����



#endif // PE_VA_MOTOR_H
