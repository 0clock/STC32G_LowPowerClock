#ifndef _TASK_H_
#define _TASK_H_

#include "ins5699s.h"
#include "sht41.h"


extern time_t current_time;
extern sht41_data_t sht41;


void test_time_update_1s();
void test_show_dig_num();
void test_update_sht41_500ms();

#endif