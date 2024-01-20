/*
 * chat.c
 *
 *  Created on: Dec 29, 2023
 *      Author: 12484
 */
#include "project.h"

extern int chat_offset;
history_msg hist_msg[10][20];
int msg_num[10];       // 各用户信息数�??
int status[10];        // 各用户状�??
u8 tmp_buf[110];       // 2.4g读取消息
char message[110];     // 提取消息内容
char message_type;     //"I":�??请聊�?? "A":我在线上 "U":上线 "D":下线 "M":发消�?? ‘C’: 接受聊天请求 ‘R’: 拒绝聊天请求
int user_id;           // 用户编号
int talking_user = -1; // 当前模式，聊天模式下在哪个窗口（-1为聊天主界面�??
int rec_user, sender;  // 消息接收者，发�?��?�，
// int quit_invite_mode = 0;
void send_ImOnline() {
    char online_mess[50];
    sprintf(online_mess, "%d0A", user_id);
    NRF24L01_TX_Mode();
    NRF24L01_TxPacket((uint8_t *) online_mess);
    NRF24L01_RX_Mode();
}

void send_Invite(int rec_user) {
    char online_mess[50];
    sprintf(online_mess, "%d%dI", user_id, rec_user);
    NRF24L01_TX_Mode();
    NRF24L01_TxPacket((uint8_t *) online_mess);
    NRF24L01_RX_Mode();
}

void send_Accept(int rec_user) {
    char online_mess[50];
    sprintf(online_mess, "%d%dC", user_id, rec_user);
    NRF24L01_TX_Mode();
    NRF24L01_TxPacket((uint8_t *) online_mess);
    NRF24L01_RX_Mode();
}

void send_Reject(int rec_user) {
    char online_mess[50];
    sprintf(online_mess, "%d%dR", user_id, rec_user);
    NRF24L01_TX_Mode();
    NRF24L01_TxPacket((uint8_t *) online_mess);
    NRF24L01_RX_Mode();
}

void send_online() {
    char online_mess[50];
    sprintf(online_mess, "%d0U", user_id);
    NRF24L01_TX_Mode();
    NRF24L01_TxPacket((uint8_t *) online_mess);
    NRF24L01_RX_Mode();
}

void send_offline() {
    char offline_mess[50];
    sprintf(offline_mess, "%d0D", user_id);
    NRF24L01_TX_Mode();
    NRF24L01_TxPacket((uint8_t *) offline_mess);
    NRF24L01_RX_Mode();
}

void handle_message(u8 *tmp_buf) { // 消息内容提取
    char rec[100];
    for (int i = 0; i < 32; i++) {
        rec[i] = tmp_buf[i];
    }
    sender = rec[0] - '0';
    rec_user = rec[1] - '0';
    message_type = rec[2];
    for (int i = 3; i < 32; i++) {
        message[i - 3] = rec[i];
    }
}

void Pop_up(char message_tp, int sender) { // 弹窗
    if (message_tp == 'I') {
        app_mode = 6;
        screen_reprint_6();
        return;
    }
    if (message_tp == 'U') {
        status[sender] = 1;
        HAL_Delay(10 + 10 * user_id);
        send_ImOnline();

        // 先将(50,20)(75,200)涂白
        LCD_Fill(0, 40, 240, 80, WHITE);
        POINT_COLOR = BLACK;
        LCD_DrawRectangle(0, 40, 240, 80);
        // 拼接上线提示
        char online_mess[50];
        sprintf(online_mess, "User %d is online", sender);

        LCD_ShowString(50, 50, 200, 16, 16, (u8 *) online_mess);
        POINT_COLOR = BLUE;
        delay_ms(2000);
        screen_clear();
        screen_reprint();
        return;
    }
    if (message_tp == 'D') {
        status[sender] = 0;

        LCD_Fill(0, 40, 240, 80, WHITE);
        POINT_COLOR = BLACK;
        LCD_DrawRectangle(0, 40, 240, 80);
        // 拼接下线提示
        char offline_mess[50];
        sprintf(offline_mess, "User %d is offline", sender);
        LCD_ShowString(50, 50, 200, 16, 16, (u8 *) offline_mess);
        POINT_COLOR = BLUE;
        delay_ms(2000);
        screen_clear();
        screen_reprint();
        return;
    }
}

void try_receive_message() { // 尝试接收消息
    if (NRF24L01_RxPacket(tmp_buf) == 0) {
        handle_message(tmp_buf); // 消息处理
        int rec = 0;             // 是否给我
        if (rec_user == user_id || rec_user == 0)
            rec = 1;
        if (!rec)
            return;
        if (message_type == 'M' && app_mode != 3 /*聊天界面*/)
            return;
        if (message_type == 'A') {
            status[sender] = 1;
            return;
        }
        if (message_type == 'C') {
            app_mode = 3;
            talking_user = sender;
            return;
        }
        if (message_type == 'R') {
            app_mode = prev_mode;
            screen_clear();
            screen_reprint();
            return;
        }
        if (app_mode != 3 || message_type != 'M')
            Pop_up(message_type, sender); // 显示上线、下线�?�邀请弹�??
        else {
            if (talking_user != 0 && talking_user == sender && rec_user == user_id) {
                msg_num[sender]++;
                memcpy(hist_msg[sender][msg_num[sender] - 1].msg, message, sizeof(message));
                hist_msg[sender][msg_num[sender] - 1].sender = sender;

                chat_offset = 0;
//			    delay_ms(125);
//			    LCD_Fill(0, 81, 159, 320, WHITE);
                screen_clear();
                screen_reprint();
                // 私聊
            } else {
                if (talking_user == 0 && rec_user == 0) {
                    msg_num[0]++;
                    memcpy(hist_msg[0][msg_num[0] - 1].msg, message, sizeof(message));
                    hist_msg[0][msg_num[0] - 1].sender = sender;

                    chat_offset = 0;
//				    delay_ms(125);
//				    LCD_Fill(0, 81, 159, 320, WHITE);
	                screen_clear();
	                screen_reprint();
                    // 群聊
                }
            }
        }
    } else
        return;
}
