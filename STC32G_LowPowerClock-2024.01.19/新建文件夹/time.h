#ifndef __time_H
#define __time_H	
#include "STC32F.h"
#include "intrins.h"


void Timer0Init(void);

void Delay50us();

void delay_ms(u8 ms);

void Delay10us();

void Delay();

void Sleep_delay_HalfMs(u16 ms);

void Sleep_delay_Time(u16 sleep_dat);

void Sleep_Thread();
	


#endif