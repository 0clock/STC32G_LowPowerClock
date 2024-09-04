#include "dmx_all.h"
#include "gui.h"
#include "ins5699s.h"
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

void show_time(const time_t *t)
{
    // x,y为显示时间的起始坐标，也就是时间字符的左上角在屏幕上的位置

    static unsigned char x = 8, y = 20, last_sec = 0, set_blink_count = 0;
    static bit x_dir = 0, y_dir = 0; // 移动的方向，0为正，1为反
    char pos = 0, x_range = 8 + 3, y_range = 20 + 5, x_range0 = 8 - 3, y_range0 = 20 - 5;
    unsigned char x0 = x;
    if (t->hour / 10 == 1)
    {
        x0 = x - 6;
    }
    show_dig_num(x0, y, t->hour / 10, oled_gram, 1);
    show_dig_num(x0 + 16, y, t->hour % 10, oled_gram, 1);

    show_dig_num(x0 + 39, y, t->min / 10, oled_gram, 1);
    show_dig_num(x0 + 55, y, t->min % 10, oled_gram, 1);

    show_dig_num(x0 + 79, y, t->sec / 10, oled_gram, 1);
    show_dig_num(x0 + 95, y, t->sec % 10, oled_gram, 1);

    if (t->sec % 2 == 0)
    {
        oled_draw_image(x0 + 33, y + 4, 2, 14, Pixel_Dot, oled_gram, 1);
        oled_draw_image(x0 + 73, y + 4, 2, 14, Pixel_Dot, oled_gram, 1);
    }

    // 显示年月日、星期

#if 1 // 6x8字体，有点小
    if (t->hour / 10 == 1)
    {
        pos = 10;
    }
    else
    {
        pos = 0;
    }
    oled_draw_int(x0 + pos, y + 27, t->year + 2000, 4, oled_gram, 1, Show6x8);
    oled_draw_char(x0 + pos + 24, y + 27, '/', oled_gram, 1, Show6x8);
    oled_draw_int(x0 + pos + 30, y + 27, t->month, 2, oled_gram, 1, Show6x8);
    if (t->month < 10)
    {
        oled_draw_char(x0 + pos + 36, y + 27, '/', oled_gram, 1, Show6x8);
        oled_draw_int(x0 + pos + 42, y + 27, t->day, 2, oled_gram, 1, Show6x8);
    }
    else
    {
        oled_draw_char(x0 + pos + 42, y + 27, '/', oled_gram, 1, Show6x8);
        oled_draw_int(x0 + pos + 48, y + 27, t->day, 2, oled_gram, 1, Show6x8);
    }

    switch (t->week)
    {
    case 0x01:
        oled_draw_string(x0 + 91, y + 27, "Sun", oled_gram, 1, Show6x8);
        break;
    case 0x02:
        oled_draw_string(x0 + 91, y + 27, "Mon", oled_gram, 1, Show6x8);
        break;
    case 0x04:
        oled_draw_string(x0 + 91, y + 27, "Tue", oled_gram, 1, Show6x8);
        break;
    case 0x08:
        oled_draw_string(x0 + 91, y + 27, "Wed", oled_gram, 1, Show6x8);
        break;
    case 0x10:
        oled_draw_string(x0 + 91, y + 27, "Thu", oled_gram, 1, Show6x8);
        break;
    case 0x20:
        oled_draw_string(x0 + 91, y + 27, "Fri", oled_gram, 1, Show6x8);
        break;
    case 0x40:
        oled_draw_string(x0 + 91, y + 27, "Sat", oled_gram, 1, Show6x8);
        break;
    default:
        oled_draw_string(x0 + 79, y + 27, "Oops!", oled_gram, 1, Show6x8);
        break;
    }

    // 温湿度数据显示
    // oled_draw_string(0, 0, "℃", oled_gram, 1, Show6x8);
    // oled_draw_char(0,0,"℃", oled_gram, 1, Show6x8);
    oled_draw_float(x0 + pos, y - 10, sht41.temperature, 2, oled_gram, 1, Show6x8);
    // oled_draw_string(0, 10, "Humi:", oled_gram, 1, Show6x8);
    oled_draw_float(x0 + 74, y - 10, sht41.humidity, 2, oled_gram, 1, Show6x8);
    oled_draw_char(x0 + 104, y - 10, '%', oled_gram, 1, Show6x8);

#elif // 8x16字体，有点不协调
    pos = -3;

    oled_draw_int(x + pos, y + 27, t->year + 2000, 4, oled_gram, 1, Show8x16);
    oled_draw_char(x + pos + 32, y + 27, '/', oled_gram, 1, Show8x16);
    oled_draw_int(x + pos + 40, y + 27, t->month, 2, oled_gram, 1, Show8x16);
    if (t->month < 10)
    {
        oled_draw_char(x + pos + 48, y + 27, '/', oled_gram, 1, Show8x16);
        oled_draw_int(x + pos + 56, y + 27, t->day, 2, oled_gram, 1, Show8x16);
    }
    else
    {
        oled_draw_char(x + pos + 56, y + 27, '/', oled_gram, 1, Show8x16);
        oled_draw_int(x + pos + 64, y + 27, t->day, 2, oled_gram, 1, Show8x16);
    }

    switch (t->week)
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

    // 如果是设置时间模式，显示闪烁效果
    set_blink_count++;
    if (task_id == TASKID_SET_TIME && set_blink_count % 32 < 16)
    {
        switch (which_time_set)
        {
        case SET_HOUR:
            show_dig_num(x0, y, t->hour / 10, oled_gram, 0);
            show_dig_num(x0 + 16, y, t->hour % 10, oled_gram, 0);
            break;
        case SET_MIN:
            show_dig_num(x0 + 39, y, t->min / 10, oled_gram, 0);
            show_dig_num(x0 + 55, y, t->min % 10, oled_gram, 0);
            break;
        case SET_SEC:
            show_dig_num(x0 + 79, y, t->sec / 10, oled_gram, 0);
            show_dig_num(x0 + 95, y, t->sec % 10, oled_gram, 0);
            break;
        case SET_YEAR:
            oled_draw_int(x0 + pos, y + 27, t->year + 2000, 4, oled_gram, 0, Show6x8);
            break;
        case SET_MON:
            oled_draw_int(x0 + pos + 30, y + 27, t->month, 2, oled_gram, 0, Show6x8);
            break;
        case SET_DAY:
            if (t->month < 10)
            {
                oled_draw_int(x0 + pos + 42, y + 27, t->day, 2, oled_gram, 0, Show6x8);
            }
            else
            {
                oled_draw_int(x0 + pos + 48, y + 27, t->day, 2, oled_gram, 0, Show6x8);
            }
            break;
        case SET_WEEK:
            switch (t->week)
            {
            case 0x01:
                oled_draw_string(x0 + 91, y + 27, "Sun", oled_gram, 0, Show6x8);
                break;
            case 0x02:
                oled_draw_string(x0 + 91, y + 27, "Mon", oled_gram, 0, Show6x8);
                break;
            case 0x04:
                oled_draw_string(x0 + 91, y + 27, "Tue", oled_gram, 0, Show6x8);
                break;
            case 0x08:
                oled_draw_string(x0 + 91, y + 27, "Wed", oled_gram, 0, Show6x8);
                break;
            case 0x10:
                oled_draw_string(x0 + 91, y + 27, "Thu", oled_gram, 0, Show6x8);
                break;
            case 0x20:
                oled_draw_string(x0 + 91, y + 27, "Fri", oled_gram, 0, Show6x8);
                break;
            case 0x40:
                oled_draw_string(x0 + 91, y + 27, "Sat", oled_gram, 0, Show6x8);
                break;
            default:
                oled_draw_string(x0 + 79, y + 27, "Oops!", oled_gram, 0, Show6x8);
                break;
            }
            break;
        default:
            break;
        }
    }

    // 时间显示的宽占110个像素，所以x的范围为0~17，y的范围为0~40
    // 接下来实现一个时间在屏幕上移动的过程,以保护屏幕
    if (last_sec != t->min) // 控制变动的周期
    {
        last_sec = t->min;

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