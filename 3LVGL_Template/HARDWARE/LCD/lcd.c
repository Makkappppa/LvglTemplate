#include "lcd.h"

__LCD LCDdev;
/*----------------------------------------------- 类型定义 -------------------------------------------*/

void __LCD_GPIO_Init(void){
	GPIO_InitTypeDef GI;
	// GPIO 时钟使能
	LCD_SCK_CLK_CMD(LCD_SCK_CLK, ENABLE);
	LCD_SDA_CLK_CMD(LCD_SDA_CLK, ENABLE);
	LCD_CS_CLK_CMD	(LCD_CS_CLK, ENABLE);
	LCD_DC_CLK_CMD	(LCD_DC_CLK, ENABLE);
	LCD_BL_CLK_CMD	(LCD_BL_CLK, ENABLE);
	
	// 开启复用
	GPIO_PinAFConfig(LCD_SCK_PORT, LCD_SCK_PinSource,LCD_SCK_AF );
	GPIO_PinAFConfig(LCD_SDA_PORT, LCD_SDA_PinSource,LCD_SDA_AF );

	GI.GPIO_Mode 	= GPIO_Mode_AF;
	GI.GPIO_Speed 	= GPIO_Speed_100MHz;
	GI.GPIO_OType 	= GPIO_OType_PP;
	GI.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	// SCK
	GI.GPIO_Pin		= LCD_SCK_PIN;
	GPIO_Init(LCD_SCK_PORT, &GI);
	// SDA
	GI.GPIO_Pin 	= LCD_SDA_PIN;
	GPIO_Init(LCD_SDA_PORT, &GI);
	// CS
	GI.GPIO_Mode 	= GPIO_Mode_OUT;
	GI.GPIO_Pin 	= LCD_CS_PIN;
	GPIO_Init(LCD_CS_PORT, &GI);
	// DC
	GI.GPIO_Pin 	= LCD_DC_PIN;
	GPIO_Init(LCD_CS_PORT, &GI);
	// BL背光
	GI.GPIO_Pin 	= LCD_BL_PIN;
	GPIO_Init(LCD_CS_PORT, &GI);
	LCD_DC_Data();
	LCD_CS_H();
	LCD_BL_OFF();
}
void __LCD_SPI_Init(void){
	SPI_InitTypeDef SI;

	LCD_SPI_CLK_CMD(LCD_SPI_CLK, ENABLE);

	SI.SPI_Direction 	= SPI_Direction_1Line_Tx;  	// SPI单线发送	
	SI.SPI_Mode 		= SPI_Mode_Master;			// 主机
	SI.SPI_DataSize 	= SPI_DataSize_8b;			// 8位数据宽度
	SI.SPI_CPOL 		= SPI_CPOL_Low;				// SCLK时钟 空闲为低电平
	SI.SPI_CPHA			= SPI_CPHA_1Edge;			// 奇数跳变沿有效
	SI.SPI_NSS 			= SPI_NSS_Soft;				// 软件控制片选信号
	// SPI3 挂载在APB1总线，总线时钟42MHz	
	// 设置为2分频，得到21MHz的SPI驱动时钟
	SI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	// 2分频
	SI.SPI_FirstBit 		 = SPI_FirstBit_MSB;		// 先发送高位
	SPI_Init(LCD_SPI, &SI);
	SPI_Cmd(LCD_SPI, ENABLE);
}
/*----------------------------------------------- 基础函数 -------------------------------------------*/

void LCD_Write_Command(u8 cmd){
	while((LCD_SPI->SR & SPI_I2S_FLAG_BSY) != RESET){} // 等待SPI空闲
	LCD_DC_Command();
	LCD_SPI->DR = cmd;
	
	while( (LCD_SPI->SR & SPI_I2S_FLAG_TXE) == RESET){}		// 等待发送缓冲区清空
	while( (LCD_SPI->SR & SPI_I2S_FLAG_BSY) != RESET){}	//	等待通信完成
	LCD_DC_Data();
}
void LCD_Write_Data8(uint8_t data){
	LCD_SPI->DR = data;
	while((LCD_SPI->SR & SPI_I2S_FLAG_TXE) == RESET){}
}
void LCD_Write_Data16(uint16_t data){
	LCD_SPI->DR = data>>8;			// 发送数据，高8位
	while((LCD_SPI->SR & SPI_I2S_FLAG_TXE) == RESET){}
	LCD_SPI->DR = data;				// 发送数据，低8位
	while((LCD_SPI->SR & SPI_I2S_FLAG_TXE) == RESET){}
}
void LCD_SetRegion(uint16_t xS, uint16_t xE, uint16_t yS, uint16_t yE){
	LCD_CS_L();
	LCD_Write_Command(LCDdev.SetXcmd);	// 列地址 X
	LCD_Write_Data16(xS);
	LCD_Write_Data16(xE);
	LCD_Write_Command(LCDdev.SetYcmd);	// 行地址 Y
	LCD_Write_Data16(yS);
	LCD_Write_Data16(yE);

	LCD_Write_Command(LCDdev.WRAMcmd);
	while((LCD_SPI->SR & SPI_I2S_FLAG_BSY) != RESET); //	等待通信完成
	LCD_CS_H();	// 片选拉高		
}
uint16_t LCD_RGB2U16(uint32_t c){
    uint16_t  r, g, b, rgb;
    b =  (uint16_t)((c & 0x00F80000)>>8);   // 转换成 16位 的RGB565颜色
    g =  (uint16_t)((c & 0x0000FC00)>>5);
    r =  (uint16_t)((c & 0x000000F8)>>3);
    rgb = r|g|b;
    return (rgb);
}

void LCD_Init(void){
	__LCD_GPIO_Init();
	__LCD_SPI_Init();

	for (int j = 5; j > 0; j--){
		for (int i = 5000; i > 0; i--){}
		}
	LCD_CS_L();	// 片选拉低，使能IC
 	LCD_Write_Command(0x36);   	// 显存访问控制 指令，用于设置访问显存的方式
	LCD_Write_Data8(0x00); 	// 配置成 从上到下、从左到右，RGB像素格式

	LCD_Write_Command(0x3A);		// 接口像素格式 指令，用于设置使用 12位、16位还是18位色
	LCD_Write_Data8(0x05);  	// 此处配置成 16位 像素格式(RGB 5-6-5-bit input)
	// 接下来很多都是电压设置指令，直接使用厂家给设定值
 	LCD_Write_Command(0xB2);			
	LCD_Write_Data8(0x0C);
	LCD_Write_Data8(0x0C); 
	LCD_Write_Data8(0x00); 
	LCD_Write_Data8(0x33); 
	LCD_Write_Data8(0x33); 			

	LCD_Write_Command(0xB7);		// 栅极电压设置指令	
	LCD_Write_Data8(0x35); 		// VGH = 13.26V，VGL = -10.43V

	LCD_Write_Command(0xBB);		// 公共电压设置指令
	LCD_Write_Data8(0x19); 		// VCOM = 1.35V

	LCD_Write_Command(0xC0);
	LCD_Write_Data8(0x2C);

	LCD_Write_Command(0xC2);   	// VDV 和 VRH 来源设置
	LCD_Write_Data8(0x01); 		// VDV 和 VRH 由用户自由配置

	LCD_Write_Command(0xC3);		// VRH电压 设置指令  
	LCD_Write_Data8(0x12); 		// VRH电压 = 4.6+( vcom+vcom offset+vdv)
				
	LCD_Write_Command(0xC4);		// VDV电压 设置指令	
	LCD_Write_Data8(0x20); 		// VDV电压 = 0v

	LCD_Write_Command(0xC6); 	// 正常模式的帧率控制指令
	LCD_Write_Data8(0x03); 		// 设置屏幕控制器的刷新帧率为99帧    

	LCD_Write_Command(0xD0);		// 电源控制指令
	LCD_Write_Data8(0xA4); 		// 无效数据，固定写入0xA4
	LCD_Write_Data8(0xA1); 		// AVDD = 6.8V ，AVDD = -4.8V ，VDS = 2.3V

	LCD_Write_Command(0xE0);  	// 正极电压伽马值设定
	LCD_Write_Data8(0xD0);
	LCD_Write_Data8(0x04);
	LCD_Write_Data8(0x0D);
	LCD_Write_Data8(0x11);
	LCD_Write_Data8(0x13);
	LCD_Write_Data8(0x2B);
	LCD_Write_Data8(0x3F);
	LCD_Write_Data8(0x54);
	LCD_Write_Data8(0x4C);
	LCD_Write_Data8(0x18);
	LCD_Write_Data8(0x0D);
	LCD_Write_Data8(0x0B);
	LCD_Write_Data8(0x1F);
	LCD_Write_Data8(0x23);

	LCD_Write_Command(0xE1);      // 负极电压伽马值设定
	LCD_Write_Data8(0xD0);
	LCD_Write_Data8(0x04);
	LCD_Write_Data8(0x0C);
	LCD_Write_Data8(0x11);
	LCD_Write_Data8(0x13);
	LCD_Write_Data8(0x2C);
	LCD_Write_Data8(0x3F);
	LCD_Write_Data8(0x44);
	LCD_Write_Data8(0x51);
	LCD_Write_Data8(0x2F);
	LCD_Write_Data8(0x1F);
	LCD_Write_Data8(0x1F);
	LCD_Write_Data8(0x20);
	LCD_Write_Data8(0x23);

	LCD_Write_Command(0x21);       // 打开反显，因为面板是常黑型，操作需要反过来

 	// 退出休眠指令，LCD控制器在刚上电、复位时，会自动进入休眠模式 ，因此操作屏幕之前，需要退出休眠  
	LCD_Write_Command(0x11);       // 退出休眠 指令
	for (int j = 120; j > 0; j--){
		for (int i = 5000; i > 0; i--){}
		}				
	// 需要等待120ms，让电源电压和时钟电路稳定下来
	
 	// 打开显示指令，LCD控制器在刚上电、复位时，会自动关闭显示 
	LCD_DisplayOn();       // 打开显示  	

	while((LCD_SPI->SR & SPI_I2S_FLAG_BSY) != RESET){} //	等待通信完成
	LCD_CS_H();	// 片选拉高		

	// 以下进行一些驱动的默认设置
	LCD_Write_Command(0x36);    		// 显存访问控制 指令，用于设置访问显存的方式
	LCD_Write_Data8(0x00);        // 垂直显示
		
	LCDdev.Width = LCD_Width, LCDdev.Height = LCD_Height;
	LCDdev.BackColor = LCD_RGB2U16(0x000000);		// 设置背景色
	LCDdev.Color = LCD_RGB2U16(0xFFFFFF);
	LCDdev.SetXcmd = 0x2a, LCDdev.SetYcmd = 0x2b, LCDdev.WRAMcmd = 0x2c;
	LCD_Clear();                           // 清屏

	// 全部设置完毕之后，打开背光	
	LCD_BL_ON();  // 引脚输出高电平点亮背光
}    

void LCD_Fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* color){
	uint32_t i;
	
	LCD_SetRegion(x, x+width-1 ,y ,y+height-1);			//设置坐标	
	
	LCD_SPI->CR1 &= 0xFFBF;				// 关闭SPI
	LCD_SPI->CR1 |= SPI_DataSize_16b;	// 切换成16位数据格式
	LCD_SPI->CR1 |= 0x0040;				// 使能SPI
	
	LCD_CS_L();	// 片选拉低，使能IC
	
	for(i = 0; i < width * height; i++)				
	{

		LCD_SPI->DR = color[i];

		while ( (LCD_SPI->SR & SPI_I2S_FLAG_TXE) == 0);	

	}
	while((LCD_SPI->SR & SPI_I2S_FLAG_BSY) != RESET); //	等待通信完成
	LCD_CS_H();	// 片选拉高	
	
	LCD_SPI->CR1 &= 0xFFBF;	// 关闭SPI
	LCD_SPI->CR1 &= 0xF7FF;	// 切换成8位数据格式
	LCD_SPI->CR1 |= 0x0040;	// 使能SPI
}

void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetRegion(x,x,y,y);	
	
	LCD_CS_L();	

	LCD_Write_Data16(color)	;
	
	while((LCD_SPI->SR & SPI_I2S_FLAG_BSY) != RESET); 
	LCD_CS_H();
} 

void LCD_Clear(void)
{
	uint32_t i;

	LCD_SetRegion(0,LCDdev.Width-1,0,LCDdev.Height-1);		

   LCD_SPI->CR1 &= 0xFFBF;					
   LCD_SPI->CR1 |= SPI_DataSize_16b;	
   LCD_SPI->CR1 |= 0x0040;					
	
	LCD_CS_L();	
	
	for(i=0;i<LCDdev.Width*LCDdev.Height;i++)				
	{

		LCD_SPI->DR = LCDdev.BackColor;
		while ( (LCD_SPI->SR & SPI_I2S_FLAG_TXE) == 0);	

	}
	while((LCD_SPI->SR & SPI_I2S_FLAG_BSY) != RESET); //	µÈ´ýÍ¨ÐÅÍê³É
	LCD_CS_H();	// Æ¬Ñ¡À­¸ß	
	
	LCD_SPI->CR1 &= 0xFFBF;	// ¹Ø±ÕSPI
   LCD_SPI->CR1 &= 0xF7FF;	// ÇÐ»»³É8Î»Êý¾Ý¸ñÊ½
   LCD_SPI->CR1 |= 0x0040;	// Ê¹ÄÜSPI
}

