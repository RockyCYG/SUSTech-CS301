#include "cal.h"
//#include "usart.h"

#define NORMAL_MODE 0
#define EQUATION_MODE 1
#define BINARY_MODE 2
#define BUTTON_SIZE_X 40
#define BUTTON_SIZE_Y 30
#define MAX_LEN 19

int cal_mode = NORMAL_MODE;
char expression[100];

int pos_xs[5][5];
int pos_ys[5][5];

int cal_flag = 1;
int expression_idx = 0;

int button_num_pos[10][2] = {{4, 2},
                             {3, 1},
                             {3, 2},
                             {3, 3},
                             {2, 1},
                             {2, 2},
                             {2, 3},
                             {1, 1},
                             {1, 2},
                             {1, 3}};

int button_operator_pos[8][2] = {{0, 1},
                                 {0, 2},
                                 {0, 3},
                                 {0, 4},
                                 {1, 4},
                                 {2, 4},
                                 {3, 4},
                                 {4, 4}};

char operators[] = {'(',
                    ')',
                    '^',
                    '+',
                    '-',
                    '*',
                    '/',
                    '='};
char button_variable_pos[2][2] = {{2, 0},
                                  {3, 0}};
char button_left_right_pos[2][2] = {{4, 1},
                                    {4, 3}};

// 判断字符是否为操作符
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// 获取运算符优先级
int getPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// 执行四则运算
long double performOperation(long double operand1, long double operand2, char operator) {
    switch (operator) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 == 0) {
                cal_flag = 0;
                return 0;
            }
            return operand1 / operand2;
        case '^':
            return pow(operand1, operand2);
        default:
            return 0;
    }
}

// 计算带括号的四则运算表达式
long double evaluateExpression(const char *expression) {
    long double operands[100];
    int operators[100];
    int operands_idx = 0;
    int operators_idx = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
        if (expression[i] == ' ') {
            continue;
        } else if (isdigit(expression[i])) {
            long long operand = 0LL;
            while (isdigit(expression[i])) {
                operand = operand * 10 + (expression[i] - '0');
                i++;
            }
            i--;
            operands[operands_idx++] = operand;
        } else if (expression[i] == '(') {
            operators[operators_idx++] = expression[i];
        } else if (isOperator(expression[i])) {
            while (operators_idx > 0 && getPriority(operators[operators_idx - 1]) >= getPriority(expression[i])) {
                long double operand2 = operands[--operands_idx];
                long double operand1 = operands[--operands_idx];
                char operator = operators[--operators_idx];
                long double result = performOperation(operand1, operand2, operator);
                if (cal_flag == 0) {
                    return 0;
                }
                operands[operands_idx++] = result;
            }
            operators[operators_idx++] = expression[i];
        } else if (expression[i] == ')') {
            while (operators_idx > 0 && operators[operators_idx - 1] != '(') {
                long double operand2 = operands[--operands_idx];
                long double operand1 = operands[--operands_idx];
                char operator = operators[--operators_idx];
                long double result = performOperation(operand1, operand2, operator);
                if (cal_flag == 0) {
                    return 0;
                }
//                push(operands, result);
                operands[operands_idx++] = result;
            }
            if (operators_idx > 0 && operators[operators_idx - 1] == '(') {
                operators_idx--; // 弹出左括号
            }
        }
    }

    while (operators_idx > 0) {
        long double operand2 = operands[--operands_idx];
        long double operand1 = operands[--operands_idx];
        char operator = operators[--operators_idx];
        long double result = performOperation(operand1, operand2, operator);
        if (cal_flag == 0) {
            return 0;
        }
        operands[operands_idx++] = result;
    }

    if (operators_idx != 0 || operands_idx != 1) {
        cal_flag = 0;
        return 0;
    }

    long double finalResult = operands[operands_idx - 1];

    return finalResult;
}

void binaryAddition(const char a[], const char b[], char res[]) {
//    int m = strlen(a), n = strlen(b);
//    if (m < n) {
//        char *c = a;
//        a = b;
//        b = c;
//        int t = m;
//        m = n;
//        n = t;
//    }
//    int carry = 0;
//    for (int i = m - 1; i >= 0; i--) {
//        res[i] = (a[i] - '0' + (n - m + i >= 0 ? b[n - m + i] - '0' : 0) + carry) % 2 + '0';
//        carry = (a[i] - '0' + (n - m + i >= 0 ? b[n - m + i] - '0' : 0) + carry) / 2;
//    }
//    if (carry) {
//        for (int i = m - 1; i >= 0; i--) {
//            res[i + 1] = res[i];
//        }
//        res[0] = '1';
//    }
    int m = strlen(a), n = strlen(b);
    long long x = 0LL, y = 0LL;
    for (int i = 0; i < m; i++) {
        x <<= 1;
        int z = a[i] - '0';
        x |= z;
    }
    for (int i = 0; i < n; i++) {
        y <<= 1;
        int z = b[i] - '0';
        y |= z;
    }
    long long z = x + y;
    char t[100] = {0};
    for (int i = 0; z > 0; z >>= 1, i++) {
        t[i] = (z & 1) + '0';
    }
    int len = strlen(t);
    for (int i = 0; i < len; i++) {
        res[len - i - 1] = t[i];
    }
}

void binarySubtraction(const char a[], const char b[], char res[]) {
//    int m = strlen(a), n = strlen(b);
//    if (m < n) {
//        char *c = a;
//        a = b;
//        b = c;
//        int t = m;
//        m = n;
//        n = t;
//    }
//    int carry = 0;
//    for (int i = m - 1; i >= 0; i--) {
//        res[i] = ((a[i] - '0' - (n - m + i >= 0 ? b[n - m + i] - '0' : 0) + carry) % 2 + 2) % 2 + '0';
//        carry = (a[i] - '0' - (n - m + i >= 0 ? b[n - m + i] - '0' : 0) - carry) < 0;
//    }
    int m = strlen(a), n = strlen(b);
    long long x = 0LL, y = 0LL;
    for (int i = 0; i < m; i++) {
        x <<= 1;
        int z = a[i] - '0';
        x |= z;
    }
    for (int i = 0; i < n; i++) {
        y <<= 1;
        int z = b[i] - '0';
        y |= z;
    }
    long long z = x - y;
    char t[100] = {0};
    for (int i = 0; z != 0 && i < 16; z >>= 1, i++) {
        t[i] = (z & 1) + '0';
    }
    int len = strlen(t);
    for (int i = 0; i < len; i++) {
        res[len - i - 1] = t[i];
    }
}

void binaryMultiplication(const char a[], const char b[], char res[]) {
    int m = strlen(a), n = strlen(b);
    long long x = 0LL, y = 0LL;
    for (int i = 0; i < m; i++) {
        x <<= 1;
        int z = a[i] - '0';
        x |= z;
    }
    for (int i = 0; i < n; i++) {
        y <<= 1;
        int z = b[i] - '0';
        y |= z;
    }
    long long z = x * y;
    char t[100] = {0};
    for (int i = 0; z > 0; z >>= 1, i++) {
        t[i] = (z & 1) + '0';
    }
    int len = strlen(t);
    for (int i = 0; i < len; i++) {
        res[len - i - 1] = t[i];
    }
}

void performBinaryOperation(const char a[], const char b[], char operator, char res[]) {
    switch (operator) {
        case '+':
            binaryAddition(a, b, res);
            break;
        case '-':
            binarySubtraction(a, b, res);
            break;
        case '*':
            binaryMultiplication(a, b, res);
            break;
        default:;
    }
}

// 计算带括号的四则运算表达式
void evaluateBinaryExpression(const char *expression) {
    char operands[100][100];
    memset(operands, 0, sizeof(operands));
    int operators[100];
    int operands_idx = 0;
    int operand_idx = 0;
    int operators_idx = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
        if (expression[i] == ' ') {
            continue;
        } else if (isdigit(expression[i])) {
            operand_idx = 0;
            memset(operands[operands_idx], 0, sizeof(operands[operands_idx]));
            while (isdigit(expression[i])) {
                operands[operands_idx][operand_idx++] = expression[i];
                i++;
            }
            i--;
            operands_idx++;
        } else if (isOperator(expression[i])) {
            while (operators_idx > 0 && getPriority(operators[operators_idx - 1]) >= getPriority(expression[i])) {
                char *operand2 = operands[--operands_idx];
                char *operand1 = operands[--operands_idx];
                char operator = operators[--operators_idx];
                char res[100];
                memset(res, 0, sizeof(res));
                performBinaryOperation(operand1, operand2, operator, res);
                if (cal_flag == 0) {
                    return;
                }
                memset(operands[operands_idx], 0, sizeof(operands[operands_idx]));
                strcpy(operands[operands_idx], res);
                operands_idx++;
            }
            operators[operators_idx++] = expression[i];
        }
    }

    while (operators_idx > 0) {
        char *operand2 = operands[--operands_idx];
        char *operand1 = operands[--operands_idx];
        char operator = operators[--operators_idx];
        char res[100];
        memset(res, 0, sizeof(res));
        performBinaryOperation(operand1, operand2, operator, res);
        if (cal_flag == 0) {
            return;
        }
        memset(operands[operands_idx], 0, sizeof(operands[operands_idx]));
        strcpy(operands[operands_idx], res);
        operands_idx++;
    }

    if (operators_idx != 0 || operands_idx != 1) {
        cal_flag = 0;
        return;
    }

    char *res = operands[operands_idx - 1];
    LCD_ShowString(10, 40, 240, 240, 16, res);
}

void solveEquation1(const char *str) {
    char var;
    char str1[100] = {0}, str2[100] = {0};
    int i = 0, loc1 = 0, loc2 = 0;
    if (str[0] == '-') {
        str1[loc1++] = '0';
        str2[loc2++] = '0';
    }
    while (str[i] != '\0') {
        if (isalpha(str[i])) {
            var = str[i];
            if (i == 0 || (i > 0 && !isdigit(str[i - 1]))) {
                str1[loc1++] = '1';
                str2[loc2++] = '0';
            } else {
                str1[loc1++] = '*';
                str1[loc1++] = '1';
                str2[loc2++] = '*';
                str2[loc2++] = '0';
            }
        } else if (str[i] == '=') {
            str1[loc1++] = '-';
            str1[loc1++] = '(';
            str2[loc2++] = '-';
            str2[loc2++] = '(';
            if (str[i + 1] == '-' || str[i + 1] == '+') {
                str1[loc1++] = '0';
                str2[loc2++] = '0';
            }
        } else if (str[i] == '(' && (str[i + 1] == '-' || str[i + 1] == '+')) {
            str1[loc1++] = '(';
            str2[loc2++] = '(';
            str1[loc1++] = '0';
            str2[loc2++] = '0';
        } else {
            str1[loc1++] = str[i];
            str2[loc2++] = str[i];
        }
        ++i;
    }
    str1[loc1] = ')';
    str2[loc2] = ')';
    long double sum = evaluateExpression(str1);
    long double b = evaluateExpression(str2);
    long double a = sum - b;
    long double finalResult = -b / a;
    char res[100];
    sprintf(res, "%c=%Lf", var, finalResult);
    LCD_ShowString(10, 40, 240, 240, 16, res);
}

void solveEquation2(const char *str) {
    char var;
    char str1[100] = {0}, str2[100] = {0}, str3[100] = {0};
    int i = 0, loc1 = 0, loc2 = 0, loc3 = 0;
    if (str[0] == '-') {
        str1[loc1++] = '0';
        str2[loc2++] = '0';
        str3[loc3++] = '0';
    }
    while (str[i] != '\0') {
        if (isalpha(str[i])) {
            var = str[i];
            if (i == 0 || (i > 0 && !isdigit(str[i - 1]))) {
                str1[loc1++] = '1';
                str2[loc2++] = '0';
                str3[loc3++] = '(';
                str3[loc3++] = '0';
                str3[loc3++] = '-';
                str3[loc3++] = '1';
                str3[loc3++] = ')';
            } else {
                str1[loc1++] = '*';
                str1[loc1++] = '1';
                str2[loc2++] = '*';
                str2[loc2++] = '0';
                str3[loc3++] = '*';
                str3[loc3++] = '(';
                str3[loc3++] = '0';
                str3[loc3++] = '-';
                str3[loc3++] = '1';
                str3[loc3++] = ')';
            }
        } else if (str[i] == '=') {
            str1[loc1++] = '-';
            str1[loc1++] = '(';
            str2[loc2++] = '-';
            str2[loc2++] = '(';
            str3[loc3++] = '-';
            str3[loc3++] = '(';
            if (str[i + 1] == '-' || str[i + 1] == '+') {
                str1[loc1++] = '0';
                str2[loc2++] = '0';
                str3[loc3++] = '0';
            }
        } else if (str[i] == '(' && (str[i + 1] == '-' || str[i + 1] == '+')) {
            str1[loc1++] = '(';
            str2[loc2++] = '(';
            str3[loc3++] = '(';
            str1[loc1++] = '0';
            str2[loc2++] = '0';
            str3[loc3++] = '0';
        } else {
            str1[loc1++] = str[i];
            str2[loc2++] = str[i];
            str3[loc3++] = str[i];
        }
        ++i;
    }
    str1[loc1] = ')';
    str2[loc2] = ')';
    str3[loc3] = ')';
    printf("%s\n", str1);
    long double x = evaluateExpression(str1);
    printf("%s\n", str2);
    long double y = evaluateExpression(str2);
    printf("%s\n", str3);
    long double z = evaluateExpression(str3);
    long double c = y;
    long double a = (x + z - 2 * c) / 2;
    long double b = (x - z) / 2;
    long double delta = b * b - 4 * a * c;
    long double x1 = (-b - sqrt(delta)) / (2 * a);
    long double x2 = (-b + sqrt(delta)) / (2 * a);
    char res[100];
    sprintf(res, "%c=%Lf | %c=%Lf", var, x1, var, x2);
    LCD_ShowString(10, 40, 240, 240, 16, res);
}

void solveEquation(const char *expression) {
    int n = strlen(expression);
    int one = 1;
    for (int i = 0; i < n - 1; i++) {
        if (expression[i] == '^' && expression[i + 1] == '2') {
            one = 0;
        }
    }
    if (one) {
        solveEquation1(expression);
    } else {
        solveEquation2(expression);
    }
}

void CAL_Normal_Init() {
    for (int y = 125, i = 0; y <= 310; y += BUTTON_SIZE_Y + 10, i++) {
        for (int x = 10, j = 0; x <= 190; x += BUTTON_SIZE_X + 5, j++) {
            pos_ys[i][j] = y;
            pos_xs[i][j] = x;
            LCD_DrawRectangle(x, y, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y);
            if (i == 0 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'C', 16, 0);
            } else if (i == 0 && j == 1) {
                LCD_ShowChar(x + 15, y + 6, '(', 16, 0);
            } else if (i == 0 && j == 2) {
                LCD_ShowChar(x + 15, y + 6, ')', 16, 0);
            } else if (i == 0 && j == 3) {
                LCD_ShowChar(x + 15, y + 6, '^', 16, 0);
            } else if (i == 0 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '+', 16, 0);
            } else if (i == 1 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'V', 16, 0);
            } else if (i == 1 && j == 1) {
                LCD_ShowxNum(x + 15, y + 6, 7, 1, 16, 0);
            } else if (i == 1 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 8, 1, 16, 0);
            } else if (i == 1 && j == 3) {
                LCD_ShowxNum(x + 15, y + 6, 9, 1, 16, 0);
            } else if (i == 1 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '-', 16, 0);
            } else if (i == 2 && j == 1) {
                LCD_ShowxNum(x + 15, y + 6, 4, 1, 16, 0);
            } else if (i == 2 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 5, 1, 16, 0);
            } else if (i == 2 && j == 3) {
                LCD_ShowxNum(x + 15, y + 6, 6, 1, 16, 0);
            } else if (i == 2 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '*', 16, 0);
            } else if (i == 3 && j == 1) {
                LCD_ShowxNum(x + 15, y + 6, 1, 1, 16, 0);
            } else if (i == 3 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 2, 1, 16, 0);
            } else if (i == 3 && j == 3) {
                LCD_ShowxNum(x + 15, y + 6, 3, 1, 16, 0);
            } else if (i == 3 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '/', 16, 0);
            } else if (i == 4 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'T', 16, 0);
            } else if (i == 4 && j == 1) {
                LCD_ShowChar(x + 15, y + 6, 'L', 16, 0);
            } else if (i == 4 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 0, 1, 16, 0);
            } else if (i == 4 && j == 3) {
                LCD_ShowChar(x + 15, y + 6, 'R', 16, 0);
            } else if (i == 4 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '=', 16, 0);
            } else {
                LCD_Fill(x, y, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y, GRAY);
            }
        }
    }
    LCD_ShowString(10, 70, 240, 240, 16, "Normal mode");
}

void CAL_Equation_Init() {
    for (int y = 125, i = 0; y <= 310; y += BUTTON_SIZE_Y + 10, i++) {
        for (int x = 10, j = 0; x <= 190; x += BUTTON_SIZE_X + 5, j++) {
            pos_ys[i][j] = y;
            pos_xs[i][j] = x;
            LCD_DrawRectangle(x, y, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y);
            if (i == 0 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'C', 16, 0);
            } else if (i == 0 && j == 1) {
                LCD_ShowChar(x + 15, y + 6, '(', 16, 0);
            } else if (i == 0 && j == 2) {
                LCD_ShowChar(x + 15, y + 6, ')', 16, 0);
            } else if (i == 0 && j == 3) {
                LCD_ShowChar(x + 15, y + 6, '^', 16, 0);
            } else if (i == 0 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '+', 16, 0);
            } else if (i == 1 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'V', 16, 0);
            } else if (i == 1 && j == 1) {
                LCD_ShowxNum(x + 15, y + 6, 7, 1, 16, 0);
            } else if (i == 1 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 8, 1, 16, 0);
            } else if (i == 1 && j == 3) {
                LCD_ShowxNum(x + 15, y + 6, 9, 1, 16, 0);
            } else if (i == 1 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '-', 16, 0);
            } else if (i == 2 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'x', 16, 0);
            } else if (i == 2 && j == 1) {
                LCD_ShowxNum(x + 15, y + 6, 4, 1, 16, 0);
            } else if (i == 2 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 5, 1, 16, 0);
            } else if (i == 2 && j == 3) {
                LCD_ShowxNum(x + 15, y + 6, 6, 1, 16, 0);
            } else if (i == 2 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '*', 16, 0);
            } else if (i == 3 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'y', 16, 0);
            } else if (i == 3 && j == 1) {
                LCD_ShowxNum(x + 15, y + 6, 1, 1, 16, 0);
            } else if (i == 3 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 2, 1, 16, 0);
            } else if (i == 3 && j == 3) {
                LCD_ShowxNum(x + 15, y + 6, 3, 1, 16, 0);
            } else if (i == 3 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '/', 16, 0);
            } else if (i == 4 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'T', 16, 0);
            } else if (i == 4 && j == 1) {
                LCD_ShowChar(x + 15, y + 6, 'L', 16, 0);
            } else if (i == 4 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 0, 1, 16, 0);
            } else if (i == 4 && j == 3) {
                LCD_ShowChar(x + 15, y + 6, 'R', 16, 0);
            } else if (i == 4 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '=', 16, 0);
            } else {
                LCD_Fill(x, y, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y, GRAY);
            }
        }
    }
    LCD_ShowString(10, 70, 240, 240, 16, "Equation mode");
}

void CAL_Binary_Init() {
    for (int y = 125, i = 0; y <= 310; y += BUTTON_SIZE_Y + 10, i++) {
        for (int x = 10, j = 0; x <= 190; x += BUTTON_SIZE_X + 5, j++) {
            pos_ys[i][j] = y;
            pos_xs[i][j] = x;
            LCD_DrawRectangle(x, y, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y);
            if (i == 0 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'C', 16, 0);
            } else if (i == 0 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '+', 16, 0);
            } else if (i == 1 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'V', 16, 0);
            } else if (i == 1 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '-', 16, 0);
            } else if (i == 2 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '*', 16, 0);
            } else if (i == 3 && j == 1) {
                LCD_ShowxNum(x + 15, y + 6, 1, 1, 16, 0);
            } else if (i == 4 && j == 0) {
                LCD_ShowChar(x + 15, y + 6, 'T', 16, 0);
            } else if (i == 4 && j == 1) {
                LCD_ShowChar(x + 15, y + 6, 'L', 16, 0);
            } else if (i == 4 && j == 2) {
                LCD_ShowxNum(x + 15, y + 6, 0, 1, 16, 0);
            } else if (i == 4 && j == 3) {
                LCD_ShowChar(x + 15, y + 6, 'R', 16, 0);
            } else if (i == 4 && j == 4) {
                LCD_ShowChar(x + 15, y + 6, '=', 16, 0);
            } else {
                LCD_Fill(x, y, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y, GRAY);
            }
        }
    }
    LCD_ShowString(10, 70, 240, 240, 16, "Binary mode");
}

void CAL_Init() {
    cal_flag = 1;
    memset(expression, 0, sizeof(expression));
    expression_idx=0;
    LCD_Clear(WHITE);
    LCD_ShowString(10, 10, 240, 240, 16, expression);
    LCD_DrawLine(10, 30, 170, 30);
    LCD_DrawLine(10, 60, 230, 60);
    if (cal_mode == NORMAL_MODE) {
        CAL_Normal_Init();
    } else if (cal_mode == EQUATION_MODE) {
        CAL_Equation_Init();
    } else {
        CAL_Binary_Init();
    }
}

void CAL_Init_Re() {
    cal_flag = 1;
//    memset(expression, 0, sizeof(expression));
    LCD_Clear(WHITE);
//    LCD_ShowString(10, 10, 240, 240, 16, expression);
    LCD_DrawLine(10, 30, 170, 30);
    LCD_DrawLine(10, 60, 230, 60);
    if (cal_mode == NORMAL_MODE) {
        CAL_Normal_Init();
    } else if (cal_mode == EQUATION_MODE) {
        CAL_Equation_Init();
    } else {
        CAL_Binary_Init();
    }
    int pos = strlen(expression) - 1;
    if (pos > MAX_LEN) {
        char display_str[100];
        strncpy(display_str, expression + expression_idx, MAX_LEN);
        LCD_ShowString(10, 10, 240, 240, 16, display_str);
    } else {
        LCD_ShowString(10, 10, 240, 240, 16, expression);
    }
}

void CAL_LCD_Callback(int x, int y) {
    // 数字
    for (int i = 0; i < 10; i++) {
        int r = button_num_pos[i][0], c = button_num_pos[i][1];
        if (x > pos_xs[r][c] && x < pos_xs[r][c] + BUTTON_SIZE_X && y > pos_ys[r][c] &&
            y < pos_ys[r][c] + BUTTON_SIZE_Y) {
            if (i >= 2 && cal_mode == BINARY_MODE) {
                continue;
            }
            int num = i;
            int pos = strlen(expression);
            expression[pos++] = num + '0';
            expression[pos] = 0;
            if (pos > MAX_LEN) {
            	expression_idx++;
                char display_str[100];
                strncpy(display_str, expression + expression_idx, MAX_LEN);
                LCD_ShowString(10, 10, 240, 240, 16, display_str);
            } else {
                LCD_ShowString(10, 10, 240, 240, 16, expression);
            }
            HAL_Delay(500);
            return;
        }
    }
    for (int i = 0; i < 8; i++) {
        int r = button_operator_pos[i][0], c = button_operator_pos[i][1];
        if (x > pos_xs[r][c] && x < pos_xs[r][c] + BUTTON_SIZE_X && y > pos_ys[r][c] &&
            y < pos_ys[r][c] + BUTTON_SIZE_Y) {
            char operator = operators[i];
            if (operator == '/' && cal_mode == BINARY_MODE) {
                continue;
            }
            if (operator == '=') {
                if (cal_mode == NORMAL_MODE) {
                    long double finalResult = evaluateExpression(expression);
                    if (cal_flag == 0) {
                        LCD_ShowString(10, 40, 240, 240, 16, "Error");
                        HAL_Delay(500);
                        return;
                    }
                    char res[100];
                    sprintf(res, "%Lf", finalResult);
                    LCD_ShowString(10, 40, 240, 240, 16, res);
                    HAL_Delay(500);
                    return;
                } else if (cal_mode == EQUATION_MODE) {

                } else {
                    evaluateBinaryExpression(expression);
                    if (cal_flag == 0) {
                        LCD_ShowString(10, 40, 240, 240, 16, "Error");
                        HAL_Delay(500);
                        return;
                    }
                    HAL_Delay(500);
                    return;
                }
            }
            int pos = strlen(expression);
            expression[pos++] = operator;
            expression[pos] = 0;
            if (pos > MAX_LEN) {
            	expression_idx++;
                char display_str[100];
                strncpy(display_str, expression + expression_idx, MAX_LEN);
                LCD_ShowString(10, 10, 240, 240, 16, display_str);
            } else {
                LCD_ShowString(10, 10, 240, 240, 16, expression);
            }
            HAL_Delay(500);
            return;
        }
    }
    // 清除
    if (x > pos_xs[0][0] && x < pos_xs[0][0] + BUTTON_SIZE_X && y > pos_ys[0][0] &&
        y < pos_ys[0][0] + BUTTON_SIZE_Y) {
        memset(expression, 0, sizeof(expression));
        cal_flag = 1;
        LCD_Fill(0, 0, 240, 60, WHITE);
        LCD_DrawLine(10, 30, 170, 30);
        LCD_DrawLine(10, 60, 230, 60);
        HAL_Delay(500);
        return;
    }
    // 回退
//    if (x > pos_xs[1][0] && x < pos_xs[1][0] + BUTTON_SIZE_X && y > pos_ys[1][0] &&
//        y < pos_ys[1][0] + BUTTON_SIZE_Y) {
//        int pos = strlen(expression);
//        expression[--pos] = 0;
//        cal_flag = 1;
//        LCD_Fill(0, 0, 240, 60, WHITE);
//        LCD_ShowString(10, 10, 240, 240, 16, expression);
//        HAL_Delay(500);
//        return;
//    }
    // Verify
    if (cal_mode == EQUATION_MODE) {
        if (x > pos_xs[1][0] && x < pos_xs[1][0] + BUTTON_SIZE_X && y > pos_ys[1][0] &&
            y < pos_ys[1][0] + BUTTON_SIZE_Y) {
            solveEquation(expression);
        }
    }
    // 转换模式
    if (x > pos_xs[4][0] && x < pos_xs[4][0] + BUTTON_SIZE_X && y > pos_ys[4][0] &&
        y < pos_ys[4][0] + BUTTON_SIZE_Y) {
        cal_mode = (cal_mode + 1) % 3;
        CAL_Init();
    }
    // x, y
    if (cal_mode == EQUATION_MODE) {
        for (int i = 0; i < 2; i++) {
            int r = button_variable_pos[i][0], c = button_variable_pos[i][1];
            if (x > pos_xs[r][c] && x < pos_xs[r][c] + BUTTON_SIZE_X && y > pos_ys[r][c] &&
                y < pos_ys[r][c] + BUTTON_SIZE_Y) {
                int pos = strlen(expression);
                expression[pos++] = 'x' + i;
                expression[pos] = 0;
                if (pos > MAX_LEN) {
                	expression_idx++;
                    char display_str[100];
                    strncpy(display_str, expression + expression_idx, MAX_LEN);
                    LCD_ShowString(10, 10, 240, 240, 16, display_str);
                } else {
                    LCD_ShowString(10, 10, 240, 240, 16, expression);
                }
                HAL_Delay(500);
            }
        }
    }
    // left, right
    for (int i = 0; i < 2; i++) {
        int r = button_left_right_pos[i][0], c = button_left_right_pos[i][1];
        if (x > pos_xs[r][c] && x < pos_xs[r][c] + BUTTON_SIZE_X && y > pos_ys[r][c] &&
            y < pos_ys[r][c] + BUTTON_SIZE_Y) {
            int pos = strlen(expression);
            if (pos > MAX_LEN) {
                char display_str[100];
                if (i == 0 && --expression_idx < 0) {
                    expression_idx = 0;
                } else if (i == 1 && ++expression_idx >= pos - MAX_LEN) {
                    expression_idx = pos - MAX_LEN;
                }
                strncpy(display_str, expression + expression_idx, MAX_LEN);
                LCD_ShowString(10, 10, 240, 240, 16, display_str);
            }
            HAL_Delay(500);
            return;
        }
    }
}
