#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);				//延时初始化

void delay_ms(uint32_t nms);		//毫秒延时
void delay_us(uint32_t nus);

#endif


