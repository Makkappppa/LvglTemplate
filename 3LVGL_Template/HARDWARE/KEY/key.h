#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f4xx.h"

#define    KEY0_GPIO_CLK_CMD    RCC_AHB1PeriphClockCmd
#define    KEY0_GPIO_CLK     	RCC_AHB1Periph_GPIOA
#define    KEY0_GPIO_PORT    	GPIOA			   
#define    KEY0_GPIO_PIN		GPIO_Pin_15

//#define    KEY1_GPIO_CLK     	RCC_APB2Periph_GPIOE
//#define    KEY1_GPIO_PORT    	GPIOE		   
//#define    KEY1_GPIO_PIN		GPIO_Pin_3

//#define    KEY2_GPIO_CLK     	RCC_APB2Periph_GPIOE
//#define    KEY2_GPIO_PORT    	GPIOE			   
//#define    KEY2_GPIO_PIN		GPIO_Pin_2

//#define    KEYUP_GPIO_CLK     	RCC_APB2Periph_GPIOA
//#define    KEYUP_GPIO_PORT    	GPIOA	   
//#define    KEYUP_GPIO_PIN		GPIO_Pin_0


/* 按下为低电平则设置KEY_ON为0 */
#define KEY_ON	1
#define KEY_OFF	0

void 	KEY_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#define Key0_Scan()  Key_Scan(KEY0_GPIO_PORT, KEY0_GPIO_PIN)



#endif

