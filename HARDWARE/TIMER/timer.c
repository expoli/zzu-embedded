#include "timer.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "math.h"

//全局变量
u16 Global_delay_time = 2; //延时时间
u16 Global_key_num = 0;	//按键
u16 Global_press_time = 0; //按键时间
u16 last_press = 0;		   //长按控制变量
int Global_lcd_x = 0;	  //LCDX轴变量
int Global_lcd_y = 0;	  //LCDY轴变量
int Global_lcd_last_x = 0;
int Global_lcd_last_y = 0;
int Global_done_sign = 0;
u16 time = 0;	// 时间记录器
u16 count = 0;   // 绘图控制变量
int xdir = 1;	//x递增控制器
int ydir = 1;	//y递增控制器
double temp = 0; // sin函数
int change = 0;

// 三角波
void triangle_wave(void);
// 方波
void square_wave(void);
// 正弦波
void sin_wave(void);
// 锯齿波
// 500
void juchi_wave(void);

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig( //使能或者失能指定的TIM中断
		TIM3,	 //TIM2
		TIM_IT_Update,
		ENABLE //使能
	);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);							  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE); //使能TIMx外设
}

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig( //使能或者失能指定的TIM中断
		TIM2,	 //TIM2
		TIM_IT_Update,
		ENABLE //使能
	);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);							  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE); //使能TIMx外设
}

void TIM3_IRQHandler(void) //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除TIMx的中断待处理位:TIM 中断源

		// 按键值
		Global_key_num = KEY_Scan(1);
		// 时间记录器
		// time++;
		// 长按控制
		if (Global_key_num)
		{
			last_press = 1;
		}
		else
		{
			last_press = 0;
		}
		// 按键时间
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

// 锯齿波
// 500
void juchi_wave(void)
{

	count++;
	// 第一条斜线
	if (count <= 50) //&&Global_lcd_x == 0&&Global_lcd_y == 0
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// 第一条竖线
	else if (count > 50 && count <= 100)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// 第二条斜线
	else if (count > 100 && count <= 150) //&&Global_lcd_x == 0&&Global_lcd_y == 0
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// 第2条竖线
	else if (count > 150 && count <= 200)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// 第3条斜线
	else if (count > 200 && count <= 250)
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// 第3条竖线
	else if (count > 250 && count <= 300)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// 第4条斜线
	else if (count > 300 && count <= 350)
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// 第4条竖线
	else if (count > 350 && count <= 400)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// 第5条斜线
	else if (count > 400 && count <= 450)
	{
		xdir = 1;
		ydir = -1;
		change = 1;
	}
	// 第5条竖线
	else if (count > 450 && count <= 500)
	{
		xdir = 0;
		ydir = 1;
		change = 1;
	}
	// // 第6条斜线
	// else if (count > 500 && count <= 550)
	// {
	// 	xdir = 1;
	// 	ydir = -1;
	// }
	// // 第6条竖线
	// else if (count > 550 && count <= 600)
	// {
	// 	xdir = 0;
	// 	ydir = 1;
	// }
	// 定范围
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

void TIM2_IRQHandler(void) //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除TIMx的中断待处理位:TIM 中断源
		// 按键值
		Global_key_num = KEY_Scan(1);
		// 时间记录器
		// time++;
		// 长按控制
		if (Global_key_num)
		{
			last_press = 1;
		}
		else
		{
			last_press = 0;
		}
		// 按键时间
		if (last_press && Global_key_num)
		{
			Global_press_time++;
		}
		// 画图函数
		//triangle_wave();
		//square_wave();
		//sin_wave();
	}
}

// 三角波
void triangle_wave(void)
{
	/////////////////////////////////////
	// 软件分频
	if (time % (Global_delay_time / 100) == 0) //100为100ms
	{
		// X轴步进量
		if (Global_lcd_x == 0)
			xdir = 1;
		// 控制范围
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}
		// Y三角波绘制
		if (Global_lcd_y == 0)
			ydir = 1;
		if (Global_lcd_y >= 50)
			ydir = -1;

		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //
		// 串口
		printf("%d,%d,%d,%d", Global_lcd_x, Global_lcd_y, Global_press_time, Global_delay_time);
		printf("\r\n");
	}
}

// 方波
void square_wave(void)
{
	/////////////////////////////////////
	if (time % (Global_delay_time / 100) == 0) //100为100ms
	{
		count++;
		// 第一条横线
		if (count <= 50)
		{
			xdir = 1;
			ydir = 0;
		}
		// 第一条竖线
		if (count > 50 && count <= 100)
		{
			xdir = 0;
			ydir = 1;
		}
		// 第二条横线
		if (count > 100 && count <= 150)
		{
			xdir = 1;
			ydir = 0;
		}
		// 第2条竖线
		if (count > 150 && count <= 200)
		{
			xdir = 0;
			ydir = -1;
		}
		// 第3条横线
		if (count > 200 && count <= 250)
		{
			xdir = 1;
			ydir = 0;
		}
		// 第3条竖线
		if (count > 250 && count <= 300)
		{
			xdir = 0;
			ydir = 1;
		}
		// 第4条横线
		if (count > 300 && count <= 350)
		{
			xdir = 1;
			ydir = 0;
		}
		// 第4条竖线
		if (count > 350 && count <= 400)
		{
			xdir = 0;
			ydir = -1;
		}
		// 第5条横线
		if (count > 400 && count <= 450)
		{
			xdir = 1;
			ydir = 0;
		}
		// 定范围
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

// 正弦波

void sin_wave(void)
{
	/////////////////////////////////////
	if (time % (Global_delay_time / 100) == 0) //100为100ms
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
