#include "string.h"
#include "stdio.h"

#include "main.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "rtc.h"
#include "24l01.h"
#include "touch.h"
#include "img_enter.h"

// Project file
#include "app.h"
#include "chat.h"
#include "cal.h"
#include "game.h"

extern UART_HandleTypeDef huart1;

int chat_offset = 0;
history_msg display_msg[4];

void screen_clear()
{
    LCD_Clear(WHITE); // 清屏
}

void screen_reprint()
{
    if (app_mode == 0)
    {
        POINT_COLOR = BLACK;
        LCD_ShowString(30, 50, 200, 36, 24, (u8 *)"Smartwatch APP");
        char w_user[50];
        sprintf(w_user, "Welcome, User %d", user_id);
        LCD_ShowString(30, 90, 200, 36, 24, (u8 *)w_user);
        POINT_COLOR = BLUE; // 设置字体为蓝色
        LCD_ShowString(30, 130, 200, 16, 16, (u8 *)"    -  -     ");
        LCD_ShowString(30, 166, 200, 16, 16, (u8 *)"  :  :  ");
        LCD_DrawRectangle(130, 130, 220, 182);
        LCD_ShowString(140, 148, 90, 16, 16, (u8 *)"Reset");

        LCD_DrawRectangle(30, 200, 220, 300);
        LCD_ShowString(60, 240, 180, 16, 24, (u8 *)"Enter Menu");
    }
    else if (app_mode == 1)
    {
        // 240x320 屏幕上方留80像素显示时间 下面切成4个区域 每个区域120x120

        // 1. 顶部显示时间
        LCD_ShowString(10, 20, 200, 16, 16, (u8 *)"    -  -     ");
        LCD_ShowString(100, 20, 200, 16, 16, (u8 *)"  :  ");

        // 显示四个区域
        LCD_DrawRectangle(0, 80, 120, 200);
        LCD_DrawRectangle(120, 80, 240, 200);
        LCD_DrawRectangle(0, 200, 120, 320);
        LCD_DrawRectangle(120, 200, 240, 320);

        // 显示四个区域的内容
        LCD_ShowString(20, 100, 60, 32, 16, (u8 *)"Private Chat");
        LCD_ShowString(140, 100, 45, 32, 16, (u8 *)"Group Chat");
        LCD_ShowString(20, 220, 80, 16, 16, (u8 *)"Calculator");
        LCD_ShowString(140, 220, 60, 16, 16, (u8 *)"Photo");

        // 显示游戏
        POINT_COLOR = RED;
        LCD_DrawRectangle(180, 50, 240, 80);
        LCD_ShowString(195, 57, 60, 16, 16, (u8 *)"Game");
        POINT_COLOR = BLUE;
    }
    else if (app_mode == 2)
    {
        int line = 0;
        // 显示在线用户,一列80像素高 名字显示在中间
        for (int i = 0; i < 9; i++)
        {
            if (status[i] == 1)
            {
                char online_mess[50];
                sprintf(online_mess, "User %d is online", i);
                LCD_ShowString(20, 80 * line + 25, 240, 16, 16, (u8 *)online_mess);
                LCD_DrawRectangle(0, 80 * line, 240, 80 * line + 80);

                // 右侧放个图片 gImage_enter
                LCD_ShowPicture(160, 80 * line + 10, 80, 55, (uint16_t *)gImage_enter);

                line++;
            }
        }
    }
    else if (app_mode == 3)
    {
        // 上方显示聊天对象,80像素高
        char online_mess[50];
        if(talking_user==0){
            sprintf(online_mess, "Talk to: Group Chat");
        }
        else{
            sprintf(online_mess, "Talk to: User %d", talking_user);
        }
        LCD_ShowString(40, 30, 240, 16, 16, (u8 *)online_mess);
        LCD_DrawRectangle(0, 0, 240, 80);


        // 从y=80像素开始绘制聊天记录
        //  高从80-320一共显示 4条聊天记录 每条80像素高 先画框 然后居中靠左画一个圆形 写对面的id或者我自己(Me)
        //  然后在居中画一个框,里面写消息内容 消息内容强制要求12字符一行,超过12字符换行,最多不会超过3行
        //  最后留出右边从(160,80)到(240,320)的区域用于两个按钮 上下切换聊天记录

        int cnt = msg_num[talking_user] - 1;
        int i = 3;
        cnt -= chat_offset;

        while (cnt >= 0 && i >= 0)
        {
            display_msg[i] = hist_msg[talking_user][cnt];
            cnt--;
            i--;
        }
        cnt = msg_num[talking_user] - 1 - chat_offset - 4;
        for (int i = 0; i < 4; i++)
        {
            cnt++;
            if (cnt < 0)
                continue;
            LCD_DrawRectangle(0, 60 * i + 80, 160, 60 * i + 140); // 绘制聊天记录框

            // 绘制圆形和ID

            LCD_Draw_Circle(30, 60 * i + 110, 20); // 绘制圆形
            if (display_msg[i].sender != user_id)
            {
                char id[10];
                sprintf(id, "%d", display_msg[i].sender);                // 获取对方的ID
                LCD_ShowString(27, 60 * i + 100, 160, 16, 16, (u8 *)id); // 显示对方的ID
            }
            else
            {
                LCD_ShowString(23, 60 * i + 100, 160, 16, 16, (u8 *)"Me"); // 显示自己的ID
            }

            // 显示消息内容,msg=-1和-2的时候展示gImage_happy和gImage_sad
            // if(display_msg[i].msg == -1){
            //     LCD_ShowPicture(180, 60 * i + 100, 60, 60, gImage_happy);
            // }
            // else if(display_msg[i].msg == -2){
            //     LCD_ShowPicture(180, 60 * i + 100, 60, 60, gImage_sad);
            // }
            // else{
            if (display_msg[i].msg[0] == '.'){ // 该消息是emoji
            	int emoji_id = display_msg[i].msg[1] - '0';
            	if (emoji_id == 1)
                    LCD_ShowPicture(70, 60 * i + 87, 26, 24, (u8 *)gImage_emoji1);	// emoji1
            	else if (emoji_id == 2)
                    LCD_ShowPicture(70, 60 * i + 87, 26, 25, (u8 *)gImage_emoji2);	// emoji2
            	else if (emoji_id == 3)
                    LCD_ShowPicture(70, 60 * i + 87, 24, 24, (u8 *)gImage_emoji3);	// emoji3
            	else if (emoji_id == 4)
                    LCD_ShowPicture(70, 60 * i + 87, 25, 24, (u8 *)gImage_emoji4);	// emoji4
            }else
            	LCD_ShowString(70, 60 * i + 87, 80, 48, 16, (u8 *)display_msg[i].msg);
            // }
        }

        // 绘制按钮
        LCD_DrawRectangle(160, 80, 240, 160);               // 上按钮
        LCD_DrawRectangle(160, 240, 240, 320);              // 下按钮
        LCD_ShowString(180, 100, 80, 16, 16, (u8 *)"Up");   // 上按钮文本
        LCD_ShowString(180, 260, 80, 16, 16, (u8 *)"Down"); // 下按钮文本

        LCD_ShowPicture(167, 167, 26, 24, (u8 *)gImage_emoji1);	// emoji1
        LCD_ShowPicture(207, 167, 26, 25, (u8 *)gImage_emoji2);	// emoji2
        LCD_ShowPicture(167, 207, 24, 24, (u8 *)gImage_emoji3);	// emoji3
        LCD_ShowPicture(207, 207, 25, 24, (u8 *)gImage_emoji4);	// emoji4
    }
    else if (app_mode == 4)
    {
        LCD_Clear(WHITE);
        CAL_Init_Re();
    }
    else if (app_mode == 5)
    {
    }
    else if (app_mode == 6)
    {
        // 无操作
    }
    else if (app_mode == 7)
    {
        // 无操作
    }
    else if (app_mode == 8)
    {
    	LCD_Clear(WHITE);//清屏
		POINT_COLOR=BLUE;//设置字体为蓝色
		LCD_ShowString(15,140,200,24,16, "Please select your weapon");
		LCD_ShowPicture(34,222,35,35,scissors);
		LCD_ShowPicture(103,222,35,35,rock);
		LCD_ShowPicture(172,222,35,35,paper);
    }
}

void screen_reprint_6()
{
    LCD_Fill(0, 0, 240, 80, WHITE);
    // x=160处画条线 左边是accept,右边是reject
    POINT_COLOR = BLACK;
    LCD_DrawRectangle(0, 40, 240, 80);
    LCD_DrawLine(120, 40, 120, 80);
    // 拼接邀请提示
    char invite_mess[50];
    sprintf(invite_mess, "User %d invites you to chat  ", sender);
    LCD_ShowString(10, 10, 220, 16, 16, (u8 *)invite_mess);
    POINT_COLOR = GREEN;
    LCD_ShowString(30, 50, 200, 16, 16, (u8 *)"Accept");
    POINT_COLOR = RED;
    LCD_ShowString(150, 50, 200, 16, 16, (u8 *)"Reject");
    POINT_COLOR = BLUE;
}

void screen_reprint_7()
{
    POINT_COLOR = BLACK;
    // 画面居中显示"Waiting"
    LCD_ShowString(80, 148, 90, 16, 16, (u8 *)"Waiting");
    // 给他套个框
    LCD_DrawRectangle(70, 130, 160, 182);
    POINT_COLOR = BLUE;
}

u16 t = 0;

void app_ui()
{
    if (app_mode == 0)
    {
        // 显示时间
        RTC_Get();
        if (t != calendar.sec)
        {
            t = calendar.sec;
            LCD_ShowNum(30, 130, calendar.w_year, 4, 16);
            LCD_ShowNum(70, 130, calendar.w_month, 2, 16);
            LCD_ShowNum(94, 130, calendar.w_date, 2, 16);

            RTC_LCD_ShowWeek(30, 148);

            LCD_ShowNum(30, 166, calendar.hour, 2, 16);
            LCD_ShowNum(54, 166, calendar.min, 2, 16);
            LCD_ShowNum(78, 166, calendar.sec, 2, 16);
            LED0 = !LED0;
        }
    }
    else if (app_mode == 1)
    {
        RTC_Get();
        LCD_ShowNum(10, 20, calendar.w_year, 4, 16);
        LCD_ShowNum(50, 20, calendar.w_month, 2, 16);
        LCD_ShowNum(74, 20, calendar.w_date, 2, 16);
        LCD_ShowNum(100, 20, calendar.hour, 2, 16);
        LCD_ShowNum(124, 20, calendar.min, 2, 16);
    }
    else if (app_mode == 2)
    {
        // 无操作
    }
    else if (app_mode == 3)
    {
    }
    else if (app_mode == 4)
    {
    }
    else if (app_mode == 5)
    {
    }
    else if (app_mode == 6)
    {
        // 无操作
    }
}

void app_touch()
{
    tp_dev.scan(0);
    if (!(tp_dev.sta & TP_PRES_DOWN)) // 触摸屏被按下
    {
        delay_ms(10);
        return;
    }
    if (!(tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height))
    {
        delay_ms(10);
        return;
    }
    if (app_mode == 0)
    {
        // 两个按钮 一个用于reset时间,一个用于进入菜单

        // reset 140, 130, 220, 182
        if (tp_dev.x[0] > 140 && tp_dev.y[0] > 130 && tp_dev.x[0] < 220 && tp_dev.y[0] < 182)
        {
            int mode = app_mode;
            app_mode = -1;
            LCD_ShowString(140, 148, 90, 16, 16, (u8 *)"Waiting");
            HAL_UART_Transmit(&huart1, (uint8_t *)"Please enter the time IN yy/mm/dd/hh/mm/ss:\n", 45, HAL_MAX_DELAY);
            timeset = 0;
            while (timeset == 0)
            {
                delay_ms(10);
            }
            app_mode = mode;
            screen_clear();
            screen_reprint();
        }

        // enter menu 30, 200, 220, 300
        if (tp_dev.x[0] > 30 && tp_dev.y[0] > 200 && tp_dev.x[0] < 220 && tp_dev.y[0] < 300)
        {
            app_mode = 1;
        }
    }
    else if (app_mode == 1)
    {
        // 4个区域
        if (tp_dev.x[0] > 0 && tp_dev.y[0] > 80 && tp_dev.x[0] < 120 && tp_dev.y[0] < 200)
        {
            screen_clear();
            screen_reprint();
            app_mode = 2;
            talking_user = -1;
        }
        if (tp_dev.x[0] > 120 && tp_dev.y[0] > 80 && tp_dev.x[0] < 240 && tp_dev.y[0] < 200)
        {
            app_mode = 3;
            talking_user = 0;
        }
        if (tp_dev.x[0] > 0 && tp_dev.y[0] > 200 && tp_dev.x[0] < 120 && tp_dev.y[0] < 320)
        {
            app_mode = 4;
        	LCD_Clear(WHITE);
    		CAL_Init();
        }
        if (tp_dev.x[0] > 120 && tp_dev.y[0] > 200 && tp_dev.x[0] < 240 && tp_dev.y[0] < 320)
        {
            app_mode = 5;
        }

        //游戏区域
        if (tp_dev.x[0] > 180 && tp_dev.y[0] > 50 && tp_dev.x[0] < 240 && tp_dev.y[0] < 80)
		{
			app_mode = 8;
		}
    }
    else if (app_mode == 2)
    {
        // 9个用户
        int line = 0;
        for (int i = 0; i < 9; i++)
        {
            if (status[i] == 1)
            {
                if (tp_dev.x[0] > 0 && tp_dev.y[0] > 80 * line && tp_dev.x[0] < 240 && tp_dev.y[0] < 80 * line + 80)
                {
                    send_Invite(i);
                    app_mode = 7;
                    screen_reprint_7();
                    break;
                }
                line++;
            }
        }
    }
    else if (app_mode == 3)
    {
        // LCD_DrawRectangle(160, 80, 240, 160); // 上按钮
        // LCD_DrawRectangle(160, 240, 240, 320); // 下按钮
        if (tp_dev.x[0] > 160 && tp_dev.y[0] > 80 && tp_dev.x[0] < 240 && tp_dev.y[0] < 160)
        {
            if (chat_offset < msg_num[talking_user] - 4){
                chat_offset++;
//                LCD_Fill(0, 81, 159, 320, WHITE);
                screen_clear();
                screen_reprint();
            }
                
        }
        if (tp_dev.x[0] > 160 && tp_dev.y[0] > 240 && tp_dev.x[0] < 240 && tp_dev.y[0] < 320)
        {
            if (chat_offset > 0)
                {
                chat_offset--;
//                LCD_Fill(0, 81, 159, 320, WHITE);
                screen_clear();
                screen_reprint();
            }
        }
        int id_emoji = 0;
        // (160, 160, 200, 200) emoji1
        if (tp_dev.x[0] > 160 && tp_dev.y[0] > 160 && tp_dev.x[0] < 200 && tp_dev.y[0] < 200)
        	id_emoji = 1;

        // (200, 160, 240, 200) emoji2
        if (tp_dev.x[0] > 200 && tp_dev.y[0] > 160 && tp_dev.x[0] < 240 && tp_dev.y[0] < 200)
        	id_emoji = 2;

        // (160, 200, 200, 240) emoji3
        if (tp_dev.x[0] > 160 && tp_dev.y[0] > 200 && tp_dev.x[0] < 200 && tp_dev.y[0] < 240)
        	id_emoji = 3;

        // (200, 200, 240, 200) emoji4
        if (tp_dev.x[0] > 200 && tp_dev.y[0] > 200 && tp_dev.x[0] < 240 && tp_dev.y[0] < 240)
        	id_emoji = 4;

        if (id_emoji != 0){
        	msg_num[talking_user]++;
        	hist_msg[talking_user][msg_num[talking_user] - 1].msg[0] = '.';
        	hist_msg[talking_user][msg_num[talking_user] - 1].msg[1] = id_emoji + '0';
        	hist_msg[talking_user][msg_num[talking_user] - 1].sender = user_id; // 存储历史消息:emoji1

            char emoji_msg[50];
            sprintf(emoji_msg, "%d%dM.%d", user_id, talking_user, id_emoji);
            NRF24L01_TX_Mode();
            NRF24L01_TxPacket((uint8_t *) emoji_msg);
            NRF24L01_RX_Mode();

            chat_offset = 0;
            screen_clear();
            screen_reprint();
        }
    }
    else if (app_mode == 4)
    {
//		while (1) {
//			tp_dev.scan(0);
		if (tp_dev.sta & TP_PRES_DOWN) {
			if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
				CAL_LCD_Callback(tp_dev.x[0], tp_dev.y[0]);
			}
		}
//		}
    }
    else if (app_mode == 5)
    {
    }
    else if (app_mode == 6)
    {
        if (tp_dev.x[0] > 0 && tp_dev.y[0] > 0 && tp_dev.x[0] < 120 && tp_dev.y[0] < 80)
        {
            // accept
            app_mode = 3;
            talking_user = sender;
            screen_clear();
            screen_reprint();
            send_Accept(sender);
        }
        if (tp_dev.x[0] > 120 && tp_dev.y[0] > 0 && tp_dev.x[0] < 240 && tp_dev.y[0] < 80)
        {
            // reject
            app_mode = prev_mode;
            screen_clear();
            screen_reprint();
            send_Reject(sender);
        }
    }
    else if (app_mode == 8)
    {
    	computer_choice = rand()%3;
		if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
		{
			if(tp_dev.x[0]>34 && tp_dev.x[0]<69 && tp_dev.y[0]>222 && tp_dev.y[0]<257){
				human_choice = 0;
			}
			else if(tp_dev.x[0]>103 && tp_dev.x[0]<138 && tp_dev.y[0]>222 && tp_dev.y[0]<257){
				human_choice = 1;
			}
			else if(tp_dev.x[0]>172 && tp_dev.x[0]<207 && tp_dev.y[0]>222 && tp_dev.y[0]<257){
				human_choice = 2;
			}
		}
		if (human_choice != 5){
			LCD_Clear(WHITE);
			if (human_choice == 0)LCD_ShowPicture(103,222,35,35,scissors);
			if (human_choice == 1)LCD_ShowPicture(103,222,35,35,rock);
			if (human_choice == 2)LCD_ShowPicture(103,222,35,35,paper);
			judge(human_choice,computer_choice);
			print_result(result);
			delay_ms(1500);
			screen_reprint();
			human_choice = 5;
		}
    }
}
