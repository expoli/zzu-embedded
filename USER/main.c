#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "timer.h"
#include "key.h"

//ALIENTEK Mini STM32�����巶������11
//TFTLCD��ʾʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

void my_LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);

void init(void)
{
	u8 lcd_id[12];			//���LCD ID�ַ���	
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	LCD_Init();
	KEY_Init();
	POINT_COLOR=RED; 
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣	
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	TIM3_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  	 
	LCD_Clear(WHITE);
	//LCD_ShowString(30,40,200,24,24,"1");
	TIM2_Int_Init(5000,7199);
}

 int main(void)
 { 
	init();
  while(1) 
	{		 
//			if (Global_press_time)
//			{
//				if (Global_press_time>=1&&Global_press_time<=100)
//				{
//					 Global_delay_time += 500;
//					Global_press_time = 0;
//				}
//				else if (Global_press_time >= 100)
//				{
//					Global_delay_time -= 500;
//					Global_press_time = 0;
//				}
//				if (Global_delay_time>=1500)
//				{
//					Global_delay_time=1500;
//				}
//				if (Global_delay_time<=500)
//				{
//					Global_delay_time=500;
//				}
//				if (Global_press_time>=400)
//				{
//					Global_press_time = 0;
//				}
//				if (Global_delay_time==500)
//				{
//					LCD_ShowString(30,40,200,24,24,"1");
//				}
//				else if (Global_delay_time==1000)
//				{
//					LCD_ShowString(30,40,200,24,24,"2");
//				}
//				else if (Global_delay_time==1500)
//				{
//					LCD_ShowString(30,40,200,24,24,"3");
//				}

//			}

;
		//delay_ms(300);
	}
}


