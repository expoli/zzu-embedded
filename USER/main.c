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
 int main(void)
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
  while(1) 
	{		 
			if (Global_press_time)
			{
				if (Global_press_time>=1&&Global_press_time<=60)
				{
					 Global_delay_time += 1000;
					Global_press_time = 0;
				}
				else if (Global_press_time > 60)
				{
					Global_delay_time -= 1000;
					Global_press_time = 0;
				}
				if (Global_delay_time>=3000)
				{
					Global_delay_time=3000;
				}
				if (Global_delay_time<=1000)
				{
					Global_delay_time=1000;
				}
				if (Global_press_time>=400)
				{
					Global_press_time = 0;
				}
				if (Global_delay_time==1000)
				{
					LCD_ShowString(30,40,200,24,24,"1");
				}
				else if (Global_delay_time==2000)
				{
					LCD_ShowString(30,40,200,24,24,"2");
				}
				else if (Global_delay_time==3000)
				{
					LCD_ShowString(30,40,200,24,24,"3");
				}
			}

		LCD_DrawPoint(Global_lcd_x,Global_lcd_y);
		LCD_DrawPoint(Global_lcd_x,Global_lcd_y+60);
		LCD_DrawPoint(Global_lcd_x,Global_lcd_y+120);
	}
}
