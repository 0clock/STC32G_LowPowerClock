#ifndef __SleepMode_H
#define __SleepMode_H
#include "STC32F.h"




void SleepMode_ON();

void SleepMode_OFF();

void Sleep_task();

void Sleep_Display_int();

void Sleep_Display_UI();

u8 Decide_AT();


#endif