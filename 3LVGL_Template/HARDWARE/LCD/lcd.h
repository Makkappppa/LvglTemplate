
#ifndef __LCD_H__
#define	__LCD_H__

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "usart.h" 

typedef struct	//LCD相关参数结构体
{
	uint16_t Color;  				// LCD当前画笔颜色
	uint16_t BackColor;				// 背景色
	uint16_t Width;            		// 屏幕像素长度
	uint16_t Height;           		// 屏幕像素宽度
	uint8_t  Direction;				// 显示方向
	uint8_t  SetXcmd;
	uint8_t  SetYcmd;
	uint8_t  WRAMcmd;
}__LCD;
extern __LCD LCDdev;
/*----------------------------------------------- 参数宏 -------------------------------------------*/

#define LCD_Width     240		// LCD的像素长度
#define LCD_Height    320		// LCD的像素宽度

// 显示方向参数
// 使用示例：LCD_DisplayDirection(Direction_H) 设置屏幕横屏显示
#define	Direction_H	            0					//LCD横屏显示
#define	Direction_H_Flip        1					//LCD横屏显示,上下翻转
#define	Direction_V				2					//LCD竖屏显示 
#define	Direction_V_Flip        3					//LCD竖屏显示,上下翻转 
/*------------------------------------------------ 函数声明 ----------------------------------------------*/

void    LCD_Init(void);         // 液晶屏以及SPI初始化   
void    LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);    // 设置坐标

void 	LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void 	LCD_Fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* color);

uint16_t LCD_RGB2U16(uint32_t c);
void 	LCD_Clear(void);

#define LCD_Fill_xy(x1, y1, x2, y2, color) LCD_Fill(x1, y1, (x2-x1+1), (y2-y1+1), color)
#define LCD_DisplayOn()             LCD_Write_Command(0x29)
#define LCD_DisplayOff()            LCD_Write_Command(0x28)
/*------------------------------------------------------  引脚配置宏 -------------------------------------------------*/	

#define 	LCD_SPI                             SPI3				        // LCD用到的是SPI3
#define     LCD_SPI_CLK_CMD 	                RCC_APB1PeriphClockCmd      // 使能外设时钟
#define     LCD_SPI_CLK                         RCC_APB1Periph_SPI3

#define 	LCD_SCK_PIN      		 			GPIO_Pin_3					// SCK引脚， 需要重定义SPI1的IO口复用
#define 	LCD_SCK_PORT     		 			GPIOB                 		// SCK引脚用到的端口  
#define 	LCD_SCK_CLK_CMD      			    RCC_AHB1PeriphClockCmd 		// SCK引脚IO口时钟
#define 	LCD_SCK_CLK      			        RCC_AHB1Periph_GPIOB  		// SCK引脚IO口时钟
#define 	LCD_SCK_PinSource       	        GPIO_PinSource3
#define 	LCD_SCK_AF              	        GPIO_AF_SPI3

#define 	LCD_SDA_PIN      		 			GPIO_Pin_5					// SDA引脚， 需要重定义SPI1的IO口复用
#define 	LCD_SDA_PORT    		 			GPIOB                 		// SDA引脚用到的端口  
#define 	LCD_SDA_CLK_CMD      			    RCC_AHB1PeriphClockCmd 		// SCK引脚IO口时钟
#define 	LCD_SDA_CLK      			        RCC_AHB1Periph_GPIOB  		// SDA引脚IO口时钟
#define 	LCD_SDA_PinSource       	        GPIO_PinSource5
#define 	LCD_SDA_AF              	        GPIO_AF_SPI3

#define 	LCD_CS_PIN       				    GPIO_Pin_11					// CS片选引脚，低电平有效
#define 	LCD_CS_PORT      				    GPIOD                 		// CS引脚用到的端口 
#define 	LCD_CS_CLK_CMD      			    RCC_AHB1PeriphClockCmd 		// SCK引脚IO口时钟
#define 	LCD_CS_CLK     			            RCC_AHB1Periph_GPIOD		// CS引脚IO口时钟

#define     LCD_DC_PIN					        GPIO_Pin_12				    // 数据指令选择  引脚				
#define	    LCD_DC_PORT				            GPIOD						// 数据指令选择  GPIO端口
#define 	LCD_DC_CLK_CMD      			    RCC_AHB1PeriphClockCmd 		// SCK引脚IO口时钟
#define 	LCD_DC_CLK     			            RCC_AHB1Periph_GPIOD		// 数据指令选择  GPIO时钟 	

#define     LCD_BL_PIN				            GPIO_Pin_13				    // 背光  引脚				
#define	    LCD_BL_PORT			                GPIOD						// 背光 GPIO端口
#define 	LCD_BL_CLK_CMD      			    RCC_AHB1PeriphClockCmd 		// SCK引脚IO口时钟
#define 	LCD_BL_CLK                          RCC_AHB1Periph_GPIOD		// 背光 GPIO时钟 	


/*--------------------------------------------------------- 控制宏 ---------------------------------------------------*/

// 因为片选引脚需要频繁操作，使用寄存器效率会高些
#define 	LCD_CS_H()    		 	    GPIOD->BSRR  = LCD_CS_PIN		// 片选拉高, 关闭通信
#define 	LCD_CS_L()     			    GPIOD->BSRR  = (LCD_CS_PIN<<16)		// 片选拉低，开启通信

#define	    LCD_DC_Command()		    GPIO_ResetBits(LCD_DC_PORT, LCD_DC_PIN) // 低电平，指令传输 
#define 	LCD_DC_Data()		        GPIO_SetBits(LCD_DC_PORT, LCD_DC_PIN)	// 高电平，数据传输

#define 	LCD_BL_ON()                 GPIO_SetBits(LCD_BL_PORT,LCD_BL_PIN)    // 高电平，开启背光
#define 	LCD_BL_OFF()   	            GPIO_ResetBits(LCD_BL_PORT,LCD_BL_PIN)	// 低电平，关闭背光


#endif  // __LCD_H__


