#include "STC32G.h"
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

extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

/*************  本地变量声明  功能寄存器  **************/
extern u8 xdata DMABuffer[96];

/*************  中断任务 标志位 专用变量（妈的别优化了）  **************/
volatile u32 Interrupt_flag = 0;

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];

extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u8 xdata Time_New[7];
extern u8 xdata Time_Low[7];
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

void Buzzer_task()
{
    if (dat00[2] & 0x0001)
    {
        if (dat00[0xC3] == 1)
        {
            if (dat00[0xC1] == 1)
            {
                Buzzer_Open();
                SleepQueue_aheadW1(dat00[0xC2], 0x0020, 0);
            }
        }
        dat00[2] &= ~0x0001;
    }
}

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

    EAXFR = 1;
    CKCON = 0x00;
    WTST = 0x00; // 设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    P54RST = 1;

    //  IRC主时钟30M
    CLKSEL |= 0x80;  // 选择PLL输出144M
    USBCLK &= ~0x60; // PLL输入时钟1分频
    USBCLK |= 0x20;  // PLL输入时钟2分频
    USBCLK |= 0x80;  // 使能PLL倍频
    delay_ms(1);     // 等待PLL锁频
    // 选择HSPWM/HSSPI时钟
    CLKSEL |= 0x40; // HSPWM/HSSPI选择PLL输出时钟为时钟源
    HSCLKDIV = 6;   // HSPWM/HSSPI时钟源不分频

    //		P0 = 0xff;  P1 = 0xff;
    //		P2 = 0xff;  P3 = 0xff;
    //		P4 = 0xfF;  P5 = 0xff;
    //		P6 = 0xff;  P7 = 0xff;
    //
    //		P0M0 = 0xff;     P0M1 = 0xff;
    //		P1M0 = 0xff;     P1M1 = 0xff;
    //		P2M0 = 0xff;     P2M1 = 0xff;
    //		P3M0 = 0xff;     P3M1 = 0xff;
    //		P4M0 = 0xff;     P4M1 = 0xff;
    //		P5M0 = 0xef; 		 P5M1 = 0xef;
    //		P6M0 = 0xff;     P6M1 = 0xff;
    //		P7M0 = 0xff;     P7M1 = 0xff;

    //	  P0IE=0x00;  P1IE=0x00;
    //	  P2IE=0x00;  P3IE=0x10;//0x10
    //  	P4IE=0x00;  P5IE=0x10;
    //	  P6IE=0x00;  P7IE=0x00;

    //  SleepMode_ON();

    _nop_();
    _nop_();
    _nop_();
    _nop_();
    //	  _nop_();

    IO_Init();
    OLED_POW(0);
    RESET54();

    ADC_init();
    Clock_INT();
    key_int();
    IIC_int();

    Array_Initialization_();

    IIC_W2bit(0x64, 0x21, 0x80); // 关闭芯片K2开关
    Delay10us();
    IIC_W2bit(0x64, 0x0F, 0x71); // 配置开启1S定时 0x60
    Delay10us();
    //	  IIC_W2bit(0x64,0x0D,0x02);//关闭定时器
    //	  IIC_W2bit(0x64,0x0E,0x00);//清除警告

    BH1745_Init();
    delay_ms(1);
    SHT41_Init();
    delay_ms(1);
    SPL06_Init();
    delay_ms(1);
    delay_ms(1);
    SPI_init();
    SPI_DMA_Config();

    Magnetometer_initi();

    //	    delay_ms(1);

    //    IIC_W2bit(0x22,0x4b,0x01);
    //    delay_ms(1);
    //    IIC_W2bit(0x22,0x4C,0x00);
    //    delay_ms(1);
    //    IIC_W2bit(0x22,0x4b,0x00);

    i = 1;
    delay_ms(1);
    IIC_W2bit(0x32, 0x11, 0x5E);
    delay_ms(1);
    i = IIC_W2bit(0x32, 0x11, 0x9E);

    //		Magnetometer_initi();

    //		b = IIC_ResponseText();

    //    delay_ms(1);
    //    IIC_W2bit(0x32,0x11,0x5E);//低功耗模式
    //    delay_ms(1);
    //    IIC_W2bit(0x32,0x11,0x9E);//从低功耗切换到挂起模式
    //    delay_ms(1);

    P2PU = 0x01; // 打开OLED  RSET上拉电阻
    OLED_Init();
    OLED_Fill(0x00);

    P2PU = 0x01; // 打开OLED  RSET上拉电阻

    //     TVMode_int();
    //    OLED_32X32(0,0,0,0,32);

    //    while(1);

    //		     OLED_ED();
    //    		 OLED_POW_LOW();
    //    			P1PU = 0x00;
    //          P1DR = 0x00;
    //    			P1SR = 0x00;
    //    			P2PU = 0x00;
    //          P2DR = 0x00;
    //    			P2SR = 0x00;

    //          P0IE = 0;
    //    			P1IE = 0;
    //    			P2IE = 0x00;
    //    			P3IE = 0;
    //    			P5IE = 0x10;
    //    			P4IE = 0;
    //          P6IE = 0;
    //          P7IE = 0;

    //    		  P1 = 0xf8;
    //    		  P2 = 0xff;
    //    		  P3 = 0xff;
    //    		  P4 = 0xfF;
    //    		  P5 = 0xf3;
    //    		  P6 = 0xff;
    //    		  P7 = 0xff;

    //    			I2CCFG &=0x7f;
    //    			SPCTL &=0xBf;

    //    			P0M0 = 0xff;                                //设置P3.0~P3.7为开漏模式
    //          P0M1 = 0xff;

    //    			P1M0 = 0x07;                                //设置P3.0~P3.7为开漏模式
    //          P1M1 = 0x07;

    //    			P2M0 = 0x3f;                                //设置P3.0~P3.7为开漏模式
    //          P2M1 = 0x3f;

    //    			P3M0 = 0xff;                                //设置P3.0~P3.7为开漏模式
    //          P3M1 = 0xff;

    //    			P4M0 = 0xff;                                //设置P3.0~P3.7为开漏模式
    //          P4M1 = 0xff;

    //    			P6M0 = 0xff;                                //设置P3.0~P3.7为开漏模式
    //          P6M1 = 0xff;

    //    			P7M0 = 0xff;                                //设置P3.0~P3.7为开漏模式
    //          P7M1 = 0xff;

    //    		  ADC_POWER = 0;

    //    P0INTE = 0x00;//允许端口中断
    //    P0WKUE = 0x00;//允许掉电唤醒
    //
    //    P3INTE = 0x10;//允许端口中断
    //	  P3WKUE = 0x10;//允许掉电唤醒
    //
    //	  P5WKUE = 0x00;//允许掉电唤醒
    //
    //	  P4INTE = 0x00;//允许端口中断
    //	  P4WKUE = 0x00;//允许掉电唤醒
    //	  P2INTE = 0x00;//允许端口中断
    //	  P2WKUE = 0x00;//允许掉电唤醒

    //	  _nop_();
    //		_nop_();
    //		_nop_();
    //		_nop_();
    //		PD = 1;                            //MCU进入掉电模式
    //		_nop_();
    //		_nop_();
    //		_nop_();
    //		_nop_();

    INS5699_timeW(Time_int);
    delay_ms(1);
    INS5699_timeR();
    //		INS5699_timer_clean();

    dat00[0] |= 0x0002;
    dat00[0x10] = 0x0020;
    dat00_flag[0x10] = 1;
    SleepQueue_aheadW1(5, 1, 0);

    P0INTE |= 0x02; // 允许端口中断
    P0IM1 |= 0xFD;  // 下降沿中断
    P0IM0 &= 0xFD;
    P0WKUE |= 0x02; // 允许掉电唤醒
    EA = 1;

    //    SleepMode_ON();

    //	P23=1;
    //	P25=1;
    // while (1);

    while (1)
    {

        Time_Interruption_1S();
        flag_handle();
        dat00_Change();
        IU_Thread();
        key_thread1_new();

        // Buzzer_task();

        // debug_8x8(b,64,1);

        if (Sleep_Queue_task[0] == 0)
        {
            SleepQueue_MoveLeft(1);
        }

        SPI_DMA_delay();
        Sleep_Thread();

        UI_switch(); // 1号任务
        SHT41_falg_0004();
        SPL06_PR_R_flag0008();
        SPL06_PR_P_flag0010();
        BH1745R_flag0002();
        Buzzer_falg_0020();
        ADC_falg_0040();
        Magnetometer_falg_0080();
    }
}

void INT0_Isr() interrupt 0
{
}

void INT1_Isr() interrupt 2
{
}

void KZ_isr() interrupt 13
{
    u8 intf = 0;

    intf = P0INTF;
    if (intf)
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
        if (intf & 0x20)
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

        // miao(P2INTF);
        P2INTF = 0;
        if (intf & 0x80)
        {

            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            // Delay10us();
            Add_key(0);
            dat00[1] &= 0x0002;
        }
        if (intf & 0x40)
        {
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            // Delay10us();
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

    //    UART1_DMA();

    // nian(gg++,6);
    SPI_DMA();
}
