/*****


��ȡ���еĴ�����ֵ


****/

#include "IO_GPIO.h"
#include <sys.h>


//�ⲿ�����Ŀ�������ж϶�����ֵȥ�������˺���ֻ�Ƕ�����Ҫ���ϲ��װ�жϴ���
int PE_Get_GPIO_ShuterClose(IO_GPIO_Pin_State* bitstatus);
//�ⲿ�����Ŀ�������
int PE_Get_GPIO_ShuterOpen(IO_GPIO_Pin_State * bitstatus);


//��ȡ�ж��Ƿ�ص�λ����Ӧ���ſ��ش�����
int PE_Get_GPIO_ShuterCloseSensor(IO_GPIO_Pin_State* bitstatus);
//�ж��Ƿ񿪵�λ
int PE_Get_GPIO_ShuterOpenSensor(IO_GPIO_Pin_State * bitstatus);


//��ȡ�¶�,���ֵΪtemperature
int PE_Get_GPIO_TempertureSensor(uint16_t* temperature);

//ʱ����¼�����ֵΪduration
int PE_Get_GPIO_DurationRecorderSensor(uint16_t* duration);

//�⵼�ܴ������Ƿ����
int PE_Get_GPIO_LightTubeDetectionSensor(IO_GPIO_Pin_State* bitstatus);

//��ȡ���������������������Ϊ�������������ֵΪ������
 int PE_Get_GPIO_LightEnergySensor(u8 times,uint16_t* LightEnergy);


//�ж��˹�Ƭ�Ƿ�ΪSMEEԤ�����࣬�Ƿ���1���񷵻�0
int PE_Is_SMEE_Filter();

















