#ifndef AP_GE_H
#define AP_GE_H

#include "24cxx.h"

typedef enum {
    NO_Error = 0,                    // 操作成功
    ERROR_SYSTEM_INFO = 1,          // 设置或获取系统信息错误
    ERROR_NAME_ID = 2,              // 设置或获取系统名称ID错误
    ERROR_SOFTWARE_VERSION = 3,     // 设置或获取设备软件版本号错误
    ERROR_RUNNING_TIME = 4,         // 设置或获取设备运行时间错误
    ERROR_OPERATION_MODE = 5,       // 设置或获取操作模式错误
    ERROR_LED_LEVEL = 6,            // 设置或获取光源档位错误
    ERROR_LED_STATE = 7,            // 获取LED状态错误
    ERROR_EXPOSURE_RUNTIME = 8,     // 设置或获取曝光运行时间错误
    ERROR_EVENT_TEXT = 9,           // 获取事件文本错误
    ERROR_ERROR_TEXT = 10,          // 获取错误文本错误
    ERROR_SHUTTER = 11,             // 打开或关闭快门错误
    ERROR_CLOSE_SHUTTER = 12,       // 关闭快门错误
    ERROR_OPEN_SHUTTER = 13,        // 打开快门错误
    ERROR_GET_EVENT_TEXT = 14,      // 获取事件文本错误
    ERROR_GET_ERROR_TEXT = 15,      // 获取错误文本错误
    ERROR_Initialized = 16,      // 获取错误文本错误
    ERROR_LG_Not_Insert = 17,      // 光导管未插入错误
    ERROR_FAN_or_BlowerFAN = 18,      // 风扇鼓风机转速错误
    ERROR_LIGHT_POWER = 19,      // 风扇鼓风机转速错误
    ERROR_VA_LEVEL = 20,      // 电机档位错误
    ERROR_OPEN_LIGHT = 21,      // 电机档位错误
    // 可以根据需要添加更多错误码
} ErrorCode;


typedef enum {
    TRIGGER_MODE = 0,
    TIMER_MODE = 1
} OperationMode;

//注：AP4A接口只提供光源控制器内部调用，不对外提供。
int AP_Initialize();          //初始化流程
int AP4A_Set_System_Info(char * info);    //设置系统信息,长度不超过16
int AP_Get_System_Info(char * info);   //获取系统信息
int AP4A_Set_System_NameID(char * nameID);   //设置系统名称ID，长度不超过16
int AP_Get_System_NameID(char * nameID);   //获取系统名称ID
int AP4A_Set_Device_Software_Version(char * version);   //设置设备软件版本号，长度不超过16
int AP_Get_Device_Software_Version(char * version);   //获取设备软件版本号
int AP_Set_Device_Running_Time(int run_time);   //获取设备运行时间
int AP_Get_Device_Running_Time(int *run_time);   //获取设备运行时间
//operation mode:0.Triggle mode 1:Timer mode  目前默认Timer mode
int AP_Set_Operation_Mode(OperationMode ope_mode);   //设置操作模式
int AP_Get_Operation_Mode(OperationMode* ope_mode);   //获取操作模式

//0度：notch检测传感器         15度：0档     285度:100档   档间距：27度
int AP_Set_Led_Level(int ope_mode);   //设置光源档位， 0~100
int AP_Get_Led_Level(int* ope_mode);   //获取光源档位
int AP_Switch_On_Led();   //打开光源
int AP_Switch_Off_Led();   //关闭光源
int AP_Change_Led_Power(int power);   //改变光强档位到power档，需要在光源打开的情况下进行操作
int AP_Switch_On_Led_With_Power(int power);   //打开光源并到power档
int AP_Get_Led_State(char * led_state);   //获取Led状态
int AP_Get_Exposure_Runtime(int * run_time);   //获取曝光运行时间
int AP_Set_Exposure_Runtime(int run_time);   //设置曝光运行时间  ，需要再shutter关的条件下操作
int AP_Get_Event_Text();   //
int AP_Get_Error_Text();   //
int AP_Open_Shutter();   //
int AP_Close_Shutter();   //

#endif
