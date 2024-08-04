/*
 * @Author: 0clock
 * @Date: 2024-08-03 00:48:43
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-04 04:34:41
 * @FilePath: \Dmx_STC32F12K54_Library_V1.3.1\User\main.c
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#include "dmx_all.h"

// #include "u8g2_porting.h"
/**	����������������
* �ÿ�Դ�ⵥƬ����ƵΪ���ǿ�������ڲ�IRC��Ƶ,STC-ISP����ѡ��Ч
* �����޸ĵ�Ƭ����ƵӦȥdmx_board.h���޸�MAIN_FOSC�궨��
����������������**/

int a = 0, i = 0;
void main(void)
{
  // ��ʼ��оƬ
  init_chip();

  // ���¿ɷ����û������
  init_pit_ms(PIT0, 1);
  Buzzer_Close();
  OLED_POW(0);
  Init_OLED();

  // oled_drawline(0,0,127,63,oled_gram,1,1);
  // oled_drawline(127,0,0,63,oled_gram,1,1);
  // oled_drawcircle(64,32,20,oled_gram,1);
  // oled_draw_round_rect(20,20,107,43,5,oled_gram,1,1);
  // oled_draw_arc(64, 32, 0, 30, 20, oled_gram, 1, 1);
  oled_refresh_flag = 1;

  test_draw_waveform(oled_gram);

  //test_draw_rect_animation(oled_gram);

  while (1)
  {

    delay_ms(16);
  }
}