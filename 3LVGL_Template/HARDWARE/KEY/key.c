#include "key.h"


void KEY_Config(void)
{
	GPIO_InitTypeDef GPIOInit;
	
	KEY0_GPIO_CLK_CMD(KEY0_GPIO_CLK,ENABLE);
	
	GPIOInit.GPIO_Mode 	= GPIO_Mode_IN;
	GPIOInit.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIOInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInit.GPIO_Pin 	= KEY0_GPIO_PIN;
	GPIO_Init(KEY0_GPIO_PORT, &GPIOInit);
	
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/* 扫描 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/* 等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON){} 
		return 	KEY_ON;	 
	}
	else
		return	KEY_OFF;
}




