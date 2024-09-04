/*
 * @Author: 0clock
 * @Date: 2024-08-03 01:12:38
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-09-04 20:24:20
 * @FilePath: \STC32_LowPowerClock\1.firmware\Code\myoled.c
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#include "stc32g.h"
#include "dmx_oled.h"
#include "myoled.h"
#include <math.h>
#include "dmx_all.h"
#include "dmx_function.h"

unsigned char xdata oled_gram[128 * 8] = {0};
bit oled_refresh_flag = 0;
//========================================================================
// 函数: void OLED_POW(unsigned char dat)
// 描述: OLED供电启动 0~3 为5/6/7/8V选择
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void OLED_POW(unsigned char dat)
{
    P53 = 1;
    P5M0 &= 0xf7; // p54双向
    P5M1 &= 0xf7;
    P05 = ~(dat & 0x01);
    P06 = ~(dat & 0x02);
}

/**
 *
 * @brief    屏幕刷新函数
 * @param    buff         图像刷新缓存
 * @return   void
 * @notes    在中断中执行
 * Example:  oled_refresh(oled_gram);
 *
 **/
void oled_refresh(const unsigned char *buff)
{
    unsigned int i = 0;
    unsigned char x0 = 0, y0 = 0;
    for (y0 = 0; y0 < OLED_HEIGH / 8; y0++)
    {
        Set_Pos_OLED(0, y0);
        for (x0 = 0; x0 < OLED_WIDTH; x0++)
        {
            Draw_Point_OLED(buff[i++]);
        }
    }
}
void oled_clear(unsigned char *buff, bit color)
{
    unsigned int i = 0;
    if (color)
    {
        for (i = 0; i < 128 * 8; i++)
        {
            buff[i] = 0xff;
        }
    }
    else
    {
        for (i = 0; i < 128 * 8; i++)
        {
            buff[i] = 0x0;
        }
    }
}

void oled_drawpoint(unsigned char x, unsigned char y, unsigned char *buff, bit color)
{
    unsigned char y0 = y / 8;

    unsigned int pos = x + (y0 * 128);

    if (x >= OLED_WIDTH || y >= OLED_HEIGH)
    {
        return;
    }

    if (color)
    {
        buff[pos] |= 0x01 << (y - y0 * 8);
    }
    else
    {
        buff[pos] &= ~(0x01 << (y - y0 * 8));
    }
}

/**
 *
 * @brief    绘制直线
 * @param    x0          起点x坐标
 * @param    y0          起点y坐标
 * @param    x1          终点x坐标
 * @param    y1          终点y坐标
 * @param    buff        图像刷新缓存
 * @param    color       绘制颜色（1表示点亮，0表示熄灭）
 * @param    thickness   线条粗细
 * @return   void
 * @notes    使用Bresenham算法绘制直线
 * Example:  oled_drawline(0, 0, 127, 63, oled_gram, 1, 1);
 *
 **/
void oled_drawline(unsigned char x0, unsigned char y0,
                   unsigned char x1, unsigned char y1,
                   unsigned char *buff, bit color,
                   unsigned char thickness)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int e2;
    unsigned int i, j;

    while (1)
    {
        for (i = 0; i < thickness; i++)
        {
            for (j = 0; j < thickness; j++)
            {
                oled_drawpoint((unsigned int)(x0 + i), y0 + j, buff, color);
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 *
 * @brief    绘制圆形
 * @param    x0          圆心x坐标
 * @param    y0          圆心y坐标
 * @param    radius      圆的半径
 * @param    buff        图像刷新缓存
 * @param    color       绘制颜色（1表示点亮，0表示熄灭）
 * @return   void
 * @notes    使用Bresenham算法绘制圆形
 * Example:  oled_drawcircle(64, 32, 20, oled_gram, 1);
 *
 **/
void oled_drawcircle(unsigned char x0, unsigned char y0,
                     unsigned char radius, unsigned char *buff, bit color)
{
    int x = 0;
    int y = radius;
    int err = 3 - 2 * radius;

    while (x <= y)
    {
        oled_drawpoint(x0 + x, y0 + y, buff, color);
        oled_drawpoint(x0 + y, y0 + x, buff, color);
        oled_drawpoint(x0 - y, y0 + x, buff, color);
        oled_drawpoint(x0 - x, y0 + y, buff, color);
        oled_drawpoint(x0 - x, y0 - y, buff, color);
        oled_drawpoint(x0 - y, y0 - x, buff, color);
        oled_drawpoint(x0 + y, y0 - x, buff, color);
        oled_drawpoint(x0 + x, y0 - y, buff, color);

        if (err < 0)
        {
            err += 4 * x + 6;
        }
        else
        {
            err += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

/**
 *
 * @brief    绘制圆角矩形
 * @param    x0          左上角x坐标
 * @param    y0          左上角y坐标
 * @param    x1          右下角x坐标
 * @param    y1          右下角y坐标
 * @param    radius      圆角的半径
 * @param    buff        图像刷新缓存
 * @param    color       绘制颜色（1表示点亮，0表示熄灭）
 * @param    thickness   线条粗细
 * @return   void
 * @notes    使用oled_drawline和oled_drawcircle函数绘制圆角矩形
 * Example:  oled_draw_round_rect(10, 10, 100, 50, 10, oled_gram, 1, 1);
 *
 **/
void oled_draw_round_rect(unsigned char x0, unsigned char y0,
                          unsigned char x1, unsigned char y1,
                          unsigned char radius, unsigned char *buff, bit color,
                          unsigned char thickness)
{
    unsigned int x = radius;
    unsigned int y = 0;
    int err = 0;
    // Draw the four sides of the rectangle, leaving space for the corners
    oled_drawline(x0 + radius, y0, x1 - radius, y0, buff, color, thickness); // Top
    oled_drawline(x0 + radius, y1, x1 - radius, y1, buff, color, thickness); // Bottom
    oled_drawline(x0, y0 + radius, x0, y1 - radius, buff, color, thickness); // Left
    oled_drawline(x1, y0 + radius, x1, y1 - radius, buff, color, thickness); // Right

    // Draw the four corners
    while (x >= y)
    {
        oled_drawpoint(x1 + x - radius, y1 + y - radius, buff, color); // Bottom-right corner
        oled_drawpoint(x1 + y - radius, y1 + x - radius, buff, color);

        oled_drawpoint(x0 - y + radius, y1 + x - radius, buff, color); // Bottom-left corner
        oled_drawpoint(x0 - x + radius, y1 + y - radius, buff, color);

        oled_drawpoint(x0 - x + radius, y0 - y + radius, buff, color); // Top-left corner
        oled_drawpoint(x0 - y + radius, y0 - x + radius, buff, color);

        oled_drawpoint(x1 + y - radius, y0 - x + radius, buff, color); // Top-right corner
        oled_drawpoint(x1 + x - radius, y0 - y + radius, buff, color);

        y += 1;
        if (err <= 0)
        {
            err += 2 * y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

/**
 *
 * @brief    绘制波形
 * @param    waveform_data   波形数据数组
 * @param    data_length     波形数据长度
 * @param    y_offset        波形在y方向上的偏移量
 * @param    amplitude       波形的幅度
 * @param    buff            图像刷新缓存
 * @param    color           绘制颜色（1表示点亮，0表示熄灭）
 * @return   void
 * @notes    根据波形数据逐点绘制波形
 * Example:  oled_drawwaveform(waveform_data, 128, 32, 30, oled_gram, 1);
 *
 **/
void oled_drawwaveform(unsigned char *waveform_data, unsigned int data_length,
                       unsigned char y_offset, unsigned char amplitude,
                       unsigned char *buff, bit color)
{
    unsigned char x, y;
    unsigned char prev_y = y_offset + (amplitude / 2);

    for (x = 0; x < data_length; x++)
    {
        y = y_offset + (amplitude / 2) - waveform_data[x];
        if (x == 0)
        {
            // oled_drawline(x, prev_y, x, y, buff, color, 2);
        }
        else
        {
            oled_drawline(x - 1, prev_y, x, y, buff, color, 2);
        }
        prev_y = y;
    }
}

// 写了一个正弦波滚动的函数
void test_draw_waveform(unsigned char *buff)
{
    unsigned char waveform_data[128]; // 假设OLED宽度为128
    unsigned int data_length = sizeof(waveform_data) / sizeof(waveform_data[0]);
    unsigned int i, j;
    // 生成一个简单的波形数据，例如正弦波
    while (1)
    {
        while (1)
        {
            for (i = 0; i < data_length; i++)
            {
                waveform_data[i] = (unsigned char)(15 * sin((i + j) * 6 * 3.14 / data_length) + 16); // 幅度30，偏移16
            }

            // 清空缓冲区
            oled_clear(buff, 0);

            // 绘制波形
            oled_drawwaveform(waveform_data, data_length, 32, 30, buff, 1);
            oled_refresh_flag = 1;
            // delay_ms(1);
            j++;
        }

#if 0
        while (j > 0)
        {
            for (i = 0; i < data_length; i++)
            {
                waveform_data[i] = (unsigned char)(15 * sin((i + j) * 2 * 3.14 / data_length) + 16); // 幅度30，偏移16
            }

            // 清空缓冲区
            oled_clear(buff, 0);

            // 绘制波形
            oled_drawwaveform(waveform_data, data_length, 32, 30, buff, 1);
            oled_refresh_flag = 1;
            delay_ms(5);
            j--;
        }
#endif
    }
}

/**
 *
 * @brief    动画函数，绘制一个矩形从小变大再变小
 * @param    buff         图像刷新缓存
 * @return   void
 * @notes    在中断中执行
 * Example:  test_draw_rect_animation(oled_gram);
 *
 **/
void test_draw_rect_animation(unsigned char *buff)
{
    unsigned char max_size = 64;   // 矩形的最大尺寸
    unsigned char min_size = 10;   // 矩形的最小尺寸
    unsigned char size_step = 1;   // 每次变化的尺寸步长
    unsigned char size = min_size; // 初始尺寸
    bit growing = 1;               // 标志，表示矩形是变大还是变小

    // 计算左上角和右下角坐标
    unsigned char x0;
    unsigned char y0;
    unsigned char x1;
    unsigned char y1;

    while (1)
    {
        x0 = (128 - size) / 2;
        y0 = (64 - size) / 2;
        x1 = x0 + size;
        y1 = y0 + size;
        // 清空缓冲区
        oled_clear(buff, 0);

        // 绘制矩形
        oled_draw_round_rect(x0, y0, x1, y1, (x1 - x0) / 6, buff, 1, 1);

        // 刷新显示
        oled_refresh_flag = 1;
        delay_ms(5);

        // 更新尺寸
        if (growing)
        {
            size += size_step;
            if (size >= max_size)
            {
                growing = 0; // 开始缩小
            }
        }
        else
        {
            size -= size_step;
            if (size <= min_size)
            {
                growing = 1; // 开始变大
            }
        }
    }
}

/**
 *
 * @brief    在指定位置绘制整数
 * @param    x           起始x坐标
 * @param    y           起始y坐标
 * @param    number      需要绘制的整数
 * @param    buff        图像刷新缓存
 * @param    color       绘制颜色（1表示点亮，0表示熄灭）
 * @return   void
 * @notes    将整数转换为字符串，然后逐个字符绘制
 * Example:  oled_draw_int(10, 10, 123, oled_gram, 1);
 *
 **/
void oled_draw_int(unsigned char x, unsigned char y,
                   const long dat, unsigned char num, unsigned char *gram,
                   bit color, SHOW_size_enum fontsize)
{
    long multiple = 1;
    long remainder = dat;
    char buff[12] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    if (num > 10)
        num = 10;
    if (num < 1)
        num = 1;
    buff[num + 1] = '\0';

    if (num < 10)
    {
        while (num > 0)
        {
            num--;
            multiple *= 10;
        }
        remainder %= multiple;
    }

    int_to_str_func(remainder, buff);
    oled_draw_string(x, y, buff, gram, color, fontsize);
}

/**
 *
 * @brief    在指定位置绘制字符
 * @param    x           起始x坐标
 * @param    y           起始y坐标
 * @param    ch          需要绘制的字符
 * @param    buff        图像刷新缓存
 * @param    color       绘制颜色（1表示点亮，0表示熄灭）
 * @param    fontsize    字体大小选择（6表示6x8字体，8表示8x16字体）
 * @return   void
 * @notes    从字体数据中取出字符点阵并绘制
 * Example:  oled_draw_char(10, 10, 'A', oled_gram, 1, 6);
 *
 **/
void oled_draw_char(unsigned char x, unsigned char y, char ch,
                    unsigned char *buff, bit color,
                    SHOW_size_enum fontsize)
{
    unsigned char i, j;
    unsigned char c;
    unsigned char line;

   
      c = ch - ' '; // Default mapping, assuming the font array starts with space character
   

    if (fontsize == Show6x8)
    {
        for (i = 0; i < 6; i++) // Each character is 6x8 pixels
        {
            line = Char6x8[c][i]; // Get font data from Char6x8 array
            for (j = 0; j < 8; j++)
            {
                if (line & 0x01)
                {
                    oled_drawpoint(x + i, y + j, buff, color);
                }
                line >>= 1;
            }
        }
    }
    else if (fontsize == Show8x16)
    {
        for (i = 0; i < 8; i++) // Each character is 8x16 pixels
        {
            unsigned char line1 = Char8x16[c][i];     // Get upper part of character data from Char8x16 array
            unsigned char line2 = Char8x16[c][i + 8]; // Get lower part of character data from Char8x16 array

            for (j = 0; j < 8; j++)
            {
                if (line1 & 0x01)
                {
                    oled_drawpoint(x + i, y + j, buff, color);
                }
                line1 >>= 1;
            }

            for (j = 0; j < 8; j++)
            {
                if (line2 & 0x01)
                {
                    oled_drawpoint(x + i, y + j + 8, buff, color);
                }
                line2 >>= 1;
            }
        }
    }
    else
    {
        // Handle invalid fontsize if necessary
    }
}

/**
 *
 * @brief    在指定位置绘制字符串
 * @param    x           起始x坐标
 * @param    y           起始y坐标
 * @param    str         需要绘制的字符串
 * @param    buff        图像刷新缓存
 * @param    color       绘制颜色（1表示点亮，0表示熄灭）
 * @param    font_size   字体大小选择（6表示6x8字体，8表示8x16字体）
 * @return   void
 * @notes    逐字符调用oled_draw_char函数绘制字符串
 * Example:  oled_draw_string(10, 10, "Hello", oled_gram, 1, 6);
 *
 **/
void oled_draw_string(unsigned char x, unsigned char y, const char *str,
                      unsigned char *buff, bit color, SHOW_size_enum font_size)
{
    unsigned char startX = x;

    while (*str)
    {
        oled_draw_char(x, y, *str, buff, color, font_size);
        if (font_size == Show6x8)
        {
            x += 6; // Move x position for the next character (6x8 font)
        }
        else if (font_size == Show8x16)
        {
            x += 8; // Move x position for the next character (8x16 font)
        }

        // Check if the x position exceeds the screen width and handle accordingly
        // For example, you might want to wrap the text to a new line.

        str++; // Move to the next character in the string
    }
}

/**
 * @brief 将浮点数转换为字符串
 * @param num      需要转换的浮点数
 * @param decimals 保留的小数位数
 * @param buffer   用于存储转换后的字符串
 * @return void
 */
void float_to_string(float num, int decimals, char *buffer)
{
    int int_part = (int)num;                 // 取整数部分
    float frac_part = num - (float)int_part; // 取小数部分
    int frac_as_int;
    int i = 0, j;
      int temp_int = int_part;
    int int_digits = 0;

    // 处理负数情况
    if (num < 0)
    {
        buffer[i++] = '-';
        int_part = -int_part;
        frac_part = -frac_part;
    }

    // 将整数部分转换为字符串

    do
    {
        int_digits++;
        temp_int /= 10;
    } while (temp_int);

    for (j = int_digits - 1; j >= 0; j--)
    {
        buffer[i + j] = (int_part % 10) + '0';
        int_part /= 10;
    }
    i += int_digits;

    // 添加小数点
    buffer[i++] = '.';

    // 将小数部分转换为字符串
    for (j = 0; j < decimals; j++)
    {
        frac_part *= 10;
    }
    frac_as_int = (int)(frac_part + 0.5f); // 四舍五入

    for (j = decimals - 1; j >= 0; j--)
    {
        buffer[i + j] = (frac_as_int % 10) + '0';
        frac_as_int /= 10;
    }
    i += decimals;

    // 添加字符串结束符
    buffer[i] = '\0';
}

/**
 * @brief 在OLED屏幕上绘制浮点数
 * @param x         起始x坐标
 * @param y         起始y坐标
 * @param num       需要显示的浮点数
 * @param decimals  显示的小数位数
 * @param buff      图像刷新缓存
 * @param color     绘制颜色（1表示点亮，0表示熄灭）
 * @param font_size 字体大小选择（6表示6x8字体，8表示8x16字体）
 * @return void
 */
void oled_draw_float(unsigned char x, unsigned char y,
                     float num, int decimals, unsigned char *buff,
                     bit color, SHOW_size_enum font_size)
{
    char buffer[20]; // 用于存储转换后的浮点数字符串

    // 手动将浮点数转换为字符串
    float_to_string(num, decimals, buffer);

    // 调用 oled_draw_string 函数来绘制格式化后的浮点数字符串
    oled_draw_string(x, y, buffer, buff, color, font_size);
}

/**
 *
 * @brief    绘制图像
 * @param    x            起始x坐标
 * @param    y            起始y坐标
 * @param    width        图像的宽度
 * @param    height       图像的高度
 * @param    image        图像数据数组
 * @param    buff         图像刷新缓存
 * @param    color        绘制颜色（1表示点亮，0表示熄灭）
 * @return   void
 * @notes    根据图像数据逐像素绘制图像
 * Example:  oled_draw_image(0, 0, 16, 16, my_image, oled_gram, 1);
 *
 **/
void oled_draw_image(unsigned char x, unsigned char y,
                     unsigned char width, unsigned char height,
                     const unsigned char *image, unsigned char *buff, bit color)
{
    unsigned char i, j, k, m = 0, line;
    if (height % 8 != 0)
    {
        height += (height % 8);
    }
    for (i = 0; i < height / 8; i++)
    {
        for (k = 0; k < width; k++)
        {
            line = image[m++];
            for (j = 0; j < 8; j++)
            {
                if (line & 0x01)
                {
                    oled_drawpoint(x + k, y + j + 8 * i, buff, color);
                }
                line >>= 1;
            }
        }
    }

#if 0 // 下面这个方法会导致y轴只会按照8行那种显示，不行没办法做到像素级控制坐标，淘汰
    unsigned char y0, y1, i, j, k = 0;

    unsigned int pos;

    if (y % 8 == 0)
        y0 = y / 8;
    else
        y0 = y / 8 + 1;

    if (height % 8 == 0)
        y1 = height / 8;
    else
        y1 = height / 8 + 1;

    if (x >= OLED_WIDTH || y >= OLED_HEIGH)
    {
        return;
    }

    for (j = 0; j < y1; j++)
    {
        pos = x + ((y0 + j) * 128);
        for (i = 1; i <= width; i++)
        {
            pos++;
            buff[pos] = image[k++];
        }
    }
#endif
}

void show_dig_num(unsigned char x, unsigned char y,
                  const unsigned char num,
                  const unsigned char *gram, bit color)
{
    switch (num)
    {
    case 0:
        oled_draw_image(x, y, 14, 23, Num_0, gram, color);
        break;
    case 1:
        oled_draw_image(x, y, 14, 23, Num_1, gram, color);
        break;
    case 2:
        oled_draw_image(x, y, 14, 23, Num_2, gram, color);
        break;
    case 3:
        oled_draw_image(x, y, 14, 23, Num_3, gram, color);
        break;
    case 4:
        oled_draw_image(x, y, 14, 23, Num_4, gram, color);
        break;
    case 5:
        oled_draw_image(x, y, 14, 23, Num_5, gram, color);
        break;
    case 6:
        oled_draw_image(x, y, 14, 23, Num_6, gram, color);
        break;
    case 7:
        oled_draw_image(x, y, 14, 23, Num_7, gram, color);
        break;
    case 8:
        oled_draw_image(x, y, 14, 23, Num_8, gram, color);
        break;
    case 9:
        oled_draw_image(x, y, 14, 23, Num_9, gram, color);
        break;
    default:
        break;
    }
}
