/*
 * @Author: 0clock
 * @Date: 2024-08-03 00:48:43
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-03 17:56:02
 * @FilePath: \Dmx_STC32F12K54_Library_V1.3.1\User\main.c
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#include "dmx_all.h"

#include "buzzer.h"
#include "dmx_oled.h"
#include "myoled.h"
#include "BMP.h"
//#include "u8g2_porting.h"
/**	����������������
* �ÿ�Դ�ⵥƬ����ƵΪ���ǿ�������ڲ�IRC��Ƶ,STC-ISP����ѡ��Ч
* �����޸ĵ�Ƭ����ƵӦȥdmx_board.h���޸�MAIN_FOSC�궨��
����������������**/

int a = 0;
void main(void)
{
  // ��ʼ��оƬ
  init_chip();

  // ���¿ɷ����û������
  Buzzer_Close();
  OLED_POW(0);
  Init_OLED();
  // LCD_Init();
  printf("��ʼ����Ļ��ɣ�\n\r");
  // LCD_show();

  Show_String_OLED(0, 0, "Hello World!", Show6x8);
  while(1);
  // while (1)
  // {
  //   Show_Int_OLED(0, 7, a++, 5, Show6x8);
  //   // Show_BMP_OLED(0, 0, 128, 64, BWeLyHqW);
  //   // Show_BMP_OLED(0, 0, 128, 64, Apple_Hello);
  //   // Show_BMP_OLED(0, 0, 128, 64, Apple_Logo);
  //   Show_BMP_OLED(0, 0, 128, 64, Time_Logo);
  // }
  
}