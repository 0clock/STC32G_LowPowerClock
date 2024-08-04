/*
 * @Author: 0clock
 * @Date: 2024-07-15 22:26:14
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-02 23:40:16
 * @FilePath: \STC32G_LowPowerClock\firmware\device\oled\oled_ssd13063.h
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#ifndef _OLED_SSD13063_H_
#define _OLED_SSD13063_H_

#include "headfile.h"

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据
#define SIZE 16
#define XLevelL 0x02
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xff
#define X_WIDTH 128
#define Y_WIDTH 64

/*************  OLED引脚定义   **************/
sbit DC = P2 ^ 1;
sbit RST = P2 ^ 0;
sbit CS = P2 ^ 2;
sbit SI = P2 ^ 3;
sbit SCK = P2 ^ 5;

void SPI_Init(void);

void OLED_Init(void);

void OLED_WrCmd(u8 dat1);

void OLED_Fill(u8 bmp_dat);

void OLED_ShowChar(u8 x, u8 y, u8 chr,u8 size);
void OLED_ShowString(u8 x, u8 y, u8 str[5], u8 size);
#endif