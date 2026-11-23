/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <string.h>
#include "functions_ge2.h"

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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

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

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Habilitación del contador de ciclos (DWT->CYCCNT)
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;   // Habilita el bloque de traza (DWT)
  DWT->CTRL |= 1 << DWT_CTRL_CYCCNTENA_Pos;         // Habilita el contador de ciclos

  /* ----------- Ejercicio 1 ----------- */
  int16_t lat_C[LONGITUD_EJ1], long_C[LONGITUD_EJ1];
  int16_t lat_ASM[LONGITUD_EJ1], long_ASM[LONGITUD_EJ1];
  uint32_t ciclos_C_ej1, ciclos_ASM_ej1;

  DWT->CYCCNT = 0;
  GPS_unpack((int32_t *)GPS_data, lat_C, long_C, LONGITUD_EJ1);
  ciclos_C_ej1 = DWT->CYCCNT;

  DWT->CYCCNT = 0;
  asm_GPS_unpack((int32_t *)GPS_data, lat_ASM, long_ASM, LONGITUD_EJ1);
  ciclos_ASM_ej1 = DWT->CYCCNT;
  // Esperado lat:  0, 10, 100, 1000, 2000, -2048, -1, 1, -64, 291
  // Esperado long: 0, 10, 100, 200, 2000, -2048, 1, -1, 64, -872
  /* ----------- Ejercicio 1 ----------- */

  /* ----------- Ejercicio 2 ----------- */
  uint32_t indice_C, indice_ASM, indiceVacio_C, indiceVacio_ASM;
  uint32_t ciclos_C_ej2, ciclos_ASM_ej2;

  DWT->CYCCNT = 0;
  indice_C = max((int32_t *)vectorIn_ej3, LONGITUD_EJ3);
  ciclos_C_ej2 = DWT->CYCCNT;

  DWT->CYCCNT = 0;
  indice_ASM = asm_max((int32_t *)vectorIn_ej3, LONGITUD_EJ3);
  ciclos_ASM_ej2 = DWT->CYCCNT;
  // Esperado: 9 (el máximo 912 aparece en los índices 9 y 12; se devuelve el primero)

  indiceVacio_C = max((int32_t *)vectorIn_ej3, 0);
  indiceVacio_ASM = asm_max((int32_t *)vectorIn_ej3, 0);
  // Esperado: 0 (no hay muestras)
  /* ----------- Ejercicio 2 ----------- */

  /* ----------- Ejercicio 3 ----------- */
  // Los vectores del .h son const (están en flash): se copian a RAM para poder invertirlos
  uint16_t bufPar_C[LONGITUD_EJ4_PAR], bufPar_ASM[LONGITUD_EJ4_PAR];
  uint16_t bufImpar_C[LONGITUD_EJ4_IMPAR], bufImpar_ASM[LONGITUD_EJ4_IMPAR];
  uint32_t ciclos_C_ej3, ciclos_ASM_ej3;

  memcpy(bufPar_C, buffer_ej4_in_par, sizeof(buffer_ej4_in_par));
  memcpy(bufPar_ASM, buffer_ej4_in_par, sizeof(buffer_ej4_in_par));
  memcpy(bufImpar_C, buffer_ej4_in_impar, sizeof(buffer_ej4_in_impar));
  memcpy(bufImpar_ASM, buffer_ej4_in_impar, sizeof(buffer_ej4_in_impar));

  DWT->CYCCNT = 0;
  invertir(bufPar_C, LONGITUD_EJ4_PAR);
  ciclos_C_ej3 = DWT->CYCCNT;

  DWT->CYCCNT = 0;
  asm_invertir(bufPar_ASM, LONGITUD_EJ4_PAR);
  ciclos_ASM_ej3 = DWT->CYCCNT;
  // Esperado: 1200, 1100, 1000, ..., 200, 100

  invertir(bufImpar_C, LONGITUD_EJ4_IMPAR);
  asm_invertir(bufImpar_ASM, LONGITUD_EJ4_IMPAR);
  // Esperado: 130, 120, ..., 20, 10 (el 70 queda en el centro)
  /* ----------- Ejercicio 3 ----------- */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
