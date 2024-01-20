/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RTC global interrupt.
  */
void RTC_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_IRQn 0 */

  /* USER CODE END RTC_IRQn 0 */
  HAL_RTCEx_RTCIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_IRQn 1 */
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, hrtc.DateToUpdate.Year);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, hrtc.DateToUpdate.Month);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, hrtc.DateToUpdate.Date);
  /* USER CODE END RTC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY_WK_Pin);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY0_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY1_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */
char decode(char *s) {
    for (int i = 0; i < 26; i++) {
        if (strcmp(s, huffman_code_map[i]) == 0) {
            return 'A' + i;
        }
    }
    return '\0';
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    HAL_Delay(100);
    switch (GPIO_Pin) {
        case KEY0_Pin:
            if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET) {
//                HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                input[input_index++] = '0';
                encode_cnt = 0;
            }
            break;
        case KEY1_Pin:
            if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) {
//                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                input[input_index++] = '1';
                encode_cnt = 0;
            }
            break;
        case KEY_WK_Pin:
            if (HAL_GPIO_ReadPin(KEY_WK_GPIO_Port, KEY_WK_Pin) == GPIO_PIN_SET) {
                if (strlen(input) == 0) {
                    mode = ~mode;
                    memset(input, 0, sizeof(input));
                    memset(output, 0, sizeof(output));
                    input_index = 0;
                    output_index = 0;
                    memset(decode_input_str, 0, sizeof(decode_input_str));
                    memset(decode_output_str, 0, sizeof(decode_output_str));
                    memset(encode_input_str, 0, sizeof(encode_input_str));
                    memset(encode_output_str, 0, sizeof(encode_output_str));
                    encode_left = 0;
                    memset(encode_alphabet, 0, sizeof(encode_alphabet));
                    encode_alphabet_index = 0;
                    encode_cnt = 0;
                    memset(split_arr, 0, sizeof(split_arr));
                    decode_output_height = 140;
                    LCD_Clear(WHITE);
                } else {
                    if (mode == DECODE) {
                        output_index = 0;
                        int left = 0, right = 0;
                        int error_flag = 0;
                        for (right = 0; right < input_index; right++) {
                            char s[1024];
                            strlcpy(s, input + left, right - left + 2);
                            char ch = decode(s);
                            if (ch != '\0') {
//                                output[output_index++] = ch;

                                HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
                                HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
                                char now_time[1024];
                                sprintf(now_time, "%%%d/%d/%d %d:%d:%d%%", 2000 + GetDate.Year,
                                        GetDate.Month,
                                        GetDate.Date, GetTime.Hours, GetTime.Minutes, GetTime.Seconds);

                                char decode_res[1024];
                                sprintf(decode_res, "%s %c", now_time, ch);
//                                strlcpy(output + output_index, decode_res, strlen(decode_res) + 1);
                                if (output[output_index] == NULL) {
                                    output[output_index] = (char *) malloc(strlen(decode_res) + 1);
                                }
                                strcpy(output[output_index], decode_res);
                                output_index++;
//                                output[output_index++] = decode_res;


                                left = right + 1;
                                split_arr[right] = 1;
                                LCD_Clear(WHITE);
                                Print_Lcd();
                                LCD_ShowString(10, decode_output_height, 200, 24, 12, (uint8_t *) decode_res);
                                decode_output_height += DELTA_HEIGHT;
                                LCD_Show_Success(10, 280, 21, 100);
                                for (int i = 0; i < strlen(s); i++) {
                                    if (s[i] == '0') {
                                        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                        HAL_Delay(500);
                                        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                        HAL_Delay(500);
                                    } else {
                                        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                        HAL_Delay(500);
                                        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                        HAL_Delay(500);
                                    }
                                }
                                for (int j = 0; j < 4; j++) {
                                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                    HAL_Delay(500);
                                }
                            } else if (right == input_index - 1) {

                                error_flag = 1;
                                char decode_error_str[1024];
                                sprintf(decode_error_str, "Decode Error: %s", input + left);
//                                strlcpy(output + output_index, decode_error_str, strlen(decode_error_str) + 1);
//                                output_index += strlen(decode_error_str);
                                LCD_Clear(WHITE);
                                Print_Lcd();
                                if (output[output_index] == NULL) {
                                    output[output_index] = (char *) malloc(strlen(decode_error_str) + 1);
                                }
                                strcpy(output[output_index], decode_error_str);
                                output_index++;
//                                output[output_index++] = decode_error_str;
                                LCD_ShowString(10, decode_output_height, 200, 24, 12, (uint8_t *) decode_error_str);
                                decode_output_height += DELTA_HEIGHT;
                                LCD_Show_Failure(140, 280, 31, 100);
                            }
                        }

//                        sprintf(decode_output_str, "Output: %s", output);
//                        LCD_ShowString(10, 100, 200, 24, 12, (uint8_t *) decode_output_str);
//                        for (int i = 0; i < input_index; i++) {
//                            if (input[i] == '0') {
//                                HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//                                HAL_Delay(500);
//                                HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//                                HAL_Delay(500);
//                            } else {
//                                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                                HAL_Delay(500);
//                                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                                HAL_Delay(500);
//                            }
//                            if (split_arr[i] == 1) {
//                                for (int j = 0; j < 4; j++) {
//                                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//                                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                                    HAL_Delay(500);
//                                }
//                            }
//                        }
                        if (error_flag) {
                            for (int j = 0; j < 6; j++) {
                                HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                HAL_Delay(500);
                            }
                        }
                        memset(input, 0, sizeof(input));
                    } else {
                        if (++encode_cnt == 1) {
                            int x = 0;
                            for (int i = encode_left; i < input_index; i++) {
                                x |= (input[i] - '0');
                                x <<= 1;
                            }
                            x >>= 1;
                            encode_left = input_index;
                            encode_alphabet[encode_alphabet_index++] = x;
                        } else {
//                            char encode_tmp[1024];
//                            int encode_tmp_index = 0;
                            for (int i = 0; i < encode_alphabet_index; i++) {
                                char ch = encode_alphabet[i];
                                int idx = -1;
                                if ('A' <= ch && ch <= 'Z') {
                                    idx = ch - 'A';
                                } else if ('a' <= ch && ch <= 'z') {
                                    idx = ch - 'a';
                                } else if (ch == ' ') {
                                    continue;
                                } else {
                                    char encode_error_str[1024];
                                    sprintf(encode_error_str, "Encode Error: %c ", ch);
//                                    strlcpy(output + output_index, encode_error_str, strlen(encode_error_str) + 1);
//                                    output_index += strlen(encode_error_str);
                                    LCD_Clear(WHITE);
                                    if (output[output_index] == NULL) {
                                        output[output_index] = (char *) malloc(strlen(encode_error_str) + 1);
                                    }
                                    strcpy(output[output_index], encode_error_str);
                                    output_index++;
//                                    output[output_index++] = encode_error_str;
                                    Print_Lcd();
                                    LCD_ShowString(10, decode_output_height, 200, 24, 12,
                                                   (uint8_t *) decode_output_str);
                                    decode_output_height += DELTA_HEIGHT;
                                    LCD_Show_Failure(140, 280, 31, 100);
//                                    encode_tmp[encode_tmp_index++] = '2';
                                    for (int j = 0; j < 6; j++) {
                                        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                        HAL_Delay(500);
                                    }
                                    continue;
                                }

                                HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
                                HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
                                char now_time[1024];
                                sprintf(now_time, "%%%d/%d/%d %d:%d:%d%%", 2000 + GetDate.Year,
                                        GetDate.Month,
                                        GetDate.Date, GetTime.Hours, GetTime.Minutes, GetTime.Seconds);

                                char encode_res[1024];
                                sprintf(encode_res, "%s ", now_time);
//                                strlcpy(output + output_index, encode_res, strlen(encode_res) + 1);
//                                output_index += strlen(encode_res);

                                char *res = huffman_code_map[idx];
                                strcat(encode_res, res);
//                                for (int j = 0; j < strlen(res); j++) {
//                                    output[output_index++] = res[j];
//                                }
                                LCD_Clear(WHITE);
                                Print_Lcd();
                                if (output[output_index] == NULL) {
                                    output[output_index] = (char *) malloc(strlen(encode_res) + 1);
                                }
                                strcpy(output[output_index], encode_res);
                                output_index++;
//                                output[output_index++] = encode_res;
                                LCD_ShowString(10, decode_output_height, 200, 24, 12, (uint8_t *) encode_res);
                                decode_output_height += DELTA_HEIGHT;
                                LCD_Show_Success(10, 280, 21, 100);
//                                    encode_tmp[encode_tmp_index++] = res[j];
                                for (int j = 0; j < strlen(res); j++) {
                                    if (res[j] == '0') {
                                        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                        HAL_Delay(500);
                                        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                        HAL_Delay(500);
                                    } else {
                                        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                        HAL_Delay(500);
                                        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                        HAL_Delay(500);
                                    }
                                }
//                                output[output_index++] = ' ';
                                for (int j = 0; j < 4; j++) {
                                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                                    HAL_Delay(500);
                                }
//                                split_arr[encode_tmp_index - 1] = 1;
                                Print_Lcd();

//                            for (int i = 0; i < encode_tmp_index; i++) {
//                                if (encode_tmp[i] == '0') {
//                                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//                                    HAL_Delay(500);
//                                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//                                    HAL_Delay(500);
//                                } else if (encode_tmp[i] == '1') {
//                                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                                    HAL_Delay(500);
//                                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                                    HAL_Delay(500);
//                                } else {
//                                    for (int j = 0; j < 6; j++) {
//                                        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//                                        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                                        HAL_Delay(500);
//                                    }
//                                }
//                                if (split_arr[i] == 1) {
//                                    for (int j = 0; j < 4; j++) {
//                                        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//                                        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                                        HAL_Delay(500);
//                                    }
//                                }
                            }
                            memset(input, 0, sizeof(input));
                        }
                    }
                }
            }
            break;
    }
    Print_Lcd();
}
/* USER CODE END 1 */
