#include "DATA.h"
#include "UI.h"
#include "OLED.h"
#include "time.h"
#include "UART1.h"
#include "INS5699S.h"
#include "SleepMode.h"

/****************************** 控制寄存器组 ***********************************/
volatile u16 xdata dat00[System_data_num] =
    {
        // 0程序系统参数
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 1UI光标寄存器组
        0, 0, 0, 0, 0x8000, 0x8000, 0, 0x000A, 0x000A, 0, 0, 0, 0, 0, 0, 0,

        // 2标准模式寄存器组
        6, 1, 40, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 3休眠模式寄存器组
        6, 1, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 4环境信息寄存器组
        0x0007, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 5亮度设置寄存器组
        0x0003, 0x00FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 6指南针寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 7水平尺寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 8时间设置寄存器组
        8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,

        // 9闹钟寄存器组
        9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 10增量闹钟寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 11秒表寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 12声音设置寄存器组
        0x0003, 0, 20, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 13手电筒设置寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 14电源管理寄存器组
        0x0006, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 15系统设置寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 16系统信息寄存器组
        0x000C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 17工厂模式寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

volatile u16 xdata dat00_low[System_data_num] =
    {
        // 0程序系统参数
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 1UI光标寄存器组
        0, 0, 0, 0, 0x8000, 0x8000, 0, 0x000A, 0x000A, 0, 0, 0, 0, 0, 0, 0,

        // 2标准模式寄存器组
        6, 1, 40, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 3休眠模式寄存器组
        6, 1, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 4环境信息寄存器组
        0x0007, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 5亮度设置寄存器组
        0x0003, 0x00FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 6指南针寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 7水平尺寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 8时间设置寄存器组
        8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,

        // 9闹钟寄存器组
        9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 10增量闹钟寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 11秒表寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 12声音设置寄存器组
        0x0003, 0, 20, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 13手电筒设置寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 14电源管理寄存器组
        0x0006, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 15系统设置寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 16系统信息寄存器组
        0x000C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 17工厂模式寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

volatile u8 xdata dat00_flag[System_data_num] = {0};

volatile u16 xdata dat00_max[System_data_num] =
    {
        // 0程序系统参数
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 1UI光标寄存器组
        0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 2标准模式寄存器组
        0XFF, 0x1001, 0x110C, 0x1103, 0x1103, 0x1103, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 3休眠模式寄存器组
        0XFF, 0x1001, 0x110C, 0x1103, 0x1103, 0x1103, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 4环境信息寄存器组
        0XFF, 0xA38D, 0x948D, 0x378D, 0x2C8D, 0x2C8D, 0x2C8D, 0x2C8D, 0, 0, 0, 0, 0, 0, 0, 0,

        // 5亮度设置寄存器组
        0XFF, 0x0A, 0x0B, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 6指南针寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 7水平尺寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 8时间设置寄存器组
        0xff, 0x00ff, 0x000F, 0x000F, 0x000E, 0x0010, 0x0012, 0x0011, 0x0013, 0x1001, 0x1001, 0x1001, 0x1103, 0, 0, 0,

        // 9闹钟寄存器组
        0xff, 0x000F, 0x000F, 0x000E, 0x0012, 0x0011, 0x1001, 0x1001, 0x1103, 0x1001, 0, 0, 0, 0, 0, 0,

        // 10增量闹钟寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 11秒表寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 12声音设置寄存器组
        0XFF, 1, 0x210C, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 13手电筒设置寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 14电源管理寄存器组
        0xFF, 0x0081, 0x0081, 0xDA8D, 0x298D, 0x548D, 0x0001, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 15系统设置寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 16系统信息寄存器组
        0XFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // 17工厂模式寄存器组
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};
// 最大值寄存器：
// 0:0~0xffff
// 1:0~1
// 2:0~10
// 3:0~100
// 4:0~1000
// 5:0~10000
// 6:0~0xff
// 7:0~0x80
// 8:0~0x7F
// 9:0~UI_main_branchnum 主菜单分支数-1
// 10:1~0xFF
// 11:0~3
// 12:1~100
// 13:0~0xFFFF
// 14:0~23
// 15:0~59
// 16:0~6
// 17:1~11
// 18:1~31
// 19:0~99
// 20:1~100

// 7bit=1:不可修改
// 6bit=1:不显示数值

/****************************** UI图形缓冲存器组 ***********************************/
u8 UI_dat[8][16] = {0};

u8 UI_TX[3][64] = {0};

/****************************** 睡眠寄存器组 ***********************************/
u16 Sleep_Queue[Sleep_queue_num] = {0};
u16 Sleep_Queue_count = 0;
u16 Sleep_Queue_count_target = 0;

u16 Sleep_Queue_task[Sleep_queue_num] = {0};

/****************************** 时间寄存器组 ***********************************/
// 年 月 日 周 时 分 秒
u8 xdata Time_New[7] = {0};
u8 xdata Time_Low[7] = {0};
// u8 xdata Time_int[7] = {0x50,0x30,0x09,0x01,0x27,0x02,0x23};
u8 xdata Time_int[7] = {0x50, 0x59, 0x23, 0x01, 0x31, 0x01, 0x23};

/****************************** 闹钟存器组 ***********************************/

// volatile u16 xdata AlarmClock_max[16]={0xff,0x00ff,0x000F,0x000F,0x0011,0x0010,0x0012,0x0011,0x0013,0,0,0,0,0,0,0};

// u16 AlarmClock_SET[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u32 AlarmClock[16] = {0x02100000, 0x02100000, 0x02100000, 0x02100000, 0x02100000, 0x02100000, 0x02100000, 0x02100000, 0x02100000, 0x02100000, 0, 0, 0, 0, 0, 0};

void Array_Initialization_8(u8 *array_name, u16 array_num)
{
    u16 i = 0;
    for (i = 0; i < array_num; i++)
    {
        array_name[i] = 0;
    }
}

void Array_Initialization_16(u16 *array_name, u16 array_num)
{
    u16 i = 0;
    for (i = 0; i < array_num; i++)
    {
        array_name[i] = 0;
    }
}

void Array_Initialization_()
{
    Array_Initialization_16(Sleep_Queue, Sleep_queue_num);
    Array_Initialization_16(Sleep_Queue_task, Sleep_queue_num);
    Array_Initialization_8(Time_Low, 7);
    Array_Initialization_8(Time_New, 7);
    Array_Initialization_8(dat00_flag, System_data_num);
}

void array_fill(u16 *array_name, u16 array_num, u16 array_dat, u8 w)
{
    u8 j = 0;
    array_num = array_num + w;
    for (j = w; j < array_num; j++)
    {
        array_name[j] = array_dat;
    }
}

u8 RFF()
{
    u8 i = 0;
    u8 b = 0;
    u8 a = 0;
    for (i = 7; i < Sleep_queue_num; i = i + 8)
    {
        if (Sleep_Queue[i] == 0)
        {
            b = i;
            i = 200;
        }
    }

    for (i = b - 7; i <= b; i++)
    {
        if (Sleep_Queue[i] == 0)
        {
            a = i;
            i = 200;
        }
    }
    return a;
}

void SleepQueue_MoveLeft(u8 p)
{
    u8 j = 0;
    u8 h = 0;
    h = RFF();
    for (j = 0; j < h; j++)
    {
        Sleep_Queue[j] = Sleep_Queue[j + p];
        Sleep_Queue_task[j] = Sleep_Queue_task[j + p];
    }
    Sleep_Queue[h] = 0;
    Sleep_Queue_task[h] = 0;
}

void SleepQueue_MoveLeft2()
{
    u8 j = 0;
    u8 h = 0;
    h = Sleep_queue_num - 2;
    for (j = 0; j < h; j++)
    {
        Sleep_Queue[j] = Sleep_Queue[j + 1];
        Sleep_Queue_task[j] = Sleep_Queue_task[j + 1];
    }
    Sleep_Queue[h] = 0;
    Sleep_Queue_task[h] = 0;
}

void SleepQueue_behindW(u16 time, u16 num)
{
    u8 j = 0;
    j = RFF();

    Sleep_Queue[j] = time;
    Sleep_Queue_task[j] = num;
}

void SleepQueue_aheadW1(u16 time, u16 num, u8 f)
{
    u8 i = 0;
    u8 k = 0;
    u16 j = 0;
    for (i = f; i < Sleep_queue_num; i++)
    {
        j = j + Sleep_Queue[i];

        if (Sleep_Queue[i] != 0 && time < j)
        {
            for (k = RFF(); k > i; k--)
            {
                Sleep_Queue[k] = Sleep_Queue[k - 1];
                Sleep_Queue_task[k] = Sleep_Queue_task[k - 1];
            }
            Sleep_Queue[i] = time - (j - Sleep_Queue[i + 1]);
            Sleep_Queue_task[i] = num;
            Sleep_Queue[i + 1] = Sleep_Queue[i + 1] - Sleep_Queue[i];
            i = 200;
        }
        else if (Sleep_Queue[i] != 0 && time == j)
        {
            Sleep_Queue_task[i] |= num;
            i = 200;
        }
        else if (Sleep_Queue[i] == 0)
        {
            Sleep_Queue[i] = time - j;
            Sleep_Queue_task[i] = num;
            i = 200;
        }
    }
}

void fast_SleepQueue_aheadW1(u16 dat, u16 task, u8 num)
{
    u8 a = 0;
    u8 i = 0;
    u16 j = 0;
    a = RFF(); // 获得数组有数值的长度
    if (a == 0)
    {
        array_fill(Sleep_Queue, num, dat, 0);
        array_fill(Sleep_Queue_task, num, task, 0);
    }
    else
    {
        array_fill(Sleep_Queue, num, dat, a);
        array_fill(Sleep_Queue_task, num, task, a);
        for (i = 0; i < a; i++) // 共处理 n 个数据
        {
            j = j + Sleep_Queue[i];
            SleepQueue_aheadW1(j, Sleep_Queue_task[i], a);
        }
    }
    if (a)
        SleepQueue_MoveLeft(a);
}

void fast_SleepQueue_aheadW2()
{
    u8 i = 0;
    for (i = 0; i < 48; i++) // 共处理 n 个数据
    {
        SleepQueue_aheadW1(99, 1, 0);
    }
}

u16 data_GetMax(u16 dat)
{
    u16 k = 0;
    dat = dat00_max[dat];
    dat &= 0x003F;
    switch (dat)
    {
    case 0x00:
        k = 0xffff;
        break;
    case 0x01:
        k = 1;
        break;
    case 0x02:
        k = 10;
        break;
    case 0x03:
        k = 100;
        break;
    case 0x04:
        k = 1000;
        break;
    case 0x05:
        k = 10000;
        break;
    case 0x06:
        k = 0xff;
        break;
    case 0x07:
        k = 0x80;
        break;
    case 0x08:
        k = 0x7f;
        break;
    case 0x09:
        k = UI_main_branchnum;
        break;
    case 0x0A:
        k = 0xff;
        break;
    case 0x0B:
        k = 0x03;
        break;
    case 0x0C:
        k = 100;
        break;
    case 0x0D:
        k = 0xFFFF;
        break;
    case 0x0E:
        k = 0x0017;
        break;
    case 0x0F:
        k = 0x003B;
        break;
    case 0x10:
        k = 0x0006;
        break;
    case 0x11:
        k = 0x000B;
        break;
    case 0x12:
        k = 0x001F;
        break;
    case 0x13:
        k = 99;
        break;
    case 0x14:
        k = 100;
        break;
    case 0xFF:
        k = 0;
        break;
    default:
        k = 0;
        break;
        //**********************************************
    }
    return k;
}

u16 data_GetMin(u16 dat)
{
    u16 k = 0;
    dat = dat00_max[dat];
    dat &= 0x003F;

    if (dat == 0x0A | dat == 0x0C | dat == 0x11 | dat == 0x12 | dat == 0x14)
    {
        k = 1;
    }
    return k;
}

void data00_Change(u16 address, u16 add)
{
    if (add)
    {
        if (dat00[address] == data_GetMax(address))
        {
            dat00[address] = data_GetMin(address);
        }
        else
        {
            dat00[address]++;
        }
    }
    else
    {
        if (dat00[address] == data_GetMin(address))
        {
            dat00[address] = data_GetMax(address);
        }
        else
        {
            dat00[address]--;
        }
    }
    dat00_flag[address] = 1;
}

u16 GET_SC() // 获取目前是调用0x0?0组标号
{
    u16 dat = 0;
    dat = (dat00[0x10] + 1) * 16;
    return dat;
}

u16 GET_SCdat() // 获取目前是调用0x0?0组 的值（当前组寄存器个数）
{
    return dat00[GET_SC()];
}

u16 GET_GBdat() // 获取目前是调用0x0?0组 的光标 的值
{
    return dat00[GET_SC() + 0x0f];
}

u16 GET_SCdat3() // 获取目前是调用0x0?0组 的 指向
{
    return GET_GBdat() + 1 + GET_SC();
}

void dat00_Change()
{
    if (dat00_flag[0x14] == 0x01) // 主菜单界面移动触发
    {
        dat00[0x11] = dat00[0x14] % 16;
        dat00_flag[0x14] = 0;
    }
    if (dat00_flag[0x17] == 0x01) //
    {
        dat00[GET_SC() + 0x000f] = dat00[0x17] % GET_SCdat();
        dat00_flag[0x17] = 0;
    }
    if (dat00_flag[0x81] == 0x01)
    {
        SET_time_c();
        INS5699_timeW(Time_int);
        dat00_flag[0x81] = 0;
    }
    if (dat00_flag[0x36] == 0x01)
    {
        SleepMode_ON();

        SleepMode_OFF();
        dat00_flag[0x36] = 0;
    }
}

u8 BCD_to_X10(u8 a)
{
    a = (a / 16) * 10 + (a % 16);
    return a;
}

u8 X10_to_BCD(u16 a)
{
    a = (a / 10) * 16 + (a % 10);
    return (u8)a;
}

u8 GET_WEEK(u8 a)
{
    u8 i = 0;
    u8 b = 0x01;
    for (i = 0; i < 8; i++)
    {
        if (a == b)
            break;
        b = b << 1;
    }

    return i;
}

u8 SET_WEEK(u16 a)
{
    u8 i = 0;
    u8 b = 0x01;

    for (i = 0; i < a; i++)
    {
        b = b << 1;
    }

    return b;
}

void GET_time_c()
{
    u8 i = 0;
    dat00[0x0082] = BCD_to_X10(Time_New[0]);
    dat00[0x0083] = BCD_to_X10(Time_New[1]);
    dat00[0x0084] = BCD_to_X10(Time_New[2]);
    dat00[0x0086] = BCD_to_X10(Time_New[4]);
    dat00[0x0087] = BCD_to_X10(Time_New[5]);
    dat00[0x0088] = BCD_to_X10(Time_New[6]);
    dat00[0x0085] = GET_WEEK(Time_New[3]);
    for (i = 0; i < 7; i++)
    {
        dat00_low[0x0082 + i] = dat00[0x0082 + i];
    }
}

void SET_time_c()
{
    Time_int[0] = X10_to_BCD(dat00[0x0082]);
    Time_int[1] = X10_to_BCD(dat00[0x0083]);
    Time_int[2] = X10_to_BCD(dat00[0x0084]);
    Time_int[3] = SET_WEEK(dat00[0x0085]);
    Time_int[4] = X10_to_BCD(dat00[0x0086]);
    Time_int[5] = X10_to_BCD(dat00[0x0087]);
    Time_int[6] = X10_to_BCD(dat00[0x0088]);
    // dat00[0x0084] = GET_WEEK(Time_New[3]);
}

bit Judging_Range(u16 a, u16 b, u16 c)
{
    int a1 = (int)a;
    int b1 = (int)b;
    int c1 = (int)c;

    if (a > (b + c) || a < (b - c))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

u16 Abs_Difference_16(u16 a, u16 b)
{
    if (a > b)
    {
        b = a - b;
    }
    else
    {
        b = b - a;
    }
    return b;
}

void Alarm_conversion_16TO32(u8 a)
{
    u32 dat = 0;
    u8 b = 0;
    u8 c = 0;

    dat = (u32)dat00[0x0095];

    dat = dat << 5;
    dat = dat | (u32)dat00[0x0094];

    dat = dat << 5;
    dat = dat | (u32)dat00[0x0093];

    dat = dat << 6;
    dat = dat | (u32)dat00[0x0092];

    dat = dat << 6;
    dat = dat | (u32)dat00[0x0091];

    dat = dat << 1;
    dat = dat | (u32)dat00[0x0096];

    dat = dat << 1;
    dat = dat | (u32)dat00[0x0097];

    dat = dat << 1;
    dat = dat | (u32)dat00[0x0099];

    AlarmClock[a] = dat;

    b = a / 4 + 10;
    c = (a % 4) * 8;
    dat = 0xff;
    dat = dat << c;
    dat = ~dat;
    AlarmClock[b] = AlarmClock[b] & dat;
    dat = ((u32)dat00[0x0098]) << c;
    AlarmClock[b] = AlarmClock[b] | dat;
}

void Alarm_conversion_32TO16(u8 a)
{
    u32 dat = 0;
    u8 b = 0;
    u8 c = 0;

    dat = AlarmClock[a];

    dat00[0x0099] = (dat & 0x00000001);
    dat = dat >> 1;

    dat00[0x0097] = (dat & 0x00000001);
    dat = dat >> 1;

    dat00[0x0096] = (dat & 0x00000001);
    dat = dat >> 1;

    dat00[0x0091] = (dat & 0x0000003F);
    dat = dat >> 6;

    dat00[0x0092] = (dat & 0x0000003F);
    dat = dat >> 6;

    dat00[0x0093] = (dat & 0x0000001F);
    dat = dat >> 5;

    dat00[0x0094] = (dat & 0x0000001F);
    dat = dat >> 5;

    dat00[0x0095] = (dat & 0x0000000F);

    b = a / 4 + 10;
    c = (a % 4) * 8;
    dat = 0xff;
    dat = dat << c;
    dat = AlarmClock[b] & dat;
    dat = dat >> c;
    dat00[0x0098] = dat;
}
