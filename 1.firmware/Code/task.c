#include "dmx_all.h"

/**
 * @brief 打印time_t结构体中的时间信息
 *
 * @param time 指向time_t结构体的指针，用于打印时间数据
 * @return void
 */
time_t current_time;

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

// a->b
int close_to(int a, int b)
{
    int out;
    if (a > b)
    {
        return --a;
    }
    if (a < b)
    {
        return ++a;
    }

    return a;
}

void show_time()
{
    // x,y为显示时间的起始坐标，也就是时间字符的左上角在屏幕上的位置

    static unsigned char x = 0, y = 0, last_sec = 0;
    static bit x_dir = 0, y_dir = 0; // 移动的方向，0为正，1为反

    if (0 != current_time.hour / 10)
    {
        show_dig_num(x, y, current_time.hour / 10, oled_gram, 1);
    }
    show_dig_num(x + 16, y, current_time.hour % 10, oled_gram, 1);

    show_dig_num(x + 39, y, current_time.min / 10, oled_gram, 1);
    show_dig_num(x + 55, y, current_time.min % 10, oled_gram, 1);

    show_dig_num(x + 79, y, current_time.sec / 10, oled_gram, 1);
    show_dig_num(x + 95, y, current_time.sec % 10, oled_gram, 1);

    if (last_sec % 2 == 0)
    {
        oled_draw_image(x + 33, y + 4, 2, 14, Pixel_Dot, oled_gram, 1);
        oled_draw_image(x + 73, y + 4, 2, 14, Pixel_Dot, oled_gram, 1);
    }

    // 时间显示的宽占110个像素，所以x的范围为0~17，y的范围为0~40
    // 接下来实现一个时间在屏幕上移动的过程
    if (last_sec != current_time.sec && 1) // 每秒变动一次
    {

        last_sec = current_time.sec;

        if (x_dir == 0)
        {
            x = close_to(x, 17);
            if (x == 17)
            {
                x_dir = 1;
            }
        }
        else
        {
            x = close_to(x, 0);
            if (x == 0)
            {
                x_dir = 0;
            }
        }

        if (y_dir == 0)
        {
            y = close_to(y, 40);
            if (y == 40)
            {
                y_dir = 1;
            }
        }
        else
        {
            y = close_to(y, 0);
            if (y == 0)
            {
                y_dir = 0;
            }
        }
#if 0
        if (x_dir == 1)
        {
            x--;
            if (x < 0)
            {
                x_dir = 0;
            }
        }
        else
        {
            x++;
            if (x > 17)
            {
                x_dir = 1;
            }
        }

        if (y_dir == 1)
        {
            y--;
            if (y < 0)
            {
                y_dir = 0;
            }
        }
        else
        {
            y++;
            if (y > 40)
            {
                y_dir = 1;
            }
        }
#endif
    }
}