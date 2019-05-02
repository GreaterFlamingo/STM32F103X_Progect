#ifndef _PWM_H_

#define _PWM_H_

#include <main.h>


void Pwm_Init(void);
void Set_PWM(uint16_t i);

//供外部调用的函数
void PWM_Stop(void);
void PWM_Start(void);
void PWM_Left_B(void);   //左侧B级别转向
void PWM_Left_C(void);   //左侧C级别转向
void PWM_Right_B(void);  //左侧B级别转向
void PWM_Right_C(void);  ////左侧C级别转向

//蓝牙模式下的操作
void PWM_BL(void);       //直行
void PWM_Left_BL(void);  //左转
void PWM_Right_BL(void); //右转


#endif

