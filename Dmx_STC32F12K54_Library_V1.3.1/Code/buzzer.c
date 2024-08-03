/*
 * @Author: 0clock
 * @Date: 2024-08-03 00:57:51
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-03 01:00:24
 * @FilePath: \STC32G_LowPowerClock\Dmx_STC32F12K54_Library_V1.3.1\Code\buzzer.c
 * @Description: 
 * 
 * Copyright (c) 2024 by 0clock, All Rights Reserved. 
 */

#include "stc32g.h"
#include "buzzer.h"

//========================================================================
// 函数: void Buzzer_Open()
// 描述: 蜂鸣器打开
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void Buzzer_Open()
{
    P4M0 |= 0x01;
    P4M1 &= 0xFE;
    P40=1;
}
//========================================================================
// 函数: void Buzzer_Close()
// 描述: 蜂鸣器关闭
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void Buzzer_Close()
{
    P4M0 |= 0x01;
    P4M1 |= 0x01;
    P40=0;
}