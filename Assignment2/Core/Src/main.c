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
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <time.h>
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
extern UART_HandleTypeDef huart1;
extern uint8_t rxBuffer[20];
extern unsigned char uRx_Data[1024];
extern unsigned char uLength;
extern TIM_HandleTypeDef htim3;
char question[1024];
int qid = 0;
int num1 = 0;
int num2 = 0;
int op = 0;
int tim_cnt = 0;
int game_flag = 0;

RTC_DateTypeDef GetDate;
RTC_TimeTypeDef GetTime;

char get_op(int op) {
    switch (op) {
        case 0:
            return '+';
        case 1:
            return '-';
        case 2:
            return '*';
        case 3:
            return '/';
        default:
            return '\0';
    }
}

int cal(int num1, int num2, int op) {
    switch (op) {
        case 0:
            return num1 + num2;
        case 1:
            return num1 - num2;
        case 2:
            return num1 * num2;
        case 3:
            return num1 / num2;
        default:
            return 0;
    }
}

void generate_question() {
    op = rand() % 4;
    switch (op) {
        case 0:
            do {
                num1 = rand() % 100;
                num2 = rand() % 100;
            } while (num1 + num2 > 100);
            break;
        case 1:
            do {
                num1 = rand() % 100;
                num2 = rand() % 100;
            } while (num1 - num2 < 0);
            break;
        case 2:
            num1 = rand() % 100;
            num2 = rand() % 100;
            break;
        case 3:
            do {
                num1 = rand() % 100;
                num2 = rand() % 100;
            } while (num2 == 0 || num1 % num2 > 0);
            break;
        default:
            break;
    }
    sprintf(question, "%d%c%d=?\n", num1, get_op(op), num2);
}

void start_new_game() {
    game_flag = 1;
    tim_cnt = 0;
    HAL_UART_Transmit(&huart1, (uint8_t *) "Start Game\n", strlen("Start Game\n"), HAL_MAX_DELAY);
    generate_question();
    HAL_UART_Transmit(&huart1, (uint8_t *) question, strlen(question), HAL_MAX_DELAY);
}
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
    MX_USART1_UART_Init();
    MX_IWDG_Init();
    MX_TIM3_Init();
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    struct tm tm_new;
    HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
    tm_new.tm_sec = GetTime.Seconds;
    tm_new.tm_min = GetTime.Minutes;
    tm_new.tm_hour = GetTime.Hours;
    tm_new.tm_mday = GetDate.Date;
    tm_new.tm_mon = GetDate.Month - 1;
    tm_new.tm_year = GetDate.Year + 100;
    srand((unsigned int) mktime(&tm_new));
    HAL_UART_Receive_IT(&huart1, (uint8_t *) rxBuffer, 1);
    HAL_TIM_Base_Start_IT(&htim3);
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET) {
        HAL_UART_Transmit(&huart1, (uint8_t *) "[INFO]Time out. Game over. Please press KEY0 to try again.\n",
                          strlen("[INFO]Time out. Game over. Please press KEY0 to try again.\n"), HAL_MAX_DELAY);
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        HAL_Delay(100);
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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
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
