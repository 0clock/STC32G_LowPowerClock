#include "STC32F.h"
#include "stdio.h"
#include "intrins.h"
#include "time.h"
#include "IIC.h"
#include "UART1.h"
#include "SHT41.h"
#include "SPL06.h"
#include "IO.h"
#include "BH1745.h"
#include "OLED.h"
#include "UI.h"
#include "DATA.h"
#include "logic.h"
#include "SecondaryMenu.h"
#include "ThreeMenu.h"
#include "flag.h"
#include "TV_Mode.h"
#include "INS5699S.h"
#include "ADC.h"
#include "BMC050.h"
#include "Time_Interruption.h"

#include "SleepMode.h"

////REMOVEUNUSED

/****************************** 用户定义宏 ***********************************/

/*****************************************************************************/

/*************  本地常量声明    **************/
// u16 xdata JCQ03_00[32];
u32 xdata JCQ06_00[5];

//**************JCQ06_00***********************
// 00 温度 ×100 ℃
// 01 湿度 ×100 RH
// 02 SPL06气压 原始值
// 03 SPL06温度 原始值
// 04 气压 ×1   Pa
//*************************************

extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

/*************  本地变量声明  功能寄存器  **************/
extern u8 xdata DMABuffer[96];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];

extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u8 Time_New[7];
extern u8 Time_Low[7];
extern u8 xdata Time_int[7];

/********************* 主函数 *************************/
u32 dat1 = 0;
u32 dat2 = 0;
u32 dat3 = 0;
u32 dat4 = 0;
u32 dat5 = 0;

u32 dat6 = 0;
u32 dat7 = 0;
u8 uu = 0;
u16 sleep_dat = 0;

u16 gg = 0;

// REMOVEUNUSED

void main(void)
{
    u8 i = 0;
    u8 a = 0;
    u8 b = 0;
    u16 c = 0;
    u32 d = 0;
    u8 ii = 0;
    u8 datac = 0;
    u16 kk = 0;

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

    IO_Init();   // IO初始化
    OLED_POW(0); // OLED5V！ ----- 启动！
    RESET54();   // 设置P54为复位脚

    ADC_init();  // ADC初始化
    Clock_INT(); // RTC中断
    key_int();   // 按键初始化
    IIC_int();   // IIC初始化

    //    UART1_config();
    //    DMA_Config();

    Array_Initialization_();  // 寄存器初始化
    INS5699_Initialization(); // 时钟初始化
    BH1745_Init();            // 光强计初始化
    delay_ms(1);
    SHT41_Init(); // 温度湿度计初始化
    delay_ms(1);
    SPL06_Init(); // 气压计初始化
    delay_ms(1);
    SPI_init();           // SPI初始化
    SPI_DMA_Config();     // SPI DMA初始化
    Magnetometer_initi(); // 磁强计初始化

    delay_ms(1);
    IIC_W2bit(0x32, 0x11, 0x5E); // 加速度计初始化
    delay_ms(1);
    IIC_W2bit(0x32, 0x11, 0x9E);
    delay_ms(1);

    // INS5699_timeW(Time_int); // 设定RTC时间
    delay_ms(1);

    OLED_Init();     // OLED初始化
    OLED_Fill(0x00); // 清屏
    INS5699_timeR(); // 读取当前时间
    dat00[0] |= 0x0002;
    dat00[0x10] = 0x0020;
    dat00_flag[0x10] = 1;
    dat00[0x2D] = 0xFFFF;
    SleepQueue_aheadW1(5, 1, 0);

    P0INTE |= 0x02; // 允许端口中断
    P0IM1 |= 0xFD;  // 下降沿中断
    P0IM0 &= 0xFD;
    P0WKUE |= 0x02; // 允许掉电唤醒
    ICacheOn();     // 打开ICACHE功能

    EA = 1;

    while (1)
    {

        Time_Interruption_1S();
        flag_handle();

        IU_Thread();
        OLED_Frames();
        Sleep_task();
        key_thread1_new();

        if (Sleep_Queue_task[0] == 0)
        {
            SleepQueue_MoveLeft(1); // 任务执行完了任务队列左移1
        }

        SPI_DMA_delay(); // 等待SPI DMA 发送完数据

        Sleep_Thread(); // 休眠

        UI_switch();              // 1号任务
        SHT41_falg_0004();        // 3号任务
        SPL06_PR_R_flag0008();    // 4号任务
        SPL06_PR_P_flag0010();    // 5号任务
        BH1745R_flag0002();       // 2号任务
        Buzzer_falg_0020();       // 6号任务
        ADC_falg_0040();          // 7号任务
        Magnetometer_falg_0080(); // 8号任务
        key_falg_0100();          // 9号任务
    }
}

void KZ_isr() interrupt 13
{
    u8 intf = 0;

    intf = P3INTF;
    if (intf) // 振动传感器触发
    {
        P3INTF = 0;
        if (intf & 0x10)
        {
            dat00[0] |= 0x0010;
        }
    }

    intf = P0INTF;
    if (intf) // 检测有无电池
    {
        // dat00[0] |= 0x0001;
        P0INTF = 0;
        if (intf & 0x02)
        {
            dat00_low[0x00E2]++;
            if (dat00_low[0x00E2] >= 2)
            {
                P0INTE &= ~0x02; // 允许端口中断
                P0WKUE &= ~0x02; // 允许掉电唤醒
            }
        }
    }

    intf = P4INTF;
    if (intf)
    {
        // dat00[0] |= 0x0001;
        P4INTF = 0;
        if (intf & 0x20) // 按+
        {
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            // Delay10us();
            Add_key(1);
            dat00[1] &= 0x0001;
        }
        if (intf & 0x40)
        {
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            // Delay10us();
            SET_key(); // 主菜单 按下确认件
            dat00[1] = 0;
        }
    }

    intf = P2INTF;
    if (intf)
    {
        P2INTF = 0;
        if (intf & 0x80) // 按-
        {

            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            Add_key(0);
            dat00[1] &= 0x0002;
        }
        if (intf & 0x40)
        {
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            ESC_key(); // 二级菜单 按下退出
            dat00[1] = 0;
        }
    }
    intf = P5INTF;
    if (intf)
    {
        P5INTF = 0;
        if (intf & 0x01)
        {
            dat00[0] |= 0x0002;
        }
    }
    P2INTF = 0;
    P4INTF = 0;
    P5INTF = 0;
    P0INTF = 0;
    P1INTF = 0;
    P3INTF = 0;
    P6INTF = 0;
    P7INTF = 0;

    SPI_DMA(); // SPI DMA 中断处理
    //  UART1_DMA();
    //  nian(gg++,6);
}
