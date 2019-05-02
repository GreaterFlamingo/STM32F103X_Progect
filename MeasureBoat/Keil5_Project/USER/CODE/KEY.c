/*
	定义两个按键用于调整PWM的占空比，运用中断的方式修改参数
    GPIOA的6和7号引脚用于外部中断；4，5用于LED测试（低电平点亮）
*/

#include <KEY.h>

extern unsigned int pwm;

void Key_Init(void)
{
	//NVIC、GPIO和EXTI初始化结构体的声明
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//设定初始化结构体的参数
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;          //指定IRQ通道外部中断9-5；
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;   //指定抢占优先级为1（0~3）
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;          //指定响应优先级为7（0~3） 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;             //使能
	NVIC_Init(&NVIC_InitStructure);       //开启外部中断5-9；               //利用定义的结构体初始化功能
	
	//初始化GPIO
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //使能GPIOB的时钟	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //打开复用功能的时钟
	
	
	//配置GPIOB14引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;           //上拉输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//配置GPIOB15引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	//配置端口中断
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;	
	EXTI_Init(&EXTI_InitStructure);
	//将引脚GPIOB.14绑定到外部中断源5之上
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	
	//配置端口中断
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);	
	//将引脚GPIOB.15绑定到外部中断源6之上
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
	   
}

/***************************
函数功能：初始化各LED灯引脚
参数：无
说明：GPIOA八号脚是主程序中闪烁的灯；
	GPIOA七号脚是一级中断控制闪烁的灯；
    GPIOA六号脚是二级中断控制闪烁的灯；

****************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //使能GPIOA的时钟
	//配置GPIOA的8号引脚初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//配置GPIOA的7号引脚初始化
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
	//由于5-9号中断公用一个中断函数，故需要进行二次的判断
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



