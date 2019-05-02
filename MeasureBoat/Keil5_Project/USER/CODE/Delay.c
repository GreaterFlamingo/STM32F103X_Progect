
/*
static __INLINE uint32_t SysTick_Config(uint32_t ticks);
//����װ�ض�ʱ��ʱ��ֵ
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);
//�������õδ�ʱ�ӵ�ʱ��Ƶ��

Ŀ�ģ��õδ�ʱ���γ�һ��1us�Ķ�ʱ����
1us = T*(1/f)��
T:Ҫװ�ص�����
f:�δ�ʱ�����е�ʱ��Ƶ�ʣ�

1s = 1000ms;
1ms = 1000us;
1s = 1000000us;
1M = 1000000;
1/1M = 1us;
t= 1/72M = 1/72us
1us = 72 * t;
SystemCoreClock/1000000 = 72     ---- 1us
SystemCoreClock/100000  = 720    ---- 10us
SystemCoreClock/10000   = 7200   ---- 100us
SystemCoreClock/1000    = 72000  ---- 1ms
SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//�򿪵δ�ʱ��
	
SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//�رյδ�ʱ��
*/

#include<Delay.h>
unsigned long TimingDelay;  // ��ʱʱ�䣬ע�ⶨ��Ϊȫ�ֱ��� 

/***************************************************************
                        SysTick������
****************************************************************/
void SysTick_Configuration(void) 
{ 
   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//��AHB ʱ��ΪSysTick ʱ��,72MHZ
   SysTick_Current=0; //��ǰֵ�Ĵ��� 
   SysTick_CSR=0x06;// HCLK��ΪSystickʱ�ӣ�Systick�ж�ʹ��λ
}
/***************************************************************
                        ��ʱ����Delay_Nms/Delay_Nus
****************************************************************/


void Delay_Nms(uint32_t nTime)     //ms������ʱ���� 
{  
    SysTick_Current=0; //��ǰֵΪ0
    SysTick_Reload=72000; //��װ�ؼĴ�����ϵͳʱ��72M,�ж�һ��1mS��1ms=0.001s=1/72M*72000��
    TimingDelay =nTime; // ��ȡ��ʱʱ�� 
    SysTick_CSR=0x07; // ʹ��SysTick������
    while(TimingDelay!= 0); // �ж���ʱ�Ƿ���� 
    SysTick_CSR=0x06;// �ر�SysTick������ 
} 

void Delay_Nus(uint32_t nTime)      //us������ʱ����
{ 
    SysTick_Current=0; 
    SysTick_Reload=72; //��װ�ؼĴ�����ϵͳʱ��72M�ж�һ��1uS 
    TimingDelay=nTime;
    SysTick_CSR=0x07;   // ʹ��SysTick������ 
    while(TimingDelay!= 0); // �ж���ʱ�Ƿ���� 
    SysTick_CSR=0x06;// �ر�SysTick������ 
}


/***************************************************************
                        SysTick���жϴ�����
****************************************************************/

void SysTick_Handler(void) //�жϺ��� 
{ 
    TimingDelay--; 
} 
 
 







