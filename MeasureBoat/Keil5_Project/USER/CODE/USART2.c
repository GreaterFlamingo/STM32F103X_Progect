
#include <USART2.h>

uint16_t RX_Length=0;
uchar Message_Flag=0;
extern uchar Mode;     //分别为字符'0'、'1'
//'0'为蓝牙模式，'1'为自动模式，
extern uchar Meacure_Test; 
extern char Phone_Number[13];

#define UART_RX_LEN 200

static char USART2_Rx[UART_RX_LEN];//接收数据缓冲区
char GPS_Data[30];    //存储GPS数据
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
	USART2_RCC_Configuration();//打开开启相应时钟
	USART2_GPIO_Configuration();//GPIO配置
	UART2_Configuration();//UART配置
	USART2_NVIC_Configuration();//中断优先级配置
	GPS_Data[0]='\0';   //初始化置空数据
	MS_ID[0]='\0';      //信息的存放位置

}

void USART2_RCC_Configuration(void)

{

	//打开串口对应的外设时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
}


void USART2_GPIO_Configuration(void)

{
	//配置第十号接收端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置第九号发送端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void UART2_Configuration(void)

{
	//定义结构体，初始化USART
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //使能接收和发送
	
	USART_Init(USART2,&USART_InitStructure);	//初始化串口

	//中断配置

	USART_ITConfig(USART2,USART_IT_TC,DISABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	//采用DMA方式接收
	USART_DMACmd(USART2,USART_DMAReq_Rx,DISABLE);    //使能接收的DMA请求
	
	USART_Cmd(USART2,ENABLE);

}

void USART2_NVIC_Configuration(void)

{

	NVIC_InitTypeDef NVIC_InitStructure;
	//设置UART1中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //中断占先等级2位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //中断响应优先级2位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //打开中断
	NVIC_Init(&NVIC_InitStructure);
}

/*
	USART2字符串发送函数
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
	USART字符串处理函数
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



//USART2中断处理函数（当总线空闲时）
/*
IDLE中断处理以后被清除中断位以后若总线继续空闲并不会再次被挂起，
只有等待下次接收到数据结束空闲以后才会被再次挂起
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
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  //进一步确认是接收中断

	{
		uint32_t Length;
		//果USART_CR1中的IDLEIE为’1’，则产生中断。由 软件序列清除该位(先读USART_SR，然后读USART_DR)。
		do
		{
			USART_ClearFlag(USART2, USART_FLAG_IDLE);
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);
			Length = USART2->SR;
			Length = USART2->DR;
		}while(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET);
		USART2_Rx[RX_Length]='\0';   //在接收到的数据帧之后添加结束标志
		USART2_Pr();              //处理接收到的数据
		RX_Length=0;
	}
	__nop();
}

//GPS接收字符处理函数
void GPS_Pr(void)         //GPS和部分GSM字段以‘+’号开头
{
	char * s;
	char * t;
//	USART1_SendString("This is GPS_Pr\t\n");
	//数据帧的样式：+QGNSSRD: $GNRMC,073203.000,A,3604.0109,N,11420.5335,E,0.10,0.00,150319,,,A*74
	s=strstr(USART2_Rx,"+QGNSSRD: $GNRMC");
	if(s) //进一步确认导航信息
	{
		s=strchr(s,'A');//从该指针向后读取纬度
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
			s=strrchr(s,'N');//从该指针向后读取经度(最后一次出现'N'的位置)
			//提取经度的操作
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
	if(s)//定位收到的信息的读取号
	{
		t=MS_ID;
		s+=4;                   
		while((*s)!=0x0d)          //将信息的ID存入数组中
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

//GSM接收字符处理函数
void GSM_Pr()		//短信的内容以'!'开头具体格式为"XX!XXS=x\t"
{
	char * s;
//	USART1_SendString("This is GSM_Pr\t\n");
	//当需要停机时发送的内容为“!S=0”
	s=strstr(USART2_Rx,"XX!XX");
	s=strchr(s,'S');
	if(s)
	{
		if(*(s+2)=='0')
		{
			Mode=0;           //将小船的工作模式重新调整为蓝牙工作模式
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

//获取接收到的信息
void GET_Message() 
{
	char a[15];
	a[0]='\0';
	strcat(a,"AT+CMGR=");  //添加指令的第一部分
	strcat(a,MS_ID);   //添加信息的ID
	strcat(a,"\r\n");
	USART2_SendString(a);   //从GPS/GSM模块中读取相应的短信
}

//发送信息的接口
void Send_Message(char Message[])
{
	char s[30];
	s[0]='\0';
	USART2_SendString("AT\r\n");
	Delay_Nms(100);
	strcat(s,"AT+CMGS=\"");
	strcat(s,Phone_Number);
	strcat(s,"\"\r\n\0");    //完成电话指令的拼接
	USART2_SendString(s);         //发送目的号码
	Delay_Nms(2000);
	USART2_SendString(Message);
	Delay_Nms(300);
	USART_SendData(USART2, 0x1A);
	Delay_Nms(1);
	USART2_SendString("\r\n");
}

//发送一个GPS数据回传指令
void Get_Location_Data()    //得到GPS数据
{
	USART2_SendString("AT\r\n");
	Delay_Nms(100);	
	USART2_SendString("AT+QGNSSRD=\"NMEA/RMC\"\r\n");
	Delay_Nms(300);	
}



//信息及GPS功能初始化
void GPS_GSM_Init()
{	
	USART2_SendString("AT\r\n");
	Delay_Nms(100);
	USART2_SendString("AT+CFUN=1\r\n");
	Delay_Nms(100);
	USART2_SendString("AT+QGNSSC=1\r\n");    //开启定位功能
	Delay_Nms(300);
	USART2_SendString("AT+CMGF=1\r\n");
	Delay_Nms(300);
	USART2_SendString("AT+CSCS=\"GSM\"\r\n");
	Delay_Nms(300);
	USART2_SendString("AT+CNMI=2,1\r\n");     //设置接收提醒
	Delay_Nms(300);
}





