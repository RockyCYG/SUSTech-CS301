#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "malloc.h" 
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "string.h"
#include "usmart.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"	
#include "string.h"		
#include "math.h"
#include "remote.h"
/************************************************
 ALIENTEK Mini STM32F103������ʵ��31
 ͼƬ��ʾʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 �������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO *tfileinfo;	//��ʱ�ļ���Ϣ	    			     
	tfileinfo=(FILINFO*)mymalloc(sizeof(FILINFO));//�����ڴ�
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼ 
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�  	 
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//������/��ĩβ��,�˳�	 		 
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	}  
	myfree(tfileinfo);//�ͷ��ڴ�
	return rval;
}
u8 *str=0;	
u16 curindex=0;						//ͼƬ��ǰ����
int key;								//��ֵ
int main(void)
{ 
	u8 res;
 	DIR picdir;	 						//ͼƬĿ¼
	FILINFO *picfileinfo;				//�ļ���Ϣ 
	u8 *pname;							//��·�����ļ���
	u16 totpicnum; 						//ͼƬ�ļ�����
	u8 pause=1;							//��ͣ���?
	u8 t;
	u16 temp;
	u32 *picoffsettbl;					//ͼƬ�ļ�offset������
    char tempstr[50];
	
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	usmart_dev.init(84); 		  	  	//��ʼ��USMART	
	LED_Init();							//��ʼ��LED	
	KEY_Init();							//��ʼ������
 	LCD_Init();							//��ʼ��LCD
 	mem_init();							//��ʼ���ڴ��?
	
	Remote_Init();				    	//��ʼ��	�������?	
	
	exfuns_init();						//Ϊfatfs��ر��������ڴ�?  
 	f_mount(fs[0],"0:",1); 				//����SD�� 
 	f_mount(fs[1],"1:",1); 				//����FLASH.
	POINT_COLOR=RED;      
/*	while(font_init()) 					//����ֿ�?
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//������?	     
		delay_ms(200);				  
	}  	 
 	Show_Str(30,50,200,16,"Mini STM32������",16,0);				    	 
	Show_Str(30,70,200,16,"ͼƬ��ʾ����",16,0);				    	 
	Show_Str(30,90,200,16,"KEY0:NEXT KEY1:PREV",16,0);				    	 
	Show_Str(30,110,200,16,"KEY_UP:PAUSE",16,0);				    	 
	Show_Str(30,130,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
	Show_Str(30,150,200,16,"2019��11��18��",16,0);*/
 	while(f_opendir(&picdir,"0:/PICTURE"))//��ͼƬ�ļ���
 	{	    
		Show_Str(30,170,240,16,"PICTURE FILE WRONG!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,170,240,186,WHITE);//������?	     
		delay_ms(200);				  
	}  
	totpicnum=pic_get_tnum("0:/PICTURE"); //�õ�����Ч�ļ���
  	while(totpicnum==NULL)//ͼƬ�ļ�Ϊ0		
 	{	    
		Show_Str(30,170,240,16,"NO PICTURE FILE!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,170,240,186,WHITE);//������?	     
		delay_ms(200);				  
	} 
	picfileinfo=(FILINFO*)mymalloc(sizeof(FILINFO));	//�����ڴ�
 	pname=mymalloc(_MAX_LFN*2+1);					//Ϊ��·�����ļ��������ڴ�
 	picoffsettbl=mymalloc(4*totpicnum);				//����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����?
 	while(!picfileinfo||!pname||!picoffsettbl)				//�ڴ�������
 	{	    	
		Show_Str(30,170,240,16,"Memory allocation failure!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,170,240,186,WHITE);//������?	     
		delay_ms(200);				  
	}  	
	//��¼����
    res=f_opendir(&picdir,"0:/PICTURE"); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=picdir.dptr;								//��¼��ǰdptrƫ��
	        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�	 	 
			res=f_typetell((u8*)picfileinfo->fname);	
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				picoffsettbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}   
	Show_Str(30,170,240,16,"Start Show...",16,0); 
	delay_ms(1500);
	piclib_init();										//��ʼ����ͼ	   	   
	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
		dir_sdi(&picdir,picoffsettbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�
		strcpy((char*)pname,"0:/PICTURE/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)picfileinfo->fname);//���ļ������ں���
 		LCD_Clear(BLACK);
 		ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);//��ʾͼƬ    
		Show_Str(2,2,lcddev.width,16,pname,16,1); 				//��ʾͼƬ����
        //��ʾͼƬ����
        sprintf(tempstr, "Number of pictures: %d",totpicnum);
        Show_Str(2,20,240,16,(u8*)tempstr,16,1);
		t=0;
		while(1) 
		{
//			key=Remote_Scan();
			//key=KEY_Scan(0);		//ɨ�谴��
            if(t>250)key=194;			//ģ��һ�ΰ���KEY0
			if((t%20)==0)LED0=!LED0;//LED0��˸,��ʾ������������.
			if(key==34)		//��һ�� LEFT
			{
				if(curindex)curindex--;
				else curindex=totpicnum-1;
				key=0;
				break;
			}else if(key==194)//��һ�� RIGHT
			{
				curindex++;		   	
				if(curindex>=totpicnum)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
				key=0;
				break;
			}else if(key==162) //POWER��ͣ
			{
				pause=!pause;
				LED1=!pause; 	//��ͣ��ʱ��LED1��.
				key=0;
			}
			if(pause==0)t++;
			delay_ms(10); 
		}					    
		res=0;  
	} 							    
	myfree(picfileinfo);			//�ͷ��ڴ�						   		    
	myfree(pname);				//�ͷ��ڴ�			    
	myfree(picoffsettbl);		//�ͷ��ڴ�
}

