/*
	���������������ڵ���PWM��ռ�ձȣ������жϵķ�ʽ�޸Ĳ���
    GPIOA��6��7�����������ⲿ�жϣ�4��5����LED���ԣ��͵�ƽ������
*/

#include <KEY.h>

extern unsigned int pwm;

void Key_Init(void)
{
	//NVIC��GPIO��EXTI��ʼ���ṹ�������
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//�趨��ʼ���ṹ��Ĳ���
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;          //ָ��IRQͨ���ⲿ�ж�9-5��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;   //ָ����ռ���ȼ�Ϊ1��0~3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;          //ָ����Ӧ���ȼ�Ϊ7��0~3�� 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;             //ʹ��
	NVIC_Init(&NVIC_InitStructure);       //�����ⲿ�ж�5-9��               //���ö���Ľṹ���ʼ������
	
	//��ʼ��GPIO
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //ʹ��GPIOB��ʱ��	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //�򿪸��ù��ܵ�ʱ��
	
	
	//����GPIOB14����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;           //��������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//����GPIOB15����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	//���ö˿��ж�
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;	
	EXTI_Init(&EXTI_InitStructure);
	//������GPIOB.14�󶨵��ⲿ�ж�Դ5֮��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	
	//���ö˿��ж�
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);	
	//������GPIOB.15�󶨵��ⲿ�ж�Դ6֮��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
	   
}

/***************************
�������ܣ���ʼ����LED������
��������
˵����GPIOA�˺Ž�������������˸�ĵƣ�
	GPIOA�ߺŽ���һ���жϿ�����˸�ĵƣ�
    GPIOA���Ž��Ƕ����жϿ�����˸�ĵƣ�

****************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //ʹ��GPIOA��ʱ��
	//����GPIOA��8�����ų�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//����GPIOA��7�����ų�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	Delay_Nms(100);	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	Delay_Nms(100);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}



void EXTI9_5_IRQHandler(void)
{
	//����5-9���жϹ���һ���жϺ���������Ҫ���ж��ε��ж�
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		Delay_Nms(500);	
		if(pwm>=100)
		{
			pwm-=50;
			Set_PWM(pwm);
		}
		EXTI_ClearFlag(EXTI_Line6);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	else if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		Delay_Nms(500);	
		
		if(pwm<=900)
		{
			pwm+=50;
			Set_PWM(pwm);
		}
		EXTI_ClearFlag(EXTI_Line7);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}
}



