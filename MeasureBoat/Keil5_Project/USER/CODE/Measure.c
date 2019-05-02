
#include<Measure.h>

char overflow=0;
int distance[6];


void Control_PWN(void);


void Measure_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能GPIOB的时钟	
	
	//配置GPIOB14引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;           //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//配置GPIOB15引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;            //下拉输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//将输出引脚默认设置为低电平
	GPIOB->ODR&=~(0x000f<<8);
	
	Time3_Config();
	
}

void Data_Conversion(int j)
{
	char data_str[6];
	int i=j;
	data_str[5]='\0';
	data_str[4]=(char)(i%10+48);
	data_str[3]=(char)((i/10)%10+48);
	data_str[2]=(char)((i/100)%10+48);
	data_str[1]=(char)((i/1000)%10+48);
	data_str[0]=(char)((i/10000)%10+48);
	USART1_SendString(data_str);
}

void Measure_Data(int i)
{
	uint16_t counter = 0;
	overflow=0;
	GPIOB->ODR|=(1<<(i+8));
	Delay_Nus(20);
	TIM3->CNT=0;  //清空计数器为0
	GPIOB->ODR&=~(1<<(i+8));
	while((!ME_IN(i+12))&&(counter<5000))counter++;
	if(ME_IN(i+12))
	{
		TIM3_Start;  //启用计时器3
		while(ME_IN(i+12)&&(!overflow));
//		Delay_Nus(100);
		TIM3_Close; //关闭计时器3
		if(overflow==1)
		{
			distance[i]=100;
			USART1_SendString("100\t\n");
			overflow=0;
		}
		else
		{
			distance[i]=(int)(T3_GET_Time()/20); //距离以厘米的形式存储
			USART1_SendString("\r\n");
			Data_Conversion(distance[i]);
			USART1_SendString("\r\n");
		}
	}
	
}


void Measure()
{
	int i=0;
	for(i=0;i<4;i++)
	{
		Measure_Data(i);     //检测周边数据
	}
	Control_PWN();     //控制转向部分
}


void Control_PWN()
{
	if (distance[0]<10||distance[1]<10)
	{
		if (distance[0]<=5||distance[1]<=5)
		{
			if (distance[0]<=distance[1])
			{
				PWM_Right_C();
			}
			else
			{
				PWM_Left_C();
			}
		}
		else
		{
			if (distance[0]<=distance[1])
			{
				PWM_Right_B();
			}
			else
			{
				PWM_Left_B();
			}
		}
	}
	
	if(distance[2]<10)
	{
		if (distance[2]<=5)
		{
			PWM_Right_C();
		}
		else
		{
			PWM_Right_B();
		}
	}
	if(distance[3]<10)
	{
		if (distance[3]<=5)
		{
			PWM_Left_C();
		}
		else
		{
			PWM_Left_B();
		}
	}
}



