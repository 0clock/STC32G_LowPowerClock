/*
 * @Author: 0clock
 * @Date: 2024-08-03 01:12:38
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-03 04:17:20
 * @FilePath: \STC32G_LowPowerClock\Dmx_STC32F12K54_Library_V1.3.1\Code\myoled.c
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#include "stc32g.h"
#include "dmx_oled.h"

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
 * @brief    0.96寸6脚OLED屏幕显示BMP图片
 * @param    x           x轴坐标数据(0~127)
 * @param    y           y轴坐标数据(0~7)
 * @param    wide        图片实际像素宽度(1~127)
 * @param    high        图片实际像素高度(1~64)
 * @param    bmp         图片数组首地址
 * @return   void
 * @notes    图片最大像素为128×64
 * Example:  OLED_ShowBMP(0, 0, 128, 54, OledDMXLOGO128X54);
 *
 **/
void oled_refresh(const unsigned char *buff)
{
    unsigned int i = 0;
    unsigned char x0, y0;
    for (y0 = 0; y0 <= OLED_HEIGH/8; y0++)
    {
        Set_Pos_OLED(x0, y0);
        for (x0 = 0; x0 < OLED_WIDTH; x0++)
        {  
            Draw_Point_OLED(buff[i++]);
        }
    }
}
