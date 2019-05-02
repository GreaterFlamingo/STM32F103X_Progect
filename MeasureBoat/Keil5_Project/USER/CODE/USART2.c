
#include <USART2.h>

uint16_t RX_Length=0;
uchar Message_Flag=0;
extern uchar Mode;     //�ֱ�Ϊ�ַ�'0'��'1'
//'0'Ϊ����ģʽ��'1'Ϊ�Զ�ģʽ��
extern uchar Meacure_Test; 
extern char Phone_Number[13];

#define UART_RX_LEN 200

static char USART2_Rx[UART_RX_LEN];//�������ݻ�����
char GPS_Data[30];    //�洢GPS����
char MS_ID[4];

void USART2_RCC_Configuration(void);
void USART2_GPIO_Configuration(void);
void USART2_DMA_Configuration(void);
void UART2_Configuration(void);
void USART2_NVIC_Configuration(void);
void GPS_Pr(void);
void GSM_Pr(void);
void GET_Message(void);
void Message_Action(void);

void USART2_Init(void)

{
	USART2_RCC_Configuration();//�򿪿�����Ӧʱ��
	USART2_GPIO_Configuration();//GPIO����
	UART2_Configuration();//UART����
	USART2_NVIC_Configuration();//�ж����ȼ�����
	GPS_Data[0]='\0';   //��ʼ���ÿ�����
	MS_ID[0]='\0';      //��Ϣ�Ĵ��λ��

}

void USART2_RCC_Configuration(void)

{

	//�򿪴��ڶ�Ӧ������ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
}


void USART2_GPIO_Configuration(void)

{
	//���õ�ʮ�Ž��ն˿�
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//���õھźŷ��Ͷ˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void UART2_Configuration(void)

{
	//����ṹ�壬��ʼ��USART
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ʹ�ܽ��պͷ���
	
	USART_Init(USART2,&USART_InitStructure);	//��ʼ������

	//�ж�����

	USART_ITConfig(USART2,USART_IT_TC,DISABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	//����DMA��ʽ����
	USART_DMACmd(USART2,USART_DMAReq_Rx,DISABLE);    //ʹ�ܽ��յ�DMA����
	
	USART_Cmd(USART2,ENABLE);

}

void USART2_NVIC_Configuration(void)

{

	NVIC_InitTypeDef NVIC_InitStructure;
	//����UART1�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //�ж�ռ�ȵȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�ж���Ӧ���ȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //���ж�
	NVIC_Init(&NVIC_InitStructure);
}

/*
	USART2�ַ������ͺ���
*/
void USART2_SendString(char str[])
{
	uint16_t i;
	for(i=0;str[i]!='\0';i++)
	{
		if(USART_GetFlagStatus(USART2,USART_FLAG_TC)==SET)
			USART_SendData(USART2, str[i]);
		else
		{
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
			USART_SendData(USART2, str[i]);
		}
	}
}



/*
	USART�ַ���������
*/


void USART2_Pr()
{
//	USART2_SendString("This is USART2_Pr \r\n");
	if(strstr(USART2_Rx,"+QGNSSRD: $GNRMC"))
		GPS_Pr();
	else if(strstr(USART2_Rx,"XX!XX"))
		GSM_Pr();
	else if(strstr(USART2_Rx,"+CMTI: \"SM\""))
		Message_Action();
	USART1_SendString(USART2_Rx);
}



//USART2�жϴ������������߿���ʱ��
/*
IDLE�жϴ����Ժ�����ж�λ�Ժ������߼������в������ٴα�����
ֻ�еȴ��´ν��յ����ݽ��������Ժ�Żᱻ�ٴι���
*/
void USART2_IRQHandler(void)

{
	if(USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)
	{
		USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_PE);
	}
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	if(USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
	{
		USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_FE);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART2_Rx[RX_Length++]=USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  //��һ��ȷ���ǽ����ж�

	{
		uint32_t Length;
		//��USART_CR1�е�IDLEIEΪ��1����������жϡ��� ������������λ(�ȶ�USART_SR��Ȼ���USART_DR)��
		do
		{
			USART_ClearFlag(USART2, USART_FLAG_IDLE);
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);
			Length = USART2->SR;
			Length = USART2->DR;
		}while(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET);
		USART2_Rx[RX_Length]='\0';   //�ڽ��յ�������֮֡����ӽ�����־
		USART2_Pr();              //������յ�������
		RX_Length=0;
	}
	__nop();
}

//GPS�����ַ�������
void GPS_Pr(void)         //GPS�Ͳ���GSM�ֶ��ԡ�+���ſ�ͷ
{
	char * s;
	char * t;
//	USART1_SendString("This is GPS_Pr\t\n");
	//����֡����ʽ��+QGNSSRD: $GNRMC,073203.000,A,3604.0109,N,11420.5335,E,0.10,0.00,150319,,,A*74
	s=strstr(USART2_Rx,"+QGNSSRD: $GNRMC");
	if(s) //��һ��ȷ�ϵ�����Ϣ
	{
		s=strchr(s,'A');//�Ӹ�ָ������ȡγ��
		if(s)
		{
			t=GPS_Data;
			*t++='N';
			*t++=':';
			s+=2;
			while((*s)!=',')
				{
					*t=*s;
					t++;
					s++;
				}
			s=strrchr(s,'N');//�Ӹ�ָ������ȡ����(���һ�γ���'N'��λ��)
			//��ȡ���ȵĲ���
			*t++='E';
			*t++=':';
			s+=2;
			while((*s)!=',')
			{
				*t=*s;
				t++;
				s++;
			}
			*t='\0';
			USART1_SendString(GPS_Data);
			USART1_SendString("\t\n");
		}
	}
}

void Message_Action()
{
	char *s,*t;
	s=strstr(USART2_Rx,"+CMTI: \"SM\"");
	s=strstr(s,"SM");
	if(s)//��λ�յ�����Ϣ�Ķ�ȡ��
	{
		t=MS_ID;
		s+=4;                   
		while((*s)!=0x0d)          //����Ϣ��ID����������
		{
			*t=*s;
			t++;
			s++;
		}
		*t='\0';
		USART1_SendString(MS_ID);
		USART1_SendString("\t\n");
		Message_Flag=1;
	}	
}

//GSM�����ַ�������
void GSM_Pr()		//���ŵ�������'!'��ͷ�����ʽΪ"XX!XXS=x\t"
{
	char * s;
//	USART1_SendString("This is GSM_Pr\t\n");
	//����Ҫͣ��ʱ���͵�����Ϊ��!S=0��
	s=strstr(USART2_Rx,"XX!XX");
	s=strchr(s,'S');
	if(s)
	{
		if(*(s+2)=='0')
		{
			Mode=0;           //��С���Ĺ���ģʽ���µ���Ϊ��������ģʽ
			LED2_OFF;
			LED1_ON;
			PWM_Stop();
			USART1_SendString("Mode=0\r\n");
		}
		else if (*(s+2)=='2')
		{
			Meacure_Test=1;
			LED3_ON;
			USART1_SendString("Measure_Test\r\n");
		}
	}
	
}

//��ȡ���յ�����Ϣ
void GET_Message() 
{
	char a[15];
	a[0]='\0';
	strcat(a,"AT+CMGR=");  //���ָ��ĵ�һ����
	strcat(a,MS_ID);   //�����Ϣ��ID
	strcat(a,"\r\n");
	USART2_SendString(a);   //��GPS/GSMģ���ж�ȡ��Ӧ�Ķ���
}

//������Ϣ�Ľӿ�
void Send_Message(char Message[])
{
	char s[30];
	s[0]='\0';
	USART2_SendString("AT\r\n");
	Delay_Nms(100);
	strcat(s,"AT+CMGS=\"");
	strcat(s,Phone_Number);
	strcat(s,"\"\r\n\0");    //��ɵ绰ָ���ƴ��
	USART2_SendString(s);         //����Ŀ�ĺ���
	Delay_Nms(2000);
	USART2_SendString(Message);
	Delay_Nms(300);
	USART_SendData(USART2, 0x1A);
	Delay_Nms(1);
	USART2_SendString("\r\n");
}

//����һ��GPS���ݻش�ָ��
void Get_Location_Data()    //�õ�GPS����
{
	USART2_SendString("AT\r\n");
	Delay_Nms(100);	
	USART2_SendString("AT+QGNSSRD=\"NMEA/RMC\"\r\n");
	Delay_Nms(300);	
}



//��Ϣ��GPS���ܳ�ʼ��
void GPS_GSM_Init()
{	
	USART2_SendString("AT\r\n");
	Delay_Nms(100);
	USART2_SendString("AT+CFUN=1\r\n");
	Delay_Nms(100);
	USART2_SendString("AT+QGNSSC=1\r\n");    //������λ����
	Delay_Nms(300);
	USART2_SendString("AT+CMGF=1\r\n");
	Delay_Nms(300);
	USART2_SendString("AT+CSCS=\"GSM\"\r\n");
	Delay_Nms(300);
	USART2_SendString("AT+CNMI=2,1\r\n");     //���ý�������
	Delay_Nms(300);
}





