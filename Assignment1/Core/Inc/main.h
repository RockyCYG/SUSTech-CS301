/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern int mode;
extern char input[1024];
extern char *output[1024];
extern int input_index;
extern int output_index;
extern char decode_input_str[1024];
extern char decode_output_str[1024];
extern char encode_input_str[1024];
extern int encode_left;
extern char encode_output_str[1024];
extern char encode_alphabet[1024];
extern int encode_alphabet_index;
extern int encode_cnt;
extern int split_arr[1024];
extern int decode_output_height;

extern char *huffman_code_map[26];

extern RTC_DateTypeDef GetDate;
extern RTC_TimeTypeDef GetTime;
extern RTC_HandleTypeDef hrtc;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define DELTA_HEIGHT 20
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Print_Lcd(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_WK_Pin GPIO_PIN_0
#define KEY_WK_GPIO_Port GPIOA
#define KEY_WK_EXTI_IRQn EXTI0_IRQn
#define KEY0_Pin GPIO_PIN_5
#define KEY0_GPIO_Port GPIOC
#define KEY0_EXTI_IRQn EXTI9_5_IRQn
#define LED0_Pin GPIO_PIN_8
#define LED0_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_15
#define KEY1_GPIO_Port GPIOA
#define KEY1_EXTI_IRQn EXTI15_10_IRQn
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
#define DECODE 0
#define ENCODE 1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
