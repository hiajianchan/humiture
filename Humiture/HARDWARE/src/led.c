#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//技术论坛:
//修改日期:
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB0和PB1为输出口.并使能这两个口的时钟		    
//LED IO初始化

void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //LED0-->PB.0 1 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 				 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					             //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_0);						                   //PB.0 输出高
	GPIO_SetBits(GPIOB,GPIO_Pin_1);						                   //PB.1 输出高

}
 
