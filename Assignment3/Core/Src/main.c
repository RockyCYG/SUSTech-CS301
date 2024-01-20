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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "delay.h"
//#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
//#include "usmart.h"
#include "touch.h"
#include "24cxx.h"
#include "24l01.h" //通信驱动 基于spi进行通信
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int state_num = 0;
u8 STATE[30];
unsigned char DATA_TO_SEND[800];
extern UART_HandleTypeDef huart1;
// 0: 初始界面 1: 滑块验证�? 2:
int mode = 0;
RTC_DateTypeDef GetDate;
RTC_TimeTypeDef GetTime;
unsigned char *cur_picture;
int origin_mx, origin_my;
int x, y, mx, my;
int sx = 0, sy = 230 - BLOCK_SIZE / 2;
int time_cnt = 0;
extern TIM_HandleTypeDef htim3;
#define LETTER_SIZE 4
char letters[LETTER_SIZE + 1];
int pos[LETTER_SIZE][2];
const char chinese_characters[] = "南方科技大学";
//const char inclines[16]={0,0,0,0,0,0,0,0,0,0,'0','1','2','a','b','c'};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//清空屏幕并在右上角显�???????"RST"
void Load_Drow_Dialog(void) {
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = BLUE;//设置字体为蓝�???????
    LCD_ShowString(lcddev.width - 24, 0, 200, 16, 16, "RST");//显示清屏区域
    POINT_COLOR = RED;//设置画笔蓝色
}

////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部�???????
//画水平线
//x0,y0:坐标
//len:线长�???????
//color:颜色
void gui_draw_hline(u16 x0, u16 y0, u16 len, u16 color) {
    if (len == 0)return;
    LCD_Fill(x0, y0, x0 + len - 1, y0, color);
}

//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0, u16 y0, u16 r, u16 color) {
    u32 i;
    u32 imax = ((u32) r * 707) / 1000 + 1;
    u32 sqmax = (u32) r * (u32) r + (u32) r / 2;
    u32 x = r;
    gui_draw_hline(x0 - r, y0, 2 * r, color);
    for (i = 1; i <= imax; i++) {
        if ((i * i + x * x) > sqmax)// draw lines from outside
        {
            if (x > imax) {
                gui_draw_hline(x0 - i + 1, y0 + x, 2 * (i - 1), color);
                gui_draw_hline(x0 - i + 1, y0 - x, 2 * (i - 1), color);
            }
            x--;
        }
        // draw lines from inside (center)
        gui_draw_hline(x0 - x, y0 + i, 2 * x, color);
        gui_draw_hline(x0 - x, y0 - i, 2 * x, color);
    }
}

//两个数之差的绝对�???????
//x1,x2：需取差值的两个�???????
//返回值：|x1-x2|
u16 my_abs(u16 x1, u16 x2) {
    if (x1 > x2)return x1 - x2;
    else return x2 - x1;
}
//画一条粗�???????
//(x1,y1),(x2,y2):线条的起始坐�???????
//size：线条的粗细程度
//color：线条的颜色

void screen_print() {
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = BLUE;//设置字体为蓝�??????
    LCD_ShowString(lcddev.width - 24, 0, 200, 16, 16, "RST");//显示清屏区域
    LCD_ShowString(0, 0, 200, 24, 24, "SHOW PICTURE");
    LCD_ShowString(60, 60, 200, 24, 24, "SEND MESSAGE");
    LCD_ShowString(0, lcddev.height - 24, 200, 16, 16, STATE);
    POINT_COLOR = RED;//设置画笔为红�??????

}


void screen_norm_print() {
//	LCD_Clear(WHITE);//清屏
    POINT_COLOR = BLUE;//设置字体为蓝�??????
    LCD_ShowString(lcddev.width - 24, 0, 200, 16, 16, "RST");//显示清屏区域
    LCD_ShowString(0, 0, 200, 24, 24, "SHOW PICTURE");
    LCD_ShowString(60, 60, 200, 24, 24, "SEND MESSAGE");
    LCD_ShowString(0, lcddev.height - 24, 200, 16, 16, STATE);
    POINT_COLOR = RED;//设置画笔为红�??????

}

void change_state() {
    if (state_num == 0) {
        state_num = 1;
        sprintf(STATE, "STATE: ON");
    } else {
        state_num = 0;
        sprintf(STATE, "STATE: OFF");
    }
}

void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color) {
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    if (x1 < size || x2 < size || y1 < size || y2 < size)return;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)incx = 1; //设置单步方向
    else if (delta_x == 0)incx = 0;//垂直�???????
    else {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0;//水平�???????
    else {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标�???????
    else distance = delta_y;
    for (t = 0; t <= distance + 1; t++)//画线输出
    {
        gui_fill_circle(uRow, uCol, size, color);//画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//5个触控点的颜�???????(电容触摸屏用)
const u16 POINT_COLOR_TBL[5] = {RED, GREEN, BLUE, BROWN, GRED};

//电阻触摸屏测试函�???????
void rtp_test(void) {
    u8 key;
    u8 i = 0;
    while (1) {
        key = KEY_Scan(0);
        tp_dev.scan(0);
        if (tp_dev.sta & TP_PRES_DOWN)            //触摸屏被按下
        {
            if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
                if (tp_dev.x[0] > (lcddev.width - 24) && tp_dev.y[0] < 16) {

                } else if (tp_dev.x[0] > sx && tp_dev.x[0] < sx + BLOCK_SIZE && tp_dev.y[0] > sy &&
                           tp_dev.y[0] < sy + BLOCK_SIZE) {
                    LCD_Fill(sx, sy, sx + BLOCK_SIZE, sy + BLOCK_SIZE, BLUE);
                } else {
                    sx = 0, sy = 230 - BLOCK_SIZE / 2;
                }
//                else if (tp_dev.x[0] < 80 && tp_dev.y[0] < 24) {
//                    LCD_ShowImage2(40,80);
//                } else if (tp_dev.x[0] > 60 && tp_dev.y[0] > 60 && tp_dev.x[0] < 180 && tp_dev.y[0] < 100) {
//                    sprintf(DATA_TO_SEND, "SEND DATA");
//                    HAL_UART_Transmit(&huart1, (uint8_t *) DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);
//                } else if (tp_dev.x[0] > 0 && tp_dev.y[0] > lcddev.height - 24 && tp_dev.x[0] < 80 &&
//                           tp_dev.y[0] < lcddev.height) {
//                    change_state();
//                } else {
//                    TP_Draw_Big_Point(tp_dev.x[0], tp_dev.y[0], RED);//画图
//                }
            }
        } else delay_ms(10);    //没有按键按下的时�???????
        if (key == KEY0_PRES)    //KEY0按下,则执行校准程�???????
        {
            LCD_Clear(WHITE);    //清屏
            TP_Adjust();        //屏幕校准
            TP_Save_Adjdata();
            Load_Drow_Dialog();
        }
    }
}

//电容触摸屏测试函�???????
void ctp_test(void) {
    u8 t = 0;
    u8 i = 0;
    u16 lastpos[5][2];        //�???????后一次的数据
    while (1) {
        tp_dev.scan(0);
        for (t = 0; t < 5; t++) {
            if ((tp_dev.sta) & (1 << t)) {
                //printf("X坐标:%d,Y坐标:%d\r\n",tp_dev.x[0],tp_dev.y[0]);
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height) {
                    if (lastpos[t][0] == 0XFFFF) {
                        lastpos[t][0] = tp_dev.x[t];
                        lastpos[t][1] = tp_dev.y[t];
                    }

                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], 2, POINT_COLOR_TBL[t]);//画线
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];
                    if (tp_dev.x[t] > (lcddev.width - 24) && tp_dev.y[t] < 20) {
                        Load_Drow_Dialog();//清除
                    }
                }
            } else lastpos[t][0] = 0XFFFF;
        }

        delay_ms(5);
        i++;
        if (i % 20 == 0)LED0 = !LED0;
    }
}
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
#include "lcd.h"
#include "picture.h"
#include <time.h>

int min(int x, int y) {
    return x < y ? x : y;
}

int max(int x, int y) {
    return x > y ? x : y;
}

char generate_random_character() {
    char res;
    int val = rand() % 80;
    if (val < 26) {
        res = 'A' + val;
    } else if (val < 52) {
        res = 'a' + val - 26;
    } else if (val < 62) {
        res = '0' + val - 52;
    } else if (val < 68) {
        res = val - 62 + 1;
    } else if (val < 74) {
        res = 10 + val - 68;
    } else {
        res = 20 + val - 74;
    }
    return res;
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
    LCD_Init();

    Stm32_Clock_Init(RCC_PLL_MUL9);    //设置时钟,72M
    delay_init(72);                    //初始化延时函�???????
//	uart_init(115200);					//初始化串�???????
//	usmart_dev.init(84); 		  	  	//初始化USMART
    LED_Init();                            //初始化LED
    KEY_Init();                            //初始化按�???????
    LCD_Init();                            //初始化LCD
    tp_dev.init();                        //触摸屏初始化
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_RTC_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
//  MX_SPI1_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim3);

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

    BACK_COLOR = WHITE;
    POINT_COLOR = RED;


    if (tp_dev.touchtype & 0X80) {
        ctp_test();
    }
        // 电阻
    else {
        u8 key;
        u8 i = 0;
        while (1) {
            LCD_Clear(WHITE);

            if (mode == 0) {
                POINT_COLOR = RED;
                LCD_ShowString(30, 260, 240, 30, 24, (uint8_t *) "Go to verify");
                while (1) {
                    key = KEY_Scan(0);
                    tp_dev.scan(0);
                    if (tp_dev.sta & TP_PRES_DOWN)            //触摸屏被按下
                    {
                        if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
                            if (tp_dev.x[0] > 30 && tp_dev.x[0] < 30 + 240 && tp_dev.y[0] > 260 &&
                                tp_dev.y[0] < 260 + 30) {
                                mode = 1;
                                break;
                            }
                        }
                    } else {
                        delay_ms(10);
                    }
                }
            } else if (mode == 1) {
                POINT_COLOR = RED;
                cur_picture = picture1;
//    LCD_DrawRectangle(200, 200, 260, 230);
                LCD_Fill(0, 230 - BLOCK_SIZE / 2, 240, 230 + BLOCK_SIZE / 2, GRAY);
                LCD_Fill(0, 230 - BLOCK_SIZE / 2, BLOCK_SIZE, 230 + BLOCK_SIZE / 2, BLUE);
                x = rand() % (120 - BLOCK_SIZE) + 120;
                y = rand() % (180 - BLOCK_SIZE);
//                x = 110;
//                y = 90;
                sx = 0, sy = 230 - BLOCK_SIZE / 2;
                origin_mx = mx = rand() % (x - BLOCK_SIZE);
                origin_my = my = y;
                LCD_ShowPicture(0, 0, 240, 180, (uint16_t *) cur_picture);
                LCD_Fill(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE, GRAY);
                LCD_ShowMissingPicture(x, y, mx, my, 240, 180, (uint16_t *) cur_picture);

                int verify = 0;
                time_cnt = 0;
                while (1) {
                    key = KEY_Scan(0);
                    tp_dev.scan(0);

                    if (tp_dev.sta & TP_PRES_DOWN)            //触摸屏被按下
                    {
                        if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
                            if (tp_dev.x[0] > sx && tp_dev.x[0] < sx + BLOCK_SIZE && tp_dev.y[0] > sy &&
                                tp_dev.y[0] < sy + BLOCK_SIZE) {

                                while (1) {
                                    tp_dev.scan(0);
                                    if (tp_dev.sta & TP_PRES_DOWN) {
                                        sx = tp_dev.x[0], sy = tp_dev.y[0];
//                                LCD_Fill(0, 230 - BLOCK_SIZE / 2, 240, 230 + BLOCK_SIZE / 2, WHITE);
                                        LCD_Fill(0, 230 - BLOCK_SIZE / 2, 240, 230 + BLOCK_SIZE / 2, GRAY);
                                        LCD_Fill(min(sx, 240 - BLOCK_SIZE), 230 - BLOCK_SIZE / 2,
                                                 min(sx, 240 - BLOCK_SIZE) + BLOCK_SIZE, 230 + BLOCK_SIZE / 2, BLUE);
                                        mx = origin_mx + sx;
                                        mx = min(mx, 240 - BLOCK_SIZE);
                                        LCD_ShowPicture(0, 0, 240, 180, (uint16_t *) cur_picture);
                                        LCD_Fill(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE, GRAY);
                                        LCD_ShowMissingPicture(x, y, mx, my, 240, 180, (uint16_t *) cur_picture);
                                    } else {
                                        // 验证成功
                                        if (abs(x - mx) >= 0.01 * BLOCK_SIZE && abs(x - mx) <= 0.1 * BLOCK_SIZE &&
                                            time_cnt >= 1 && time_cnt <= 50) {
                                            LCD_ShowPicture(0, 0, 240, 180, (uint16_t *) cur_picture);
                                            LCD_ShowString(30, 260, 240, 30, 24, (uint8_t *) "Verify success!");
                                            while (1) {
                                                tp_dev.scan(0);
                                                if (tp_dev.sta & TP_PRES_DOWN)            //触摸屏被按下
                                                {
                                                    if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
                                                        if (tp_dev.x[0] > 30 && tp_dev.x[0] < 30 + 240 &&
                                                            tp_dev.y[0] > 260 &&
                                                            tp_dev.y[0] < 260 + 30) {
                                                            mode = 2;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        } else {
                                            time_cnt = 0;
                                            mode = 0;
                                        }
                                        verify = 1;
                                        break;
//                                        sx = 0, sy = 230 - BLOCK_SIZE / 2;
//                                        mx = origin_mx, my = origin_my;
//                                        LCD_Fill(0, 230 - BLOCK_SIZE / 2, 240, 230 + BLOCK_SIZE / 2, GRAY);
//                                        LCD_Fill(sx, 230 - BLOCK_SIZE / 2, sx + BLOCK_SIZE, 230 + BLOCK_SIZE / 2, BLUE);
//                                        LCD_ShowPicture(0, 0, 240, 180, (uint16_t *) cur_picture);
//                                        LCD_Fill(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE, GRAY);
//                                        LCD_ShowMissingPicture(x, y, mx, my, 240, 180, (uint16_t *) cur_picture);
//                                        break;
                                    }
                                }
                            } else {

                            }
                        }
                    } else {
                        delay_ms(10);
                    }   //没有按键按下的时�???????
                    if (verify) {
                        break;
                    }
                }
            } else if (mode == 2) {
                POINT_COLOR = RED;
                LCD_DrawRectangle(30, 20, 200, 100);
                LCD_Fill(60, 150, 170, 190, BLUE);
                LCD_ShowString(70, 160, 100, 100, 24, (uint8_t *) "Refresh");
                POINT_COLOR = BLACK;
                memset(letters, 0, sizeof(letters));
                for (int i = 0; i < LETTER_SIZE; i++) {
                    while (1) {
                        int flag = 1;
                        letters[i] = generate_random_character();
                        for (int j = 0; j < i; j++) {
                            if (letters[i] == letters[j]) {
                                flag = 0;
                            }
                        }
                        if (flag) {
                            break;
                        }
                    }
                    while (1) {
                        int x = rand() % 140 + 30, y = rand() % 50 + 20;
                        int flag = 1;
                        for (int j = 0; j < i; j++) {
                            if (abs(x - pos[j][0]) < 15 && abs(y - pos[j][1]) < 15) {
                                flag = 0;
                                break;
                            }
                        }
                        if (flag) {
                            pos[i][0] = x, pos[i][1] = y;
                            break;
                        }
                    }

                    if (letters[i] >= ' ') {
                        LCD_ShowChar(pos[i][0], pos[i][1], letters[i], 24, 0);
                    } else if (letters[i] < 10) {
                        LCD_ShowChinese(pos[i][0], pos[i][1], letters[i] - 1, 24, BLACK, WHITE, 0);
                    } else if (letters[i] < 20) {
                        LCD_ShowInclineChar(pos[i][0], pos[i][1], letters[i] - 10, 24, BLACK, WHITE, 0);
                    } else {
                        LCD_ShowVariantChar(pos[i][0], pos[i][1], letters[i] - 20, 24, BLACK, WHITE, 0);
                    }
                }
                // 生成点
                for (int i = 0; i < 100; i++) {
                    int x = rand() % 170 + 30, y = rand() % 80 + 20;
                    LCD_DrawPoint(x, y);
                }

                // 生成随机折线
                for (int i = 0; i < 3; i++) {
                    int x1 = rand() % 170 + 30, y1 = rand() % 80 + 20;
                    int x2 = rand() % 170 + 30, y2 = rand() % 80 + 20;
                    x1 = max(x1, 30);
                    x1 = min(x1, 200);
                    x2 = max(x2, 30);
                    x2 = min(x2, 200);
                    y1 = max(y1, 20);
                    y1 = min(y1, 100);
                    y2 = max(y2, 20);
                    y2 = min(y2, 100);
                    LCD_DrawLine(x1, y1, x2, y2);
                }

                // 生成波浪线
                for (int i = 0; i < 5; i++) {
                    int cx = rand() % 30 + 30, cy = rand() % 50 + 50;
                    for (int i = 0; i < 5; i++) {
                        int dis = rand() % 20 + 1;
                        if (i % 2) {
                            LCD_DrawLine(cx, cy, cx + dis, cy + dis);
                            cx += dis, cy += dis;
                            cx = max(cx, 30);
                            cx = min(cx, 200);
                            cy = max(cy, 20);
                            cy = min(cy, 100);
                        } else {
                            LCD_DrawLine(cx, cy, cx + dis, cy - dis);
                            cx += dis, cy -= dis;
                            cx = max(cx, 30);
                            cx = min(cx, 200);
                            cy = max(cy, 20);
                            cy = min(cy, 100);
                        }
                    }
                }
//                char message[1024] = "Press [";
                char message[1024] = {'[', '\0'};
                for (int i = 0; i < LETTER_SIZE; i++) {
                    int len = strlen(message);

                    message[len++] = letters[i];
                    if (i < LETTER_SIZE - 1) {
                        message[len++] = ',';
                        message[len++] = ' ';
                    }
                }
                message[strlen(message)] = ']';
//                strcat(message, " in order");
                LCD_ShowString(30, 220, 240, 30, 24, (uint8_t *) message);

                char seq[LETTER_SIZE + 1];
                memset(seq, 0, sizeof(seq));
                int flags[LETTER_SIZE];
                memset(flags, 0, sizeof(flags));
                int idx = 0;
                int verify = 0;
                while (1) {
                    if (verify) {
                        break;
                    }
                    tp_dev.scan(0);
                    if (tp_dev.sta & TP_PRES_DOWN)            //触摸屏被按下
                    {
                        if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
                            // 刷新
                            if (tp_dev.x[0] > 70 && tp_dev.x[0] < 70 + 100 && tp_dev.y[0] > 160 &&
                                tp_dev.y[0] < 160 + 100) {
                                break;
                            } else {
                                for (int i = 0; i < LETTER_SIZE; i++) {
                                    int x = pos[i][0], y = pos[i][1];
                                    if (tp_dev.x[0] >= x && tp_dev.x[0] <= x + 15 && tp_dev.y[0] >= y &&
                                        tp_dev.y[0] <= y + 15) {
                                        if (flags[i]) {
                                            break;
                                        }
                                        POINT_COLOR = RED;
                                        if (letters[i] >= ' ') {
                                            LCD_ShowChar(pos[i][0], pos[i][1], letters[i], 24, 0);
                                        } else if (letters[i] < 10) {
                                            LCD_ShowChinese(pos[i][0], pos[i][1], letters[i] - 1, 24, POINT_COLOR,
                                                            BACK_COLOR, 24);
                                        } else if (letters[i] < 20) {
                                            LCD_ShowInclineChar(pos[i][0], pos[i][1], letters[i] - 10, 24, POINT_COLOR,
                                                                BACK_COLOR, 0);
                                        } else {
                                            LCD_ShowVariantChar(pos[i][0], pos[i][1], letters[i] - 20, 24, POINT_COLOR,
                                                                BACK_COLOR, 0);
                                        }
                                        seq[idx++] = letters[i];
                                        flags[i] = 1;
                                    }
                                }
                            }
                        }
                        if (idx == LETTER_SIZE) {
                            // 验证成功
                            if (strcmp(letters, seq) == 0) {
                                verify = 1;
                                LCD_ShowString(30, 260, 240, 30, 24, (uint8_t *) "Verify success!");
                                while (1) {
                                    tp_dev.scan(0);
                                    if (tp_dev.sta & TP_PRES_DOWN)            //触摸屏被按下
                                    {
                                        if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
                                            if (tp_dev.x[0] > 30 && tp_dev.x[0] < 30 + 240 &&
                                                tp_dev.y[0] > 260 &&
                                                tp_dev.y[0] < 260 + 30) {
                                                mode = 0;
                                                break;
                                            }
                                        }
                                    }
                                }
                            } else {
                                break;
                            }
                        }
                    } else {
                        delay_ms(10);
                    }
                }
            }


            if (key == KEY0_PRES)    //KEY0按下,则执行校准程�???????
            {
                LCD_Clear(WHITE);    //清屏
                TP_Adjust();        //屏幕校准
                TP_Save_Adjdata();
                Load_Drow_Dialog();
            }
        }
    }
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */


//    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//
//
//    }
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
