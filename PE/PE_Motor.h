// PE_VA_Motor.h

#ifndef PE_MOTOR_H
#define PE_MOTOR_H

#include "IO_GPIO.h"  // 假设你有一个GPIO模块用于处理IO
#include "IO_Encoder.h"
#include "canfestival_master.h"

// 定义电机方向
typedef enum {
    MOTOR_DIRECTION_FORWARD,   // 电机正转
    MOTOR_DIRECTION_REVERSE    // 电机反转
} MotorDirection;

// 定义电机运动模式
typedef enum {
    MOTOR_MODE_POSITION,   // 位置轮廓模式
    MOTOR_MODE_VELOCITY    // 速度轮廓模式
} MotorMode;

//VA电机
int PE_Motor_Init();   // 初始化电机   ,VA点击和Shutter电机同时初始化
int PE_VA_Motor_SetMode(MotorMode mode);   // 设置电机运动模式
int PE_VA_Motor_SetSpeed(int speed);   // 设置电机速度
int PE_VA_Move_Position(int cur_position, int end_position); // 电机移动功能
int PE_VA_Get_Zero_Pos_Sensor(IO_GPIO_Pin_State* bitstatus); // 检测零位传感器是否触发
int PE_VA_Move_Abs(int distance); // 电机绝对运动某个位置
int PE_VA_Move_Rel(int distance); // 电机相对运动到某个距离
double PE_VA_Get_Position(double     *       distance); // 获取电机当前位置
int PE_VA_Home(); // VA电机回到Home位置 即零位传感器触发位置
int PE_VA_Stop(); // VA电机急停功能

// 定义电机运动模式
typedef enum {
    Shutter_MOTOR_MODE_POSITION,   // 位置轮廓模式
    Shutter_MOTOR_MODE_VELOCITY    // 速度轮廓模式
} Shutter_MotorMode;

//Shutte电机
int PE_Shutter_Motor_SetMode(MotorMode mode);   // 设置电机运动模式
int PE_Shutter_Motor_SetSpeed(int speed);   // 设置电机速度
int PE_Shutter_Move_Position(int cur_position, int end_position); // 电机移动功能
int PE_Shutter_Move_Abs(int distance); // 电机绝对运动某个位置
int PE_Shutter_Move_Rel(int distance); // 电机相对运动到某个距离
int PE_Shutter_Open();  //shutter打开
int PE_Shutter_Close();  //shutter关闭
int PE_Shutter_Home();  //默认为回到shutter关到位传感器触发位置
int PE_Shutter_Stop(); // VA电机急停功能



#endif // PE_VA_MOTOR_H
