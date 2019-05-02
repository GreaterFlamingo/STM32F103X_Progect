
#ifndef _DELAY_H_

#define _DELAY_H_


#include <main.h>

/***************************************************************
                        SysTick�ĸ��Ĵ����ĵ�ַ��ڶ���
****************************************************************/

#define SysTick_Tenms    (*((volatile unsigned long *)0xE000E01C))  //Ҫ�����volatile����
#define SysTick_Current   (*((volatile unsigned long *)0xE000E018)) 
#define SysTick_Reload    (*((volatile unsigned long *)0xE000E014)) 
#define SysTick_CSR        (*((volatile unsigned long *)0xE000E010)) 



//�ⲿ�ӿں���

void SysTick_Configuration(void);
void Delay_Nms(uint32_t nTime);
void Delay_Nus(uint32_t nTime);
void SysTick_Handler(void); 	 //�ж����


#endif
