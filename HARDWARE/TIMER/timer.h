#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

void TIM3_Int_Init(u16 arr, u16 psc);

void TIM2_Int_Init(u16 arr, u16 psc);

extern u16 Global_delay_time;

extern u16 Global_key_num;

extern u16 Global_press_time;

extern int Global_lcd_x;

extern int Global_lcd_y;

extern u16 Global_flag;

extern int xdir;

extern int ydir;

extern int Global_done_sign;

extern int change;
#endif
