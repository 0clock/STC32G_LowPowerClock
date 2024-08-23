#ifndef __BMS_H_
#define __BMS_H_

#define pin_battery_adc ADC12_P04
#define pin_current_adc ADC11_P03

typedef struct bms
{
    unsigned int voltage;
    unsigned int current;
    unsigned char soc;
    unsigned char is_charge : 1; 
}bms_t;


extern bms_t battery;

void bms_init();
void bms_main_task_500ms();



#endif

#if 0
#ifndef __ADC_H
#define __ADC_H
#include "STC32F.h"

void ADC_init();


u16 Get_ADC12bitResult(u8 channel);

void ADC_Get_V1();

void Battery_percentage(u32 a);

void ADC_falg_0040();

#endif

#endif