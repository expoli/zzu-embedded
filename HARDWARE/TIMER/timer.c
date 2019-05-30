#include "timer.h"
#include "led.h"
#include "key.h"

#include "math.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 
//ȫ�ֱ���
u16 Global_delay_time = 5000; //��ʱʱ��
u16 Global_key_num = 0;		 //����
u16 Global_press_time = 0;   //����ʱ��
u16 last_press = 0;			 //�������Ʊ���
double Global_lcd_x = 0;		 //LCDX�����
double Global_lcd_y = 0;		 //LCDY�����
u16 Global_flag = 0;
u16 time = 0;
u16 count = 0;
int xdir = 1;				 //����������
int ydir = 1;				 //
double temp = 0;

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void triangle_wave(void);
void square_wave(void);
void sin_wave(void);

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			Global_key_num = KEY_Scan(1);
			time ++;
			if (Global_key_num)
			{
				last_press=1;
			}
			else 
			{
				last_press=0;
			}
			if (last_press&&Global_key_num)
			{
				Global_press_time ++;
			}
			sin_wave();

		}
}
// ���ǲ�
void triangle_wave(void)
{
	/////////////////////////////////////			
	if(time%(Global_delay_time/100)==0)//100Ϊ100ms  	 
	{
		if (Global_lcd_x == 0)
			xdir = 1;
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}

		if (Global_lcd_y == 0)
			ydir = 1;
		if (Global_lcd_y >= 50)
			ydir = -1;
		
		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //

		printf("%d,%d,%d,%d",Global_lcd_x,Global_lcd_y,Global_press_time,Global_delay_time);
		printf("\r\n");
	}
}
// ����
void square_wave(void)
{
	/////////////////////////////////////			
	if(time%(Global_delay_time/100)==0)//100Ϊ100ms  	 
	{
		count ++;
		// ��һ������
		if (count <= 50)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��һ������
		if (count > 50 && count <=100)
		{
			xdir = 0;
			ydir = 1;
		}
		// �ڶ�������
		if (count > 100 && count <=150)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��2������
		if (count > 150 && count <=200)
		{
			xdir = 0;
			ydir = -1;
		}
		// ��3������
		if (count > 200 && count <=250)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��3������
		if (count > 250 && count <=300)
		{
			xdir = 0;
			ydir = 1;
		}
		// ��4������
		if (count > 300 && count <=350)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��4������
		if (count > 350 && count <=400)
		{
			xdir = 0;
			ydir = -1;
		}
		// ��5������
		if (count > 400 && count <=450)
		{
			xdir = 1;
			ydir = 0;
		}
		// ����Χ
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
			count = 0;
		}
		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //

		printf("%d,%d,%d,%d",Global_lcd_x,Global_lcd_y,Global_press_time,Global_delay_time);
		printf("\r\n");
	}
}

// ���Ҳ�

void sin_wave(void)
{
	/////////////////////////////////////			
	if(time%(Global_delay_time/100)==0)//100Ϊ100ms  	 
	{
		if (Global_lcd_x == 0)
			xdir = 1;
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}
		
		Global_lcd_x = Global_lcd_x + xdir; //
		temp = Global_lcd_x;
		if((int)temp%2==0)Global_lcd_y = 30*sin(3*temp); //
		else Global_lcd_y = -30*sin(3*temp);
		Global_lcd_y = (int)Global_lcd_y;

		printf("%f,%f,%d,%d",Global_lcd_x,Global_lcd_y,Global_press_time,Global_delay_time);
		printf("\r\n");
	}
}









