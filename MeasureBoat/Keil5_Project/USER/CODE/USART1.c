
#include <USART1.h>

extern uchar Mode;     //分别为字符'0'、'1'
//'0'为蓝牙模式，'1'为自动模式，
extern uchar Meacure_Test;   
extern uchar ADC_Mode;       //是否开启ADC，以及ADC的数目
extern uchar MOTO_Mode;      //电机是否开启
extern uchar ADC_Number;

#define UART_RX_LEN 120
static char USART1_Rx[UART_RX_LEN];//接收数据缓冲区


void UART1_RCC_Configuration(void);
void UART1_GPIO_Configuration(void);
void UART1_DMA_Configuration(void);
void UART1_Configuration(void);
void USART1_NVIC_Configuration(void);

void USART1_Init(void)

{

	UART1_RCC_Configuration();//打开开启相应时钟
	UART1_GPIO_Configuration();//GPIO配置
	UART1_DMA_Configuration();//DMA配置
	UART1_Configuration();//UART配置
	USART1_NVIC_Configuration();//中断优先级配置

}

void UART1_RCC_Configuration(void)

{

	//打开串口对应的外设时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//启动DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

}


void UART1_GPIO_Configuration(void)

{
	//配置第十号接收端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置第九号发送端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void UART1_DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel5);//DMA1通道5配置
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
	//内存地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_Rx;
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设作为数据传输的来源
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize = UART_RX_LEN;    //接收数组的长度
	//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	//设置DMA的内存递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //内存中地址寄存器递增
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //外设的数据长度位8位
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //内存的数据长度位8位
	//设置DMA的传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;      //工作在正常模式
	//设置DMA的优先级别
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //拥有非常高的优先级
	//设置DMA的2个memory中的变量互相访问
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //非内存到内存传输
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);          //DMA1通道5初始化
	//使能通道5
	DMA_Cmd(DMA1_Channel5,ENABLE);                       //使能通道5

}

void UART1_Configuration(void)

{
	//定义结构体，初始化USART
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //使能接收和发送
	
	USART_Init(USART1,&USART_InitStructure);	//初始化串口

	//中断配置

	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);

	//采用DMA方式接收
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);    //使能接收的DMA请求
	USART_Cmd(USART1,ENABLE);

}

void USART1_NVIC_Configuration(void)

{

	NVIC_InitTypeDef NVIC_InitStructure;
	//设置UART1中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //中断占先等级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //中断响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //打开中断
	NVIC_Init(&NVIC_InitStructure);
}

/*
	USART1字符串发送函数
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
	USART字符串处理函数
*/

char Phone_Number[13]="18937207581\0";  //要发送的手机号码

//提取电话号码
void Get_Phone_Number(char * t)
{
	char *s=Phone_Number;
	//提取手机号码
	while((*t)!='\r')
	{
		*s=*t;
		s++;
		t++;
	}
	USART1_SendString(Phone_Number);
	USART1_SendString("\r\n");
}

//蓝牙接收字符处理函数
void USART1_Pr()           //蓝牙回传的数据以‘*’开头格式为"*B:x\r"
{
	char * s;
//	USART1_SendString("BL_Pr...\r\n");
	s=strchr(USART1_Rx,'B');
	if(s)   //判断数据是否可被处理
	{
		s+=2;
		switch(*s)
		{
			case '1': Mode=1;LED1_OFF;LED2_ON;PWM_Start();break;    //由蓝牙模式切换为自动行驶模式
			case '2': Meacure_Test=1;LED3_ON;break;    //启用一次测量   
			case '3': PWM_Start();break;   //控制PWM的开启与关闭
			case '4': PWM_Stop();break;
			case '5': PWM_BL();break;    //控制小船的转向
			case '6': PWM_Left_BL();break;
			case '7': PWM_Right_BL();break; 
			case '8': Get_Phone_Number(s+1);break;  //接收电话号码(*B:x123...\r)
			case '9': GPS_GSM_Init();break;
			case 'a': ADC_Mode=1;ADC_Number=*(s+1)-48;break;        //控制是否启用ADC转换功能,并设置ADC换的路数
			case 'b': ADC_Mode=0;break;
			case 'c': MOTO_Mode=1;break;        //控制是否启用步\\\进电机
			case 'd': MOTO_Mode=0;break;
			case 'e': Send_Message("This is a Testing.\r\n");break;
			case 'f': Get_Location_Data();break;
			default:;
		}
	}
	USART1_SendString(USART1_Rx);  //回传发送的数据
}


//USART1中断处理函数（当总线空闲时）
/*
IDLE中断处理以后被清除中断位以后若总线继续空闲并不会再次被挂起，
只有等待下次接收到数据结束空闲以后才会被再次挂起
*/
void USART1_IRQHandler(void)

{
	uint32_t Length = 0;//数据长度
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //进一步确认是接收中断

	{
		DMA_Cmd(DMA1_Channel5,DISABLE);  //DMA通道失能
		//果USART_CR1中的IDLEIE为’1’，则产生中断。由 软件序列清除该位(先读USART_SR，然后读USART_DR)。 
		do
		{
			USART_ClearFlag(USART1, USART_FLAG_IDLE);
			USART_ClearITPendingBit(USART1, USART_IT_IDLE);
			Length = USART1->SR;
			Length = USART1->DR;
		}while(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET);
		
		/*拿缓冲区的总长度，减去通道内的待传输数据数目得到接收数据的长度*/
		Length = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel5); 
		USART1_Rx[Length+1]='\0';   //在接收到的数据帧之后添加结束标志
		USART1_Pr();              //处理接收到的数据
		
		//对一些可能的错误进行清除；
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
		
		DMA1_Channel5->CNDTR = UART_RX_LEN;//DMA 通道5待传输数据数目
		DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA
	}
	__nop();
}


