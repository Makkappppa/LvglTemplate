#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"

#define ON  0
#define OFF 1

// 直接操作寄存器
#define	digitalHi(p,i)		 	{p->BSRR=i;}		//输出高
#define digitalLo(p,i)		 	{p->BRR=i;}			//输出低
#define digitalToggle(p,i) 		{p->ODR ^=i;} 		//输出反转

#define LED1_GPIO_PORT    	GPIOC		           
#define LED1_GPIO_CLK 	    RCC_AHB1Periph_GPIOC	
#define LED1_GPIO_PIN		GPIO_Pin_13			 
#define LED1_GPIO_CLK_CMD	RCC_AHB1PeriphClockCmd			        

#define LED2_GPIO_PORT    	GPIOE			       
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOE		
#define LED2_GPIO_PIN		GPIO_Pin_5	

#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

/* 控制LED的宏 */
#define LED1_TOGGLE		 	digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   	digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   	digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		 	digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   	digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   	digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)
					
					
void LED_Init(void);


#endif

