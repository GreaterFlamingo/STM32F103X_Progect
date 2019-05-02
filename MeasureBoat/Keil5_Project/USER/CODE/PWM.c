/* 

������·PWM���ڿ��Ƶ����ת��,PWM����ֵԽ��ת�����ٶ�Ҳ��Խ�졣

*/

#include <PWM.h>

unsigned int pwm;

void PWM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;          //ָ��IRQͨ���ⲿ�ж�9-5��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //ָ����ռ���ȼ�Ϊ1��0~3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //ָ����Ӧ���ȼ�Ϊ7��0~3�� 
	NVIC_InitStructure.NVIC_IRQChannelCmd=DISABLE;             //ʹ��
	NVIC_Init(&NVIC_InitStructure); 
}


void Pwm_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM2_TimeBaseStructure;
	TIM_OCInitTypeDef TIM2_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//������Ӧ��ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	PWM_NVIC_Config();
	//GPIOA��0��1�Ŷ˿�
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;    //��©���           
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
	//����ͨ��1
	TIM_SetCompare1(TIM2,i);
	//����ͨ��2
	TIM_SetCompare2(TIM2,i);
	
}

void PWM_Stop()   //ֹͣ�����ת��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_Cmd(TIM2,DISABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;    //��©���           
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
}

void PWM_Start()  //�������
{	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;    //��©���           
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	Set_PWM(pwm);
}


void PWM_Left_B()   //���B����ת��
{
	//����ͨ��1
	TIM_SetCompare1(TIM2,750);
	//����ͨ��2
	TIM_SetCompare2(TIM2,250);
	Delay_Nms(500);
	Set_PWM(pwm);
}

void PWM_Left_C()   //���C����ת��
{
	//����ͨ��1
	TIM_SetCompare1(TIM2,750);
	//����ͨ��2
	TIM_SetCompare2(TIM2,250);
	Delay_Nms(1000);
	Set_PWM(pwm);
}

void PWM_Right_B()  //���B����ת��
{
	//����ͨ��1
	TIM_SetCompare1(TIM2,250);
	//����ͨ��2
	TIM_SetCompare2(TIM2,750);
	Delay_Nms(500);
	Set_PWM(pwm);
}

void PWM_Right_C()  ////���C����ת��
{
	//����ͨ��1
	TIM_SetCompare1(TIM2,250);
	//����ͨ��2
	TIM_SetCompare2(TIM2,750);
	Delay_Nms(1000);	
	Set_PWM(pwm);
}

//����ģʽֱ��
void PWM_BL()
{
	PWM_Start();
	//����ͨ��1
	TIM_SetCompare1(TIM2,600);
	//����ͨ��2
	TIM_SetCompare2(TIM2,600);
	Delay_Nms(1000);
	PWM_Stop();
}

//����ģʽ��ת
void PWM_Left_BL()
{
	PWM_Start();
	//����ͨ��1
	TIM_SetCompare1(TIM2,750);
	//����ͨ��2
	TIM_SetCompare2(TIM2,250);
	Delay_Nms(1000);
	PWM_Stop();
}

//����ģʽ��ת
void PWM_Right_BL()
{
	PWM_Start();
	//����ͨ��1
	TIM_SetCompare1(TIM2,250);
	//����ͨ��2
	TIM_SetCompare2(TIM2,750);
	Delay_Nms(1000);
	PWM_Stop();
}



