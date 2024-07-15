/*
 * @Author: 0clock
 * @Date: 2024-07-15 21:52:05
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-07-15 22:18:12
 * @FilePath: \STC32G_LowPowerClock\firmware\src\main.c
 * @Description: 主函数
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */

#include "headfile.h"

int main(void)
{   
    SPI_Init();  
    OLED_Init();
    
    return 0;
}