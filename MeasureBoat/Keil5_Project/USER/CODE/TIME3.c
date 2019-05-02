
/*
��ʼ��������ͨ�ö�ʱ��TIME3��
*/

#include<TIME3.h>

extern char overflow;


void Time3_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;             //ָ��IRQͨ���ⲿ�жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //ָ����ռ���ȼ�Ϊ1��0~3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;          //ָ����Ӧ���ȼ�Ϊ7��0~3�� 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;             //ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	//������Ӧ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM3_TimeBaseStructure.TIM_Period = 0xFFFF-1;                   //��������
	TIM3_TimeBaseStructure.TIM_Prescaler = 72-1;                    //������ʱ�ӽ���6��Ƶ�����Ϊ6Mhz
	TIM3_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);     //��ʱ�������������ж�
	TIM3_Close;
}

float T3_GET_Time()
{
	float data;
	data=TIM_GetCounter(TIM3)*0.34; //�õ����׼��������
	return data;
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //����жϱ�־λ
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		overflow=1;
	}
}
