#ifndef __DMA_H__

#include "stm32f4xx.h"
#include "usart.h"



/*---------------------DMA配置宏 ------------------------*/

#define USART_DMA_CLK           RCC_AHB1Periph_DMA2
#define USART_DMA_CLK_CMD       RCC_AHB1PeriphClockCmd
#define USART_DMA_CHANNEL       DMA_Channel_4
#define USART_DMA_STREAM        DMA2_Stream7
#define USART_DMA_DR_BASE       ((uint32_t)(USART1_BASE+0x04))
#define USART_DMA_MEMORY_BASE   ((uint32_t)USART_Buffer)

#define USART_DMA_BUFF_SIZE     USART_BUFFER_SIZE



/*---------------------- 函数声明 ----------------------------*/

void USART_DMA_Config(void);

#endif // !__DMA_H__
