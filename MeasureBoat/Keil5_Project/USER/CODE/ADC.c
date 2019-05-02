/*

ADC转换功能，使用用GPIOC的2、3 通道，分别对应ADC的12、13

*/

#include <ADC.h>

//uchar counter = 0;
uchar ADC_Number=2;
uint16_t ADC_Value[24];     //最多采集2路，循环6次后去平均值
char ADC_CData[4][7]={{'\0'},{'\0'},{'\0'},{'\0'}};      //存放最终结果的字符形式用于传输

void ADC1_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn; //通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //中断占先等级2位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //中断响应优先级2位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //打开中断
	NVIC_Init(&NVIC_InitStructure);
}

void ADC1_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);//DMA1通道1配置
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&ADC1->DR);
	//内存地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value;
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设作为数据传输的来源
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize = ADC_Number*6;    //接收数组的长度
	//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	//设置DMA的内存递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //内存中地址寄存器递增
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设的数据长度位16位
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //内存的数据长度位16位
	//设置DMA的传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;      //工作在正常模式
	//设置DMA的优先级别
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //拥有非常高的优先级
	//设置DMA的2个memory中的变量互相访问
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //非内存到内存传输
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);          //DMA1通道1初始化
	//使能通道5
	DMA_Cmd(DMA1_Channel1,ENABLE);                       //不使能通道1
}

void ADC1_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//ADC12、13
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	//模拟输入
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void ADC1_Init()
{
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   //ADC1 和 ADC2 工作在独立模式 
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;       //选择是否工作在多通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   //模数转换工作在连续还是单次模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //转换由软件而不是外部触发启动 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //ADC 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADC_Number; 
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_55Cycles5);
	
//	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC1|DMA1_FLAG_HT1|DMA1_FLAG_TE1);	
	DMA1_Channel1->CNDTR = ADC_Number*6;//DMA 通道5待传输数据数目
	DMA_Cmd(DMA1_Channel1, ENABLE);//处理完,重开DMA
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);  		//使能ADC1
	
	ADC_ResetCalibration(ADC1);//重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));//获取ADC重置校准寄存器的状态
	
	ADC_StartCalibration(ADC1);//开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC1));//获取指定ADC的校准程序	
	
}


void ADC1_Config()
{
	/*	打开端口对应的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//APB2的时钟六分频为12M  最大14M 设置ADC时钟（ADCCLK）
	
//	ADC1_NVIC_Init();
	ADC1_GPIO_Init();
	ADC1_DMA_Init();
	ADC1_Init();
	
	ADC_Cmd(ADC1,DISABLE);  		//默认关闭ADC1
}

//void ADC1_2_IRQHandler()	
//{
//	if(ADC_GetITStatus(ADC1,ADC_IT_EOC))
//	{
//		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
//		ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
//		ADC_Value[counter++]=ADC1->DR;
//	}	
//}
	
void ADC_Data_Process(int *data)    //jia
{
	uchar i;
	int j;
	for (i=0;i<ADC_Number;i++)
	{
		j=(data[i]/4096.0*3300.0);
		ADC_CData[i][6]='\0';
		ADC_CData[i][5]=(char)(j%10+48);
		ADC_CData[i][4]=(char)((j/10)%10+48);
		ADC_CData[i][3]=(char)((j/100)%10+48);
		ADC_CData[i][2]='.';
		ADC_CData[i][1]=(char)((j/1000)%10+48);
		ADC_CData[i][0]=(char)(65+i);
	}
	
	USART1_SendString("ADC Data is:\t");
	for(i=0;i<ADC_Number;i++)
		USART1_SendString(ADC_CData[i]);
	USART1_SendString("\r\n");
}
 
void ADC_Conversion()
{
	char i,j;
	int data[6]={0,0,0,0,0,0,};
	
	ADC1_Init(); 
	Delay_Nus(10);
	
	for(i=0;i<=6;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //启动ADC转换
		Delay_Nus(10);  //预留充足的时间让转换完成
	}
	for(i=0;i<ADC_Number;i++)
	{
		for(j=0;j<6;j++)
			data[i]+=ADC_Value[i+j*ADC_Number];
		data[i]/=6;    //得到一个平均值
	}
	ADC_Data_Process(data);	  //处理数据
//	counter=0;
	DMA_Cmd(DMA1_Channel1, DISABLE);
	ADC_Cmd(ADC1,DISABLE);  		//使用完成以后关闭相应的功能
}
 


