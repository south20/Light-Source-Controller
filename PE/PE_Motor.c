// PE_VA_Motor.c

#include "PE_Motor.h"
#include "IO_GPIO.h"  // 请替换成你的GPIO模块的头文件
#include "PE_BASE.h"  // 请替换成你的GPIO模块的头文件
#include <stdio.h>

// 模拟电机控制器的实现
static MotorDirection currentDirection = MOTOR_DIRECTION_FORWARD;
static MotorMode currentMode = MOTOR_MODE_POSITION;
static int currentSpeed = 0;
static int currentPosition = 0;

int m_initialized;
int m_shutter_initialized;

//常见字典状态说明
//位置轮廓模式：
// 607Ah:目标位置    bit6(0:绝对定位       1：相对定位)   
// 6081h:轮廓速度         它接收 16.16 型数据。 MSB 为整数部分， LSB 为小数部分。
// 6083h:轮廓加速度
// 6085h:快速停止减速度
// 2023h:加加速时间
// 6086h: 运动轮廓类型
// 6062h:位置需求值
// 6063h:位置实际内部值
// 6064h:位置实际值
// 6065h:跟随错误窗
// 6066h:跟随错误超时
// 6067h:位置窗
// 6060h:电机运动模式    ：    	1位置轮廓模式 3.速度轮廓模式
// 6068h:位置窗门时间
// 607Bh:位置范围限制
// 60F2h:定位选项代码
// 60F4h:跟随误差实际值
// 60FCh:位置需求内部值
// 2022h:控制结果
// 2081h:设置/更改实际电机位置0 对象 2088h:电机传感器的实际内部位置
// 208Dh:辅助编码器位置


//将十进制数字转化为32为的16进制数字
void VerIntToHex(int a, int *v1, int *v2, int *v3, int *v4)
{
    // 将整数转换为十六进制字符串
    char hex_string[9]; // 8 characters plus null terminator
    
    sprintf(hex_string, "%08x", a);
    
    // 将字符串拆分为四部分
    sscanf(hex_string, "%2x%2x%2x%2x", v1, v2, v3, v4);
}


// 初始化电机
//初始化默认选择位置轮廓模式
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
	
	 
	  //位置轮廓模式
	uint16_t message_sdo[][9]=
	{
		{0x601,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // 将节点状态从 Switch on disabled 更改为 Ready to switch on
		{0x601,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // 将节点状态从 Ready to switch on 更改为 Switched on
		{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
		{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
		{0x601,0x23,0x7a,0x60,0x00,0xd0,0x07,0x00,0x00},  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
		{0x601,0x23,0x81,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
		{0x601,0x23,0x83,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 83 60 00 01 00 00 80 将目标加速度设置为1rps/s
//		{0x601,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
		
	};
		
		  //位置轮廓模式
	uint16_t message_sdo2[][9]=
	{
		{0x602,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // 将节点状态从 Switch on disabled 更改为 Ready to switch on
		{0x602,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // 将节点状态从 Ready to switch on 更改为 Switched on
		{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
		{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
		{0x602,0x23,0x7a,0x60,0x00,0xd0,0x07,0x00,0x00},  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
		{0x602,0x23,0x81,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
		{0x602,0x23,0x83,0x60,0x00,0x10,0x00,0x00,0x00},  // 601,23 83 60 00 01 00 00 80 将目标加速度设置为1rps/s
//		{0x602,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
		
	};

	delay_ms(3000);
	//i = 0;
	for(int i = 0;i < 2; ++i)
	{
		canopen_send_sdo(message_sdo[i]);     //初始化VA_Motor电机
	    delay_ms(280);
	}
	
	delay_ms(1200);
	canopen_send_sdo(message_sdo[2]);	  //初始化VA_Motor电机
	for(int i = 3; i < 7;++i)
	{
		delay_ms(280);
		canopen_send_sdo(message_sdo[i]);	  //初始化VA_Motor电机
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
		canopen_send_sdo2(message_sdo2[i]);     //初始化Shutter_Motor电机
	    delay_ms(280);
	}
	
	delay_ms(1200);
	canopen_send_sdo2(message_sdo2[2]);	  //初始化Shutter_Motor电机
	for(int i = 3; i < 7;++i)
	{
		delay_ms(280);
		canopen_send_sdo2(message_sdo2[i]);	  //初始化Shutter_Motor电机
	}

	
		delay_ms(280);
	
	m_initialized = TRUE;
	m_shutter_initialized = TRUE;

}


// 设置VA电机运动模式
int PE_VA_Motor_SetMode(MotorMode mode) 
{
	if(TRUE == m_initialized)
	{
		  //位置轮廓模式
		if(MOTOR_MODE_POSITION == mode)
		{
			uint16_t message_sdo[][9]=
			{
				{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
			};
			
			canopen_send_sdo(message_sdo[0]);
			delay_ms(300);
		}
		else if(MOTOR_MODE_VELOCITY == mode)
		{
			uint16_t message_sdo[][9]=
			{
				{0x601,0x2f,0x60,0x60,0x00,0x03,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择速度模式 01:Profile Position Mode
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
    // 实现设置模式的代码
}

// 设置电机速度
int PE_VA_Motor_SetSpeed(int speed) 
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(speed, &v1, &v2, &v3, &v4);

	uint16_t message_sdo[][9]=
  	{
		//0x01,0x00,0x00,0x00:表示1.00    默认速度为1，后期再优化
		{0x601,0x23,0x81,0x60,0x00,v4,v3,v2,v1},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
	};
	
	canopen_send_sdo(message_sdo[0]);
	delay_ms(300);
    currentSpeed = speed;
    // 实现设置速度的代码
}


// 电机移动功能
int PE_VA_Move_Position(int cur_position, int end_position) {
    // 实现电机移动功能的代码

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
			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
			{0x601,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
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
    // 返回值可能表示成功或失败，具体情况根据实际需求定义
    return 0;
}

// 检测零位传感器是否触发
int PE_VA_Get_Zero_Pos_Sensor(IO_GPIO_Pin_State* bitstatus) {
    // 实现检测零位传感器的代码
    // 返回值可能表示成功或失败，具体情况根据实际需求定义
    int rtn = 0;
    rtn = IO_GPIO_Read_Pin(IO_GPIO_ZERO_POS, bitstatus);
    return 0;
}

// 电机绝对运动某个位置
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
			{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
			{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
			{0x601,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // 禁用限位功能
			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
			{0x601,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
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

// 电机相对运动到某个距离
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
			{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
			{0x601,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
			{0x601,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // 禁用限位功能
			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
			{0x601,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
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
    // 返回值可能表示成功或失败，具体情况根据实际需求定义
    
//	int32_t Stop_Count_Val = IO_Encoder_Get_Count_Val();

	//精确移动到目标位置
//	int32_t dart = distance - (Stop_Count_Val - Start_Count_Val);
//	VerIntToHex(dart, &v1, &v2, &v3, &v4);
//	if(TRUE == m_shutter_initialized)
//	{
//		uint16_t message_sdo[][9]=
//	  	{
//			{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
//			{0x601,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
//			{0x601,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
//			{0x601,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
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

//返回的是当前角度值。
double PE_VA_Get_Position(double     *       distance){
	*distance = IO_Encoder_Get_Only_Rel_Pos();
    // 返回值可能表示成功或失败，具体情况根据实际需求定义
    return 0;
}

//零位监测传感器触发时，此时光阑运动到180度的位置（大概如此）
//0度~300度 对应0档~100档
//所以0档为0位监测传感器旋转一定角度后的位置。该位置需要标定
// func:
int PE_VA_Home() 
{
	int rtn = 0;
		//选择负限位和
		  //位置轮廓模式
		  //零位设置为零位检测传感器触发后旋转180度位置
		  //此时 [0~300度]对应 [0档到100档]           需要标定
		  //偏置：offset
	uint16_t message_sdo3[][9]=
	{
		{0x601,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // 将节点状态从 Switch on disabled 更改为 Ready to switch on
		{0x601,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // 将节点状态从 Ready to switch on 更改为 Switched on
		{0x601,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
		{0x601,0x23,0x99,0x60,0x02,0x03,0x00,0x00,0x00},  // 寻找零点的回零速度
		{0x601,0x23,0x99,0x60,0x01,0x01,0x00,0x00,0x00},  // 寻找开关时的回零速度
		{0x601,0x2b,0xB8,0x60,0x00,0x0E,0x0E,0x00,0x00},  // 启用限位功能
		{0x601,0x23,0x9A,0x60,0x00,0x0A,0x00,0x00,0x00},  //  回零加速度
//		{0x601,0x23,0x7c,0x60,0x00,0x00,0x00,0x00,0x00},  //  原点偏置          TODO:需要标定20231221
		{0x601,0x23,0x7c,0x60,0x00,0xC8,0x00,0x00,0x00},  //  原点偏置          TODO:需要标定20231221
		{0x601,0x2F,0x98,0x60,0x00,0x11,0x00,0x00,0x00},  // 回零方式
		{0x601,0x2F,0x60,0x60,0x00,0x06,0x00,0x00,0x00},  // 运行模式
		{0x601,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  //开始回零
		//等待回零结束
		{0x601,0x40,0x64,0x60,0x00,0x00,0x00,0x00,0x00},  //监测电机实际位置
		
	};

	delay_ms(300);
	//i = 0;
	for(int i = 0;i < 13; ++i)
	{
		canopen_send_sdo(message_sdo3[i]);     //初始化VA_Motor电机
	    delay_ms(300);
	}
	
	   // delay_ms(9000);
//	IO_GPIO_Pin_State bitstatus = 0;
//	PE_VA_Get_Zero_Pos_Sensor(&bitstatus);
//
//	//如果监测到零位传感器，回零档位才有意义，没有监测到则报错
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


// VA电机急停功能
int PE_VA_Stop() 
{
	uint16_t message_sdo[][9]=
	{
		{0x601,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
	};
	
	canopen_send_sdo(message_sdo[0]);
	delay_ms(300);
	m_initialized  = FALSE;
	return 0;

}

// 设置VA电机运动模式
int PE_Shutter_Motor_SetMode(MotorMode mode) 
{
	  //位置轮廓模式
	if(MOTOR_MODE_POSITION == mode)
	{
		uint16_t message_sdo[][9]=
		{
			{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
		};
		
		canopen_send_sdo2(message_sdo[0]);
		delay_ms(300);
	}
	else if(MOTOR_MODE_VELOCITY == mode)
	{
		uint16_t message_sdo[][9]=
		{
			{0x602,0x2f,0x60,0x60,0x00,0x03,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择速度模式 01:Profile Position Mode
		};

		canopen_send_sdo2(message_sdo[0]);
		delay_ms(300);
	}
    currentMode = mode;
    // 实现设置模式的代码
}

int PE_Shutter_Motor_SetSpeed(int speed)// 设置电机速度
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(speed, &v1, &v2, &v3, &v4);

	uint16_t message_sdo[][9]=
  	{
		//0x01,0x00,0x00,0x00:表示1.00    默认速度为1，后期再优化
		{0x602,0x23,0x81,0x60,0x00,v4,v3,v2,v1},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
	};
	
	canopen_send_sdo2(message_sdo[0]);
	delay_ms(300);
    currentSpeed = speed;
    // 实现设置速度的代码
}


int PE_Shutter_Move_Position(int cur_position, int end_position) // 电机移动功能
{
    // 实现电机移动功能的代码

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
			{0x602,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
			{0x602,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
			{0x602,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
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
    // 返回值可能表示成功或失败，具体情况根据实际需求定义
    return 0;
}

int PE_Shutter_Move_Abs(int distance) // 电机绝对运动某个位置
{
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	VerIntToHex(distance, &v1, &v2, &v3, &v4);
	
	if(TRUE == m_shutter_initialized)
	{
		//abs:绝对运动
		uint16_t message_sdo[][9]=
	  	{
			{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
			{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
			{0x602,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // 禁用限位功能
			{0x602,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
			{0x602,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
			{0x602,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
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
    // 返回值可能表示成功或失败，具体情况根据实际需求定义
    return 0;
}

int PE_Shutter_Move_Rel(int distance) // 电机相对运动到某个距离
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
			{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
			{0x602,0x2f,0x60,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,2f 60 60 00 01 00 00 80 选择位置模式 01:Profile Position Mode
			{0x602,0x2b,0xB8,0x60,0x00,0x4E,0x0E,0x00,0x00},  // 禁用限位功能
			{0x602,0x23,0x7a,0x60,0x00,v4  ,v3  ,v2  ,v1  },  // 601,23 7a 60 00 d0 07 00 80 选择目标位置：将目标位置设置为5圈 500*4=2000
			{0x602,0x23,0x81,0x60,0x00,0x01,0x00,0x00,0x00},  // 601,23 81 60 00 01 00 00 80 将目标速度设置为1rpm
			{0x602,0x2b,0x40,0x60,0x00,0x5F,0x00,0x00,0x00},  // 开始轮廓运动 1F是绝对运动 5F是相对运动
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
    // 返回值可能表示成功或失败，具体情况根据实际需求定义
    
    return 0;
}

int PE_Shutter_Open()  //shutter打开
{
	if(TRUE == m_shutter_initialized)
	{
		int distance = 0;
		PE_Shutter_Move_Abs(0);  //需要标定Shutter_open时的绝对位置是多少
	}
	else
	{
		ErrorStatus;
	}

}
int PE_Shutter_Close()  //shutter关闭
{
	if(TRUE == m_shutter_initialized)
	{
		int distance = 0;
		PE_Shutter_Move_Abs(35);  //需要标定Shutter_Close时的绝对位置是多少
	}
	else
	{
		ErrorStatus;
	}
}
// Shutter电机回到Home位置
int PE_Shutter_Home() 
{
		//选择负限位和
		  //位置轮廓模式
	uint16_t message_sdo3[][9]=
	{
		{0x602,0x2b,0x40,0x60,0x00,0x06,0x00,0x00,0x00},  // 将节点状态从 Switch on disabled 更改为 Ready to switch on
		{0x602,0x2b,0x40,0x60,0x00,0x07,0x00,0x00,0x00},  // 将节点状态从 Ready to switch on 更改为 Switched on
		{0x602,0x2b,0x40,0x60,0x00,0x0f,0x00,0x00,0x00},  // 将节点状态从 Switched on 更改为 Operation enable
		{0x602,0x23,0x99,0x60,0x02,0x03,0x00,0x00,0x00},  // 寻找零点的回零速度
		{0x602,0x2b,0xB8,0x60,0x00,0x0E,0x0E,0x00,0x00},  // 禁用限位功能
		{0x602,0x23,0x99,0x60,0x01,0x01,0x00,0x00,0x00},  // 寻找开关时的回零速度
		{0x602,0x23,0x9A,0x60,0x00,0x0A,0x00,0x00,0x00},  //  回零加速度
		{0x602,0x23,0x7c,0x60,0x00,0x00,0x00,0x00,0x00},  //  原点偏置          TODO:需要标定20231221
		{0x602,0x2F,0x98,0x60,0x00,0x11,0x00,0x00,0x00},  // 回零方式
		{0x602,0x2F,0x60,0x60,0x00,0x06,0x00,0x00,0x00},  // 运行模式
		{0x602,0x2b,0x40,0x60,0x00,0x1F,0x00,0x00,0x00},  //开始回零
		//等待回零结束
		{0x602,0x40,0x64,0x60,0x00,0x00,0x00,0x00,0x00},  //监测电机实际位置
		
	};

	delay_ms(300);
	//i = 0;
	for(int i = 0;i < 12; ++i)
	{
		canopen_send_sdo2(message_sdo3[i]);     //初始化VA_Motor电机
	    delay_ms(300);
	}
    PE_Shutter_Move_Abs(35);

}


// Shutter电机急停功能
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


