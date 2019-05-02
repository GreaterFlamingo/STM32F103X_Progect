/*

ADCת�����ܣ�ʹ����GPIOC��2��3 ͨ�����ֱ��ӦADC��12��13

*/

#include <ADC.h>

//uchar counter = 0;
uchar ADC_Number=2;
uint16_t ADC_Value[24];     //���ɼ�2·��ѭ��6�κ�ȥƽ��ֵ
char ADC_CData[4][7]={{'\0'},{'\0'},{'\0'},{'\0'}};      //������ս�����ַ���ʽ���ڴ���

void ADC1_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn; //ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //�ж�ռ�ȵȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�ж���Ӧ���ȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //���ж�
	NVIC_Init(&NVIC_InitStructure);
}

void ADC1_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);//DMA1ͨ��1����
	//�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&ADC1->DR);
	//�ڴ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value;
	//dma���䷽����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // ������Ϊ���ݴ������Դ
	//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_BufferSize = ADC_Number*6;    //��������ĳ���
	//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //�ڴ��е�ַ�Ĵ�������
	//���������ֳ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //��������ݳ���λ16λ
	//�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //�ڴ�����ݳ���λ16λ
	//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;      //����������ģʽ
	//����DMA�����ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //ӵ�зǳ��ߵ����ȼ�
	//����DMA��2��memory�еı����������
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //���ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);          //DMA1ͨ��1��ʼ��
	//ʹ��ͨ��5
	DMA_Cmd(DMA1_Channel1,ENABLE);                       //��ʹ��ͨ��1
}

void ADC1_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//ADC12��13
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	//ģ������
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void ADC1_Init()
{
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   //ADC1 �� ADC2 �����ڶ���ģʽ 
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;       //ѡ���Ƿ����ڶ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   //ģ��ת���������������ǵ���ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //ת��������������ⲿ�������� 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //ADC �����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADC_Number; 
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_55Cycles5);
	
//	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC1|DMA1_FLAG_HT1|DMA1_FLAG_TE1);	
	DMA1_Channel1->CNDTR = ADC_Number*6;//DMA ͨ��5������������Ŀ
	DMA_Cmd(DMA1_Channel1, ENABLE);//������,�ؿ�DMA
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);  		//ʹ��ADC1
	
	ADC_ResetCalibration(ADC1);//����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//��ȡADC����У׼�Ĵ�����״̬
	
	ADC_StartCalibration(ADC1);//��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));//��ȡָ��ADC��У׼����	
	
}


void ADC1_Config()
{
	/*	�򿪶˿ڶ�Ӧ��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//APB2��ʱ������ƵΪ12M  ���14M ����ADCʱ�ӣ�ADCCLK��
	
//	ADC1_NVIC_Init();
	ADC1_GPIO_Init();
	ADC1_DMA_Init();
	ADC1_Init();
	
	ADC_Cmd(ADC1,DISABLE);  		//Ĭ�Ϲر�ADC1
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
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //����ADCת��
		Delay_Nus(10);  //Ԥ�������ʱ����ת�����
	}
	for(i=0;i<ADC_Number;i++)
	{
		for(j=0;j<6;j++)
			data[i]+=ADC_Value[i+j*ADC_Number];
		data[i]/=6;    //�õ�һ��ƽ��ֵ
	}
	ADC_Data_Process(data);	  //��������
//	counter=0;
	DMA_Cmd(DMA1_Channel1, DISABLE);
	ADC_Cmd(ADC1,DISABLE);  		//ʹ������Ժ�ر���Ӧ�Ĺ���
}
 


