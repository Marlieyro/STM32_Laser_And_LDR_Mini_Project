/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

static I2C_HandleTypeDef hi2c1 = {0};
static DMA_HandleTypeDef dma1_handle_i2c1_tx = {0};
static DMA_HandleTypeDef dma1_handle_i2c1_rx = {0};

void MX_I2C1_Init(void) {
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK){
    Error_Handler();
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __HAL_RCC_I2C1_CLK_ENABLE();

    I2C_DMA_Init();
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle) {
  if(i2cHandle->Instance==I2C1) {
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
  }
}

// DMA1 - I2C1_Rx Stream0 CH1
// DMA1 - I2C1_Tx Stream6 CH1
static void I2C_DMA_Init(void) {

  /** ------------------------------------
   *   DMA1 I2C1_TX Stream0 Configuration
   * -----------------------------------**/
  dma1_handle_i2c1_tx.Instance = DMA1_Stream0;
  dma1_handle_i2c1_tx.Init.Channel = DMA_CHANNEL_1;
  dma1_handle_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  dma1_handle_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE; // Не потрібно інкременту
  dma1_handle_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
  dma1_handle_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // Поки що байт
  dma1_handle_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; // Поки що байт
  dma1_handle_i2c1_tx.Init.Mode = DMA_CIRCULAR;
  dma1_handle_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW; // Немає значення
  dma1_handle_i2c1_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE; // буферизуємо
  dma1_handle_i2c1_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL; // 1/2
  dma1_handle_i2c1_tx.Init.MemBurst = DMA_MBURST_INC8;
  dma1_handle_i2c1_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
  if (HAL_DMA_Init(&dma1_handle_i2c1_tx) != HAL_OK)
    Error_Handler();
 __HAL_LINKDMA(&hi2c1, hdmatx, dma1_handle_i2c1_tx);

  /** ------------------------------------
  *   DMA1 I2C1_RX Stream6 Configuration
  * -----------------------------------**/
  dma1_handle_i2c1_rx.Instance = DMA1_Stream6;
  dma1_handle_i2c1_rx.Init.Channel = DMA_CHANNEL_1;
  dma1_handle_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  dma1_handle_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE; // Не потрібно інкременту
  dma1_handle_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
  dma1_handle_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // Поки що байт
  dma1_handle_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; // Поки що байт
  dma1_handle_i2c1_rx.Init.Mode = DMA_CIRCULAR;
  dma1_handle_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW; // Немає значення
  dma1_handle_i2c1_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE; // буферизуємо
  dma1_handle_i2c1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL; // 1/2
  dma1_handle_i2c1_rx.Init.MemBurst = DMA_MBURST_INC8;
  dma1_handle_i2c1_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
  if (HAL_DMA_Init(&dma1_handle_i2c1_rx) != HAL_OK)
    Error_Handler();
  __HAL_LINKDMA(&hi2c1, hdmarx, dma1_handle_i2c1_rx);
}

I2C_HandleTypeDef* I2C1_Get_HandleTypeDef(void) {
  return &hi2c1;
}
