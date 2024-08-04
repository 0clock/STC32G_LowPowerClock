#ifndef __DATA_H
#define __DATA_H
#include "STC32F.h"

#define     UI_main_branchnum       15   //主菜单分支数量-1


void data00_Change(u16 address,u16 add);

void array_fill(u16 *array_name,u16 array_num,u16 array_dat,u16 w);

u16 data_GetMax(u16 dat);

u16 data_GetMin(u16 dat);

u16 GET_SC();

u16 GET_SCdat();

u16 GET_GBdat();

u16 GET_SCdat3();

void SleepQueue_MoveLeft(u16 p);

void SleepQueue_aheadW1(u16 time,u16 num,u16 f);

u16 RFF();

void fast_SleepQueue_aheadW1(u16 dat,u16 task,u8 num);




void fast_SleepQueue_aheadW2();

void SleepQueue_behindW(u16 time,u16 num);

void SleepQueue_MoveLeft2();//睡眠队列_向左移动

void SleepQueue_MoveLeft2();

void GET_time_c();

void SET_time_c();

u8 Judging_Range(u16 a,u16 b, u16 c);

u16 Abs_Difference_16(u16 a,u16 b);

void SET_time_c();

void Array_Initialization_();


u8 X10_to_BCD(u16 a);

u8 BCD_to_X10(u8 a);

u16 Date_restrictions1(u16 year,u16 moon,u16 day_addr,u16 max);



#endif