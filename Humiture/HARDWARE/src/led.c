#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//������̳:
//�޸�����:
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) 
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB0��PB1Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //LED0-->PB.0 1 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 				 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					             //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_0);						                   //PB.0 �����
	GPIO_SetBits(GPIOB,GPIO_Pin_1);						                   //PB.1 �����

}
 
