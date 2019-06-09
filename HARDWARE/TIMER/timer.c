#include "timer.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "math.h"

//ȫ�ֱ���
u16 Global_delay_time = 2; //��ʱʱ��
u16 Global_key_num = 0;	//����
u16 Global_press_time = 0; //����ʱ��
u16 last_press = 0;		   //�������Ʊ���
int Global_lcd_x = 0;	  //LCDX�����
int Global_lcd_y = 0;	  //LCDY�����
int Global_lcd_last_x = 0;
int Global_lcd_last_y = 0;
int Global_done_sign = 0;
u16 time = 0;	// ʱ���¼��
u16 count = 0;   // ��ͼ���Ʊ���
int xdir = 1;	//x����������
int ydir = 1;	//y����������
double temp = 0; // sin����
int change = 0;

// ���ǲ�
void triangle_wave(void);
// ����
void square_wave(void);
// ���Ҳ�
void sin_wave(void);
// ��ݲ�
// 500
void juchi_wave(void);

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig( //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3,	 //TIM2
		TIM_IT_Update,
		ENABLE //ʹ��
	);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE); //ʹ��TIMx����
}

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig( //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2,	 //TIM2
		TIM_IT_Update,
		ENABLE //ʹ��
	);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE); //ʹ��TIMx����
}

void TIM3_IRQHandler(void) //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���TIMx���жϴ�����λ:TIM �ж�Դ

		// ����ֵ
		Global_key_num = KEY_Scan(1);
		// ʱ���¼��
		// time++;
		// ��������
		if (Global_key_num)
		{
			last_press = 1;
		}
		else
		{
			last_press = 0;
		}
		// ����ʱ��
		if (last_press && Global_key_num)
		{
			Global_press_time++;
		}

		time++;
		if (time % Global_delay_time == 0)
		{
			juchi_wave();
		}
	}
}

// ��ݲ�
// 500
void juchi_wave(void)
{

	count++;
	// ��һ��б��
	if (count <= 50) //&&Global_lcd_x == 0&&Global_lcd_y == 0
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// ��һ������
	else if (count > 50 && count <= 100)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// �ڶ���б��
	else if (count > 100 && count <= 150) //&&Global_lcd_x == 0&&Global_lcd_y == 0
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// ��2������
	else if (count > 150 && count <= 200)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// ��3��б��
	else if (count > 200 && count <= 250)
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// ��3������
	else if (count > 250 && count <= 300)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// ��4��б��
	else if (count > 300 && count <= 350)
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// ��4������
	else if (count > 350 && count <= 400)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// ��5��б��
	else if (count > 400 && count <= 450)
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// ��5������
	else if (count > 450 && count <= 500)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// // ��6��б��
	// else if (count > 500 && count <= 550)
	// {
	// 	xdir = 1;
	// 	ydir = -1;
	// }
	// // ��6������
	// else if (count > 550 && count <= 600)
	// {
	// 	xdir = 0;
	// 	ydir = 1;
	// }
	// ����Χ
	else if (count >= 500)
	{
		Global_lcd_x = 0;
		Global_lcd_y = 0;
		count = 0;
		Global_done_sign = 1;
	}
	if (change == 1)
	{
		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //
	}
}

void TIM2_IRQHandler(void) //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //���TIMx���жϴ�����λ:TIM �ж�Դ
		// ����ֵ
		Global_key_num = KEY_Scan(1);
		// ʱ���¼��
		// time++;
		// ��������
		if (Global_key_num)
		{
			last_press = 1;
		}
		else
		{
			last_press = 0;
		}
		// ����ʱ��
		if (last_press && Global_key_num)
		{
			Global_press_time++;
		}
		// ��ͼ����
		//triangle_wave();
		//square_wave();
		//sin_wave();
	}
}

// ���ǲ�
void triangle_wave(void)
{
	/////////////////////////////////////
	// �����Ƶ
	if (time % (Global_delay_time / 100) == 0) //100Ϊ100ms
	{
		// X�Ჽ����
		if (Global_lcd_x == 0)
			xdir = 1;
		// ���Ʒ�Χ
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}
		// Y���ǲ�����
		if (Global_lcd_y == 0)
			ydir = 1;
		if (Global_lcd_y >= 50)
			ydir = -1;

		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //
		// ����
		printf("%d,%d,%d,%d", Global_lcd_x, Global_lcd_y, Global_press_time, Global_delay_time);
		printf("\r\n");
	}
}

// ����
void square_wave(void)
{
	/////////////////////////////////////
	if (time % (Global_delay_time / 100) == 0) //100Ϊ100ms
	{
		count++;
		// ��һ������
		if (count <= 50)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��һ������
		if (count > 50 && count <= 100)
		{
			xdir = 0;
			ydir = 1;
		}
		// �ڶ�������
		if (count > 100 && count <= 150)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��2������
		if (count > 150 && count <= 200)
		{
			xdir = 0;
			ydir = -1;
		}
		// ��3������
		if (count > 200 && count <= 250)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��3������
		if (count > 250 && count <= 300)
		{
			xdir = 0;
			ydir = 1;
		}
		// ��4������
		if (count > 300 && count <= 350)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��4������
		if (count > 350 && count <= 400)
		{
			xdir = 0;
			ydir = -1;
		}
		// ��5������
		if (count > 400 && count <= 450)
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

		printf("%d,%d,%d,%d", Global_lcd_x, Global_lcd_y, Global_press_time, Global_delay_time);
		printf("\r\n");
	}
}

// ���Ҳ�

void sin_wave(void)
{
	/////////////////////////////////////
	if (time % (Global_delay_time / 100) == 0) //100Ϊ100ms
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
		if ((int)temp % 2 == 0)
			Global_lcd_y = 30 * sin(3 * temp); //
		else
			Global_lcd_y = -30 * sin(3 * temp);
		Global_lcd_y = (int)Global_lcd_y;

		printf("%d,%d,%d,%d", Global_lcd_x, Global_lcd_y, Global_press_time, Global_delay_time);
		printf("\r\n");
	}
}
