
#include <NVIC.h>

void GPIOABC_Init(void)
{
	/* 打开各端口的时钟和复用的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE);
}

//
void set_PriorityGroup(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置2位抢占优先级，2位响应优先级
}

