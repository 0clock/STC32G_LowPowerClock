#ifndef __DATA_H
#define __DATA_H	
#include "STC32G.h"

#define     UI_main_branchnum       15   //主菜单分支数量-1


void data00_Change(u16 address,u16 add);

void dat00_Change();

void array_fill(u16 *array_name,u16 array_num,u16 array_dat,u8 w);

u16 data_GetMax(u16 dat);

u16 data_GetMin(u16 dat);

u16 GET_SC();

u16 GET_SCdat();

u16 GET_GBdat();

u16 GET_SCdat3();

void SleepQueue_MoveLeft(u8 p);

void SleepQueue_aheadW1(u16 time,u16 num,u8 f);

u8 RFF();

void fast_SleepQueue_aheadW1(u16 dat,u16 task,u8 num);

void fast_SleepQueue_aheadW2();

void SleepQueue_behindW(u16 time,u16 num);

void SleepQueue_MoveLeft2();//睡眠队列_向左移动

void SleepQueue_MoveLeft2();

void GET_time_c();

void SET_time_c();

bit Judging_Range(u16 a,u16 b, u16 c);

u16 Abs_Difference_16(u16 a,u16 b);

void SET_time_c();

void Array_Initialization_();

void Alarm_conversion_16TO32(u8 a);

void Alarm_conversion_32TO16(u8 a);

#endif