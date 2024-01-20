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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// Project driver
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "rtc.h"
#include "24l01.h"
#include "string.h"
#include "stdio.h"
#include "touch.h"

// Project file
#include "app.h"
#include "chat.h"
#include "cal.h"
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
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int username = 0, timeset = 0;

/*
 * -1 -> 上电初始�?
 *  0 -> 主页
 *  1 -> 菜单
 *  2 -> 私聊选人
 *  3 -> 聊天页面
 *  4 -> 计算�?
 *  5 -> 相册
 *  6 -> 被邀请聊天弹�?
 *  7 -> 邀请他人
 *  8 -> 小游戏
 */
int app_mode = -1;
int prev_mode = -1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;
extern uint8_t rxBuffer[20];

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
  Stm32_Clock_Init(RCC_PLL_MUL9); // 设置时钟,72M
  delay_init(72);                 // 初始化延时函�??????
  // uart_init(115200);					//初始化串�??????
  LED_Init();      // 初始化LED
  KEY_Init();      // 初始化按�??????
  LCD_Init();      // 初始化LCD
  NRF24L01_Init(); // 初始化NRF24L01
  tp_dev.init();   // 触摸屏初始化
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);

  POINT_COLOR = RED;
  LCD_ShowString(30, 50, 200, 16, 16, (u8 *)"Mini STM32");
  LCD_ShowString(30, 70, 200, 16, 16, (u8 *)"Smartwatch");
  LCD_ShowString(30, 90, 200, 16, 16, (u8 *)"OOAD Project");
  LCD_ShowString(30, 110, 200, 16, 16, (u8 *)"2023/12/29");
  while (NRF24L01_Check())
  {
    LCD_ShowString(30, 130, 200, 16, 16, (u8 *)"NRF24L01 Error");
    delay_ms(200);
    LCD_Fill(30, 130, 239, 130 + 16, WHITE);
    delay_ms(200);
  }
  LCD_ShowString(30, 130, 200, 16, 16, (u8 *)"NRF24L01 OK");
  while (RTC_Init()) // RTC初始�????,�????定要初始化成�????
  {
    LCD_ShowString(30, 150, 200, 16, 16, (u8 *)"RTC ERROR!   ");
    delay_ms(800);
    LCD_ShowString(30, 150, 200, 16, 16, (u8 *)"RTC Trying...");
  }
  LCD_ShowString(30, 150, 200, 16, 16, (u8 *)"RTC OK       ");
  delay_ms(500);
  // 程序初始化完�???

  // 用户初始化设�???
  // LCD_ShowString(30, 170, 200, 16, 16, (u8 *)"Please input in UART");
  //    HAL_UART_Transmit(&huart1, (uint8_t *)"Please enter your username:\n", 29, HAL_MAX_DELAY);
  //    while (username == 0)
  //    {
  //      delay_ms(10);
  //    }
  //    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0X5050) // 是否第一次配�?
  //    {
  //      HAL_UART_Transmit(&huart1, (uint8_t *)"Please enter the time IN yy/mm/dd/hh/mm/ss:\n", 45, HAL_MAX_DELAY);
  //    }
  //    else
  //    {
  //      timeset = 1;
  //    }
  //    while (timeset == 0)
  //    {
  //      delay_ms(10);
  //    }

  LCD_ShowString(30, 170, 200, 16, 16, (u8 *)"Please select username:");

  // 选择用户 1 2 3 在屏幕下方从0,200 开始 从左到右放三个矩形 代表1 2 3
  LCD_DrawRectangle(0, 200, 80, 320);
  LCD_DrawRectangle(80, 200, 160, 320);
  LCD_DrawRectangle(160, 200, 240, 320);

  LCD_ShowString(20, 240, 80, 16, 16, (u8 *)"1");
  LCD_ShowString(100, 240, 80, 16, 16, (u8 *)"2");
  LCD_ShowString(180, 240, 80, 16, 16, (u8 *)"3");

  RTC_Get();
  while (1)
  {
    tp_dev.scan(0);
    if (!(tp_dev.sta & TP_PRES_DOWN))
      delay_ms(10);
    if (!(tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height))
      delay_ms(10);
    if (tp_dev.x[0] < 80 && tp_dev.y[0] > 200 && tp_dev.y[0] < 320)
    {
      user_id = 1;
      break;
    }
    if (tp_dev.x[0] < 160 && tp_dev.x[0] > 80 && tp_dev.y[0] > 200 && tp_dev.y[0] < 320)
    {
      user_id = 2;
      break;
    }
    if (tp_dev.x[0] < 240 && tp_dev.x[0] > 160 && tp_dev.y[0] > 200 && tp_dev.y[0] < 320)
    {
      user_id = 3;
      break;
    }
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // test
  // user_id = 1;
//  char message[] = "hello";
//  memcpy(hist_msg[1][0].msg, message, sizeof(message));
//  hist_msg[1][0].sender = 1;
//  char message2[] = "hellos";
//  memcpy(hist_msg[1][1].msg, message2, sizeof(message2));
//  hist_msg[1][1].sender = 2;
//  char message3[] = "hellosk111111111111111";
//  memcpy(hist_msg[1][2].msg, message3, sizeof(message3));
//  hist_msg[1][2].sender = 1;
//  char message4[] = "hellosk2";
//  memcpy(hist_msg[1][3].msg, message4, sizeof(message3));
//  hist_msg[1][3].sender = 2;
//  char message5[] = "hellosk3";
//  memcpy(hist_msg[1][4].msg, message5, sizeof(message3));
//  hist_msg[1][4].sender = 1;
//  msg_num[1] = 5;
//  LCD_ShowString(30, 170, 200, 16, 16, hist_msg[1][0].msg);
//  delay_ms(2000);
  // test

  LED0 = 1;
  LED1 = 1;
  app_mode = 0;
  screen_clear();
  screen_reprint();

  send_online();

  while (1)
  {
    delay_ms(10);
    if (prev_mode != app_mode && app_mode != 6 && app_mode != 7)
    {
      screen_clear();
      screen_reprint();
      prev_mode = app_mode;
    }

    // 右上角打印app_mode
//    LCD_ShowNum(220, 0, msg_num[1], 2, 16);

    // ui绘制以及触屏逻辑
    app_ui();
    app_touch();
    try_receive_message();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */
  __HAL_RCC_BKP_CLK_ENABLE(); // �?启后备区域时�?
  __HAL_RCC_PWR_CLK_ENABLE(); // 使能电源时钟PWR
  HAL_PWR_EnableBkUpAccess(); // 取消备份区域写保�?????
  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
   */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0X5050) // 是否第一次配�?
  {

    /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date
     */
    sTime.Hours = 0x0;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;

    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }
    DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
    DateToUpdate.Month = RTC_MONTH_DECEMBER;
    DateToUpdate.Date = 0x1;
    DateToUpdate.Year = 0x23;

    if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }
    /* USER CODE BEGIN RTC_Init 2 */
  }
  /* USER CODE END RTC_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7199;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : KEY_WK_Pin */
  GPIO_InitStruct.Pin = KEY_WK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY_WK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY0_Pin */
  GPIO_InitStruct.Pin = KEY0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED0_Pin */
  GPIO_InitStruct.Pin = LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY1_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
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
