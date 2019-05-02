
#include <Test.h>

void Test_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //ʹ��GPIOA��ʱ��
	//����GPIOA��8�����ų�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//����GPIOA��7�����ų�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIOC->BSRR|=0x00FF;
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	Delay_Nms(100);	
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	Delay_Nms(100);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	
}

void Run()
{
	int i;
	for(i=0;i<8;i++)
	{
		GPIOC->ODR&=~(0x0001<<i); //��һλ��Ϊ��λ��
		Delay_Nms(1000);
		GPIOC->ODR|=(0x00ff);   //��8λ��Ϊ��λ
	}
	Delay_Nms(1000);
	for(i=0;i<8;i++)
	{
		GPIOC->ODR&=~(0x0080>>i); //��һλ��Ϊ��λ��
		Delay_Nms(1000);
		GPIOC->ODR|=(0x00ff);   //��8λ��Ϊ��λ
	}

}

