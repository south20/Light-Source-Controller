#ifndef AP_GE_H
#define AP_GE_H

#include "24cxx.h"

typedef enum {
    NO_Error = 0,                    // �����ɹ�
    ERROR_SYSTEM_INFO = 1,          // ���û��ȡϵͳ��Ϣ����
    ERROR_NAME_ID = 2,              // ���û��ȡϵͳ����ID����
    ERROR_SOFTWARE_VERSION = 3,     // ���û��ȡ�豸����汾�Ŵ���
    ERROR_RUNNING_TIME = 4,         // ���û��ȡ�豸����ʱ�����
    ERROR_OPERATION_MODE = 5,       // ���û��ȡ����ģʽ����
    ERROR_LED_LEVEL = 6,            // ���û��ȡ��Դ��λ����
    ERROR_LED_STATE = 7,            // ��ȡLED״̬����
    ERROR_EXPOSURE_RUNTIME = 8,     // ���û��ȡ�ع�����ʱ�����
    ERROR_EVENT_TEXT = 9,           // ��ȡ�¼��ı�����
    ERROR_ERROR_TEXT = 10,          // ��ȡ�����ı�����
    ERROR_SHUTTER = 11,             // �򿪻�رտ��Ŵ���
    ERROR_CLOSE_SHUTTER = 12,       // �رտ��Ŵ���
    ERROR_OPEN_SHUTTER = 13,        // �򿪿��Ŵ���
    ERROR_GET_EVENT_TEXT = 14,      // ��ȡ�¼��ı�����
    ERROR_GET_ERROR_TEXT = 15,      // ��ȡ�����ı�����
    ERROR_Initialized = 16,      // ��ȡ�����ı�����
    ERROR_LG_Not_Insert = 17,      // �⵼��δ�������
    ERROR_FAN_or_BlowerFAN = 18,      // ���ȹķ��ת�ٴ���
    ERROR_LIGHT_POWER = 19,      // ���ȹķ��ת�ٴ���
    ERROR_VA_LEVEL = 20,      // �����λ����
    ERROR_OPEN_LIGHT = 21,      // �����λ����
    // ���Ը�����Ҫ��Ӹ��������
} ErrorCode;


typedef enum {
    TRIGGER_MODE = 0,
    TIMER_MODE = 1
} OperationMode;

//ע��AP4A�ӿ�ֻ�ṩ��Դ�������ڲ����ã��������ṩ��
int AP_Initialize();          //��ʼ������
int AP4A_Set_System_Info(char * info);    //����ϵͳ��Ϣ,���Ȳ�����16
int AP_Get_System_Info(char * info);   //��ȡϵͳ��Ϣ
int AP4A_Set_System_NameID(char * nameID);   //����ϵͳ����ID�����Ȳ�����16
int AP_Get_System_NameID(char * nameID);   //��ȡϵͳ����ID
int AP4A_Set_Device_Software_Version(char * version);   //�����豸����汾�ţ����Ȳ�����16
int AP_Get_Device_Software_Version(char * version);   //��ȡ�豸����汾��
int AP_Set_Device_Running_Time(int run_time);   //��ȡ�豸����ʱ��
int AP_Get_Device_Running_Time(int *run_time);   //��ȡ�豸����ʱ��
//operation mode:0.Triggle mode 1:Timer mode  ĿǰĬ��Timer mode
int AP_Set_Operation_Mode(OperationMode ope_mode);   //���ò���ģʽ
int AP_Get_Operation_Mode(OperationMode* ope_mode);   //��ȡ����ģʽ

//0�ȣ�notch��⴫����         15�ȣ�0��     285��:100��   ����ࣺ27��
int AP_Set_Led_Level(int ope_mode);   //���ù�Դ��λ�� 0~100
int AP_Get_Led_Level(int* ope_mode);   //��ȡ��Դ��λ
int AP_Switch_On_Led();   //�򿪹�Դ
int AP_Switch_Off_Led();   //�رչ�Դ
int AP_Change_Led_Power(int power);   //�ı��ǿ��λ��power������Ҫ�ڹ�Դ�򿪵�����½��в���
int AP_Switch_On_Led_With_Power(int power);   //�򿪹�Դ����power��
int AP_Get_Led_State(char * led_state);   //��ȡLed״̬
int AP_Get_Exposure_Runtime(int * run_time);   //��ȡ�ع�����ʱ��
int AP_Set_Exposure_Runtime(int run_time);   //�����ع�����ʱ��  ����Ҫ��shutter�ص������²���
int AP_Get_Event_Text();   //
int AP_Get_Error_Text();   //
int AP_Open_Shutter();   //
int AP_Close_Shutter();   //

#endif
