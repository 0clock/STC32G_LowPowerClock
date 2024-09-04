#include "dmx_all.h"
#define TEST_BUTTON 0
#define TEST_SHT41 0
time_t current_time;
time_t set_time;
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

task_module task_id = TASKID_NORMAL;

// 设置时间交互界面

which_time_set_t which_time_set = SET_HOUR;

void task_set_time()
{
    button_event beid = button_get_event(BUTTON_SET);
    if (BUTTON_EVENT_CLICK == beid)
    {
        switch (which_time_set)
        {
        case SET_HOUR:
            which_time_set = SET_MIN;
            break;
        case SET_MIN:
            which_time_set = SET_SEC;
            break;
        case SET_SEC:
            which_time_set = SET_YEAR;
            break;
        case SET_YEAR:
            which_time_set = SET_MON;
            break;
        case SET_MON:
            which_time_set = SET_DAY;
            break;
        case SET_DAY:
            which_time_set = SET_WEEK;
            break;
        case SET_WEEK:
            which_time_set = SET_HOUR;
            break;
        default:
            break;
        }
    }
    else if (BUTTON_EVENT_DOUBLE_CLICK == beid)
    {
        switch (which_time_set)
        {
        case SET_HOUR:
            which_time_set = SET_WEEK;
            break;
        case SET_MIN:
            which_time_set = SET_HOUR;
            break;
        case SET_SEC:
            which_time_set = SET_MIN;
            break;
        case SET_YEAR:
            which_time_set = SET_YEAR;
            break;
        case SET_MON:
            which_time_set = SET_DAY;
            break;
        case SET_DAY:
            which_time_set = SET_MON;
            break;
        case SET_WEEK:
            which_time_set = SET_DAY;
            break;
        default:
            break;
        }
    }

    if (button_get_event(BUTTON_UP) || BUTTON_LONG_PRESSED == button_get_state(BUTTON_UP))
    {
        switch (which_time_set)
        {
        case SET_HOUR:
            set_time.hour++;
            if (set_time.hour >= 24)
            {
                set_time.hour = 0;
            }
            break;
        case SET_MIN:
            set_time.min++;
            if (set_time.min >= 60)
            {
                set_time.min = 0;
            }
            break;
        case SET_SEC:
            set_time.sec++;
            if (set_time.sec >= 60)
            {
                set_time.sec = 0;
            }
            break;
        case SET_YEAR:
            set_time.year++;
            if (set_time.year > 99)
            {
                set_time.year = 0;
            }
            break;
        case SET_MON:
            set_time.month++;
            if (set_time.month > 12)
            {
                set_time.month = 0;
            }
            break;
        case SET_DAY:
            set_time.day++;
            if (set_time.day > 31)
            {
                set_time.day = 0;
            }
            break;
        case SET_WEEK:
            set_time.week = set_time.week << 1;
            if (set_time.week > 0x40)
            {
                set_time.week = 0x01;
            }
            break;
        default:
            break;
        }
    }

    if (button_get_event(BUTTON_DOWN) || BUTTON_LONG_PRESSED == button_get_state(BUTTON_DOWN))
    {
        switch (which_time_set)
        {
        case SET_HOUR:
            set_time.hour--;
            if (set_time.hour >= 24)
            {
                set_time.hour = 23;
            }
            break;
        case SET_MIN:
            set_time.min--;
            if (set_time.min >= 60)
            {
                set_time.min = 59;
            }
            break;
        case SET_SEC:
            set_time.sec--;
            if (set_time.sec >= 60)
            {
                set_time.sec = 59;
            }
            break;
        case SET_YEAR:
            set_time.year--;
            if (set_time.year > 99)
            {
                set_time.year = 99;
            }
            break;
        case SET_MON:
            set_time.month--;
            if (set_time.month > 12)
            {
                set_time.month = 12;
            }
            break;
        case SET_DAY:
            set_time.day--;
            if (set_time.day > 31)
            {
                set_time.day = 31;
            }
            break;
        case SET_WEEK:
            set_time.week = set_time.week >> 1;
            if (set_time.week < 0x01)
            {
                set_time.week = 0x01;
            }
            break;
        default:
            break;
        }
    }

    if (oled_refresh_flag == 0)
    {
        static unsigned char i = 0;

        oled_clear(oled_gram, 0);

#if TEST_SHT41
        oled_draw_string(0, 0, "Temp:", oled_gram, 1, Show6x8);
        oled_draw_int(30, 0, sht41.temperature, 5, oled_gram, 1, Show6x8);

        oled_draw_string(0, 10, "Humi:", oled_gram, 1, Show6x8);
        oled_draw_int(30, 10, sht41.humidity, 5, oled_gram, 1, Show6x8);
#endif
        show_time(&set_time);

        oled_refresh_flag = 1;
    }
}

/*时间显示界面*/
void task_show_time()
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

void test_switch_module_task_main()
{
    button_event beid = button_get_event(BUTTON_ESC);
    if (BUTTON_EVENT_DOUBLE_CLICK == beid)
    {
        if (task_id == TASKID_NORMAL)
        {
            task_id = TASKID_SET_TIME;
            set_time = current_time;
            which_time_set = SET_HOUR;
        }
        else
        {
            task_id = TASKID_NORMAL;
            ins5699_write_time(&set_time);
            ins5699_read_time(&current_time);
        }
    }
    else if (BUTTON_EVENT_LONG_PRESS == beid)
    {
        task_id = TASKID_NORMAL;
    }

    switch (task_id)
    {
    case TASKID_NORMAL:
        task_show_time();
        break;
    case TASKID_SET_TIME:
        task_set_time();
        break;
    default:
        break;
    }
}
