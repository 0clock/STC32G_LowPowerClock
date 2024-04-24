#ifndef __IO_H
#define __IO_H
#include "STC32F.h"

void OLED_POW(u8 dat);

void OLED_POW_LOW();

void RESET54();

void key_int();

void Clock_INT();

void Buzzer_Open();

void Buzzer_Close();



void Buzzer_falg_0020();

void Battery_ON_ON();

void Battery_ON_OFF();

void ICacheOn(void);     //打开ICACHE功能

void ICacheOff(void);//关闭ICACHE功能

void IO_Init(void);





#endif