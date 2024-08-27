#include "dmx_all.h"
#define TEST_BUTTON 0
#define TEST_SHT41 1
time_t current_time;
sht41_data_t sht41;
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
void test_update_sht41_500ms()
{
    static char count = 0;
    if (count % 2 == 0)
    {
        sht41_start_measurement();
    }
    else
    {
        sht41_read_data(&sht41);
    }
    count++;
}
void test_show_dig_num()
{
    if (oled_refresh_flag == 0)
    {
        static unsigned char i = 0;
#if TEST_BUTTON
        static button_event last_event = BUTTON_EVENT_NONE;
        button_event now_eventv = button_get_event(BUTTON_SET);
#endif
        oled_clear(oled_gram, 0);
#if TEST_BUTTON
        if (last_event != now_eventv && now_eventv != BUTTON_EVENT_NONE || last_event == BUTTON_EVENT_NONE)
        {
            last_event = now_eventv;
        }

        switch (last_event)
        {
        case BUTTON_EVENT_LONG_PRESS:
            oled_draw_string(0, 0, "long_press", oled_gram, 1, Show6x8);
            break;
        case BUTTON_EVENT_NONE:
            oled_draw_string(0, 0, "none_press", oled_gram, 1, Show6x8);
            break;
        case BUTTON_EVENT_CLICK:
            oled_draw_string(0, 0, "single_click", oled_gram, 1, Show6x8);
            break;
        case BUTTON_EVENT_DOUBLE_CLICK:
            oled_draw_string(0, 0, "double_click", oled_gram, 1, Show6x8);
            break;

        default:
            break;
        }
#endif

#if TEST_SHT41
        oled_draw_string(0, 0, "Temp:", oled_gram, 1, Show6x8);
        oled_draw_int(30, 0, sht41.temperature, 5, oled_gram, 1, Show6x8);

        oled_draw_string(0, 10, "Humi:", oled_gram, 1, Show6x8);
        oled_draw_int(30, 10, sht41.humidity, 5, oled_gram, 1, Show6x8);
#endif
        show_time(&current_time);
        // show_dig_num(10, 10, (i++) % 10, oled_gram, 1);
        oled_refresh_flag = 1;
        // delay_ms(100);
    }
}
