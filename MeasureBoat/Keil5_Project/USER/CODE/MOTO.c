
#include<MOTO.h>

/*
	���ƺ͵ƿ�������
*/

uchar MOTO_Number=50;

uchar const table_1[]={0x90,0x80,0xc0,0x40,0x60,0x20,0x30,0x10}; //˳ʱ��
uchar const table_2[]={0x10,0x30,0x20,0x60,0x40,0xc0,0x80,0x90}; //��ʱ��


void MOTO_Init()    //��A4~7������������
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��GPIOB��ʱ��	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //ʹ�ܸ��ù���ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //�ر�JATG����
	//����GPIOB14����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;           //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIOB->ODR&=~(0x00EF); //���ж˿�Ĭ������͵�ƽ
}

void MOTO_Reverse()    //�����ʱ��
{
	uchar i,j;
	for(j=0;j<MOTO_Number;j++)
	{
		for (i=0;i<8;i++)
		{
			GPIOB->ODR&=0x0f;	//��ո���λ
			GPIOB->ODR|=table_2[i];
			Delay_Nms(5);
		}
	}
	GPIOB->ODR&=0x0f;	//��ո���λ
}

void MOTO_Forward()   //���˳ʱ��
{
	uchar i,j;
	for (j=0;j<MOTO_Number;j++)       //�����趨�Ĵ���ת��ָ���ĽǶ�
	{
		for (i=0;i<8;i++)
		{
			GPIOB->ODR&=0x0f;	//��ո���λ
			GPIOB->ODR|=table_1[i];
			Delay_Nms(5);
		}
	}
	GPIOB->ODR&=0x0f;	//��ո���λ
}


