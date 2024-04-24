#ifndef __ADC_H
#define __ADC_H	
#include "STC32G.h"

void ADC_init();


u16 Get_ADC12bitResult(u8 channel);

void ADC_Get_V1();

void Battery_percentage(u32 a);

void ADC_falg_0040();

#endif