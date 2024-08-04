#ifndef __INS5699S_H
#define __INS5699S_H	
#include "STC32F.h"

void INS5699_timeR();

void INS5699_timeW(u8 *a);

void time_set_thread();

void INS5699_timer_clean();


#endif