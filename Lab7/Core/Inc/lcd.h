#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////
//鏈▼搴忓彧渚涘涔犱娇鐢紝鏈粡浣滆€呰鍙紝涓嶅緱鐢ㄤ簬鍏跺畠浠讳綍鐢ㄩ€�
//ALIENTEK MiniSTM32F103寮€鍙戞澘
//2.4瀵�/2.8瀵�/3.5瀵�/4.3瀵�/7瀵� TFT娑叉櫠椹卞姩
//鏀寔椹卞姩IC鍨嬪彿鍖呮嫭:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963绛�
//姝ｇ偣鍘熷瓙@ALIENTEK
//鎶€鏈鍧�:www.openedv.com
//鍒涘缓鏃ユ湡:2010/7/4
//鐗堟湰锛歏3.0
//鐗堟潈鎵€鏈夛紝鐩楃増蹇呯┒銆�
//Copyright(C) 骞垮窞甯傛槦缈肩數瀛愮鎶€鏈夐檺鍏徃 2014-2024
//All rights reserved
//********************************************************************************
//V1.2淇敼璇存槑
//鏀寔浜哠PFD5408鐨勯┍鍔�,鍙﹀鎶婃恫鏅禝D鐩存帴鎵撳嵃鎴怘EX鏍煎紡.鏂逛究鏌ョ湅LCD椹卞姩IC.
//V1.3
//鍔犲叆浜嗗揩閫烮O鐨勬敮鎸�
//淇敼浜嗚儗鍏夋帶鍒剁殑鏋佹€э紙閫傜敤浜嶸1.8鍙婁互鍚庣殑寮€鍙戞澘鐗堟湰锛�
//瀵逛簬1.8鐗堟湰涔嬪墠(涓嶅寘鎷�1.8)鐨勬恫鏅舵ā鍧�,璇蜂慨鏀筁CD_Init鍑芥暟鐨凩CD_LED=1;涓篖CD_LED=1;
//V1.4
//淇敼浜哃CD_ShowChar鍑芥暟锛屼娇鐢ㄧ敾鐐瑰姛鑳界敾瀛楃銆�
//鍔犲叆浜嗘í绔栧睆鏄剧ず鐨勬敮鎸�
//V1.5 20110730
//1,淇敼浜咮505娑叉櫠璇婚鑹叉湁璇殑bug.
//2,淇敼浜嗗揩閫烮O鍙婃í绔栧睆鐨勮缃柟寮�.
//V1.6 20111116
//1,鍔犲叆瀵筁GDP4535娑叉櫠鐨勯┍鍔ㄦ敮鎸�
//V1.7 20120713
//1,澧炲姞LCD_RD_DATA鍑芥暟
//2,澧炲姞瀵笽LI9341鐨勬敮鎸�
//3,澧炲姞ILI9325鐨勭嫭绔嬮┍鍔ㄤ唬鐮�
//4,澧炲姞LCD_Scan_Dir鍑芥暟(鎱庨噸浣跨敤)
//6,鍙﹀淇敼浜嗛儴鍒嗗師鏉ョ殑鍑芥暟,浠ラ€傚簲9341鐨勬搷浣�
//V1.8 20120905
//1,鍔犲叆LCD閲嶈鍙傛暟璁剧疆缁撴瀯浣搇cddev
//2,鍔犲叆LCD_Display_Dir鍑芥暟,鏀寔鍦ㄧ嚎妯珫灞忓垏鎹�
//V1.9 20120911
//1,鏂板RM68042椹卞姩锛圛D:6804锛夛紝浣嗘槸6804涓嶆敮鎸佹í灞忔樉绀猴紒锛佸師鍥狅細鏀瑰彉鎵弿鏂瑰紡锛�
//瀵艰嚧6804鍧愭爣璁剧疆澶辨晥锛岃瘯杩囧緢澶氭柟娉曢兘涓嶈锛屾殏鏃舵棤瑙ｃ€�
//V2.0 20120924
//鍦ㄤ笉纭欢澶嶄綅鐨勬儏鍐典笅,ILI9341鐨処D璇诲彇浼氳璇鎴�9300,淇敼LCD_Init,灏嗘棤娉曡瘑鍒�
//鐨勬儏鍐碉紙璇诲埌ID涓�9300/闈炴硶ID锛�,寮哄埗鎸囧畾椹卞姩IC涓篒LI9341锛屾墽琛�9341鐨勫垵濮嬪寲銆�
//V2.1 20120930
//淇ILI9325璇婚鑹茬殑bug銆�
//V2.2 20121007
//淇LCD_Scan_Dir鐨刡ug銆�
//V2.3 20130120
//鏂板6804鏀寔妯睆鏄剧ず
//V2.4 20131120
//1,鏂板NT35310锛圛D:5310锛夐┍鍔ㄥ櫒鐨勬敮鎸�
//2,鏂板LCD_Set_Window鍑芥暟,鐢ㄤ簬璁剧疆绐楀彛,瀵瑰揩閫熷～鍏�,姣旇緝鏈夌敤,浣嗘槸璇ュ嚱鏁板湪妯睆鏃�,涓嶆敮鎸�6804.
//V2.5 20140211
//1,鏂板NT35510锛圛D:5510锛夐┍鍔ㄥ櫒鐨勬敮鎸�
//V2.6 20140504
//1,鏂板ASCII 24*24瀛椾綋鐨勬敮鎸�(鏇村瀛椾綋鐢ㄦ埛鍙互鑷娣诲姞)
//2,淇敼閮ㄥ垎鍑芥暟鍙傛暟,浠ユ敮鎸丮DK -O2浼樺寲
//3,閽堝9341/35310/35510,鍐欐椂闂磋缃负鏈€蹇�,灏藉彲鑳界殑鎻愰珮閫熷害
//4,鍘绘帀浜哠SD1289鐨勬敮鎸�,鍥犱负1289瀹炲湪鏄お鎱簡,璇诲懆鏈熻1us...绠€鐩村钁�.涓嶉€傚悎F4浣跨敤
//5,淇68042鍙奀505绛塈C鐨勮棰滆壊鍑芥暟鐨刡ug.
//V2.7 20140710
//1,淇LCD_Color_Fill鍑芥暟鐨勪竴涓猙ug.
//2,淇LCD_Scan_Dir鍑芥暟鐨勪竴涓猙ug.
//V2.8 20140721
//1,瑙ｅ喅MDK浣跨敤-O2浼樺寲鏃禠CD_ReadPoint鍑芥暟璇荤偣澶辨晥鐨勯棶棰�.
//2,淇LCD_Scan_Dir妯睆鏃惰缃殑鎵弿鏂瑰紡鏄剧ず涓嶅叏鐨刡ug.
//V2.9 20141130
//1,鏂板瀵筍SD1963 LCD鐨勬敮鎸�.
//2,鏂板LCD_SSD_BackLightSet鍑芥暟
//3,鍙栨秷ILI93XX鐨凴xx瀵勫瓨鍣ㄥ畾涔�
//V3.0 20150423
//淇敼SSD1963 LCD灞忕殑椹卞姩鍙傛暟.
//////////////////////////////////////////////////////////////////////////////////


//LCD閲嶈鍙傛暟闆�
typedef struct {
    uint16_t width;            //LCD 瀹藉害
    uint16_t height;            //LCD 楂樺害
    uint16_t id;                //LCD ID
    uint8_t dir;            //妯睆杩樻槸绔栧睆鎺у埗锛�0锛岀珫灞忥紱1锛屾í灞忋€�
    uint16_t wramcmd;        //寮€濮嬪啓gram鎸囦护
    uint16_t setxcmd;        //璁剧疆x鍧愭爣鎸囦护
    uint16_t setycmd;        //璁剧疆y鍧愭爣鎸囦护
} _lcd_dev;

//LCD鍙傛暟
extern _lcd_dev lcddev;    //绠＄悊LCD閲嶈鍙傛暟
//LCD鐨勭敾绗旈鑹插拰鑳屾櫙鑹�
extern uint16_t POINT_COLOR;//榛樿绾㈣壊
extern uint16_t BACK_COLOR; //鑳屾櫙棰滆壊.榛樿涓虹櫧鑹�

////////////////////////////////////////////////////////////////////
//-----------------LCD绔彛瀹氫箟----------------
//IO鍙ｆ搷浣滃畯瀹氫箟
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//IO鍙ｅ湴鍧€鏄犲皠
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //杈撳嚭
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //杈撳叆
#define    LCD_LED PCout(10)                //LCD鑳屽厜    	PC10

#define    LCD_CS_SET  GPIOC->BSRR=1<<9    //鐗囬€夌鍙�  		PC9
#define    LCD_RS_SET    GPIOC->BSRR=1<<8    //鏁版嵁/鍛戒护 		PC8
#define    LCD_WR_SET    GPIOC->BSRR=1<<7    //鍐欐暟鎹�			PC7
#define    LCD_RD_SET    GPIOC->BSRR=1<<6    //璇绘暟鎹�			PC6

#define    LCD_CS_CLR  GPIOC->BRR=1<<9     //鐗囬€夌鍙�  		PC9
#define    LCD_RS_CLR    GPIOC->BRR=1<<8     //鏁版嵁/鍛戒护		PC8
#define    LCD_WR_CLR    GPIOC->BRR=1<<7     //鍐欐暟鎹�			PC7
#define    LCD_RD_CLR    GPIOC->BRR=1<<6     //璇绘暟鎹�			PC6


//PB0~15,浣滀负鏁版嵁绾�
#define DATAOUT(x) GPIOB->ODR=x; //鏁版嵁杈撳嚭
#define DATAIN     GPIOB->IDR;   //鏁版嵁杈撳叆


//////////////////////////////////////////////////////////////////////
//鎵弿鏂瑰悜瀹氫箟
#define L2R_U2D  0 //浠庡乏鍒板彸,浠庝笂鍒颁笅
#define L2R_D2U  1 //浠庡乏鍒板彸,浠庝笅鍒颁笂
#define R2L_U2D  2 //浠庡彸鍒板乏,浠庝笂鍒颁笅
#define R2L_D2U  3 //浠庡彸鍒板乏,浠庝笅鍒颁笂

#define U2D_L2R  4 //浠庝笂鍒颁笅,浠庡乏鍒板彸
#define U2D_R2L  5 //浠庝笂鍒颁笅,浠庡彸鍒板乏
#define D2U_L2R  6 //浠庝笅鍒颁笂,浠庡乏鍒板彸
#define D2U_R2L  7 //浠庝笅鍒颁笂,浠庡彸鍒板乏

#define DFT_SCAN_DIR  L2R_U2D  //榛樿鐨勬壂鎻忔柟鍚�

//鎵弿鏂瑰悜瀹氫箟
#define L2R_U2D  0 //浠庡乏鍒板彸,浠庝笂鍒颁笅
#define L2R_D2U  1 //浠庡乏鍒板彸,浠庝笅鍒颁笂
#define R2L_U2D  2 //浠庡彸鍒板乏,浠庝笂鍒颁笅
#define R2L_D2U  3 //浠庡彸鍒板乏,浠庝笅鍒颁笂

#define U2D_L2R  4 //浠庝笂鍒颁笅,浠庡乏鍒板彸
#define U2D_R2L  5 //浠庝笂鍒颁笅,浠庡彸鍒板乏
#define D2U_L2R  6 //浠庝笅鍒颁笂,浠庡乏鍒板彸
#define D2U_R2L  7 //浠庝笅鍒颁笂,浠庡彸鍒板乏

#define DFT_SCAN_DIR  L2R_U2D  //榛樿鐨勬壂鎻忔柟鍚�

//鐢荤瑪棰滆壊
#define WHITE             0xFFFF
#define BLACK             0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE             0X07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define GREEN             0x07E0
#define CYAN             0x7FFF
#define YELLOW             0xFFE0
#define BROWN             0XBC40 //妫曡壊
#define BRRED             0XFC07 //妫曠孩鑹�
#define GRAY             0X8430 //鐏拌壊
//GUI棰滆壊

#define DARKBLUE         0X01CF    //娣辫摑鑹�
#define LIGHTBLUE         0X7D7C    //娴呰摑鑹�
#define GRAYBLUE         0X5458 //鐏拌摑鑹�
//浠ヤ笂涓夎壊涓篜ANEL鐨勯鑹�

#define LIGHTGREEN         0X841F //娴呯豢鑹�
#define LGRAY             0XC618 //娴呯伆鑹�(PANNEL),绐椾綋鑳屾櫙鑹�

#define LGRAYBLUE        0XA651 //娴呯伆钃濊壊(涓棿灞傞鑹�)
#define LBBLUE           0X2B12 //娴呮钃濊壊(閫夋嫨鏉＄洰鐨勫弽鑹�)

void LCD_Init(void); // Initialization
void LCD_DisplayOn(void); // Open the display
void LCD_DisplayOff(void); // Close the display
void LCD_Clear(uint16_t Color); // Clear the screen with specific color
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);    // Set the position of cursor
void LCD_DrawPoint(uint16_t x, uint16_t y); // Set color for a point
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color); // Set color for a point(quicker)
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y); // Read the color of a point
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r); // Draw a circle
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // Draw a line
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);    // Draw a rectangle
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);    // Fill the area with color
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);    // Fill the area with color
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);    // Display a char
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,
                 uint8_t size); // Display number without the leading zeros
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size,
                  uint8_t mode); // Display number with the leading zeros
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                    uint8_t size, uint8_t *p); // Display a string
void LCD_ShowImage(uint16_t x, uint16_t y, uint8_t row, uint8_t col,uint8_t *image);
void LCD_Show_Success(uint16_t x, uint16_t y, uint8_t row, uint8_t col);
void LCD_Show_Failure(uint16_t x, uint16_t y, uint8_t row, uint8_t col);

void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);

uint16_t LCD_ReadReg(uint16_t LCD_Reg);

void LCD_WriteRAM_Prepare(void);

void LCD_WriteRAM(uint16_t RGB_Code);

void LCD_Scan_Dir(uint8_t dir); // Set the scan direction
void LCD_Display_Dir(uint8_t dir); // Set the display direction
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);

//鍐欐暟鎹嚱鏁�
#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
}

//LCD鍒嗚鲸鐜囪缃�
#define SSD_HOR_RESOLUTION        800        //LCD姘村钩鍒嗚鲸鐜�
#define SSD_VER_RESOLUTION        480        //LCD鍨傜洿鍒嗚鲸鐜�
//LCD椹卞姩鍙傛暟璁剧疆
#define SSD_HOR_PULSE_WIDTH        1        //姘村钩鑴夊
#define SSD_HOR_BACK_PORCH        46        //姘村钩鍓嶅粖
#define SSD_HOR_FRONT_PORCH        210        //姘村钩鍚庡粖

#define SSD_VER_PULSE_WIDTH        1        //鍨傜洿鑴夊
#define SSD_VER_BACK_PORCH        23        //鍨傜洿鍓嶅粖
#define SSD_VER_FRONT_PORCH        22        //鍨傜洿鍓嶅粖
//濡備笅鍑犱釜鍙傛暟锛岃嚜鍔ㄨ绠�
#define SSD_HT    (SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS    (SSD_HOR_BACK_PORCH)
#define SSD_VT    (SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif












