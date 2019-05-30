#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "timer.h"
#include "key.h"

//ALIENTEK Mini STM32开发板范例代码11
//TFTLCD显示实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 int main(void)
 { 
	u8 lcd_id[12];			//存放LCD ID字符串	
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();
	KEY_Init();
	 
	POINT_COLOR=RED; 
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。	
	 
	
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	TIM3_Int_Init(99,7199);//10Khz的计数频率，计数到5000为500ms  	 
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
