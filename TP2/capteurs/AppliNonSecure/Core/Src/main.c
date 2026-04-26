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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include "hts221_reg.h"
#include "lis2dw12_reg.h"
#include "lis2mdl_reg.h"
#include "lps22hh_reg.h"
#include "lsm6dso_reg.h"
#include "stts751_reg.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
static void NS_LED_InitAndOn(void);
static void check_sensors_whoami(void);
void stts751_read_data_polling(void);
void hts221_read_data_polling(void);
void lis2dw12_read_data_polling(void);
void lis2mdl_read_data_simple(void);
void lps22hh_read_data_polling(void);
void lsm6dso_read_data_polling(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  NS_LED_InitAndOn();
  
  /* Check sensor connectivity before starting main loop */
  HAL_Delay(500);
  check_sensors_whoami();
  HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    printf("STTS751 : \r\n");
    stts751_read_data_polling();
    HAL_Delay(1000);

    printf("HTS221 : \r\n");
    hts221_read_data_polling();
    HAL_Delay(1000);

    printf("LIS2DW12 : \r\n");
    lis2dw12_read_data_polling();
    HAL_Delay(1000);

    printf("LIS2MDL : \r\n");
    lis2mdl_read_data_simple();
    HAL_Delay(1000);

    printf("LPS22HH : \r\n");
    lps22hh_read_data_polling();
    HAL_Delay(1000);

    printf("LSM6DSO : \r\n");
    lsm6dso_read_data_polling();
    HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10C0ECFF;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART1;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */

  /* Configure I2C1 pins */
  /* I2C1_SDA: PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* I2C1_SCL: PH9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

int __io_putchar(int ch)
{
  uint8_t c = (uint8_t)ch;
  (void)HAL_UART_Transmit(&huart2, &c, 1U, HAL_MAX_DELAY);
  return ch;
}

static void NS_LED_InitAndOn(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOG_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_8 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* On NUCLEO-N657X0-Q LEDs are active low. */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0 | GPIO_PIN_8 | GPIO_PIN_10, GPIO_PIN_RESET);
}

static void check_sensors_whoami(void)
{
  printf("\r\n========== Sensor Detection Check ==========\r\n");
  
  /* HTS221 Check - WHO_AM_I at 0x0F */
  {
    uint8_t whoami = 0;
    HAL_I2C_Mem_Read(&hi2c1, HTS221_I2C_ADDRESS, HTS221_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &whoami, 1, 1000);
    printf("HTS221 (Add: 0x%02X) - Expected: 0xBC, Got: 0x%02X - %s\r\n", 
           HTS221_I2C_ADDRESS, whoami, (whoami == HTS221_ID) ? "OK" : "FAIL");
  }

  /* LIS2DW12 Check - WHO_AM_I at 0x0F, uses ADD_H */
  {
    uint8_t whoami = 0;
    HAL_I2C_Mem_Read(&hi2c1, LIS2DW12_I2C_ADD_H, LIS2DW12_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &whoami, 1, 1000);
    printf("LIS2DW12 (Add: 0x%02X) - Expected: 0x44, Got: 0x%02X - %s\r\n",
           LIS2DW12_I2C_ADD_H, whoami, (whoami == LIS2DW12_ID) ? "OK" : "FAIL");
  }

  /* LIS2MDL Check - WHO_AM_I at 0x4F (NOT 0x0F) */
  {
    uint8_t whoami = 0;
    HAL_I2C_Mem_Read(&hi2c1, LIS2MDL_I2C_ADD, LIS2MDL_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &whoami, 1, 1000);
    printf("LIS2MDL (Add: 0x%02X) - Expected: 0x40, Got: 0x%02X - %s\r\n",
           LIS2MDL_I2C_ADD, whoami, (whoami == LIS2MDL_ID) ? "OK" : "FAIL");
  }

  /* LPS22HH Check - WHO_AM_I at 0x0F, uses ADD_H */
  {
    uint8_t whoami = 0;
    HAL_I2C_Mem_Read(&hi2c1, LPS22HH_I2C_ADD_H, LPS22HH_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &whoami, 1, 1000);
    printf("LPS22HH (Add: 0x%02X) - Expected: 0xB3, Got: 0x%02X - %s\r\n",
           LPS22HH_I2C_ADD_H, whoami, (whoami == LPS22HH_ID) ? "OK" : "FAIL");
  }

  /* LSM6DSO Check - WHO_AM_I at 0x0F, uses ADD_H */
  {
    uint8_t whoami = 0;
    HAL_I2C_Mem_Read(&hi2c1, LSM6DSO_I2C_ADD_H, LSM6DSO_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &whoami, 1, 1000);
    printf("LSM6DSO (Add: 0x%02X) - Expected: 0x6C, Got: 0x%02X - %s\r\n",
           LSM6DSO_I2C_ADD_H, whoami, (whoami == LSM6DSO_ID) ? "OK" : "FAIL");
  }

  /* STTS751 Check - PRODUCT_ID at 0xFD (NOT 0xFE which is MANUFACTURER_ID) */
  {
    uint8_t whoami = 0;
    HAL_I2C_Mem_Read(&hi2c1, STTS751_1xxxx_ADD_7K5, STTS751_PRODUCT_ID, I2C_MEMADD_SIZE_8BIT, &whoami, 1, 1000);
    printf("STTS751 (Add: 0x%02X) - Product ID, Got: 0x%02X\r\n",
           STTS751_1xxxx_ADD_7K5, whoami);
  }

  printf("==========================================\r\n\r\n");
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
