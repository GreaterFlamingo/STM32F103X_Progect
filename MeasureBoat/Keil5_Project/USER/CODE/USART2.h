#ifndef _USART2_H_

#define _USART2_H_

#include<main.h>

void USART2_Init(void);
void USART2_SendString(char str[]);
void Send_Message(char Message[]);
void Get_Location_Data(void);
void GPS_GSM_Init(void);
void GET_Message(void);


#endif
