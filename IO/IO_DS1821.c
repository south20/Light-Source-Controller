#include "IO_DS1821.h"
#include "delay.h"

//��λDS18B21
void DS18B21_Rst(void)	   
{                 
	DS18B21_IO_OUT();   //����Ϊ���
    DS18B21_DQ_OUT(0);  //����DQ
    delay_us(750);      //����750us
    DS18B21_DQ_OUT(1);  //DQ=1 
	delay_us(15);       //15US
}

//�ȴ�DS18B21�Ļ�Ӧ
//����1:δ��⵽DS18B21�Ĵ���
//����0:����
u8 DS18B21_Check(void) 	   
{   
	u8 retry=0;
	DS18B21_IO_IN();    //����Ϊ����
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

//��DS18B21��ȡһ��λ
//����ֵ��1/0
u8 DS18B21_Read_Bit(void) 
{
    u8 data;
	DS18B21_IO_OUT();   //����Ϊ���
    DS18B21_DQ_OUT(0); 
	delay_us(2);
    DS18B21_DQ_OUT(1); 
	DS18B21_IO_IN();    //����Ϊ����
	delay_us(12);
	if(DS18B21_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}

//��DS18B21��ȡһ���ֽ�
//����ֵ������������
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

//дһ���ֽڵ�DS18B21
//dat��Ҫд����ֽ�
void DS18B21_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
    DS18B21_IO_OUT();     //����Ϊ���
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // д1
        {
            DS18B21_DQ_OUT(0);
            delay_us(2);                            
            DS18B21_DQ_OUT(1);
            delay_us(60);             
        }
        else            //д0
        {
            DS18B21_DQ_OUT(0);
            delay_us(60);             
            DS18B21_DQ_OUT(1);
            delay_us(2);                          
        }
    }
}
 
//��ʼ�¶�ת��
void DS18B21_Start(void)
{   						               
    DS18B21_Rst();	   
    DS18B21_Check();	 
    //DS18B21_Write_Byte(0xcc);// skip rom
    //DS18B21_Write_Byte(0x44);// convert
    DS18B21_Write_Byte(0xEE);// skip rom
}

//��ʼ��DS18B21��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 IO_DS1821_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_2;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //��ʼ��
 
 	DS18B21_Rst();
	//printf("DS18B21_Check() = %d ! \r\n", DS18B21_Check()); 
	return DS18B21_Check();
}

//��DS18B21�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B21_Get_Temp(void)
{
    u8 temp;
    u8 Sta,TL,TH,V1_Value,V2_Value,V3_Value,V4_Value;
    short tem;
    DS18B21_Start ();           //��ʼת��
    DS18B21_Rst();
    DS18B21_Check();	 
    //DS18B21_Write_Byte(0xcc);   // skip rom
    //DS18B21_Write_Byte(0xbe);   // convert	   
		DS18B21_Write_Byte(0xAC);  //��ȡ��ǰ״̬	
	  Sta=DS18B21_Read_Byte();     // Sta
//		printf("Sta = %d.\r\n",Sta);
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xAA);  //��ȡ��ǰ״̬	
    TL=DS18B21_Read_Byte();     // LSB    
		//printf("TL = %d.\r\n", TL); 
	
	
	/*
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0x0C);  //write Status��ǰ״̬	
    DS18B21_Write_Byte(0x01);     // 
		
		*/
		
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xA0);  //��ȡA0��ǰ״̬	
    V3_Value=DS18B21_Read_Byte();     // LSB   
//		printf("Count_Value = %d.\r\n", V3_Value); 
		
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xAC);  //��ȡAC��ǰ״̬	
    V4_Value=DS18B21_Read_Byte();     // LSB   
//		printf("Status_Value = %d.\r\n", V4_Value); 
    /*
		if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(double)tem*0.625;//ת��     
	if(temp)return tem; //�����¶�ֵ
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
		DS18B21_Write_Byte(0xA1);  //��ȡA1��ǰ״̬	
    V1_Value=DS18B21_Read_Byte();     // LSB    
//		printf("TH_Value = %d.\r\n", V1_Value); 
	
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0xA2);  //��ȡA2��ǰ״̬	
    V2_Value=DS18B21_Read_Byte();     // LSB   
//		printf("LH_Value = %d.\r\n", V2_Value); 
	
		//�ж�����ʱ������Ƿ���Ч�����ʱ�䳬��2000������������������������ʾ��Ϣ��
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
    DS18B21_Start ();           //��ʼת��
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0x01);  //write A1��ǰ״̬	
    DS18B21_Write_Byte(V1_Value);     // LSB  TH
		
    DS18B21_Rst();
    DS18B21_Check();	 
		DS18B21_Write_Byte(0x02);  //write A2��ǰ״̬	
    DS18B21_Write_Byte(V2_Value);     // LSB   LH 
		
	return rtn;
}


