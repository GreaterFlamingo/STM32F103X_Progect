
#ifndef _MODE_H_

#define _MODE_H_

#include<main.h>

#define  LED1_ON (GPIOB->ODR|=(0x01))      //置高点亮灯
#define LED1_OFF (GPIOB->ODR&=~(0x01))	   //置低熄灭灯
#define  LED2_ON (GPIOB->ODR|=(0x02))
#define LED2_OFF (GPIOB->ODR&=~(0x02))
#define  LED3_ON (GPIOB->ODR|=(0x04))
#define LED3_OFF (GPIOB->ODR&=~(0x04))

//供外部调用的函数
void Mode_Init(void);
void MEASURE_Mode(void);

#endif /* MODE_H_ */



