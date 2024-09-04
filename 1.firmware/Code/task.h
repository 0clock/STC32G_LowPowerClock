#ifndef _TASK_H_
#define _TASK_H_

#include "ins5699s.h"
#include "sht41.h"

typedef enum
{
    TASKID_NORMAL,
    TASKID_SET_TIME
} task_module;

typedef enum
{
    SET_HOUR,
    SET_MIN,
    SET_SEC,
    SET_YEAR,
    SET_MON,
    SET_DAY,
    SET_WEEK
} which_time_set_t;


extern task_module task_id;
extern which_time_set_t which_time_set;
extern time_t current_time;
extern sht41_data_t sht41;


void test_time_update_1s();
void task_show_time();
void test_update_sht41_500ms();
void test_switch_module_task_main();

#endif