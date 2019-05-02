
#include <MODE.h>

uchar Mode;     //分别为字符'0'、'1'
//'0'为蓝牙模式，'1'为自动模式，
uchar Meacure_Test;   
uchar ADC_Mode;       //是否开启ADC，以及ADC的数目
uchar MOTO_Mode;      //电机是否开启
extern char GPS_Data[25];    //存储GPS数据
extern char ADC_CData[8][7];   //存放转化后的数据的字符形式
extern uchar ADC_Number; //ADC数据检测的个数

void Mode_Init()
{
	Mode=0;
	LED1_ON;
	Meacure_Test=0;
	ADC_Mode=0;
	MOTO_Mode=0;
}


//测量模式
void MEASURE_Mode()
{
	uchar i;
	char s[60];
	char *t;
	s[0]='$';
	s[1]='\0';
	t=s;
	Get_Location_Data();       //得到GPS信息
	//插入测量部分
	if (MOTO_Mode==1)
	{
		MOTO_Forward(); //电机顺时针转动
	}
	if (ADC_Mode==1)
	{
		Delay_Nms(5000);
		ADC_Conversion();
	}
	//在数据串中加入GPS数据
	if (GPS_Data[0]!='\0')
	{
		strcat(s,GPS_Data);
		GPS_Data[0]='\0';
	}
	//在数据串中加入测量的数据
	for (i=0;i<ADC_Number;i++)
	{
		if (ADC_CData[i][0]!='\0')
		{
			strcat(s,ADC_CData[i]);
			ADC_CData[i][0]='\0';
		}
	}
     //判断并发送数据
	if (Mode==1)
	{
		USART1_SendString("Duan xin :\r\n");
		Send_Message(t);	//通过短信发送
		USART1_SendString("Send over...\r\n");
	}
	else if(Mode==0)
	{
		USART1_SendString("Message is :\r\n");
		USART1_SendString(t);     //直接将结果发送至蓝牙
	}
	if (MOTO_Mode==1)
	{
		MOTO_Reverse(); //电机逆时针转动（复位）
	}
	LED3_OFF;
}


