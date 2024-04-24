#include "TV_Mode.h"
#include "OLED.h"
#include "DATA.h"
#include "ADC.h"

extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

extern u8 Time_New[7];
extern u8 Time_Low[7];

extern u32 xdata JCQ06_00[10];

u16 Low_wendu = 0;

u8 Sound_display_flag = 0;

u8 New_time_num[7];
u8 Low_time_num[7];

//========================================================================
// 函数: time_show_s()
// 描述: 初始化左上角8*16时分秒
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void time_show_s()
{
    ZF_8X16(0, 0, (u8)(New_time_num[4]));  // 显示时十位
    ZF_8X16(8, 0, (u8)(New_time_num[5]));  // 显示时个位
    ZF_8X16(16, 0, 20);                    // 显示-
    ZF_8X16(24, 0, (u8)(New_time_num[2])); // 显示分十位
    ZF_8X16(32, 0, (u8)(New_time_num[3])); // 显示分个位
    ZF_8X16(40, 0, 20);                    // 显示-
    ZF_8X16(48, 0, (u8)(New_time_num[0])); // 显示秒十位
    ZF_8X16(56, 0, (u8)(New_time_num[1])); // 显示秒个位
}

//========================================================================
// 函数: UI_humidity()
// 描述: 显示湿度
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void UI_humidity()
{
    ZF_8X16(88, 2, (u8)(dat00[0x0042] / 1000));         // 显示湿度十位
    ZF_8X16(96, 2, (u8)((dat00[0x0042] % 1000) / 100)); ////显示湿度个位
    ZF_8X16(112, 2, (u8)((dat00[0x0042] % 100) / 10));  ////显示湿度小数点后一位
}

//========================================================================
// 函数: UI_pressure()
// 描述: 初始化气压显示
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void UI_pressure()
{
    u16 k = 0;

    k = JCQ06_00[4] / 10;

    ZF_8X8(120, 5, (u8)(k % 10));
    ZF_8X8(108, 5, (u8)((k % 100) / 10));
    ZF_8X8(100, 5, (u8)((k % 1000) / 100));
    ZF_8X8(92, 5, (u8)((k % 10000) / 1000));
    k = k / 10000;
    if (k)
    {
        ZF_8X8(84, 5, (u8)(k));
    }
    else
    {
        ZF_8X8(84, 5, 10); // 显示空白
    }
}

//========================================================================
// 函数: u8 Week_conversion()
// 描述: 返回星期几在点阵中的位置
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
u8 Week_conversion()
{
    u8 p = 0;
    if (Time_New[3] & 0x01)
    {
        p = 13;
    }
    else if (Time_New[3] & 0x02)
    {
        p = 14;
    }
    else if (Time_New[3] & 0x04)
    {
        p = 15;
    }
    else if (Time_New[3] & 0x08)
    {
        p = 16;
    }
    else if (Time_New[3] & 0x10)
    {
        p = 17;
    }
    else if (Time_New[3] & 0x20)
    {
        p = 18;
    }
    else if (Time_New[3] & 0x40)
    {
        p = 19;
    }
    return p;
}

//========================================================================
// 函数: Time_comparison_GET1()
// 描述: 将秒分时的个位和十位拿出来
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void Time_comparison_GET1()
{

    New_time_num[1] = Time_New[0] % 16;
    New_time_num[0] = Time_New[0] / 16;
    New_time_num[3] = Time_New[1] % 16;
    New_time_num[2] = Time_New[1] / 16;
    New_time_num[5] = Time_New[2] % 16;
    New_time_num[4] = Time_New[2] / 16;
}

//========================================================================
// 函数: u16 Time_comparison()
// 描述: 返回年月日星期时分秒是否更新
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
u16 Time_comparison()
{
    u16 j = 0;

    Time_comparison_GET1();
    if (Time_New[0] != Time_Low[0])
    {
        if (New_time_num[0] != Low_time_num[0])
            j |= 0x0002;
        if (New_time_num[1] != Low_time_num[1])
            j |= 0x0001;
    }

    if (Time_New[1] != Time_Low[1])
    {
        if (New_time_num[2] != Low_time_num[2])
            j |= 0x0008;
        if (New_time_num[3] != Low_time_num[3])
            j |= 0x0004;
    }

    if (Time_New[2] != Time_Low[2])
    {
        if (New_time_num[4] != Low_time_num[4])
            j |= 0x0020;
        if (New_time_num[5] != Low_time_num[5])
            j |= 0x0010;
    }
    if (Time_New[3] != Time_Low[3])
        j |= 0x0040;
    if (Time_New[4] != Time_Low[4])
        j |= 0x0080;
    if (Time_New[5] != Time_Low[5])
        j |= 0x0100;
    if (Time_New[6] != Time_Low[6])
        j |= 0x0200;

    return j;
}

//========================================================================
// 函数: time_synchronization()
// 描述: 旧时间Time_Low[]更新
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void time_synchronization()
{
    u8 i = 0;
    for (i = 0; i < 7; i++)
    {
        Time_Low[i] = Time_New[i];
        Low_time_num[i] = New_time_num[i];
    }
}

//========================================================================
// 函数: TVMode_int()
// 描述: 旧时间Time_Low[]更新
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void TVMode_int()
{
    Time_comparison_GET1();
    IU_Battery_Icon_S(80, 0, 1); // 显示电池UI

    yue_ri(0, 20, 0);                                                  // 显示年份前面的20
    yue_ri(16, (u8)((Time_New[6] / 16 * 10) + (Time_New[6] % 16)), 0); // 显示年份20XX后面的XX

    ZF_8X16(32, 0, 22); //"-"

    yue_ri(37, (u8)((Time_New[5] / 16 * 10) + (Time_New[5] % 16)), 0); // 月

    ZF_8X16(53, 0, 22); //"-"

    yue_ri(59, (u8)((Time_New[4] / 16 * 10) + (Time_New[4] % 16)), 0); // 日

    ZF_8X16(112, 1, Week_conversion()); // 显示星期

    ZF_8X16(100, 1, 29); // 显示“星”
    ZF_8X16(88, 1, 28);  // 显示“期”

    // 两个冒号
    shifen(12, 40);
    shifen(12, 80);

    // 时
    shifen_dh(8, 3, (New_time_num[4]) * 32, 40);
    shifen_dh(24, 3, (New_time_num[5]) * 32, 40);

    // 分
    shifen_dh(48, 3, (New_time_num[2]) * 32, 40);
    shifen_dh(64, 3, (New_time_num[3]) * 32, 40);

    // 秒
    shifen_dh(88, 3, (New_time_num[0]) * 32, 40);
    shifen_dh(104, 3, (New_time_num[1]) * 32, 40);
    time_synchronization();
}

//========================================================================
// 函数: TVMode2_int()
// 描述: 显示温度湿度气压初始化
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void TVMode2_int()
{
    IU_Battery_Icon_S(80, 0, 1);
    time_show_s();
    shifen_dh(4, 3, (dat00[0x0041] / 1000) << 5, 40);
    shifen_dh(20, 3, ((dat00[0x0041] % 1000) / 100) << 5, 40);
    shifen_dh(36, 3, 11 << 5, 100);
    shifen_dh(44, 3, ((dat00[0x0041] % 100) / 10) << 5, 40);
    shifen_dh(60, 3, 13 << 5, 100);

    UI_humidity();
    ZF_8X16(104, 2, 11);
    ZF_8X16(120, 2, 21);
    ZF_8X8(116, 5, 15);
    ZF_8X8(120, 6, 14);
    ZF_8X8(112, 6, 13);
    ZF_8X8(104, 6, 12);
    UI_pressure();

    time_synchronization();
}

//========================================================================
// 函数: TVMode3_int()
// 描述: 显示电源显示界面初始化
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void TVMode3_int()
{
    time_show_s();
    IU_Battery_cheek();
    IU_Battery_cheek_renovate();
    time_synchronization();
}

//========================================================================
// 函数: void TVMode_1()
// 描述: 时钟界面局部刷新
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void TVMode_1()
{
    u16 dat = 0;

    if (dat00[0x0009] & 0x0001)
    {
        // 秒刷新
        shifen_dh(104, 3, (33 - dat00[0x12] + (Low_time_num[1]) * 32), 40);
        if (dat00[0x0000] & 0x0004)
        {
            IU_Battery_Icon_S(80, 0, 0);
            dat00[0x0000] &= ~0x0004;
        }
    }
    if (dat00[0x0009] & 0x0002)
    {
        shifen_dh(88, 3, (33 - dat00[0x12] + (Low_time_num[0]) * 32), 24);
    }
    if (dat00[0x0009] & 0x0004)
    {
        shifen_dh(64, 3, (33 - dat00[0x12] + (Low_time_num[3]) * 32), 40);
    }
    if (dat00[0x0009] & 0x0008)
    {
        shifen_dh(48, 3, (33 - dat00[0x12] + (Low_time_num[2]) * 32), 24);
    }
    if (dat00[0x0009] & 0x0010)
    {
        if ((u8)(Low_time_num[4]) == 2)
        {
            shifen_dh(24, 3, (33 - dat00[0x12] + (Low_time_num[5]) * 32), 16);
        }
        else
        {
            shifen_dh(24, 3, (33 - dat00[0x12] + (Low_time_num[5]) * 32), 40);
        }
    }
    if (dat00[0x0009] & 0x0020)
    {
        shifen_dh(8, 3, (33 - dat00[0x12] + (Low_time_num[4]) * 32), 12);
    }
    if (dat00[0x0009] & 0x0040)
    {
        ZF_8X16(112, 1, Week_conversion());
    }
    if (dat00[0x0009] & 0x0080)
    {
        yue_ri(59, (u8)((Time_New[4] / 16 * 10) + (Time_New[4] % 16)), 0); //
    }
    if (dat00[0x0009] & 0x0100)
    {
        yue_ri(37, (u8)((Time_New[5] / 16 * 10) + (Time_New[5] % 16)), 0); //
    }
    if (dat00[0x0009] & 0x0200)
    {
        yue_ri(16, (u8)((Time_New[6] / 16 * 10) + (Time_New[6] % 16)), 0); //
    }
    // time_synchronization();
}

//========================================================================
// 函数: void TVMode_23TimeRenew(u16 dat)
// 描述: 左上角时分秒局部刷新
// 参数: dat刷新标志位
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void TVMode_23TimeRenew(u16 dat)
{

    if (dat & 0x0001) // 秒刷新
    {
        ZF_8X16(56, 0, (u8)(New_time_num[1]));
    }
    if (dat & 0x0002)
    {
        ZF_8X16(48, 0, (u8)(New_time_num[0]));
    }

    if (dat & 0x0004)
    {
        ZF_8X16(32, 0, (u8)(New_time_num[3]));
    }
    if (dat & 0x0008)
    {
        ZF_8X16(24, 0, (u8)(New_time_num[2]));
    }

    if (dat & 0x0010)
    {
        ZF_8X16(8, 0, (u8)(New_time_num[5]));
    }
    if (dat & 0x0020)
    {
        ZF_8X16(0, 0, (u8)(New_time_num[4]));
    }
}

//========================================================================
// 函数: void TVMode_2()
// 描述: 温度湿度气压界面局部刷新
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void TVMode_2()
{
    u16 dat = 0;
    IU_Battery_Icon_S(80, 0, 0);
    TVMode_23TimeRenew(dat00[0x0009]);

    dat = dat00[0x0041] / 1000;
    if (dat != (Low_wendu / 1000))
    {
        shifen_dh(4, 3, dat << 5, 40);
    }

    dat = (dat00[0x0041] % 1000) / 100;
    if (dat != (Low_wendu % 1000) / 100)
    {
        shifen_dh(20, 3, ((dat00[0x0041] % 1000) / 100) << 5, 40);
    }

    dat = (dat00[0x0041] % 100) / 10;
    if (dat != (Low_wendu % 100) / 10)
    {
        shifen_dh(44, 3, dat << 5, 40);
    }
    //**************温湿度*****************************

    UI_humidity();
    UI_pressure();

    Low_wendu = dat00[0x0041];
    time_synchronization();
}

//========================================================================
// 函数: void TVMode_2()
// 描述: 电源显示界面局部刷新
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void TVMode_3()
{
    u16 dat = 0;

    // ADC_Get_V1();
    TVMode_23TimeRenew(dat00[0x0009]);
    IU_Battery_cheek_renovate();
    time_synchronization();
}

//========================================================================
// 函数: IU_TVMode()
// 描述: TV模式任务处理
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IU_TVMode()
{
    u8 a = 0;
    u16 DA = 0;
    u16 DB = 0;
    u16 DC = 0;

    if (dat00[0x10] == 0x0020 && dat00[0x1A] == 0 && dat00[0x002E] > 0) // TV时钟模式
    {
        if ((dat00[0x002E] & 0x0001) && !(dat00[0x002E] & 0x0002))
        {
            dat00[0x002E] |= 0x0002; // 隐藏寄存器（时间更新状态位寄存器）0x0002 开始秒刷新标志位
        }
        if ((dat00[0x002E] & 0x0001) && (dat00[0x002E] & 0x0002))
        {
            if (dat00[0x12] == 0x0000 && !(dat00[0x002E] & 0x0004)) // UI线程运算次数为零 + 不在秒刷新
            {
                dat00[0x12] = 32;                  // UI线程运算次数寄存器 = 32
                fast_SleepQueue_aheadW1(8, 1, 32); // 申请32个UI刷新任务
                dat00[0x002E] |= 0x0004;           // 隐藏寄存器（时间更新状态位寄存器）0x0004 开始秒刷新标志位
                dat00[0x0006] |= 0x0002;           // dat00[0x0006] bit1：0 时钟滚动空闲 1 时钟滚动繁忙
                dat00[0] |= 0x0100;                // dat00[0x0006] bit8：0 UI空闲 1 UI滚动
            }
            if (dat00[0x12] != 0x0000 && (dat00[0x002E] & 0x0004)) // UI线程运算次数不为零 + 还在秒刷新
            {
                TVMode_1();    // 秒刷新
                dat00[0x12]--; // UI线程运算次数-1
            }
            if (dat00[0x12] == 0x0000 && (dat00[0x002E] & 0x0004)) // UI线程运算次数为零 + 还在秒刷新
            {
                dat00[0x002E] = 0;        // 结束任务
                dat00[0x0006] &= ~0x0002; // dat00[0x0006] bit1：0 时钟滚动空闲 1 时钟滚动繁忙
                dat00[0] |= 0x0100;       // dat00[0x0006] bit1：0 时钟滚动空闲 1 时钟滚动繁忙
                time_synchronization();
            }
        }
    }
    else if (dat00[0x10] == 0x0021 && dat00[0x1A] == 0 && dat00[0x002E] > 0) // TV时钟模式
    {
        TVMode_2();
        dat00[0x002E] = 0;
        time_synchronization();
    }
    else if (dat00[0x10] == 0x0022 && dat00[0x1A] == 0 && dat00[0x002E] > 0) // TV时钟模式
    {
        TVMode_3();
        dat00[0x002E] = 0;
        time_synchronization();
    }

    a = (u8)P45 + (u8)P46 + (u8)P27; // 查询快速查询按键状态
    if ((dat00[0x0000] & 0x0008) && (a == 3) && (dat00[0x0010] >= 0x0020 && dat00[0x0010] <= 0x0022) && dat00[0x1A] == 0)
    {
        if (dat00[0x37]) // 如果是低功耗模式则调用低功耗模式的显示时长
        {
            DA = dat00[0x0031]; // 时间显示模式时长
            DB = dat00[0x0032]; // 温度湿度显示模式时长
            DC = dat00[0x0033]; // 电源显示模式
        }
        else
        {
            DA = dat00[0x0022];
            DB = dat00[0x0023];
            DC = dat00[0x0024];
        }
        dat00[0x002D]++;             // 计时
        if (dat00[0x0010] == 0x0020) // 如果是时间显示页面
        {
            if (dat00[0x002D] >= DA) // 时间显示页面到达指定的时间
            {
                if (DB > 0) // 温湿度显示设定时间不为0，跳到该页面
                {
                    dat00[0x10] = 0x0021;
                    dat00_flag[0x10] = 1;
                }
                else if (DC > 0) // 电源显示设定时间不为0，跳到该页面
                {
                    dat00[0x10] = 0x0022;
                    dat00_flag[0x10] = 1;
                }
                if ((dat00[0x37] > 0) && (DB == 0) && (DC == 0))
                    dat00[0] |= 0x0020;
                dat00[0x002D] = 0;
            }
        }
        else if (dat00[0x0010] == 0x0021)
        {
            if (dat00[0x002D] >= DB)
            {
                if (DC > 0)
                {
                    dat00[0x10] = 0x0022;
                    dat00_flag[0x10] = 1;
                }
                else if (DA > 0)
                {
                    dat00[0x10] = 0x0020;
                    dat00_flag[0x10] = 1;
                }
                if ((dat00[0x37] > 0) && (DC == 0))
                    dat00[0] |= 0x0020;
                dat00[0x002D] = 0;
            }
        }
        else if (dat00[0x0010] == 0x0022)
        {
            if (dat00[0x002D] >= DC)
            {
                dat00[0x10] = 0x0020;
                dat00_flag[0x10] = 1;
                dat00[0x002D] = 0;
                if ((dat00[0x37] > 0))
                    dat00[0] |= 0x0020;
            }
        }

        dat00[0x0000] &= ~0x0008;
    }
}
