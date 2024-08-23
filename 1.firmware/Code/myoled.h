/*
 * @Author: 0clock
 * @Date: 2024-08-03 01:12:47
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-22 02:23:21
 * @FilePath: \STC32_LowPowerClock\1.firmware\Code\myoled.h
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#ifndef _MY_OLED_H_
#define _MY_OLED_H_

extern unsigned char xdata oled_gram[128 * 8];
extern bit oled_refresh_flag;

void OLED_POW(unsigned char dat);
void oled_clear(unsigned char *buff, bit color);
void oled_refresh(const unsigned char *buff);
void oled_drawpoint(unsigned char x, unsigned char y, unsigned char *buff, bit color);
void oled_drawline(unsigned char x0, unsigned char y0,
                   unsigned char x1, unsigned char y1,
                   unsigned char *buff, bit color,
                   unsigned char thickness);
void oled_drawcircle(unsigned char x0, unsigned char y0,
                     unsigned char radius, unsigned char *buff, bit color);
void oled_draw_round_rect(unsigned char x0, unsigned char y0,
                          unsigned char x1, unsigned char y1,
                          unsigned char radius, unsigned char *buff, bit color,
                          unsigned char thickness);
void test_draw_waveform(unsigned char *buff);
void test_draw_rect_animation(unsigned char *buff);
void oled_draw_char(unsigned char x, unsigned char y, char ch,
                    unsigned char *buff, bit color,
                    unsigned char font_size);
void oled_draw_string(unsigned char x, unsigned char y, const char *str,
                      unsigned char *buff, bit color, SHOW_size_enum font_size);
void oled_draw_int(unsigned char x, unsigned char y,
                   const long dat, unsigned char num, unsigned char *gram,
                   bit color, SHOW_size_enum fontsize);
void oled_draw_image(unsigned char x, unsigned char y,
                     unsigned char width, unsigned char height,
                     const unsigned char *image, unsigned char *buff, bit color);
void show_dig_num(unsigned char x, unsigned char y,
                  const unsigned char num,
                  const unsigned char *image, bit color);
#endif