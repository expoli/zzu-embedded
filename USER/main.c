#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "timer.h"
#include "key.h"

void init(void);

void long_or_short_press(void);

int main(void)
{
	init();
	while (1)
	{
		// 长短按分辨函数
		long_or_short_press();

		if (change == 1)
		{

			LCD_DrawPoint(Global_lcd_x, Global_lcd_y + 130);
			// 串口
			printf("%d,%d,%d,%d", Global_lcd_x, Global_lcd_y, Global_press_time, Global_delay_time);
			printf("\r\n");
			LCD_ShowString(0, 180, 200, 24, 24, "T=");
			LCD_ShowNum(10, 180, (Global_delay_time)*500, 6, 24);
			change = 0;
		}

		if (Global_done_sign)
		{
			LCD_Clear(WHITE);
			Global_done_sign = 0;
		}
	}
}

//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);

// 初始化函数
void init(void)
{
	u8 lcd_id[12];	 //存放LCD ID字符串
	delay_init();	  //延时函数初始化
	uart_init(115200); //串口初始化为9600
	LED_Init();		   //初始化与LED连接的硬件接口
	LCD_Init();
	KEY_Init();

	POINT_COLOR = RED;
	sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id); //将LCD ID打印到lcd_id数组。

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组2
	TIM3_Int_Init(99, 719);							// 10Khz的计数频率，计数到99为10ms
	//TIM2_Int_Init(99,7199);	// 72*10^6 /72*10^3 1ms

	LCD_Clear(WHITE); // 清屏
}

int step = 1;
// 长短按分辨函数
void long_or_short_press(void)
{
	if (Global_press_time)
	{
		if (Global_press_time >= 1 && Global_press_time <= 60)
		{
			Global_delay_time += step;
			Global_press_time = 0;
		}
		else if (Global_press_time > 60)
		{
			Global_delay_time -= step;
			Global_press_time = 0;
		}
		if (Global_delay_time <= 2)
		{
			Global_delay_time = 2;
		}
		if (Global_press_time >= 400)
		{
			Global_press_time = 0;
		}
	}
}
