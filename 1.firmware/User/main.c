/*
 * @Author: 0clock
 * @Date: 2024-08-03 00:48:43
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-09-04 20:29:24
 * @FilePath: \STC32_LowPowerClock\1.firmware\User\main.c
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#include "dmx_all.h"

// #include "u8g2_porting.h"
/**	！！！！！！！！
* 该开源库单片机主频为软件强制设置内部IRC主频,STC-ISP中所选无效
* 如需修改单片机主频应去dmx_board.h中修改MAIN_FOSC宏定义
！！！！！！！！**/

int a = 0, i = 0;
void main(void)
{
  // 初始化芯片
  init_chip();

  // 以下可放置用户代码段
  Buzzer_Close();
  OLED_POW(0);
  Init_OLED();
  bms_init();
  iic_init();
  rtc_init();
  buttons_init();
  sht41_init();

  init_nvic(UART1_PRIORITY, 3);
  init_nvic(PIT0, 3);
  init_pit_ms(PIT0, 1);
  init_pit_ms(PIT1, 5);

// 静态测试图形
#if 0
  oled_clear(oled_gram, 1);
  oled_drawline(0,0,127,63,oled_gram,0,1);
  oled_drawline(127,0,0,63,oled_gram,0,1);
  oled_drawcircle(64,32,20,oled_gram,0);
  oled_draw_round_rect(20,20,107,43,5,oled_gram,0,1);

  oled_draw_char(10,10,'A',oled_gram,1,6);
  oled_draw_char(16,10,'B',oled_gram,1,6);
  oled_draw_char(22,10,'C',oled_gram,1,6);
  oled_draw_char(28,10,'D',oled_gram,1,6);
  oled_draw_char(34,10,'E',oled_gram,1,6);
  oled_draw_char(10,30,'A',oled_gram,1,8);
  oled_draw_char(18,30,'B',oled_gram,1,8);
  oled_draw_char(26,30,'C',oled_gram,1,8);
  oled_draw_char(34,30,'D',oled_gram,1,8);
  oled_draw_char(42,30,'E',oled_gram,1,8);

  oled_refresh_flag = 1;
#endif

#if 0
  while (1)
  {
    while (a < 128)
    {
      oled_clear(oled_gram, 0);
      oled_draw_string(a, 10, "Hello World!", oled_gram, 1, Show6x8);
      oled_draw_string(a, 30, "Hello World!", oled_gram, 1, Show8x16);
      oled_refresh_flag = 1;
      a++;
      delay_ms(5);
    }
    while (a > 0)
    {
      oled_clear(oled_gram, 0);
      oled_draw_string(a, 10, "Hello World!", oled_gram, 1, Show6x8);
      oled_draw_string(a, 30, "Hello World!", oled_gram, 1, Show8x16);
      oled_refresh_flag = 1;
      a--;
      delay_ms(5);
    }
    while (a < 54)
    {
      oled_clear(oled_gram, 0);
      oled_draw_string(0, 10+a, "Hello World!", oled_gram, 1, Show6x8);
      oled_draw_string(0, 30+a, "Hello World!", oled_gram, 1, Show8x16);
      oled_refresh_flag = 1;
      a++;
      delay_ms(5);
    }
    while (a > 0)
    {
      oled_clear(oled_gram, 0);
      oled_draw_string(0, 10+a, "Hello World!", oled_gram, 1, Show6x8);
      oled_draw_string(0, 30+a, "Hello World!", oled_gram, 1, Show8x16);
      show_dig_num(10, 10, (i++) % 10, oled_gram, 1);
      oled_refresh_flag = 1;
      a--;
      delay_ms(5);
    }
  }
#endif

  // test_draw_waveform(oled_gram);

  // test_draw_rect_animation(oled_gram);

  while (1)
  {
    test_switch_module_task_main();
    // delay_ms(1000);
  }
}