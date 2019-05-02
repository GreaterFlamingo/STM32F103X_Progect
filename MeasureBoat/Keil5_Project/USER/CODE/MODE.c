
#include <MODE.h>

uchar Mode;     //�ֱ�Ϊ�ַ�'0'��'1'
//'0'Ϊ����ģʽ��'1'Ϊ�Զ�ģʽ��
uchar Meacure_Test;   
uchar ADC_Mode;       //�Ƿ���ADC���Լ�ADC����Ŀ
uchar MOTO_Mode;      //����Ƿ���
extern char GPS_Data[25];    //�洢GPS����
extern char ADC_CData[8][7];   //���ת��������ݵ��ַ���ʽ
extern uchar ADC_Number; //ADC���ݼ��ĸ���

void Mode_Init()
{
	Mode=0;
	LED1_ON;
	Meacure_Test=0;
	ADC_Mode=0;
	MOTO_Mode=0;
}


//����ģʽ
void MEASURE_Mode()
{
	uchar i;
	char s[60];
	char *t;
	s[0]='$';
	s[1]='\0';
	t=s;
	Get_Location_Data();       //�õ�GPS��Ϣ
	//�����������
	if (MOTO_Mode==1)
	{
		MOTO_Forward(); //���˳ʱ��ת��
	}
	if (ADC_Mode==1)
	{
		Delay_Nms(5000);
		ADC_Conversion();
	}
	//�����ݴ��м���GPS����
	if (GPS_Data[0]!='\0')
	{
		strcat(s,GPS_Data);
		GPS_Data[0]='\0';
	}
	//�����ݴ��м������������
	for (i=0;i<ADC_Number;i++)
	{
		if (ADC_CData[i][0]!='\0')
		{
			strcat(s,ADC_CData[i]);
			ADC_CData[i][0]='\0';
		}
	}
     //�жϲ���������
	if (Mode==1)
	{
		USART1_SendString("Duan xin :\r\n");
		Send_Message(t);	//ͨ�����ŷ���
		USART1_SendString("Send over...\r\n");
	}
	else if(Mode==0)
	{
		USART1_SendString("Message is :\r\n");
		USART1_SendString(t);     //ֱ�ӽ��������������
	}
	if (MOTO_Mode==1)
	{
		MOTO_Reverse(); //�����ʱ��ת������λ��
	}
	LED3_OFF;
}


