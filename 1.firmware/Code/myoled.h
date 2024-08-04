#ifndef _MY_OLED_H_
#define _MY_OLED_H_

extern unsigned char xdata oled_gram[128 * 8];
extern bit oled_refresh_flag;

void OLED_POW(unsigned char dat);
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
void oled_draw_arc(unsigned char x0, unsigned char y0,
                   unsigned char start_angle, unsigned char end_angle,
                   unsigned char radius, unsigned char *buff, bit color,
                   unsigned char thickness);
void test_draw_rect_animation(unsigned char *buff);
#endif