/*
 * @Author: 0clock
 * @Date: 2024-07-15 22:26:00
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-07-15 22:54:13
 * @FilePath: \STC32G_LowPowerClock\firmware\device\oled\oled_ssd13063.c
 * @Description: oled显示屏驱动
 * 
 * Copyright (c) 2024 by 0clock, All Rights Reserved. 
 */
#include "oled_ssd13063.h"

u8 xdata SPI_DmaTxBuffer[256];  //_at_ DMA_TX_ADDR;
u8 xdata SPI_DmaTxBuffer2[256]; //_at_ DMA_TX_ADDR;
u8 xdata SPI_DmaRxBuffer[256];  //_at_ DMA_RX_ADDR

u8 SPI_DMA_USE = 0;

u8 SPI_DMA_buy = 0;


//========================================================================
// 函数: void SPI_init(void)
// 描述: SPI初始化，STC官方例程改的
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void SPI_Init(void)
{
    P_SW2 = 0x80;
    SPCTL |= (1 << 7);  // 忽略 SS 引脚功能，使用 MSTR 确定器件是主机还是从机
    SPCTL |= (1 << 6);  // 使能 SPI 功能
    SPCTL &= ~(1 << 5); // 先发送/接收数据的高位（ MSB）
    SPCTL |= (1 << 4);  // 设置主机模式

    //    SPCTL &= ~(1 << 3); //SCLK 空闲时为低电平，SCLK 的前时钟沿为上升沿，后时钟沿为下降沿
    //    SPCTL &= ~(1 << 2); //数据 SS 管脚为低电平驱动第一位数据并在 SCLK 的后时钟沿改变数据

    //    SPCTL |= (1 << 3); //SCLK 空闲时为低电平，SCLK 的前时钟沿为上升沿，后时钟沿为下降沿
    //    SPCTL |= (1 << 2); //数据 SS 管脚为低电平驱动第一位数据并在 SCLK 的后时钟沿改变数据
    SPCTL |= 0x0c;

    // SPCTL = (SPCTL & ~3) | 0;   //SPI 时钟频率选择, 0: 4T, 1: 8T,  2: 16T,  3: 2T

    SPCTL &= 0xFC;
    SPCTL |= 0x03;

    P_SW1 &= 0xf7; // IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2
    P_SW1 |= 0x04;
    // P_SW1 = (P_SW1 & ~(3<<1)) | (1<<1);  //IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2
    SCK = 1; // set clock to low initial state
    SI = 1;
    SPSTAT = 0x80 + 0x40;
    HSSPI_CFG2 |= 0x20;
    SPI_DMA_buy = 0;
}
//========================================================================
// 函数: void DMA_Config(void)
// 描述: SPI DMA 功能配置.STC官方例程
// 参数: none.
// 返回: none.
// 版本: V1.0, 2021-5-6
//========================================================================



void SPI_DMA_Config(void)
{
    DMA_SPI_STA = 0x00;
    DMA_SPI_CFG = 0xC0; // bit7 1:Enable Interrupt
    DMA_SPI_AMTH = 0x00;
    DMA_SPI_AMT = 0xff; // 设置传输总字节数：n+1

    DMA_SPI_TXAH = (u8)((u16)&SPI_DmaTxBuffer >> 8); // SPI发送数据存储地址
    DMA_SPI_TXAL = (u8)((u16)&SPI_DmaTxBuffer);
    DMA_SPI_RXAH = (u8)((u16)&SPI_DmaRxBuffer >> 8); // SPI接收数据存储地址
    DMA_SPI_RXAL = (u8)((u16)&SPI_DmaRxBuffer);

    // DMA_SPI_CFG2 = 0x01;	//01:P2.2
    DMA_SPI_CR = 0x81; // bit7 1:使能 SPI_DMA, bit6 1:开始 SPI_DMA 主机模式, bit0 1:清除 SPI_DMA FIFO
}

//========================================================================
// 函数: void SPI_DMA()
// 描述: SPI 发送
// 参数:
// 返回:
// 版本: V1.0, 2021-5-6
//========================================================================
void SPI_DMA()
{
    if (DMA_SPI_STA) // DMA 正忙就跳过
    {
        DMA_SPI_STA = 0;
        SPSTAT = 0x80 + 0x40;
        SPI_DMA_buy = 0;
    }
}
//========================================================================
// 函数: void SPI_DMA_delay()
// 描述: 等待SPI DMA发送完成，并进入IDLE模式
// 参数:
// 返回:
// 版本: V1.0, 2021-5-6
//========================================================================
void SPI_DMA_delay()
{

    while (SPI_DMA_buy) // 等DMA发完
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        PCON = IDL; // MCU进入IDLE模式
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}
//========================================================================
// 函数: void SPI_DMA_TX(u8 num)
// 描述: SPI DMA 发送数据，同时切换空闲区域标志位
// 参数: num ： 发送num+1个字节
// 返回:
// 版本: V1.0, 2021-5-6
//========================================================================
void SPI_DMA_TX(u8 num)
{

    SPI_DMA_delay();
    SPI_DMA_buy = 1;
    DMA_SPI_AMT = num;
    if (SPI_DMA_USE)
    {
        SPI_DMA_USE = 0;
    }
    else
    {
        SPI_DMA_USE = 1;
    }
    DMA_SPI_CR |= 0x40;
}

//========================================================================
// 函数: void SPI_DMA_switch()
// 描述: 根据换空闲区域标志位切换装填的DMA地址
// 参数:
// 返回:
// 版本: V1.0, 2023-12-15
//========================================================================
void SPI_DMA_switch()
{

    if (SPI_DMA_USE)
    {
        DMA_SPI_TXAH = (u8)((u16)&SPI_DmaTxBuffer2 >> 8); // SPI发送数据存储地址
        DMA_SPI_TXAL = (u8)((u16)&SPI_DmaTxBuffer2);
    }
    else
    {
        DMA_SPI_TXAH = (u8)((u16)&SPI_DmaTxBuffer >> 8); // SPI发送数据存储地址
        DMA_SPI_TXAL = (u8)((u16)&SPI_DmaTxBuffer);
    }
}

//========================================================================
// 函数: void LCD_Writ_Bus(u8 out)
// 描述: SPI写一个字节
// 参数: dat 需要发的数据
// 返回:
// 版本: V1.0, 2021-12-15
//========================================================================
void LCD_Writ_Bus(u8 out)
{
    u16 i = 500;
    SPI_DMA_delay();
    SPDAT = out;
    while (((SPSTAT & 0xC0) == 0))
        ; // 清0 SPIF和WCOL标志
    SPSTAT = 0xC0;
}
//========================================================================
// 函数: void OLED_WrDat(u8 dat)
// 描述: SPI写一个字节
// 参数: dat 需要发的数据
// 返回:
// 版本: V1.0, 2021-12-15
//========================================================================
void OLED_WrDat(u8 dat)
{
    SPI_DMA_delay();
    LCD_Writ_Bus(dat);
}

//========================================================================
// 函数: void OLED_WrDat(u8 dat)
// 描述: SPI写一个命令字节
// 参数: dat 需要发的数据
// 返回:
// 版本: V1.0, 2021-12-15
//========================================================================
void OLED_WrCmd(u8 dat1)
{
    SPI_DMA_delay();
    DC = 0; // 写命令
    LCD_Writ_Bus(dat1);
    DC = 1;
}

//========================================================================
// 函数: void OLED_Fill(u8 bmp_dat)
// 描述: OLED整屏填充
// 参数: bmp_dat 需填充的数据
// 返回:
// 版本: V1.0, 2021-12-15
//========================================================================
void OLED_Fill(u8 bmp_dat)
{
    u8 y, x;
    for (y = 0; y < 8; y++)
    {
        OLED_WrCmd((u8)(0xb0 + y));
        OLED_WrCmd(0x00);
        OLED_WrCmd(0x10);
        for (x = 0; x < X_WIDTH; x++)
            OLED_WrDat(bmp_dat);
    }
}

//========================================================================
// 函数: void OLED_Set_Pos(u8 x, u8 y)
// 描述: 设置OLED填充数据位置,中景例程
// 参数: x:X坐标  y:y坐标
// 返回:
// 版本: V1.0, 2021-12-15
//========================================================================
void OLED_Set_Pos(u8 x, u8 y)
{
    OLED_WrCmd((u8)(0xb0 + y));
    OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0f));
}



//========================================================================
// 函数: void OLED_Init(void)
// 描述: OLED初始化程序
// 参数:
// 返回:
// 版本: V1.0, 2024-02-29
//========================================================================
void OLED_Init(void)
{
    RST = 1;
    EA = 0;
    // delay_ms(50);
    Sleep_delay_Time(100);
    RST = 0;
    // delay_ms(50);
    Sleep_delay_Time(100);
    RST = 1;
    // delay_ms(50);
    Sleep_delay_Time(80);
    EA = 1;
    OLED_WrCmd(0xae);       //--turn off oled panel	关闭oled面板
    OLED_WrCmd(0x00);       //---set low column address	 设置低列地址
    OLED_WrCmd(0x10);       //---set high column address	  设置高列地址
    OLED_WrCmd(0x40);       //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)	  设置起始行地址设置映射RAM显示起始行（0x00?0x3F）
    OLED_WrCmd(0x81);       //--set contrast controlregister	 设置对比度控制寄存器
    OLED_WrCmd(Brightness); // Set SEG Output Current Brightness	设置SEG输出电流亮度
    OLED_WrCmd(0xa0);       //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WrCmd(0xc0);       // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WrCmd(0xa6);       //--set normal display			设置正常显示
    OLED_WrCmd(0xa8);       //--set multiplex ratio(1 to 64)g	  设置复用比（1到64）g
    OLED_WrCmd(0x3f);       //--1/64 duty
    OLED_WrCmd(0xd3);       //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)	设置显示偏移量移位映射RAM计数器（0x00?0x3F）
    OLED_WrCmd(0x00);       //-not offset		 不抵消
    OLED_WrCmd(0xd5);       //--set display clock divide ratio/oscillator frequency	设置显示时钟分频比/振荡器频率
    OLED_WrCmd(0x80);       //--set divide ratio, Set Clock as 100 Frames/Sec	  设置分频比，将时钟设置为100帧/秒   TV模式使用0x10或0xF1 其他用0x80 指南针0x50
//    dat00[0x0007] = 0;
    OLED_WrCmd(0xd9); //--set pre-charge period		   设定预充电时间
    OLED_WrCmd(0xf1); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock	将预充电设置为15个时钟并将放电设置为1个时钟
    OLED_WrCmd(0xda); //--set com pins hardware configuration	设置com引脚的硬件配置
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb); //--set vcomh
    OLED_WrCmd(0x40); // Set VCOM Deselect Level
    OLED_WrCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)	设置页面寻址模式
    OLED_WrCmd(0x02); //
    OLED_WrCmd(0x8d); //--set Charge Pump enable   设置电荷泵启用/禁用
    OLED_WrCmd(0x10); //--set(0x10)启用 (0x14)禁用
    OLED_WrCmd(0xa4); // Disable Entire Display On (0xa4/0xa5)禁用整个显示打开
    OLED_WrCmd(0xa6); // Disable Inverse Display On (0xa6/a7)禁用反向显示打开********
    OLED_WrCmd(0xaf); //--turn on oled panel
//    OLED_InverseDisplay2();
    OLED_Fill(0x00); // 初始清屏
    OLED_Set_Pos(0, 0);
}
