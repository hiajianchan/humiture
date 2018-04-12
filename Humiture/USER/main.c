/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_gpio.h"
#include "led.h"
#include "usart.h"
#include "timer.h"
#include "SHT2x.h"
#if 0
struct Text
{
	uint8_t head;
	char *sendbuf;
	char *recvbuf;
	uint8_t a;
	uint8_t member;	
};

typedef struct 
{
	uint8_t head;
	char *sendbuf;
	uint8_t member;	
}Text1;

#define offset(stru_name,m) (size_t)(&((struct stru_name *)0)->m)
//#define list_struct_addr(stru_name,s,m) (( size_t)((char *)(&(s.m))) - offset(stru_name,m))
#define list_struct_addr(stru_name,s,m) (( size_t)((struct stru_name *)(&(s.m))) - offset(stru_name,m))
void init(void)
{
	struct Text test,c;
	Text1 test1;
	char sbuf[100]={0,1,2,3,4,5,6,7,8};
	char rbuf[100]={11,12,12,14,4,5,63};
	unsigned long s,f;
	
	test.head = 1;
	test.sendbuf = sbuf;
	test.recvbuf = rbuf;
	test.member = 55;
	//test.a = 10;
	
	list_struct_addr(Text,test,member);
	s = offset(Text,member);
	printf("offset:%d\r\n",s);
	printf("saddr:0x%x\r\n",((char *)&(test.member)) - s);
	//f = (size_t)list_struct_addr(test,member) - s;
	printf("xxxsaddr:0x%x\r\n",list_struct_addr(Text,test,member));
}

int main(void)
{	
	
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);
	Init_USART2(9600);
	LED_Init();		  	//初始化与LED连接的硬件接口
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
	//BC95_Test_Demo();

  while(1)
	{
		printf("mainloop\r\n");
		LED0=!LED0;
		init();
		delay_ms(1000);		   
	}
}
#else


int main(void)
{

	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(115200);
	Init_USART2(9600);
	LED_Init();		  	//初始化与LED连接的硬件接口
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
	BC95_Test_Demo();

	
  while(1)
	{
		printf("mainloop\r\n");
		LED0=!LED0;
	
		delay_ms(1000);		 
		
	}

}
#endif
/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
