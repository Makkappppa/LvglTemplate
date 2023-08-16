#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f4xx.h"
#include "dma.h"

/*---------------------- 数据缓冲区 ----------------------------*/
#define USART_BUFFER_SIZE           1024
extern uint8_t USART_Buffer[USART_BUFFER_SIZE];
extern uint16_t USART_Cnt;

/*----------------------USART配置宏 ------------------------*/

#define	USART_BR					115200
#define USART_CLK                   RCC_APB2Periph_USART1
#define	USART_CLK_Cmd               RCC_APB2PeriphClockCmd

#define	USART_GPIO_CLK_Cmd          RCC_AHB1PeriphClockCmd
#define	USART_TX_PIN				GPIO_Pin_9					// TX 引脚
#define	USART_TX_PORT				GPIOA					    // TX 引脚端口
#define	USART_TX_CLK				RCC_AHB1Periph_GPIOA	    // TX 引脚时钟
#define	USART_TX_AF		        	GPIO_PinSource9			    // 引脚源

#define	USART_RX_PIN				GPIO_Pin_10                 // RX 引脚
#define	USART_RX_PORT				GPIOA                       // RX 引脚端口
#define	USART_RX_CLK				RCC_AHB1Periph_GPIOA        // RX 引脚时钟
#define	USART_RX_AF		        	GPIO_PinSource10            // 引脚源

#define USART_IRQHandler            USART1_IRQHandler
#define USART_IRQ                   USART1_IRQn


#define USE_USART_DMA   1
/*---------------------- 函数声明 ----------------------------*/

void	USART_Config (void);	// USART初始化函数

void	USART_SendByte(USART_TypeDef* pUSART, uint8_t ch);
void	USART_SendString(USART_TypeDef* pUSART, uint8_t* string);
#endif //__USART_H

