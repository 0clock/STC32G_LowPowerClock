#include "dmx_all.h"

time_t current_time;
/**
 * @brief 打印time_t结构体中的时间信息
 *
 * @param time 指向time_t结构体的指针，用于打印时间数据
 * @return void
 */
void PrintTime(const time_t *time)
{
    printf("当前时间: %02u-%02u-%02u 星期%u %02u:%02u:%02u\n",
           time->year,
           time->month,
           time->day,
           time->week,
           time->hour,
           time->min,
           time->sec);
}

void home_page()
{
    oled_clear(oled_gram, 0);
    oled_draw_int(0, 0, current_time.hour, 2, oled_gram, 1, Show8x16);
    oled_draw_char(36, 0, ":", oled_gram, 1, Show8x16);
    oled_draw_int(40, 0, current_time.min, 2, oled_gram, 1, Show8x16);
    oled_draw_char(76, 0, ":", oled_gram, 1, Show8x16);
    oled_draw_int(80, 0, current_time.sec, 2, oled_gram, 1, Show8x16);
    oled_refresh_flag = 1;
}

void test_time_update_1s()
{
    ins5699_read_time(&current_time);
    PrintTime(&current_time);
    // home_page();
}

void test_show_dig_num()
{
    if (oled_refresh_flag == 0)
    {
        static unsigned char i = 0;
        oled_clear(oled_gram, 0);
        show_time();
        // show_dig_num(10, 10, (i++) % 10, oled_gram, 1);
        oled_refresh_flag = 1;
        // delay_ms(100);
    }
}


