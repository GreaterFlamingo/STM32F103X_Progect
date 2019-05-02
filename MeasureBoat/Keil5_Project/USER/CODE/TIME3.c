
/*
初始化并配置通用定时器TIME3，
*/

#include<TIME3.h>

extern char overflow;


void Time3_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;             //指定IRQ通道外部中断；
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //指定抢占优先级为1（0~3）
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;          //指定响应优先级为7（0~3） 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;             //使能
	NVIC_Init(&NVIC_InitStructure);
	
	//开启相应的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM3_TimeBaseStructure.TIM_Period = 0xFFFF-1;                   //计数上限
	TIM3_TimeBaseStructure.TIM_Prescaler = 72-1;                    //对输入时钟进行6分频，差不多为6Mhz
	TIM3_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);     //定时器计数满更新中断
	TIM3_Close;
}

float T3_GET_Time()
{
	float data;
	data=TIM_GetCounter(TIM3)*0.34; //得到毫米级别的数据
	return data;
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除中断标志位
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		overflow=1;
	}
}
