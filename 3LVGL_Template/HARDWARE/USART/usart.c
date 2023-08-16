
#include "usart.h"  

uint8_t USART_Buffer[USART_BUFFER_SIZE];
uint16_t USART_Cnt = 0;

/* ****************初始化相关****************** */

static void NVIC_Config(void){
	NVIC_InitTypeDef NI;
	NI.NVIC_IRQChannel 						= USART_IRQ;
	NI.NVIC_IRQChannelPreemptionPriority 	= 0;
	NI.NVIC_IRQChannelSubPriority			= 8;
	NI.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NI);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

// 函数：usart IO口初始化
//
void  USART_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_GPIO_CLK_Cmd( USART_TX_CLK|USART_RX_CLK, ENABLE); 	//IO口时钟配置

	//IO复用，复用到USART1
	GPIO_PinAFConfig(USART_TX_PORT,USART_TX_AF,GPIO_AF_USART1); 
	GPIO_PinAFConfig(USART_RX_PORT,USART_RX_AF,GPIO_AF_USART1);	

	//IO配置
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   	//推挽
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  		//复用模式
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	//速度等级

	//初始化 TX	引脚
	GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;	 
	GPIO_Init(USART_TX_PORT, &GPIO_InitStructure);	
	//初始化 RX 引脚													   
	GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;	
	GPIO_Init(USART_RX_PORT, &GPIO_InitStructure);		
	
}

// 函数：USART 口初始化
void USART_Config(void)
{		
	USART_InitTypeDef USART_InitStructure;
	USART_CLK_Cmd(USART_CLK, ENABLE);
	
	// IO口初始化
	USART_GPIO_Config();
		 
	// 配置串口各项参数
	USART_InitStructure.USART_BaudRate		= USART_BR; //波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b; //数据位8位
	USART_InitStructure.USART_StopBits		= USART_StopBits_1; //停止位1位
	USART_InitStructure.USART_Parity		= USART_Parity_No ; //无校验
	USART_InitStructure.USART_Mode			= USART_Mode_Rx | USART_Mode_Tx; //发送和接收模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用硬件流控制
	
	USART_Init(USART1,&USART_InitStructure); //初始化串口1

	NVIC_Config();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART1,ENABLE);	//使能串口1
}







/* ****************发送函数****************** */

void USART_SendByte(USART_TypeDef* pUSART, uint8_t ch){
	USART_SendData(pUSART, ch);
	while(USART_GetFlagStatus(pUSART,USART_FLAG_TXE) == RESET){}
}
void USART_SendString(USART_TypeDef* pUSART, uint8_t* string){
	uint32_t cnt = 0;
	do
	{
		/* code */
		USART_SendByte(pUSART,*(string+cnt));
	} while (*(string + cnt) != '\0');
}

/* ****************C库重定义****************** */
// 函数：重定义fputc函数
int fputc(int c, FILE *fp)
{

	USART_SendData( USART1,(u8)c );	// 发送单字节数据
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完毕 

	return (c); //返回字符
}
int fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);	//等待发送完毕 

	return (int)USART_ReceiveData(USART1); //返回字符
}

/* ****************中断处理****************** */
void USART_IRQHandler(void){
	uint8_t data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data = USART_ReceiveData(USART1);
		USART_Buffer[USART_Cnt++] = data;
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		if( USART_Cnt != 0){
			USART_Buffer[USART_Cnt++] = 0;
			USART_Buffer[USART_Cnt++] = 0x0a;
			USART_Buffer[USART_Cnt] = 0x0d;
			printf("%s\r\n", USART_Buffer);
			USART_Cnt = 0;
		}
		data = USART1->SR;
		data = USART1->DR;
	}
}

