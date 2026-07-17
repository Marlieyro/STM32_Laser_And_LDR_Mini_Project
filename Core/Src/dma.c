//
// Created by user on 17.07.26.
//
#include "dma.h"

// DMA1 - I2C1_Rx Stream0 CH1
// DMA1 - I2C1_Tx Stream6 CH1

DMA_HandleTypeDef dma_handle_type_def = {0};

// Найкраще ініціалі-овувати DMA у периферії яка його використовує
void DMA_Init(void) {
    //DMA2_Stream0
    dma_handle_type_def.Instance = DMA1_Stream0;
    dma_handle_type_def.Init.Channel = DMA_CHANNEL_1;
    dma_handle_type_def.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_handle_type_def.Init.PeriphInc = DMA_PINC_DISABLE; // Не потрібно інкременту
    dma_handle_type_def.Init.MemInc = DMA_MINC_ENABLE;
    dma_handle_type_def.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // Поки що байт
    dma_handle_type_def.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; // Поки що байт
    dma_handle_type_def.Init.Mode = DMA_CIRCULAR;
    dma_handle_type_def.Init.Priority = DMA_PRIORITY_LOW; // Немає значення
    dma_handle_type_def.Init.FIFOMode = DMA_FIFOMODE_ENABLE; // буферизуємо
    dma_handle_type_def.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL; // 1/2
    dma_handle_type_def.Init.MemBurst = DMA_MBURST_INC8;
    dma_handle_type_def.Init.PeriphBurst = DMA_PBURST_SINGLE;

    __HAL_RCC_DMA1_CLK_ENABLE();

    if (HAL_DMA_Init(&dma_handle_type_def) != HAL_OK)
        Error_Handler();

    // Це лінковка з периферією
    // __HAL_LINKDMA();
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
}

