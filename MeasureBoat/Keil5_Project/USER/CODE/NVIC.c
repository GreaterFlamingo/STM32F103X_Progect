
#include <NVIC.h>

void GPIOABC_Init(void)
{
	/* �򿪸��˿ڵ�ʱ�Ӻ͸��õ�ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE);
}

//
void set_PriorityGroup(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}

