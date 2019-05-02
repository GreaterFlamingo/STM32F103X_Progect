
/*
static __INLINE uint32_t SysTick_Config(uint32_t ticks);
//用来装载定时的时间值
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);
//用来设置滴答时钟的时钟频率

目的：用滴答时钟形成一个1us的定时器；
1us = T*(1/f)；
T:要装载的数；
f:滴答时钟运行的时钟频率；

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
SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//打开滴答时钟
	
SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭滴答时钟
*/

#include<Delay.h>
unsigned long TimingDelay;  // 延时时间，注意定义为全局变量 

/***************************************************************
                        SysTick的配置
****************************************************************/
void SysTick_Configuration(void) 
{ 
   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//置AHB 时钟为SysTick 时钟,72MHZ
   SysTick_Current=0; //当前值寄存器 
   SysTick_CSR=0x06;// HCLK作为Systick时钟，Systick中断使能位
}
/***************************************************************
                        延时函数Delay_Nms/Delay_Nus
****************************************************************/


void Delay_Nms(uint32_t nTime)     //ms级的延时函数 
{  
    SysTick_Current=0; //当前值为0
    SysTick_Reload=72000; //重装载寄存器，系统时钟72M,中断一次1mS（1ms=0.001s=1/72M*72000）
    TimingDelay =nTime; // 读取延时时间 
    SysTick_CSR=0x07; // 使能SysTick计数器
    while(TimingDelay!= 0); // 判断延时是否结束 
    SysTick_CSR=0x06;// 关闭SysTick计数器 
} 

void Delay_Nus(uint32_t nTime)      //us级的延时函数
{ 
    SysTick_Current=0; 
    SysTick_Reload=72; //重装载寄存器，系统时钟72M中断一次1uS 
    TimingDelay=nTime;
    SysTick_CSR=0x07;   // 使能SysTick计数器 
    while(TimingDelay!= 0); // 判断延时是否结束 
    SysTick_CSR=0x06;// 关闭SysTick计数器 
}


/***************************************************************
                        SysTick的中断处理函数
****************************************************************/

void SysTick_Handler(void) //中断函数 
{ 
    TimingDelay--; 
} 
 
 







