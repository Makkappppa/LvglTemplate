#include "led.h"

void LED_Init(void){
	GPIO_InitTypeDef GB;
	
	LED1_GPIO_CLK_CMD(LED1_GPIO_CLK, ENABLE);
	
	GB.GPIO_Mode 	= GPIO_Mode_OUT;	// 输出
	GB.GPIO_OType 	= GPIO_OType_PP;	// 推挽输出
	GB.GPIO_Pin		= LED1_GPIO_PIN;	// GPIO_Pin_13
	GB.GPIO_PuPd	= GPIO_PuPd_UP; 	// 上拉
	GB.GPIO_Speed	= GPIO_Speed_2MHz;	// 2/25/50/100 MHz
	GPIO_Init(LED1_GPIO_PORT, &GB);
	
	LED1_OFF;
}

