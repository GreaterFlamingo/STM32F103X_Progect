
#include <USART1.h>

extern uchar Mode;     //�ֱ�Ϊ�ַ�'0'��'1'
//'0'Ϊ����ģʽ��'1'Ϊ�Զ�ģʽ��
extern uchar Meacure_Test;   
extern uchar ADC_Mode;       //�Ƿ���ADC���Լ�ADC����Ŀ
extern uchar MOTO_Mode;      //����Ƿ���
extern uchar ADC_Number;

#define UART_RX_LEN 120
static char USART1_Rx[UART_RX_LEN];//�������ݻ�����


void UART1_RCC_Configuration(void);
void UART1_GPIO_Configuration(void);
void UART1_DMA_Configuration(void);
void UART1_Configuration(void);
void USART1_NVIC_Configuration(void);

void USART1_Init(void)

{

	UART1_RCC_Configuration();//�򿪿�����Ӧʱ��
	UART1_GPIO_Configuration();//GPIO����
	UART1_DMA_Configuration();//DMA����
	UART1_Configuration();//UART����
	USART1_NVIC_Configuration();//�ж����ȼ�����

}

void UART1_RCC_Configuration(void)

{

	//�򿪴��ڶ�Ӧ������ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

}


void UART1_GPIO_Configuration(void)

{
	//���õ�ʮ�Ž��ն˿�
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//���õھźŷ��Ͷ˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void UART1_DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel5);//DMA1ͨ��5����
	//�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
	//�ڴ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_Rx;
	//dma���䷽����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // ������Ϊ���ݴ������Դ
	//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_BufferSize = UART_RX_LEN;    //��������ĳ���
	//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //�ڴ��е�ַ�Ĵ�������
	//���������ֳ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //��������ݳ���λ8λ
	//�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //�ڴ�����ݳ���λ8λ
	//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;      //����������ģʽ
	//����DMA�����ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //ӵ�зǳ��ߵ����ȼ�
	//����DMA��2��memory�еı����������
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //���ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);          //DMA1ͨ��5��ʼ��
	//ʹ��ͨ��5
	DMA_Cmd(DMA1_Channel5,ENABLE);                       //ʹ��ͨ��5

}

void UART1_Configuration(void)

{
	//����ṹ�壬��ʼ��USART
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ʹ�ܽ��պͷ���
	
	USART_Init(USART1,&USART_InitStructure);	//��ʼ������

	//�ж�����

	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);

	//����DMA��ʽ����
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);    //ʹ�ܽ��յ�DMA����
	USART_Cmd(USART1,ENABLE);

}

void USART1_NVIC_Configuration(void)

{

	NVIC_InitTypeDef NVIC_InitStructure;
	//����UART1�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //�ж�ռ�ȵȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //���ж�
	NVIC_Init(&NVIC_InitStructure);
}

/*
	USART1�ַ������ͺ���
*/
void USART1_SendString(char str[])
{
	uint16_t i;
	for(i=0;str[i]!='\0';i++)
	{
		if(USART_GetFlagStatus(USART1,USART_FLAG_TC)==SET)
			USART_SendData(USART1, str[i]);
		else
		{
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
			USART_SendData(USART1, str[i]);
		}
	}
}

/*
	USART�ַ���������
*/

char Phone_Number[13]="18937207581\0";  //Ҫ���͵��ֻ�����

//��ȡ�绰����
void Get_Phone_Number(char * t)
{
	char *s=Phone_Number;
	//��ȡ�ֻ�����
	while((*t)!='\r')
	{
		*s=*t;
		s++;
		t++;
	}
	USART1_SendString(Phone_Number);
	USART1_SendString("\r\n");
}

//���������ַ�������
void USART1_Pr()           //�����ش��������ԡ�*����ͷ��ʽΪ"*B:x\r"
{
	char * s;
//	USART1_SendString("BL_Pr...\r\n");
	s=strchr(USART1_Rx,'B');
	if(s)   //�ж������Ƿ�ɱ�����
	{
		s+=2;
		switch(*s)
		{
			case '1': Mode=1;LED1_OFF;LED2_ON;PWM_Start();break;    //������ģʽ�л�Ϊ�Զ���ʻģʽ
			case '2': Meacure_Test=1;LED3_ON;break;    //����һ�β���   
			case '3': PWM_Start();break;   //����PWM�Ŀ�����ر�
			case '4': PWM_Stop();break;
			case '5': PWM_BL();break;    //����С����ת��
			case '6': PWM_Left_BL();break;
			case '7': PWM_Right_BL();break; 
			case '8': Get_Phone_Number(s+1);break;  //���յ绰����(*B:x123...\r)
			case '9': GPS_GSM_Init();break;
			case 'a': ADC_Mode=1;ADC_Number=*(s+1)-48;break;        //�����Ƿ�����ADCת������,������ADC����·��
			case 'b': ADC_Mode=0;break;
			case 'c': MOTO_Mode=1;break;        //�����Ƿ����ò�\\\�����
			case 'd': MOTO_Mode=0;break;
			case 'e': Send_Message("This is a Testing.\r\n");break;
			case 'f': Get_Location_Data();break;
			default:;
		}
	}
	USART1_SendString(USART1_Rx);  //�ش����͵�����
}


//USART1�жϴ������������߿���ʱ��
/*
IDLE�жϴ����Ժ�����ж�λ�Ժ������߼������в������ٴα�����
ֻ�еȴ��´ν��յ����ݽ��������Ժ�Żᱻ�ٴι���
*/
void USART1_IRQHandler(void)

{
	uint32_t Length = 0;//���ݳ���
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //��һ��ȷ���ǽ����ж�

	{
		DMA_Cmd(DMA1_Channel5,DISABLE);  //DMAͨ��ʧ��
		//��USART_CR1�е�IDLEIEΪ��1����������жϡ��� ������������λ(�ȶ�USART_SR��Ȼ���USART_DR)�� 
		do
		{
			USART_ClearFlag(USART1, USART_FLAG_IDLE);
			USART_ClearITPendingBit(USART1, USART_IT_IDLE);
			Length = USART1->SR;
			Length = USART1->DR;
		}while(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET);
		
		/*�û��������ܳ��ȣ���ȥͨ���ڵĴ�����������Ŀ�õ��������ݵĳ���*/
		Length = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel5); 
		USART1_Rx[Length+1]='\0';   //�ڽ��յ�������֮֡����ӽ�����־
		USART1_Pr();              //������յ�������
		
		//��һЩ���ܵĴ�����������
		if(USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
		{
			USART_ReceiveData(USART1);
			USART_ClearFlag(USART1, USART_FLAG_PE);
		}
		if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
		{
			USART_ReceiveData(USART1);
			USART_ClearFlag(USART1, USART_FLAG_ORE);
		}
		if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
		{
			USART_ReceiveData(USART1);
			USART_ClearFlag(USART1, USART_FLAG_FE);
		}
		
		DMA_ClearFlag(DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
		
		DMA1_Channel5->CNDTR = UART_RX_LEN;//DMA ͨ��5������������Ŀ
		DMA_Cmd(DMA1_Channel5, ENABLE);//������,�ؿ�DMA
	}
	__nop();
}


