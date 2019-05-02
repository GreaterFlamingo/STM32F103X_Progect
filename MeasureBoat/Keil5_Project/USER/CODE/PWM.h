#ifndef _PWM_H_

#define _PWM_H_

#include <main.h>


void Pwm_Init(void);
void Set_PWM(uint16_t i);

//���ⲿ���õĺ���
void PWM_Stop(void);
void PWM_Start(void);
void PWM_Left_B(void);   //���B����ת��
void PWM_Left_C(void);   //���C����ת��
void PWM_Right_B(void);  //���B����ת��
void PWM_Right_C(void);  ////���C����ת��

//����ģʽ�µĲ���
void PWM_BL(void);       //ֱ��
void PWM_Left_BL(void);  //��ת
void PWM_Right_BL(void); //��ת


#endif

