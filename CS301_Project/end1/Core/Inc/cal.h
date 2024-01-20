#pragma once

#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

void CAL_Normal_Init();

void CAL_Equation_Init();

void CAL_Binary_Init();

void CAL_Init();

void CAL_LCD_Callback(int x, int y);

extern int cal_mode;
extern char expression[100];
