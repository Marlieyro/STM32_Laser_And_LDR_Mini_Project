/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "timer.h"
#include "dma.h"
#include "MPU6050.h"

// FPU subprocessor - ON

uint16_t adc_buff[100];
char log_buff[300];

typedef struct {
  float ax, ay, az, gx, gy, gz;
} motion6_float_data_t;

motion6_float_data_t MPU_data;
// 16bit ADC - 32767
// Get val
// For Accs X(ax,ay,az) / 16384 = float
// For Gyro X(gy, gz, gx) / 131 = float
// MPU6050_getMotion6 modifi by me!
volatile uint8_t con_test_rest;
int main(void){
  SystemPheri_Init();
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
  // HAL_ADC_Start_DMA(ADC1_Get_HandleTypeDef(),(uint32_t*)adc_buff,100);
  // HAL_TIM_Base_Start_IT(TIMER_GetHandleTypeDef());

  con_test_rest = MPU6050_testConnection();
  while (1){
    MPU6050_getMotion6(&MPU_data);
    snprintf(log_buff, sizeof(log_buff), "Acseleromtr: X - %f, Y - %f, Z - %f Gyro: X - %f Y - %f Z - %f\n", MPU_data.ax, MPU_data.ay, MPU_data.az, MPU_data.gx, MPU_data.gy, MPU_data.gz);
    HAL_UART_Transmit(UART_GetHandleTypeDef(), (uint8_t*)log_buff, sizeof(log_buff),1000);
    HAL_Delay(200);
  }

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void) {
  __disable_irq();
  while (1){

  }
}

void SystemPheri_Init() {
  HAL_Init();
  SystemClock_Config();
  DMA_General_Init();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_UART4_Init();
  GPIO_LaserModule_Init();
  TIMER3_Config();
  MPU6050_Config();
}

void MPU6050_Config() {
  I2Cdev_init(I2C1_Get_HandleTypeDef());
  MPU6050_initialize();

  // Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV) = 0x07
  MPU6050_setRate(0x07);
  MPU6050_setExternalFrameSync(MPU6050_EXT_SYNC_DISABLED);
  MPU6050_setDLPFMode(MPU6050_DLPF_BW_256);
  con_test_rest = MPU6050_testConnection();
}

void complemenary() {
  uint8_t angel_prev;
  uint8_t gyro;
  uint8_t dt;
  uint8_t alpha;
  uint8_t accel_angle = atan2f();

  uint8_t res = (angel_prev + gyro * dt) + (1 - alpha) * accel_angle;
  angel_prev = res;
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


/**
 * Temp MPU Logic
 */

