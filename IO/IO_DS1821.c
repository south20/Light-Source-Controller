#include "IO_DS1821.h"
#include "delay.h"

//复位DS18B21
void DS18B21_Rst(void)	   
{                 
	DS18B21_IO_OUT();   //设置为输出
    DS18B21_DQ_OUT(0);  //拉低DQ
    delay_us(750);      //拉低750us
    DS18B21_DQ_OUT(1);  //DQ=1 
	delay_us(15);       //15US
}

//等待DS18B21的回应
//返回1:未检测到DS18B21的存在
//返回0:存在
u8 DS18B21_Check(void) 	   
{   
	u8 retry=0;
	DS18B21_IO_IN();    //设置为输入
   while (!DS18B21_DQ_IN && retry<200)
	{
		retry++;
		printf("retry### = %d ! \r\n", retry); 
		printf("DS18B21_DQ_IN = %d ! \r\n", DS18B21_DQ_IN);     
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (DS18B21_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	printf("retry = %d...", retry);   
	if(retry>=240)return 1;	    
	return 0;
}

//从DS18B21读取一个位
//返回值：1/0
u8 DS18B21_Read_Bit(void) 
{
    u8 data;
	DS18B21_IO_OUT();   //设置为输出
    DS18B21_DQ_OUT(0); 
	delay_us(2);
    DS18B21_DQ_OUT(1); 
	DS18B21_IO_IN();    //设置为输入
	delay_us(12);
	if(DS18B21_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}

//从DS18B21读取一个字节
//返回值：读到的数据
u8 DS18B21_Read_Byte(void)   
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B21_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

//写一个字节到DS18B21
//dat：要写入的字节
void DS18B21_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
    DS18B21_IO_OUT();     //设置为输出
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // 写1
        {
            DS18B21_DQ_OUT(0);
            delay_us(2);                            
            DS18B21_DQ_OUT(1);
            delay_us(60);             
        }
        else            //写0
        {
            DS18B21_DQ_OUT(0);
            delay_us(60);             
            DS18B21_DQ_OUT(1);
            delay_us(2);                          
        }
    }
}
 
//开始温度转换
void DS18B21_Start(void)
{   						               
    DS18B21_Rst();	   
    DS18B21_Check();	 
    //DS18B21_Write_Byte(0xcc);// skip rom
    //DS18B21_Write_Byte(0x44);// convert
    DS18B21_Write_Byte(0xEE);// skip rom
}

//初始化DS18B21的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 IO_DS1821_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_2;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //初始化
 
 	DS18B21_Rst();
	//printf("DS18B21_Check() = %d ! \r\n", DS18B21_Check()); 
	return DS18B21_Check();
}

//从DS18B21得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B21_Get_Temp(void)
{
    u8 temp;
    u8 Sta,TL,TH,V1_Value,V2_Value,V3_Value,V4_Value;
    short tem;
    DS18B21_Start ();           //开始转换
    DS18B21_Rst();
    DS18B21_Check();	 
    //DS18B21_Write_Byte(0xcc);   // skip rom
    //DS18B21_Write_Byte(0xbe);   // convert	   
		DS18B21_Write_Byte(0xAC);  //读取当前状态	
	  Sta=DS18B21_Read_Byte();     // Sta
//		printf("Sta = %d.\r\n",Sta);
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xAA);  //读取当前状态	
    TL=DS18B21_Read_Byte();     // LSB    
		//printf("TL = %d.\r\n", TL); 
	
	
	/*
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0x0C);  //write Status当前状态	
    DS18B21_Write_Byte(0x01);     // 
		
		*/
		
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xA0);  //读取A0当前状态	
    V3_Value=DS18B21_Read_Byte();     // LSB   
//		printf("Count_Value = %d.\r\n", V3_Value); 
		
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xAC);  //读取AC当前状态	
    V4_Value=DS18B21_Read_Byte();     // LSB   
//		printf("Status_Value = %d.\r\n", V4_Value); 
    /*
		if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(double)tem*0.625;//转换     
	if(temp)return tem; //返回温度值
	else return -tem;  */
	return TL;

}

int DS18B21_Get_Time()
{

    u8 Sta,TL,TH,V1_Value,V2_Value,V3_Value,V4_Value;
	
	int rtn = 0;
	int time = 0;
	
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xA1);  //读取A1当前状态	
    V1_Value=DS18B21_Read_Byte();     // LSB    
//		printf("TH_Value = %d.\r\n", V1_Value); 
	
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xA2);  //读取A2当前状态	
    V2_Value=DS18B21_Read_Byte();     // LSB   
//		printf("LH_Value = %d.\r\n", V2_Value); 
	
		//判断设置时间参数是否有效，如果时间超过2000，报警，红灯亮，输出错误提示信息。
	time = ((V1_Value - 192)*256 +V2_Value);
	if(time > 1800 && time < 2000)
	{
//		rtn = PE_HaloDisplay_Yellow(0);
//		printf("The light source has been in use for more than 2000 hours. Please replace it as soon as possible.\r\n");
//		
//		printf("The AAA source has been in use for %d hours\r\n", (int)time);
//		return rtn;
	}
	else if(time >= 2000)
	{

//		rtn = PE_HaloDisplay_Red(0);;
//		printf("The light source has been in use for more than 2000 hours. Please replace it as soon as possible.\r\n");
		return time;
	}
	return time;

}

int DS18B21_Set_Time(int time)
{

    u8 Sta,TL,TH,V1_Value,V2_Value,V3_Value,V4_Value;
	V1_Value = time / 256 + 192;
	V2_Value = time % 256;
	int rtn = 0;
    DS18B21_Start ();           //开始转换
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0x01);  //write A1当前状态	
    DS18B21_Write_Byte(V1_Value);     // LSB  TH
		
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0x02);  //write A2当前状态	
    DS18B21_Write_Byte(V2_Value);     // LSB   LH 
		
	return rtn;
}


