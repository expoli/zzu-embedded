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

void my_LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);

void init(void)
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


