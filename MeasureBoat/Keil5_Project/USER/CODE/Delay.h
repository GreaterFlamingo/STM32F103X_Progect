
#ifndef _DELAY_H_

#define _DELAY_H_


#include <main.h>

/***************************************************************
                        SysTick四个寄存器的地址入口定义
****************************************************************/

#define SysTick_Tenms    (*((volatile unsigned long *)0xE000E01C))  //要定义成volatile类型
#define SysTick_Current   (*((volatile unsigned long *)0xE000E018)) 
#define SysTick_Reload    (*((volatile unsigned long *)0xE000E014)) 
#define SysTick_CSR        (*((volatile unsigned long *)0xE000E010)) 



//外部接口函数

void SysTick_Configuration(void);
void Delay_Nms(uint32_t nTime);
void Delay_Nus(uint32_t nTime);
void SysTick_Handler(void); 	 //中断入口


#endif
