#ifndef _INS5699S_H_
#define _INS5699S_H_

/**
 * @brief 表示时间的结构体
 * 
 * 该结构体包含了时间的各个组成部分，包括年、月、日、
 * 星期、小时、分钟和秒钟。
 */
typedef struct
{
    unsigned char year;  /**< 年（0-255） */
    unsigned char month; /**< 月（1-12） */
    unsigned char day;   /**< 日（1-31） */
    unsigned char week;  /**< 星期（0-6, 0=星期天） */
    unsigned char hour;  /**< 小时（0-23） */
    unsigned char min;   /**< 分钟（0-59） */
    unsigned char sec;   /**< 秒钟（0-59） */
} time_t;

void rtc_init();
void ins5699_read_time(time_t *time);

#endif

#if 0
#ifndef __INS5699S_H
#define __INS5699S_H
#include "STC32F.h"

void INS5699_timeR();

void INS5699_timeW(u8 *a);

void INS5699_timer_clean();

void INS5699_Initialization();

#endif
#endif