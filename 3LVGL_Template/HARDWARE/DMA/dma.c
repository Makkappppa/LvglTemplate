#include "dma.h"

void USART_DMA_Config(void){
    DMA_InitTypeDef DI;

    USART_DMA_CLK_CMD(USART_DMA_CLK, ENABLE);

    DMA_DeInit(USART_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_DMA_STREAM) != DISABLE){}
    // USART1 对应 DMA2 通道4 数据流7
    DI.DMA_Channel              = USART_DMA_CHANNEL;
    DI.DMA_PeripheralBaseAddr   = USART_DMA_DR_BASE;
    DI.DMA_Memory0BaseAddr      = USART_DMA_MEMORY_BASE;

    // 数据方向和数据量
    DI.DMA_DIR                  = DMA_DIR_MemoryToPeripheral;
    DI.DMA_BufferSize           = USART_DMA_BUFF_SIZE;

    // 内存地址自增
    DI.DMA_PeripheralInc        = DMA_PeripheralInc_Disable;
    DI.DMA_MemoryInc            = DMA_MemoryInc_Enable;

    // 数据单位
    DI.DMA_PeripheralDataSize   = DMA_PeripheralDataSize_Byte;
    DI.DMA_MemoryDataSize       = DMA_MemoryDataSize_Byte;

    // 模式：循环
    DI.DMA_Mode                 = DMA_Mode_Circular;
    DI.DMA_Priority             = DMA_Priority_High;

    // FIFO
    DI.DMA_FIFOMode             = DMA_FIFOMode_Disable;
    DI.DMA_FIFOThreshold        = DMA_FIFOThreshold_Full;

    // 突发传输
    DI.DMA_MemoryBurst          = DMA_MemoryBurst_Single;
    DI.DMA_PeripheralBurst      = DMA_PeripheralBurst_Single;

    DMA_Init(USART_DMA_STREAM, &DI);
    DMA_ClearFlag(USART_DMA_STREAM, DMA_FLAG_TCIF7);								// 清除DMA标志位
	
    while(DMA_GetCmdStatus(USART_DMA_STREAM) == ENABLE){}
}

