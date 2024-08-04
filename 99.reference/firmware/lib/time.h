#ifndef __TIME_H_
#define __TIME_H_

#include "headfile.h"


void Timer0Init(void);

void Delay50us();

void delay_ms(u8 ms);

void Delay10us();

void Delay();

void Sleep_delay_HalfMs(u16 ms);

void Sleep_delay_Time(u16 sleep_dat);

void Sleep_Thread();



#endif