#include "timer.h"
#include "led.h"
#include "lvgl.h"

void Timer_Config(void){
	TIM_TimeBaseInitTypeDef TI;
	NVIC_InitTypeDef NI;
	// APB1总线时钟4分频 42M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_DeInit(TIM2);
	
	TI.TIM_Period		= 41;
	TI.TIM_Prescaler	= 999;
	TI.TIM_CounterMode	= TIM_CounterMode_Up;
	TI.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TI);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		//中断服务开启
	NI.NVIC_IRQChannel 		= TIM2_IRQn;
	NI.NVIC_IRQChannelCmd 	= ENABLE;
	NI.NVIC_IRQChannelPreemptionPriority 	= 0;
	NI.NVIC_IRQChannelSubPriority 			= 8;
	
	NVIC_Init(&NI);
	
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)){
		lv_tick_inc(1);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	
}

