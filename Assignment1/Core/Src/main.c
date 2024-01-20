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
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
int mode = 0;
char input[1024];
char *output[1024];
int input_index = 0;
int output_index = 0;
char decode_input_str[1024];
char decode_output_str[1024];
char encode_input_str[1024];
char encode_output_str[1024];
int encode_left = 0;
char encode_alphabet[1024];
int encode_alphabet_index = 0;
int encode_cnt = 0;
int split_arr[1024];
int decode_output_height = 140;

char *huffman_code_map[26] = {"000", "101001", "00101", "11011", "011", "01000", "001001", "0011", "1100", "0010011",
                              "0010000", "11010", "01001", "1011", "1110", "101011", "0010001001", "0101", "1000",
                              "1111", "10011", "101000", "101010", "001000101", "10010", "0010001000"};

RTC_DateTypeDef GetDate;
RTC_TimeTypeDef GetTime;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Print_Lcd() {
//    LCD_Clear(WHITE);
    char mode_str[20];
    sprintf(mode_str, "HEDM Mode: %s", mode == DECODE ? "DECODE" : "ENCODE");
    LCD_ShowString(10, 10, 200, 24, 24, (uint8_t *) mode_str);
    if (mode == DECODE) {
        sprintf(decode_input_str, "Input: %s", input);
        LCD_ShowString(10, 60, 200, 24, 12, (uint8_t *) decode_input_str);
//        sprintf(decode_output_str, "Output: %s", output);
//        LCD_ShowString(10, 100, 200, 24, 12, (uint8_t *) decode_output_str);
        LCD_ShowString(10, 120, 200, 24, 12, (uint8_t *) "Output:");
    } else {
        sprintf(encode_input_str, "Input: %s", input);
        LCD_ShowString(10, 60, 200, 24, 12, (uint8_t *) encode_input_str);
//        sprintf(encode_output_str, "Output: %s", output);
//        LCD_ShowString(10, 120, 200, 24, 12, (uint8_t *) encode_output_str);
        LCD_ShowString(10, 120, 200, 24, 12, (uint8_t *) "Output:");
        char alphabet_output[1024];
        sprintf(alphabet_output, "Input Alphabet: %s", encode_alphabet);
        LCD_ShowString(10, 100, 200, 24, 12, (uint8_t *) alphabet_output);
    }
    for (int i = 0; i < output_index; i++) {
        LCD_ShowString(10, 140 + i * DELTA_HEIGHT, 200, 24, 12, (uint8_t *) output[i]);
    }
//    LCD_ShowString(10, 200, 200, 24, 12, (uint8_t *) encode_output_str);
//    HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
//    HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
//    char now_time[1024];
//    sprintf(now_time, " %%%d/%d/%d %d:%d:%d%%", 2000 + GetDate.Year,
//            GetDate.Month,
//            GetDate.Date, GetTime.Hours, GetTime.Minutes, GetTime.Seconds);
//    LCD_ShowString(10, 240, 200, 24, 12, (uint8_t *) now_time);
//    LCD_ShowImage(10, 280, 21, 100, (uint8_t *) gImage_success);
//    LCD_ShowImage(210, 280, 31, 100, (uint8_t *) gImage_failure);
//    LCD_Show_Success(10, 280, 21, 100);
//    LCD_Show_Failure(140, 280, 31, 100);
}

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
    LCD_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        Print_Lcd();
        HAL_Delay(100);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
