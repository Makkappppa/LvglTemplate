
#include "delay.h"

static u8  fac_us = 0;		//us		   
static __IO uint32_t TimingDelay;  //计数变量

void Delay_Init(void){
	// 配置SysTick 1ms 一次中断
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	// SysTick_Config(SystemCoreClock / 1000000);				//SystemCoreClock = 168000000
	fac_us = 21;											// 8分频->1 6800 0000 / 8 = 2100 0000 MHz	

}

// 在SysTick中调用的计数
void TimingDelay_Decrement(void) 
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}


void delay_ms(uint32_t nms){ 

	u32 r = nms;
	while(r){
		delay_us(1000);
		r--;
	}
}

void delay_us(u32 nus){
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间      
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;  	    				//清空计数器 
} 



