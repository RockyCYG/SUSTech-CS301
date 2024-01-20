/*
 * chat.c
 *
 *  Created on: Dec 29, 2023
 *      Author: 12484
 */
#include "project.h"
int state_num = 0;
int computer_choice;
int human_choice = 5;
int result;
void judge(int h, int c){
    if (c == 0){
        if (h == 0) result = 0;
        if (h == 1) result = 1;
        if (h == 2) result = -1;
    	LCD_ShowPicture(103,63,35,35,scissors);
    	LCD_ShowString(15,140,200,24,16, "STM32 chooses scissors    ");
    } else if (c == 1){
        if (h == 0) result = -1;
        if (h == 1) result = 0;
        if (h == 2) result = 1;
        LCD_ShowPicture(103,63,35,35,rock);
        LCD_ShowString(15,140,200,24,16, "STM32 chooses rock        ");
    } else if (c == 2){
        if (h == 0) result = 1;
        if (h == 1) result = -1;
        if (h == 2) result = 0;
        LCD_ShowPicture(103,63,35,35,paper);
        LCD_ShowString(15,140,200,24,16, "STM32 chooses paper        ");
    }
}

void print_result(int result){
	if (result==0){
		LCD_ShowString(15,164,200,24,24, "Draw!");
	} else if (result==1){
		LCD_ShowString(15,164,200,24,24, "You win!");
	}else{
		LCD_ShowString(15,164,200,24,24, "You lose!");
	}
}
