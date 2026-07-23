//
// Created by user on 17.07.26.
//
#include "dma.h"

// DMA1 - I2C1_Rx Stream0 CH1
// DMA1 - I2C1_Tx Stream6 CH1

void DMA_General_Init(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
}


