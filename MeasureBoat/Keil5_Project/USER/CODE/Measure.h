
#ifndef _MEASURE_H_

#define _MEASURE_H_

#include <main.h>

#define ME_IN(j) (GPIOB->IDR&=(0x0001<<(j)))
//外部函数调用声明区

void Measure_Init(void);   //测量距离对应的端口初始化
void Measure(void);       //完成距离测量并进行分级
void Measure_Data(int);



#endif

