
#ifndef _TIME3_H_

#define _TIME3_H_

#include <main.h>

#define TIM3_Start (TIM3->CR1|=0x0001)
#define TIM3_Close (TIM3->CR1&=(~0x0001))

void Time3_Config(void);
float T3_GET_Time(void);

#endif

