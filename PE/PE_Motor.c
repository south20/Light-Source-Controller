// PE_VA_Motor.c

#include "PE_Motor.h"
#include "IO_GPIO.h"  // ���滻�����GPIOģ���ͷ�ļ�
#include "PE_BASE.h"  // ���滻�����GPIOģ���ͷ�ļ�
#include <stdio.h>

// ģ������������ʵ��
static MotorDirection currentDirection = MOTOR_DIRECTION_FORWARD;
static MotorMode currentMode = MOTOR_MODE_POSITION;
static int currentSpeed = 0;
static int currentPosition = 0;

int m_initialized;
int m_shutter_initialized;

//�����ֵ�״̬˵��
//λ������ģʽ��
// 607Ah:Ŀ��λ��    bit6(0:���Զ�λ       1����Զ�λ)   
// 6081h:�����ٶ�         ������ 16.16 �����ݡ� MSB Ϊ�������֣� LSB ΪС�����֡�
// 6083h:�������ٶ�
// 6085h:����ֹͣ���ٶ�
// 2023h:�Ӽ���ʱ��
// 6086h: �˶���������
// 6062h:λ������ֵ
// 6063h:λ��ʵ���ڲ�ֵ
// 6064h:λ��ʵ��ֵ
// 6065h:�������
// 6066h:�������ʱ
// 6067h:λ�ô�
// 6060h:����˶�ģʽ    ��    	1λ������ģʽ 3.�ٶ�����ģʽ
// 6068h:λ�ô���ʱ��
// 607Bh:λ�÷�Χ����
// 60F2h:��λѡ�����
// 60F4h:�������ʵ��ֵ
// 60FCh:λ�������ڲ�ֵ
// 2022h:���ƽ��
// 2081h:����/����ʵ�ʵ��λ��0 ���� 2088h:�����������ʵ���ڲ�λ��
// 208Dh:����������λ��


//��ʮ��������ת��Ϊ32Ϊ��16��������
void VerIntToHex(int a, int *v1, int *v2, int *v3, int *v4)
{
    // ������ת��Ϊʮ�������ַ���
    char hex_string[9]; // 8 characters plus null terminator
    
    sprintf(hex_string, "%08x", a);
    
    // ���ַ������Ϊ�Ĳ���
    sscanf(hex_string, "%2x%2x%2x%2x", v1, v2, v3, v4);
}


// ��ʼ�����
//��ʼ��Ĭ��ѡ��λ������ģʽ
int PE_Motor_Init()
{
	IO_Encoder_Tim_Init(4095, 0);
	IO_Encoder_Start_Count();
	
	printf("Ready in CAN ...\r\n");
	//setNodeId(&masterObjdict_Data, 0x01);
	setNodeId(&masterObjdict_Data, 0x00);
	setState(&masterObjdict_Data, Initialisation);
	for(int i = 1; i < NMT_MAX_NODE_ID;i++)
	{
		masterObjdict_Data.NMTable[i] = Unknown_state;
	}
	setState(&masterObjdict_Data, Pre_operational);

	while(masterObjdict_Data.nodeState != Pre_operational)
	{
		delay_ms(10);
	}

	delay_ms(1000);
    stopSYNC(&masterObjdict_Data); 
	
	 
	  //λ������ģʽ
	uint16_t message_sdo[][9]=
	{
		{0x601,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // ���ڵ�״̬�� Switch on disabled ����Ϊ Ready to switch on
		{0x601,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // ���ڵ�״̬�� Ready to switch on ����Ϊ Switched on
		{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
		{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
		{0x601,0x23,0x7a,0x60,0x00,0xd0,0x07,0x00,0x00},  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
		{0x601,0x23,0x81,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
		{0x601,0x23,0x83,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 83 60 00 01 00 00 80 ��Ŀ����ٶ�����Ϊ1rps/s
//		{0x601,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		
	};
		
		  //λ������ģʽ
	uint16_t message_sdo2[][9]=
	{
		{0x602,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // ���ڵ�״̬�� Switch on disabled ����Ϊ Ready to switch on
		{0x602,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // ���ڵ�״̬�� Ready to switch on ����Ϊ Switched on
		{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
		{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
		{0x602,0x23,0x7a,0x60,0x00,0xd0,0x07,0x00,0x00},  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
		{0x602,0x23,0x81,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
		{0x602,0x23,0x83,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 83 60 00 01 00 00 80 ��Ŀ����ٶ�����Ϊ1rps/s
//		{0x602,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		
	};

	delay_ms(3000);
	//i = 0;
	for(int i = 0;i < 2; ++i)
	{
		canopen_send_sdo(message_sdo[i]);     //��ʼ��VA_Motor���
	    delay_ms(280);
	}
	
	delay_ms(1200);
	canopen_send_sdo(message_sdo[2]);	  //��ʼ��VA_Motor���
	for(int i = 3; i < 7;++i)
	{
		delay_ms(280);
		canopen_send_sdo(message_sdo[i]);	  //��ʼ��VA_Motor���
	}


	delay_ms(300);
	

//	setNodeId(&masterObjdict_Data, 0x02);
//	setState(&masterObjdict_Data, Initialisation);
//	setState(&masterObjdict_Data, Pre_operational);
//	while(masterObjdict_Data.nodeState != Pre_operational)
//	{
//		delay_ms(10);
//	}

		delay_ms(280);
	for(int i = 0;i < 2; ++i)
	{
		canopen_send_sdo2(message_sdo2[i]);     //��ʼ��Shutter_Motor���
	    delay_ms(280);
	}
	
	delay_ms(1200);
	canopen_send_sdo2(message_sdo2[2]);	  //��ʼ��Shutter_Motor���
	for(int i = 3; i < 7;++i)
	{
		delay_ms(280);
		canopen_send_sdo2(message_sdo2[i]);	  //��ʼ��Shutter_Motor���
	}

	
		delay_ms(280);
	
	m_initialized = TRUE;
	m_shutter_initialized = TRUE;

}


// ����VA����˶�ģʽ
int PE_VA_Motor_SetMode(MotorMode mode) 
{
	if(TRUE == m_initialized)
	{
		  //λ������ģʽ
		if(MOTOR_MODE_POSITION == mode)
		{
			uint16_t message_sdo[][9]=
			{
				{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
			};
			
			canopen_send_sdo(message_sdo[0]);
			delay_ms(300);
		}
		else if(MOTOR_MODE_VELOCITY == mode)
		{
			uint16_t message_sdo[][9]=
			{
				{0x601,0x2f,0x60,0x60,0x00,0x03,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ���ٶ�ģʽ 01:Profile Position Mode
			};

			canopen_send_sdo(message_sdo[0]);
			delay_ms(300);
		}
		currentMode = mode;
	}
	else
	{
		ErrorStatus;
	}
    // ʵ������ģʽ�Ĵ���
}

// ���õ���ٶ�
int PE_VA_Motor_SetSpeed(int speed) 
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(speed, &v1, &v2, &v3, &v4);

	uint16_t message_sdo[][9]=
  	{
		//0x01,0x00,0x00,0x00:��ʾ1.00    Ĭ���ٶ�Ϊ1���������Ż�
		{0x601,0x23,0x81,0x60,0x00,v4,v3,v2,v1},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
	};
	
	canopen_send_sdo(message_sdo[0]);
	delay_ms(300);
    currentSpeed = speed;
    // ʵ�������ٶȵĴ���
}


// ����ƶ�����
int PE_VA_Move_Position(int cur_position, int end_position) {
    // ʵ�ֵ���ƶ����ܵĴ���

	if(TRUE == m_initialized)
	{
		int dart_distance = (end_position - cur_position);
		
		int v1 = 0;
		int v2 = 0;
		int v3 = 0;
		int v4 = 0;
		VerIntToHex(dart_distance, &v1, &v2, &v3, &v4);
		
		uint16_t message_sdo[][9]=
	  	{
			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
			{0x601,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		};
		
		for(int i = 0 ; i < 3;++i)
		{
			canopen_send_sdo(message_sdo[i]);
			delay_ms(300);
		}
	}
	else
	{
		ErrorStatus;
	}
    // ����ֵ���ܱ�ʾ�ɹ���ʧ�ܣ������������ʵ��������
    return 0;
}

// �����λ�������Ƿ񴥷�
int PE_VA_Get_Zero_Pos_Sensor(IO_GPIO_Pin_State* bitstatus) {
    // ʵ�ּ����λ�������Ĵ���
    // ����ֵ���ܱ�ʾ�ɹ���ʧ�ܣ������������ʵ��������
    int rtn = 0;
    rtn = IO_GPIO_Read_Pin(IO_GPIO_ZERO_POS, bitstatus);
    return 0;
}

// ��������˶�ĳ��λ��
int PE_VA_Move_Abs(int distance)
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
//	distance += 200;
//	printf("distance = %d \r\n", distance);
	VerIntToHex(distance, &v1, &v2, &v3, &v4);
	
//	int32_t Start_Count_Val = IO_Encoder_Get_Count_Val();
	if(TRUE == m_initialized)
	{
		uint16_t message_sdo[][9]=
	  	{
			{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
			{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
			{0x601,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // ������λ����
			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
			{0x601,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		};
		
		for(int i = 0 ; i < 6;++i)
		{
			canopen_send_sdo(message_sdo[i]);
			delay_ms(10);
		}
	}
	else
	{
		ErrorStatus;
	}
    return 0;
}

// �������˶���ĳ������
int PE_VA_Move_Rel(int distance) {
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(distance, &v1, &v2, &v3, &v4);
	
//	int32_t Start_Count_Val = IO_Encoder_Get_Count_Val();
	if(TRUE == m_initialized)
	{
		uint16_t message_sdo[][9]=
	  	{
			{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
			{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
			{0x601,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // ������λ����
			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
			{0x601,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		};
		
			delay_ms(10);
		for(int i = 0 ; i < 6;++i)
		{
			canopen_send_sdo(message_sdo[i]);
			delay_ms(10);
		}
	}
	else
	{
		ErrorStatus;
	}
    // ����ֵ���ܱ�ʾ�ɹ���ʧ�ܣ������������ʵ��������
    
//	int32_t Stop_Count_Val = IO_Encoder_Get_Count_Val();

	//��ȷ�ƶ���Ŀ��λ��
//	int32_t dart = distance - (Stop_Count_Val - Start_Count_Val);
//	VerIntToHex(dart, &v1, &v2, &v3, &v4);
//	if(TRUE == m_shutter_initialized)
//	{
//		uint16_t message_sdo[][9]=
//	  	{
//			{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
//			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
//			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
//			{0x601,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
//		};
//		
//		for(int i = 0 ; i < 4;++i)
//		{
//			canopen_send_sdo(message_sdo[i]);
//			delay_ms(300);
//		}
//	}
//	else
//	{
//		ErrorStatus;
//	}
    return 0;
}

//���ص��ǵ�ǰ�Ƕ�ֵ��
double PE_VA_Get_Position(double     *       distance){
	*distance = IO_Encoder_Get_Only_Rel_Pos();
    // ����ֵ���ܱ�ʾ�ɹ���ʧ�ܣ������������ʵ��������
    return 0;
}

//��λ��⴫��������ʱ����ʱ�����˶���180�ȵ�λ�ã������ˣ�
//0��~300�� ��Ӧ0��~100��
//����0��Ϊ0λ��⴫������תһ���ǶȺ��λ�á���λ����Ҫ�궨
// func:
int PE_VA_Home() 
{
	int rtn = 0;
		//ѡ����λ��
		  //λ������ģʽ
		  //��λ����Ϊ��λ��⴫������������ת180��λ��
		  //��ʱ [0~300��]��Ӧ [0����100��]           ��Ҫ�궨
		  //ƫ�ã�offset
	uint16_t message_sdo3[][9]=
	{
		{0x601,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // ���ڵ�״̬�� Switch on disabled ����Ϊ Ready to switch on
		{0x601,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // ���ڵ�״̬�� Ready to switch on ����Ϊ Switched on
		{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
		{0x601,0x23,0x99,0x60,0x02,0x03,0x00,0x00,0x00},  // Ѱ�����Ļ����ٶ�
		{0x601,0x23,0x99,0x60,0x01,0x01,0x00,0x00,0x00},  // Ѱ�ҿ���ʱ�Ļ����ٶ�
		{0x601,0x2b,0xB8,0x60,0x00,0x0E,0x0E,0x00,0x00},  // ������λ����
		{0x601,0x23,0x9A,0x60,0x00,0x0A,0x00,0x00,0x00},  //  ������ٶ�
//		{0x601,0x23,0x7c,0x60,0x00,0x00,0x00,0x00,0x00},  //  ԭ��ƫ��          TODO:��Ҫ�궨20231221
		{0x601,0x23,0x7c,0x60,0x00,0xC8,0x00,0x00,0x00},  //  ԭ��ƫ��          TODO:��Ҫ�궨20231221
		{0x601,0x2F,0x98,0x60,0x00,0x11,0x00,0x00,0x00},  // ���㷽ʽ
		{0x601,0x2F,0x60,0x60,0x00,0x06,0x00,0x00,0x00},  // ����ģʽ
		{0x601,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  //��ʼ����
		//�ȴ��������
		{0x601,0x40,0x64,0x60,0x00,0x00,0x00,0x00,0x00},  //�����ʵ��λ��
		
	};

	delay_ms(300);
	//i = 0;
	for(int i = 0;i < 13; ++i)
	{
		canopen_send_sdo(message_sdo3[i]);     //��ʼ��VA_Motor���
	    delay_ms(300);
	}
	
	   // delay_ms(9000);
//	IO_GPIO_Pin_State bitstatus = 0;
//	PE_VA_Get_Zero_Pos_Sensor(&bitstatus);
//
//	//�����⵽��λ�����������㵵λ�������壬û�м�⵽�򱨴�
//	if(bitstatus)
//	{

   
//		PE_VA_Move_Abs(1);    //

//		IO_Encoder_Clear_Count_Val();
//	}
//	else
//	{
//		rtn = PE_ERROR_PE_VA_Home;
//	}
	return rtn;

}


// VA�����ͣ����
int PE_VA_Stop() 
{
	uint16_t message_sdo[][9]=
	{
		{0x601,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
	};
	
	canopen_send_sdo(message_sdo[0]);
	delay_ms(300);
	m_initialized  = FALSE;
	return 0;

}

// ����VA����˶�ģʽ
int PE_Shutter_Motor_SetMode(MotorMode mode) 
{
	  //λ������ģʽ
	if(MOTOR_MODE_POSITION == mode)
	{
		uint16_t message_sdo[][9]=
		{
			{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
		};
		
		canopen_send_sdo2(message_sdo[0]);
		delay_ms(300);
	}
	else if(MOTOR_MODE_VELOCITY == mode)
	{
		uint16_t message_sdo[][9]=
		{
			{0x602,0x2f,0x60,0x60,0x00,0x03,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ���ٶ�ģʽ 01:Profile Position Mode
		};

		canopen_send_sdo2(message_sdo[0]);
		delay_ms(300);
	}
    currentMode = mode;
    // ʵ������ģʽ�Ĵ���
}

int PE_Shutter_Motor_SetSpeed(int speed)// ���õ���ٶ�
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(speed, &v1, &v2, &v3, &v4);

	uint16_t message_sdo[][9]=
  	{
		//0x01,0x00,0x00,0x00:��ʾ1.00    Ĭ���ٶ�Ϊ1���������Ż�
		{0x602,0x23,0x81,0x60,0x00,v4,v3,v2,v1},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
	};
	
	canopen_send_sdo2(message_sdo[0]);
	delay_ms(300);
    currentSpeed = speed;
    // ʵ�������ٶȵĴ���
}


int PE_Shutter_Move_Position(int cur_position, int end_position) // ����ƶ�����
{
    // ʵ�ֵ���ƶ����ܵĴ���

	int dart_distance = (end_position - cur_position);
	
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(dart_distance, &v1, &v2, &v3, &v4);
	
	if(TRUE == m_shutter_initialized)
	{
		uint16_t message_sdo[][9]=
	  	{
			{0x602,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
			{0x602,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
			{0x602,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		};
		
		for(int i = 0 ; i < 3;++i)
		{
			canopen_send_sdo2(message_sdo[i]);
			delay_ms(300);
		}
	}
	else
	{
		ErrorStatus;
	}
    // ����ֵ���ܱ�ʾ�ɹ���ʧ�ܣ������������ʵ��������
    return 0;
}

int PE_Shutter_Move_Abs(int distance) // ��������˶�ĳ��λ��
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(distance, &v1, &v2, &v3, &v4);
	
	if(TRUE == m_shutter_initialized)
	{
		//abs:�����˶�
		uint16_t message_sdo[][9]=
	  	{
			{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
			{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
			{0x602,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // ������λ����
			{0x602,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
			{0x602,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
			{0x602,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		};
		
		for(int i = 0 ; i < 6;++i)
		{
			canopen_send_sdo2(message_sdo[i]);
			delay_ms(3);
		}
	}
	else
	{
		ErrorStatus;
	}  
    // ����ֵ���ܱ�ʾ�ɹ���ʧ�ܣ������������ʵ��������
    return 0;
}

int PE_Shutter_Move_Rel(int distance) // �������˶���ĳ������
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(distance, &v1, &v2, &v3, &v4);
	if(TRUE == m_shutter_initialized)
	{
		uint16_t message_sdo[][9]=
	  	{
			{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
			{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 ѡ��λ��ģʽ 01:Profile Position Mode
			{0x602,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // ������λ����
			{0x602,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 ѡ��Ŀ��λ�ã���Ŀ��λ������Ϊ5Ȧ 500*4=2000
			{0x602,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 ��Ŀ���ٶ�����Ϊ1rpm
			{0x602,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // ��ʼ�����˶� 1F�Ǿ����˶� 5F������˶�
		};
		
			delay_ms(300);
		for(int i = 0 ; i < 6;++i)
		{
			canopen_send_sdo2(message_sdo[i]);
			delay_ms(300);
		}
	}
	else
	{
		ErrorStatus;
	}
    // ����ֵ���ܱ�ʾ�ɹ���ʧ�ܣ������������ʵ��������
    
    return 0;
}

int PE_Shutter_Open()  //shutter��
{
	if(TRUE == m_shutter_initialized)
	{
		int distance = 0;
		PE_Shutter_Move_Abs(0);  //��Ҫ�궨Shutter_openʱ�ľ���λ���Ƕ���
	}
	else
	{
		ErrorStatus;
	}

}
int PE_Shutter_Close()  //shutter�ر�
{
	if(TRUE == m_shutter_initialized)
	{
		int distance = 0;
		PE_Shutter_Move_Abs(35);  //��Ҫ�궨Shutter_Closeʱ�ľ���λ���Ƕ���
	}
	else
	{
		ErrorStatus;
	}
}
// Shutter����ص�Homeλ��
int PE_Shutter_Home() 
{
		//ѡ����λ��
		  //λ������ģʽ
	uint16_t message_sdo3[][9]=
	{
		{0x602,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // ���ڵ�״̬�� Switch on disabled ����Ϊ Ready to switch on
		{0x602,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // ���ڵ�״̬�� Ready to switch on ����Ϊ Switched on
		{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // ���ڵ�״̬�� Switched on ����Ϊ Operation enable
		{0x602,0x23,0x99,0x60,0x02,0x03,0x00,0x00,0x00},  // Ѱ�����Ļ����ٶ�
		{0x602,0x2b,0xB8,0x60,0x00,0x0E,0x0E,0x00,0x00},  // ������λ����
		{0x602,0x23,0x99,0x60,0x01,0x01,0x00,0x00,0x00},  // Ѱ�ҿ���ʱ�Ļ����ٶ�
		{0x602,0x23,0x9A,0x60,0x00,0x0A,0x00,0x00,0x00},  //  ������ٶ�
		{0x602,0x23,0x7c,0x60,0x00,0x00,0x00,0x00,0x00},  //  ԭ��ƫ��          TODO:��Ҫ�궨20231221
		{0x602,0x2F,0x98,0x60,0x00,0x11,0x00,0x00,0x00},  // ���㷽ʽ
		{0x602,0x2F,0x60,0x60,0x00,0x06,0x00,0x00,0x00},  // ����ģʽ
		{0x602,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  //��ʼ����
		//�ȴ��������
		{0x602,0x40,0x64,0x60,0x00,0x00,0x00,0x00,0x00},  //�����ʵ��λ��
		
	};

	delay_ms(300);
	//i = 0;
	for(int i = 0;i < 12; ++i)
	{
		canopen_send_sdo2(message_sdo3[i]);     //��ʼ��VA_Motor���
	    delay_ms(300);
	}
    PE_Shutter_Move_Abs(35);

}


// Shutter�����ͣ����
int PE_Shutter_Stop() 
{
	uint16_t message_sdo[][9]=
	{
		{0x602,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // 
	};
	
	canopen_send_sdo2(message_sdo[0]);
	delay_ms(300);
	m_shutter_initialized = FALSE;
	return 0;

}


