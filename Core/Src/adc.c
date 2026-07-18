/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

#include "i2c.h"

static ADC_HandleTypeDef hadc1 = {0};
static DMA_HandleTypeDef dma2_handle_adc1 = {0};

void MX_ADC1_Init(void){
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
    Error_Handler();
  }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1){

    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
  if(adcHandle->Instance==ADC1)
  {
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

  }
}

// ADC1 - DMA2 Stream0 Channel0
void ADC_DMA_Init(void) {
  dma2_handle_adc1.Instance = DMA2_Stream0;
  dma2_handle_adc1.Init.Channel = DMA_CHANNEL_0;
  dma2_handle_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  dma2_handle_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  dma2_handle_adc1.Init.MemInc = DMA_MINC_ENABLE;
  dma2_handle_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  dma2_handle_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  dma2_handle_adc1.Init.Mode = DMA_CIRCULAR;
  dma2_handle_adc1.Init.Priority = DMA_PRIORITY_LOW;
  dma2_handle_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  if (HAL_DMA_Init(&dma2_handle_adc1) != HAL_OK)
    Error_Handler();

  __HAL_LINKDMA(&hadc1, DMA_Handle,dma2_handle_adc1);
}

ADC_HandleTypeDef* ADC1_Get_HandleTypeDef(void) {
  return &hadc1;
}