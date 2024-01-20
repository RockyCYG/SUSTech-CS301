/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
//#include "lcd_v4.h"
#include "stm32f1xx_hal_dma_ex.h"
#include <string.h>
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
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

int recv_end_flag = 0;
int rx_len = 0;

#define BUFFER_SIZE 1024
uint8_t rx_buffer[BUFFER_SIZE];           /* 发送数据缓冲区 */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_DMA_Init(void);

static void MX_USART1_UART_Init(void);
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
    MX_DMA_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */

    uint16_t i, k;
    uint16_t len;
    uint8_t mask = 0;
    float pro = 0;          /* 进度 */

    LCD_Init();
    LCD_ShowString(30, 50, 200, 16, 16, "CS301");
    LCD_ShowString(30, 70, 200, 16, 16, "DMA TEST");

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
//  while (1)
//  {
//    /* USER CODE END WHILE */
//
//	  /* USER CODE BEGIN 3 */
//	  if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)       /* KEY0按下 */
//	  {
//	      LCD_ShowString(30, 130, 200, 16, 16, "Start Transimit....");
//	      LCD_ShowString(30, 150, 200, 16, 16, "   %");    /* 显示百分号 */
////	      lcd_show_string(30, 130, 200, 16, 16, "Start Transimit....", BLUE);
////	      lcd_show_string(30, 150, 200, 16, 16, "   %", BLUE);    /* 显示百分号 */
//
//	      HAL_UART_Transmit_DMA(&huart1, g_sendbuf, SEND_BUF_SIZE);
//	      /* 等待DMA传输完成，此时CPU空闲，可以处理其他任务
//	       * 实际应用中，传输数据期间，可以执行另外的任务
//	       */
//
//	      while (1)
//	      {
//	          /* 等待 DMA1_Channel4 传输完成 */
//	          if ( __HAL_DMA_GET_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4))  /* 等待 DMA1_Channel4 传输完成 */
//	          {
//	              __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4); /* 清除 DMA1_Channel4 传输完成标志 */
//	              HAL_UART_DMAStop(&huart1);       /* 传输完成以后关闭串口DMA */
//	              break;
//	          }
//
//	          pro = __HAL_DMA_GET_COUNTER(&hdma_usart1_tx); /* 得到当前还剩余多少个数据 */
//	          len = SEND_BUF_SIZE;        /* 总长度 */
//	          pro = 1 - (pro / len);      /* 得到百分比 */
//	          pro *= 100;                 /* 扩大100倍 */
//	          LCD_ShowNum(30, 150, pro, 3, 16);
////	          lcd_show_num(30, 150, pro, 3, 16, BLUE);
//	      }
//	      LCD_ShowNum(30, 150, 100, 3, 16);    /* 显示100% */
//	      LCD_ShowString(30, 130, 200, 16, 16, "Transimit Finished!"); /* 提示传送完成 */
////	      lcd_show_num(30, 150, 100, 3, 16, BLUE);    /* 显示100% */
////	      lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE); /* 提示传送完成 */
//	  }
//  }

    while (1) {
        if (recv_end_flag) {
            recv_end_flag = 0;
            HAL_UART_Transmit_DMA(&huart1, rx_buffer, BUFFER_SIZE);
            LCD_ShowString(30, 110, 200, 16, 16, "The data is: ");
            LCD_ShowString(30, 130, 200, 16, 16, rx_buffer);
            memset(rx_buffer, 0, rx_len);
            rx_len = 0;
        }
        HAL_UART_Receive_DMA(&huart1, rx_buffer, BUFFER_SIZE);
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
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void) {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, rx_buffer, BUFFER_SIZE);
    /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) {

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin : KEY0_Pin */
    GPIO_InitStruct.Pin = KEY0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_GPIO_Port, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
