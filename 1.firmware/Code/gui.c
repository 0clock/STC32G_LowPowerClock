#include "dmx_all.h"

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

    static unsigned char x = 8, y = 20, last_sec = 0;
    static bit x_dir = 0, y_dir = 0; // 移动的方向，0为正，1为反
    char pos = -3, x_range = 8 + 3, y_range = 20 + 5, x_range0 = 8 - 3, y_range0 = 20 - 5;

    show_dig_num(x, y, current_time.hour / 10, oled_gram, 1);
    show_dig_num(x + 16, y, current_time.hour % 10, oled_gram, 1);

    show_dig_num(x + 39, y, current_time.min / 10, oled_gram, 1);
    show_dig_num(x + 55, y, current_time.min % 10, oled_gram, 1);

    show_dig_num(x + 79, y, current_time.sec / 10, oled_gram, 1);
    show_dig_num(x + 95, y, current_time.sec % 10, oled_gram, 1);

    if (current_time.sec % 2 == 0)
    {
        oled_draw_image(x + 33, y + 4, 2, 14, Pixel_Dot, oled_gram, 1);
        oled_draw_image(x + 73, y + 4, 2, 14, Pixel_Dot, oled_gram, 1);
    }

    // 显示年月日、星期

#if 1 // 6x8字体，有点小
    oled_draw_int(x, y + 27, current_time.year + 2000, 4, oled_gram, 1, Show6x8);
    oled_draw_char(x + 24, y + 27, '/', oled_gram, 1, Show6x8);
    oled_draw_int(x + 30, y + 27, current_time.month, 2, oled_gram, 1, Show6x8);
    if (current_time.month < 10)
    {
        oled_draw_char(x + 36, y + 27, '/', oled_gram, 1, Show6x8);
        oled_draw_int(x + 42, y + 27, current_time.day, 2, oled_gram, 1, Show6x8);
    }
    else
    {
        oled_draw_char(x + 42, y + 27, '/', oled_gram, 1, Show6x8);
        oled_draw_int(x + 48, y + 27, current_time.day, 2, oled_gram, 1, Show6x8);
    }

    switch (current_time.week)
    {
    case 1:
        oled_draw_string(x + 91, y + 27, "Sun", oled_gram, 1, Show6x8);
        break;
    case 2:
        oled_draw_string(x + 91, y + 27, "Mon", oled_gram, 1, Show6x8);
        break;
    case 3:
        oled_draw_string(x + 91, y + 27, "Tue", oled_gram, 1, Show6x8);
        break;
    case 4:
        oled_draw_string(x + 91, y + 27, "Wed", oled_gram, 1, Show6x8);
        break;
    case 5:
        oled_draw_string(x + 91, y + 27, "Thu", oled_gram, 1, Show6x8);
        break;
    case 6:
        oled_draw_string(x + 91, y + 27, "Fri", oled_gram, 1, Show6x8);
        break;
    case 7:
        oled_draw_string(x + 91, y + 27, "Sat", oled_gram, 1, Show6x8);
        break;
    default:
        oled_draw_string(x + 79, y + 27, "Oops!", oled_gram, 1, Show6x8);
        break;
    }

#elif // 8x16字体，有点不协调

    oled_draw_int(x + pos, y + 27, current_time.year + 2000, 4, oled_gram, 1, Show8x16);
    oled_draw_char(x + pos + 32, y + 27, '/', oled_gram, 1, Show8x16);
    oled_draw_int(x + pos + 40, y + 27, current_time.month, 2, oled_gram, 1, Show8x16);
    if (current_time.month < 10)
    {
        oled_draw_char(x + pos + 48, y + 27, '/', oled_gram, 1, Show8x16);
        oled_draw_int(x + pos + 56, y + 27, current_time.day, 2, oled_gram, 1, Show8x16);
    }
    else
    {
        oled_draw_char(x + pos + 56, y + 27, '/', oled_gram, 1, Show8x16);
        oled_draw_int(x + pos + 64, y + 27, current_time.day, 2, oled_gram, 1, Show8x16);
    }

    switch (current_time.week)
    {
    case 1:
        oled_draw_string(x + 89, y + 27, "Sun", oled_gram, 1, Show8x16);
        break;
    case 2:
        oled_draw_string(x + 89, y + 27, "Mon", oled_gram, 1, Show8x16);
        break;
    case 3:
        oled_draw_string(x + 89, y + 27, "Tue", oled_gram, 1, Show8x16);
        break;
    case 4:
        oled_draw_string(x + 89, y + 27, "Wed", oled_gram, 1, Show8x16);
        break;
    case 5:
        oled_draw_string(x + 89, y + 27, "Thu", oled_gram, 1, Show8x16);
        break;
    case 6:
        oled_draw_string(x + 89, y + 27, "Fri", oled_gram, 1, Show8x16);
        break;
    case 7:
        oled_draw_string(x + 89, y + 27, "Sat", oled_gram, 1, Show8x16);
        break;
    default:
        oled_draw_string(x + 79, y + 27, "Oops!", oled_gram, 1, Show8x16);
        break;
    }
#endif
    // 时间显示的宽占110个像素，所以x的范围为0~17，y的范围为0~40
    // 接下来实现一个时间在屏幕上移动的过程,以保护屏幕
    if (last_sec != current_time.min ) // 控制变动的周期
    {
        last_sec = current_time.min;

        if (x_dir == 0)
        {
            x = close_to(x, x_range);
            if (x == x_range)
            {
                x_dir = 1;
            }
        }
        else
        {
            x = close_to(x, x_range0);
            if (x == x_range0)
            {
                x_dir = 0;
            }
        }

        if (y_dir == 0)
        {
            y = close_to(y, y_range);
            if (y == y_range)
            {
                y_dir = 1;
            }
        }
        else
        {
            y = close_to(y, y_range0);
            if (y == y_range0)
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