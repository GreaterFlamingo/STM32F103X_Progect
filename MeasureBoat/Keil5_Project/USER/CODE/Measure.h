
#ifndef _MEASURE_H_

#define _MEASURE_H_

#include <main.h>

#define ME_IN(j) (GPIOB->IDR&=(0x0001<<(j)))
//�ⲿ��������������

void Measure_Init(void);   //���������Ӧ�Ķ˿ڳ�ʼ��
void Measure(void);       //��ɾ�����������зּ�
void Measure_Data(int);



#endif

