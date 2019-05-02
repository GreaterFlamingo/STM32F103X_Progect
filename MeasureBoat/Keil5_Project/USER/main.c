
#include<main.h>

extern uchar Mode;     //�ֱ�Ϊ�ַ�'0'��'1'
//'0'Ϊ����ģʽ��'1'Ϊ�Զ�ģʽ��
extern uchar Meacure_Test;
extern uchar Message_Flag;

int main(void)
{
	set_PriorityGroup();
	SysTick_Configuration();
	GPIOABC_Init();
	USART1_Init();
	USART2_Init();
	Measure_Init();
	ADC1_Config();
	Key_Init();
	LED_Init();
	Pwm_Init();
	MOTO_Init();
	Mode_Init();
		
	GPS_GSM_Init();
	
	while(1)
	{
		if(Mode) Measure();
		if (Meacure_Test==1)
		{
			MEASURE_Mode();
			Meacure_Test=0;
		}
		if(Message_Flag==1)
		{
			GET_Message();
			Message_Flag=0;
		}
	}
	
}


