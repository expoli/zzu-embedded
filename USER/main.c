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
		// ���̰��ֱ溯��
		long_or_short_press();

		if (change == 1)
		{

			LCD_DrawPoint(Global_lcd_x, Global_lcd_y + 130);
			// ����
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

//��ʾ����,��λΪ0,����ʾ
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);

// ��ʼ������
void init(void)
{
	u8 lcd_id[12];	 //���LCD ID�ַ���
	delay_init();	  //��ʱ������ʼ��
	uart_init(115200); //���ڳ�ʼ��Ϊ9600
	LED_Init();		   //��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();
	KEY_Init();

	POINT_COLOR = RED;
	sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id); //��LCD ID��ӡ��lcd_id���顣

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����2
	TIM3_Int_Init(99, 719);							// 10Khz�ļ���Ƶ�ʣ�������99Ϊ10ms
	//TIM2_Int_Init(99,7199);	// 72*10^6 /72*10^3 1ms

	LCD_Clear(WHITE); // ����
}

int step = 1;
// ���̰��ֱ溯��
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
