/* 

产生两路PWM用于控制点击的转动,PWM的数值越大转动的速度也就越快。

*/

#include <PWM.h>

unsigned int pwm;

void PWM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;          //指定IRQ通道外部中断9-5；
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //指定抢占优先级为1（0~3）
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //指定响应优先级为7（0~3） 
	NVIC_InitStructure.NVIC_IRQChannelCmd=DISABLE;             //使能
	NVIC_Init(&NVIC_InitStructure); 
}


void Pwm_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM2_TimeBaseStructure;
	TIM_OCInitTypeDef TIM2_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启相应的时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	PWM_NVIC_Config();
	//GPIOA的0和1号端口
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;    //开漏输出           
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
	
	
	TIM2_TimeBaseStructure.TIM_Period = 1000-1;
	TIM2_TimeBaseStructure.TIM_Prescaler = 3-1;
	TIM2_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM2_TimeBaseStructure);
	
	TIM2_OCInitStructure.TIM_OCMode= TIM_OCMode_PWM1;
	TIM2_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	//PWM1
	TIM2_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM2_OCInitStructure.TIM_Pulse = 500;
	TIM_OC1Init(TIM2,&TIM2_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	//PWM2
	TIM2_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM2_OCInitStructure.TIM_Pulse = 500;
	TIM_OC2Init(TIM2,&TIM2_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4);
	
	TIM_ITConfig(TIM2,TIM_IT_CC1,DISABLE);
	TIM_Cmd(TIM2,DISABLE);
	pwm=500;
	
}

void Set_PWM(uint16_t i)
{
	//设置通道1
	TIM_SetCompare1(TIM2,i);
	//设置通道2
	TIM_SetCompare2(TIM2,i);
	
}

void PWM_Stop()   //停止点击的转动
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_Cmd(TIM2,DISABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;    //开漏输出           
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
}

void PWM_Start()  //启动点击
{	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;    //开漏输出           
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	Set_PWM(pwm);
}


void PWM_Left_B()   //左侧B级别转向
{
	//设置通道1
	TIM_SetCompare1(TIM2,750);
	//设置通道2
	TIM_SetCompare2(TIM2,250);
	Delay_Nms(500);
	Set_PWM(pwm);
}

void PWM_Left_C()   //左侧C级别转向
{
	//设置通道1
	TIM_SetCompare1(TIM2,750);
	//设置通道2
	TIM_SetCompare2(TIM2,250);
	Delay_Nms(1000);
	Set_PWM(pwm);
}

void PWM_Right_B()  //左侧B级别转向
{
	//设置通道1
	TIM_SetCompare1(TIM2,250);
	//设置通道2
	TIM_SetCompare2(TIM2,750);
	Delay_Nms(500);
	Set_PWM(pwm);
}

void PWM_Right_C()  ////左侧C级别转向
{
	//设置通道1
	TIM_SetCompare1(TIM2,250);
	//设置通道2
	TIM_SetCompare2(TIM2,750);
	Delay_Nms(1000);	
	Set_PWM(pwm);
}

//蓝牙模式直行
void PWM_BL()
{
	PWM_Start();
	//设置通道1
	TIM_SetCompare1(TIM2,600);
	//设置通道2
	TIM_SetCompare2(TIM2,600);
	Delay_Nms(1000);
	PWM_Stop();
}

//蓝牙模式左转
void PWM_Left_BL()
{
	PWM_Start();
	//设置通道1
	TIM_SetCompare1(TIM2,750);
	//设置通道2
	TIM_SetCompare2(TIM2,250);
	Delay_Nms(1000);
	PWM_Stop();
}

//蓝牙模式右转
void PWM_Right_BL()
{
	PWM_Start();
	//设置通道1
	TIM_SetCompare1(TIM2,250);
	//设置通道2
	TIM_SetCompare2(TIM2,750);
	Delay_Nms(1000);
	PWM_Stop();
}



