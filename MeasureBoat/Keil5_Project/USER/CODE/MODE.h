
#ifndef _MODE_H_

#define _MODE_H_

#include<main.h>

#define  LED1_ON (GPIOB->ODR|=(0x01))      //�øߵ�����
#define LED1_OFF (GPIOB->ODR&=~(0x01))	   //�õ�Ϩ���
#define  LED2_ON (GPIOB->ODR|=(0x02))
#define LED2_OFF (GPIOB->ODR&=~(0x02))
#define  LED3_ON (GPIOB->ODR|=(0x04))
#define LED3_OFF (GPIOB->ODR&=~(0x04))

//���ⲿ���õĺ���
void Mode_Init(void);
void MEASURE_Mode(void);

#endif /* MODE_H_ */



