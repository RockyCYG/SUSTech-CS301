/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "lcd.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
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
    MX_ADC1_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    LCD_Init();
    uint16_t raw;
    char msg[20];
    double vol;
    HAL_UART_Transmit(&huart1, "Start\r\n", 7, HAL_MAX_DELAY);
//    LCD_ShowString(30, 110, 200, 16, 16, "The voltage is: ");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
//    HAL_ADC_Start(&hadc1);
//    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
// Wait for regular group conversion to be completed
//        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//        raw = HAL_ADC_GetValue(&hadc1); // Get ADC value
//        vol = (double) raw * (3.3 / 4096); // the voltage should be raw * (3.3/4096)(12 bits)
//        sprintf(msg, "Voltage = %f\r\n", vol); // Convert to string and print
//        HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
//        int adcBuf[3];
//        for (int i = 0; i < 3; i++) {
//            HAL_ADC_Start(&hadc1);
//            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//            adcBuf[i] = HAL_ADC_GetValue(&hadc1);
////            sprintf(msg, "ch:%d %d\r\n", i, adcBuf[i]);
//            vol = (double)raw * (3.3/4096); // the voltage should be raw * (3.3/4096)(12 bits)
//            sprintf(msg, "ch:%d Voltage = %f\r\n", (i + 5), vol); // Convert to string and print
//            HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
//            LCD_ShowString(30, 110 + i * 20, 200, 16, 16, msg);
//        }
//        HAL_Delay(500);

//    }

    HAL_UART_Transmit(&huart1, "Start\r\n", 7, HAL_MAX_DELAY);
    HAL_ADC_Start(&hadc1);
    while (1) {
        for (int i = 0; i < 3; i++) {
            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
            raw = HAL_ADC_GetValue(&hadc1);
            vol = (double) raw * (3.3 / 4096); // the voltage should be raw * (3.3/4096)(12 bits)
            sprintf(msg, "ch:%d %f\r\n", i + 5, vol);
            HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
            LCD_ShowString(30, 110 + i * 20, 200, 16, 16, msg);
        }
        HAL_Delay(500);
    }

    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
