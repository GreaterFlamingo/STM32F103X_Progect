
#include<MOTO.h>

/*
	控制和灯控制引脚
*/

uchar MOTO_Number=50;

uchar const table_1[]={0x90,0x80,0xc0,0x40,0x60,0x20,0x30,0x10}; //顺时针
uchar const table_2[]={0x10,0x30,0x20,0x60,0x40,0xc0,0x80,0x90}; //逆时针


void MOTO_Init()    //由A4~7号引脚来控制
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能GPIOB的时钟	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //使能复用功能时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //关闭JATG功能
	//配置GPIOB14引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;           //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIOB->ODR&=~(0x00EF); //所有端口默认输出低点平
}

void MOTO_Reverse()    //电机逆时针
{
	uchar i,j;
	for(j=0;j<MOTO_Number;j++)
	{
		for (i=0;i<8;i++)
		{
			GPIOB->ODR&=0x0f;	//清空高四位
			GPIOB->ODR|=table_2[i];
			Delay_Nms(5);
		}
	}
	GPIOB->ODR&=0x0f;	//清空高四位
}

void MOTO_Forward()   //电机顺时针
{
	uchar i,j;
	for (j=0;j<MOTO_Number;j++)       //根据设定的次数转动指定的角度
	{
		for (i=0;i<8;i++)
		{
			GPIOB->ODR&=0x0f;	//清空高四位
			GPIOB->ODR|=table_1[i];
			Delay_Nms(5);
		}
	}
	GPIOB->ODR&=0x0f;	//清空高四位
}


