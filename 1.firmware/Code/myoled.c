/*
 * @Author: 0clock
 * @Date: 2024-08-03 01:12:38
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-04 04:39:15
 * @FilePath: \Dmx_STC32F12K54_Library_V1.3.1\Code\myoled.c
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#include "stc32g.h"
#include "dmx_oled.h"
#include "myoled.h"
#include <math.h>
#include "dmx_all.h"

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
        buff[pos] &= 0x01 << (y - y0);
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
    int i, j;

    while (1)
    {
        for (i = 0; i < thickness; i++)
        {
            for (j = 0; j < thickness; j++)
            {
                oled_drawpoint(x0 + i, y0 + j, buff, color);
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
    int x = radius;
    int y = 0;
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
    unsigned char x0 ;
    unsigned char y0 ;
    unsigned char x1 ;
    unsigned char y1 ;

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
