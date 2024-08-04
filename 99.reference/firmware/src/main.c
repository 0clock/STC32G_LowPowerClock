/*
 * @Author: 0clock
 * @Date: 2024-07-15 21:52:05
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-03 00:22:36
 * @FilePath: \STC32G_LowPowerClock\firmware\src\main.c
 * @Description: 主函数
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */

#include "headfile.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//========================================================================
// 函数: void RESET54()
// 描述: P54位复位引脚
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void RESET54()
{
    P5M0 &= 0xed; // p54双向
    P5M1 &= 0xed;
    P54RST = 1;
}

//========================================================================
// 函数: void OLED_POW(u8 dat)
// 描述: OLED供电启动 0~3 为5/6/7/8V选择
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void OLED_POW(u8 dat)
{
    P53 = 1;
    P5M0 &= 0xf7; // p54双向
    P5M1 &= 0xf7;
    P05 = ~(dat & 0x01);
    P06 = ~(dat & 0x02);
}

//========================================================================
// 函数: IO_Init(void)
// 描述: IO初始化
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IO_Init(void)
{

    //*********************P0区*******************************
    //
    //  BMC050 DRDYO中断+  KEYSET + KEY+ 设置为双向口 默认高电平
    //
    //	（双向）P07 SPL06_INT        （开漏）P06 OLEDV1       （开漏）P05 OLEDV2    （高阻）P04 BATTERY（电池ADC采样脚）   （高阻）P03 ADC6（充电电流）   （开漏）PP02 EN（锂电池充电EN）   （高阻）P01 GHRG   （开漏）PP00 VIN/2

    P0M0 = 0x65;
    P0M1 = 0x7f;

    //*********************P4区*******************************
    //	P47 DRDYO        P46 KEYSET       P45 KEY+    P44 悬空   P43 悬空   P42 悬空   P41 悬空   P40 CCP0_EN（蜂鸣器控制）
    P4M0 = 0x1f;
    P4M1 = 0x1f;

    //*********************P1区*******************************
    //	（双向）P17 INT3O        （开漏）P16 悬空       （双向）P15 SCL    （双向）P14 SDA   （双向）P13 INT2O    （开漏）P12   （开漏）P11 BH_INT   （开漏）P10 悬空
    P1M0 = 0x47;
    P1M1 = 0x47;
    //		SCK = 0;
    P1PU = 0x30; // IIC上拉电阻

    //*********************P3区*******************************
    //	（双向）P37 INT3（BMC050）        （开漏）P36 悬空       （开漏）P35 悬空    （双向）P34 SW360   （开漏）P33 悬空    （开漏）P32 悬空     （开漏）P31 TX   （开漏）P30 RX
    P3M0 = 0x6f;
    P3M1 = 0x6f;

    //*********************P5区*******************************
    //	（开漏）P57         （开漏）P56        （开漏）P55     （双向）P54 RSET   （开漏）P53 5VEN    （开漏）P52 BATTERY_ON     （开漏）P51 TEXT  （双向）P50 SQW（1Hz）
    P5M0 = 0xee;
    P5M1 = 0xee;

    //*********************P2区*******************************
    //	（双向）P27  KEY-       （双向）P26  KEYMODE      （推挽）P25 SCLK    （开漏）P24 悬空   （推挽）P23 MOSI    （开漏）P22 CS     （双向）P21 DC  （双向）P20 RES
    CS = 0;
    P2M0 = 0x3c;
    P2M1 = 0x14;

    P2SR = 0xd7;
    P2DR = 0xd7;

    P6M0 = 0xff;
    P6M1 = 0xff;

    P7M0 = 0xff;
    P7M1 = 0xff;

    // 输入寄存器使能
    P0IE = 0x1b;
    P1IE = 0x30;
    P2IE = 0xc0;
    P3IE = 0x13; // 0x10
    P4IE = 0x60;
    P5IE = 0x11;
    P6IE = 0x00;
    P7IE = 0x00;
}

unsigned char a[5] = "1234";
int main(void)
{
    
    EAXFR = 1; // 扩展SFR寄存器使能
    WTST = 1;  // 设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    ELVD = 0;     // 低压检测允许关闭
    ENLVR = 0;    // 低压复位关闭
    WDT_FLAG = 0; // 看门狗关闭

    //	  USBCLK |= 0x60;//PLL输入时钟8分频  52/8=6.5M
    //	  HPLLCR = 0x80;
    //		delay_ms(3);

    //		CLKSEL |= 0x40;                       //HSPWM/HSSPI选择PLL输出时钟为时钟源
    //		HSCLKDIV = 4;

    _nop_();
    _nop_();
    _nop_();
    _nop_();
    NOP27(); // 重中之重
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    IO_Init();
    RESET54();
    OLED_POW(5);
    SPI_Init();
    OLED_Init();

    OLED_Fill(0);

    
    

    OLED_ShowString(0,0,a,16);

    OLED_ShowChar(0, 4, 'H', 16);
    OLED_ShowChar(8, 4, 'e', 16);
    OLED_ShowChar(16, 4, 'l', 16);
    OLED_ShowChar(24, 4, 'l', 16);
    OLED_ShowChar(32, 4, 'o', 16);
    while(1);

    return 0;
}