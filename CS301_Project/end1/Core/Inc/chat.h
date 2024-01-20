/*
 * chat.h
 *
 *  Created on: Dec 29, 2023
 *      Author: 12484
 */

#ifndef INC_CHAT_H_
#define INC_CHAT_H_


typedef struct{	//记录历史消息
	char msg[32];	//消息内容
	int sender;		//消息来自�?
}history_msg;
extern history_msg hist_msg[10][20];
extern int msg_num[10];                     // 各用户信息数�??
extern int status[10];                      // 各用户状�??
extern u8 tmp_buf[110];                     // 2.4g读取消息
extern char message[110];                   // 提取消息内容
extern char message_type;                   //"I":�??请聊�?? "A":我在线上 "U":上线 "D":下线 "M":发消�??
extern int user_id;                         // 用户编号
extern int talking_user;     // 当前模式，聊天模式下在哪个窗口（-1为聊天主界面�??
extern int rec_user, sender;                // 消息接收者，发�?��?�，

void send_ImOnline();
void send_online();
void send_offline();
void handle_message(u8 *tmp_buf);
void Pop_up(char message_tp, int sender);
void try_receive_message();
void send_Invite(int rec_user);
void send_Accept(int rec_user);
void send_Reject(int rec_user);
#endif /* INC_CHAT_H_ */
