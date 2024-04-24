#include "OLED.h"
#include "time.h"
#include "dianzhen.h"
#include "SecondaryMenu.h"

extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];

extern u8 UI_dat[8][16];

extern u8 UI_TX[3][64];

extern u8 Time_New[7];

extern u32 AlarmClock[16];

extern u16 Battery_PE_Low;

u8 xdata SPI_DmaTxBuffer[256];  //_at_ DMA_TX_ADDR;
u8 xdata SPI_DmaTxBuffer2[256]; //_at_ DMA_TX_ADDR;
u8 xdata SPI_DmaRxBuffer[256];  //_at_ DMA_RX_ADDR

u8 SPI_DMA_USE = 0;

u8 SPI_DMA_buy = 0;

extern u8 OLED_InverseDisplay;
extern u16 Sleep36;

//========================================================================
// 函数: void SPI_init(void)
// 描述: SPI初始化，STC官方例程改的
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void SPI_init(void)
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
// 函数: void OLED_Frames()
// 描述: OLED刷新率切换程序
// 参数:
// 返回:
// 版本: V1.0, 2024-02-29
//========================================================================
void OLED_Frames()
{
    if (dat00[0] & 0x0100)
    {
        if ((dat00[0] & 0x0080) && (dat00[0x0036]))
        {
            if ((dat00[0x0007] != 0x0003))
            {
                OLED_WrCmd(0xd5);
                OLED_WrCmd((u8)(((dat00[0x0057] << 4) & 0x00F0) + 1));
                dat00[0x0007] = 3;
            }
        }
        else if ((dat00[0x10] >= 0 && dat00[0x10] < 16 && dat00[0x1A] == 0) && (dat00[0x10] != 0x0005))
        {
            if ((dat00[0x0006] & 0x0002) && (dat00[0x0007] != 0x0000)) // UI动态，不是0x80
            {
                OLED_WrCmd(0xd5);
                OLED_WrCmd((u8)((dat00[0x0054] << 4) & 0x00F0));
                dat00[0x0007] = 0;
            }
            else if (((dat00[0x0006] & 0x0002) == 0) && (dat00[0x0007] != 0x0001)) // UI静态，不是0x50
            {
                OLED_WrCmd(0xd5);
                OLED_WrCmd((u8)((dat00[0x0055] << 4) & 0x00F0));
                dat00[0x0007] = 1;
            }
        }
        else if ((dat00[0x0010] >= 0x0020) && (dat00[0x0010] <= 0x0022))
        {
            if ((dat00[0x0006] & 0x0002) && (dat00[0x0007] != 0x0002) && (dat00[0x0010] == 0x0020)) // UI动态，不是0x80
            {
                OLED_WrCmd(0xd5);
                if (OLED_InverseDisplay && dat00[0x0021])
                {
                    OLED_WrCmd((u8)((dat00[0x0055] << 4) & 0x00F0));
                }
                else
                {
                    OLED_WrCmd((u8)((dat00[0x0056] << 4) & 0x00F0));
                }

                dat00[0x0007] = 2;
            }
            else if (((dat00[0x0006] & 0x0002) == 0) && (dat00[0x0007] != 0x0003)) // UI静态，不是0xF1
            {
                OLED_WrCmd(0xd5);
                if (OLED_InverseDisplay && dat00[0x0021])
                {
                    OLED_WrCmd((u8)((dat00[0x0055] << 4) & 0x00F0));
                }
                else
                {
                    OLED_WrCmd((u8)(((dat00[0x0057] << 4) & 0x00F0) + 1));
                }

                dat00[0x0007] = 3;
            }
        }
        else if ((dat00[0x10] == 0x0005) && (dat00[0x0007] != 0x0001))
        {
            OLED_WrCmd(0xd5);
            OLED_WrCmd((u8)((dat00[0x0055] << 4) & 0x00F0));
            dat00[0x0007] = 1;
        }

        dat00[0] &= ~0x0100;
        // debug_8x8(dat00[0x0007],80,7);
    }
    // debug_8x8(dat00[0x0007],80,7);
}

//========================================================================
// 函数: OLED_InverseDisplay2()
// 描述: 当烧屏保护打开，每天00:00~06:00反色 垃圾功能，弟中弟
// 参数:
// 返回: 无（OLED_InverseDisplay  1反色  0正常）
// 版本: V1.0 2024.02.08
//========================================================================
void OLED_InverseDisplay2()
{
    if (dat00[0x26])
    {
        if (((Time_New[2] >= 0x00) && (Time_New[2] < 0x06)) && (dat00[0x0021] > 0) && (OLED_InverseDisplay == 0))
        {
            OLED_WrCmd(0xa7);
            OLED_InverseDisplay = 1;
        }
        else if ((Time_New[2] >= 0x06) && (dat00[0x0021] > 0) && (OLED_InverseDisplay))
        {
            OLED_WrCmd(0xa6);
            OLED_InverseDisplay = 0;
        }
    }
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
    dat00[0x0007] = 0;
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
    OLED_InverseDisplay2();
    OLED_Fill(0x00); // 初始清屏
    OLED_Set_Pos(0, 0);
}

//========================================================================
// 函数: void ZF_8X8(u8 x,u8 y,u8 ee)
// 描述: 显示8*8大小的字符，数据来源：F8x8
// 参数:  x：x坐标 y:y坐标 ee:第几个字符
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void ZF_8X8(u8 x, u8 y, u8 ee) //
{
    u8 i;
    u8 *SPI_dat;
    if (SPI_DMA_USE)
    {
        SPI_dat = SPI_DmaTxBuffer2;
    }
    else
    {
        SPI_dat = SPI_DmaTxBuffer;
    }
    for (i = 0; i < 8; i++)
    {
        SPI_dat[i] = F8x8[ee][i];
    }
    OLED_Set_Pos(x, y);
    SPI_DMA_switch();
    SPI_DMA_TX(7);
}

//========================================================================
// 函数: ZF_8X16(u8 x,u8 y,u8 ee)
// 描述: 显示8*16大小的字符，数据来源：F8x16
// 参数:  x：x坐标 y:y坐标 ee:第几个字符
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void ZF_8X16(u8 x, u8 y, u8 ee) //
{
    u8 i;
    u8 k = 0;
    u8 *SPI_dat;
    if (SPI_DMA_USE)
    {
        SPI_dat = SPI_DmaTxBuffer2;
    }
    else
    {
        SPI_dat = SPI_DmaTxBuffer;
    }
    for (i = 0; i < 8; i++)
    {
        SPI_dat[i] = F8x16[ee][i];
    }
    OLED_Set_Pos(x, y);
    SPI_DMA_switch();
    SPI_DMA_TX(7);

    if (SPI_DMA_USE)
    {
        SPI_dat = SPI_DmaTxBuffer2;
    }
    else
    {
        SPI_dat = SPI_DmaTxBuffer;
    }
    for (; i < 16; i++)
    {
        SPI_dat[k] = F8x16[ee][i];
        k++;
    }
    OLED_Set_Pos(x, (u8)(y + 1));
    SPI_DMA_switch();
    SPI_DMA_TX(7);
}
//========================================================================
// 函数: void debug_8x8(int k,u8 x,u8 y)
// 描述: 用来在屏幕上打印数据，
// 参数:  x：x坐标 y:y坐标 k:需要打印的数据（只能显示4位）
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void debug_8x8(int k, u8 x, u8 y) //
{
    u8 i;
    u8 x1 = x;
    OLED_Set_Pos(x, y);
    for (i = 0; i < 5; i++)
    {
        ZF_8X8(x1, y, 10);
        x1 = x1 + 8;
    }
    OLED_Set_Pos(x, y);
    if (k < 0)
    {

        ZF_8X8(x, y, 24);
        k = k * (-1);
    }
    else
    {
        ZF_8X8(x, y, 10);
    }
    x = x + 8;

    ZF_8X8(x, y, (u8)(k / 1000));
    x = x + 8;

    ZF_8X8(x, y, (u8)((k % 1000) / 100));
    x = x + 8;

    ZF_8X8(x, y, (u8)((k % 100) / 10));
    x = x + 8;
    ZF_8X8(x, y, (u8)(k % 10));
}

//========================================================================
// 函数: void nian(u16 ee,u8 y)
// 描述: 显示8*16大小字符的年份
// 参数:  y:y坐标 ee:年份
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void nian(u16 ee, u8 y)
{
    ZF_8X16(0, y, (u8)(ee / 1000));
    ZF_8X16(8, y, (u8)((ee % 1000) / 100));
    ZF_8X16(16, y, (u8)((ee % 100) / 10));
    ZF_8X16(24, y, (u8)(ee % 10));
    ZF_8X16(32, y, 12); // 末尾带斜杠
}

//========================================================================
// 函数: void yue_ri(u8 x,u8 ee,bit g)
// 描述: 显示8*16大小字符的月份或者日
// 参数: x:x坐标 y:y坐标 ee:月/日 g:末尾是否带斜杠
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void yue_ri(u8 x, u8 ee, bit g)
{
    ZF_8X16(x, 0, (u8)((ee % 100) / 10));
    ZF_8X16((u8)(x + 8), 0, (u8)(ee % 10));
    if (g)
        ZF_8X16((u8)(x + 16), 0, 12);
}

//========================================================================
// 函数: shifen(unsigned char t,unsigned char y)
// 描述: 显示16*32大小字符的时/分/秒
// 参数: t:需要显示的数字/字符 y:x坐标
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void shifen(unsigned char t, unsigned char y) //
{
    unsigned char i;
    unsigned char j;
    unsigned char k;
    for (k = y; k < y + 16; k = k + 16)
    {
        for (j = 4; j < 8; j++)
        {
            OLED_Set_Pos(k, (u8)(j - 1));
            for (i = 0; i < 16; i++)
            {
                OLED_WrDat(F8x16d[t * 4 + j - 4][i]);
            }
        }
    }
}

//========================================================================
// 函数: shifen_2(unsigned char t,unsigned char y)
// 描述: 显示16*32大小字符的时/分/秒（空心）
// 参数: t:需要显示的数字/字符 y:x坐标
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void shifen_2(unsigned char t, unsigned char y)
{
    unsigned char i;
    unsigned char j;
    unsigned char k;
    for (k = y; k < y + 16; k = k + 16)
    {
        for (j = 4; j < 8; j++)
        {
            OLED_Set_Pos(k, (u8)(j - 2));
            for (i = 0; i < 16; i++)
            {
                OLED_WrDat(F16x32_hollow[t * 4 + j - 4][i]);
            }
        }
    }
}

//========================================================================
// 函数: OLED_16X16(u8 x,u8 y,u8 adde)
// 描述: 显示16*16大小字符的汉字或者其他字符显示，用于主菜单显示汉字
// 参数: x:x坐标 y:y坐标 adde:第几个字符
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_16X16(u8 x, u8 y, u8 adde)
{
    unsigned char i = 0;
    unsigned char j = 0;
    u8 *SPI_dat;
    adde = adde << 1;
    for (j = 0; j < 2; j++)
    {
        if (SPI_DMA_USE)
        {
            SPI_dat = SPI_DmaTxBuffer2;
        }
        else
        {
            SPI_dat = SPI_DmaTxBuffer;
        }
        for (i = 0; i < 16; i++)
        {
            SPI_dat[i] = F16x16cd[adde][i];
        }
        OLED_Set_Pos(x, y);
        SPI_DMA_switch();
        SPI_DMA_TX(15);
        adde++;
        y++;
    }
}

//========================================================================
// 函数: void OLED_8X8X3_MOV(u8 x,u8 y,u16 adde,u8 devia1,u8 devia2)
// 描述: 指南针标尺所用，显示3位角度在标尺上
// 参数: 忘了啥意思了，  devia1：从第列开始画  devia2：画到第几个
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_8X8X3_MOV(u8 x, u8 y, u16 adde, u8 devia1, u8 devia2)
{
    unsigned char i = 0;
    unsigned char j = 0;
    if (devia1 > 24)
        devia1 = 0;
    if (devia2 > 24)
        devia2 = 24;
    if (x >= 244)
    {
        devia1 = 256 - x;
        x = 0;
    }
    if (x < 128 && devia1 < 24 && devia2 <= 24)
    {
        for (i = 0; i < 24; i++)
        {
            UI_TX[0][i] = 0x00;
        }

        if (adde < 10)
        {
            for (i = 0; i < 8; i++)
            {
                UI_TX[0][8 + i] = F8x8[(adde % 10)][i];
            }
        }
        else if (adde < 100)
        {
            for (i = 0; i < 8; i++)
            {
                UI_TX[0][4 + i] = F8x8[((adde % 100) / 10)][i];
            }
            for (i = 0; i < 8; i++)
            {
                UI_TX[0][12 + i] = F8x8[(adde % 10)][i];
            }
        }
        else if (adde < 1000)
        {
            for (i = 0; i < 8; i++)
            {
                UI_TX[0][i] = F8x8[(adde / 100)][i];
            }
            for (i = 0; i < 8; i++)
            {
                UI_TX[0][8 + i] = F8x8[((adde % 100) / 10)][i];
            }
            for (i = 0; i < 8; i++)
            {
                UI_TX[0][16 + i] = F8x8[(adde % 10)][i];
            }
        }
        OLED_Set_Pos(x, y);
        for (i = devia1; i < devia2; i++)
        {
            OLED_WrDat(UI_TX[0][i]);
        }
    }
}

//========================================================================
// 函数: void OLED_16X16_MOV(u8 x,u8 y,u8 adde,u8 devia1,u8 devia2)
// 描述: 指南针标尺所用，显示东南西北汉字在标尺上
// 参数: 也忘了啥意思了，和8X8X3_MOV一样 devia1：从第列开始画  devia2：画到第几个
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_16X16_MOV(u8 x, u8 y, u8 adde, u8 devia1, u8 devia2)
{
    unsigned char i = 0;
    unsigned char j = 0;

    if (devia1 > 16)
        devia1 = 0;
    if (devia2 > 16)
        devia2 = 16;
    if (x >= 248)
    {
        devia1 = 256 - x;
        x = 0;
    }

    if (x < 128 && devia1 < 16 && devia2 <= 16)
    {
        adde = adde << 1;
        for (j = 0; j < 2; j++)
        {
            OLED_Set_Pos(x, y);
            for (i = devia1; i < devia2; i++)
            {
                OLED_WrDat(F16x16_position[adde][i]);
            }
            adde++;
            y++;
        }
    }
}

//========================================================================
// 函数: void OLED_32X32(u8 x,u8 y,u8 addr,u8 devia1,u8 devia2)
// 描述: 主菜单图标显示
// 参数: x：x坐标  y：y坐标  addr：图标指向 devia1：从第列开始画  devia2：画到第几个
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_32X32(u8 x, u8 y, u8 addr, u8 devia1, u8 devia2)
{
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;
    u8 *SPI_dat;
    if (x < 128 && devia1 < 32 && devia2 <= 32)
    {
        addr = addr << 2;
        for (j = 0; j < 4; j++)
        {

            k = 0;
            if (SPI_DMA_USE)
            {
                SPI_dat = SPI_DmaTxBuffer2;
            }
            else
            {
                SPI_dat = SPI_DmaTxBuffer;
            }
            for (i = devia1; i < devia2; i++)
            {
                SPI_dat[k] = F32x32[addr][i];
                k++;
            }
            OLED_Set_Pos(x, y);
            SPI_DMA_switch();
            SPI_DMA_TX((u8)(k - 1));
            addr++;
            y++;
        }
    }
}

//========================================================================
// 函数: void OLED_ED()
// 描述: 关闭OLED显示
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_ED()
{
    OLED_WrCmd(0xae); //--turn off oled panel	关闭oled面板
}

//========================================================================
// 函数: void OLED_GD()
// 描述: 主菜单滚动，好像没用上
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_GD()
{
    unsigned char i;
    unsigned char j = 0;
    unsigned char k = 48;
    unsigned char y = 0;
    unsigned char t = 0;

    for (j = 0; j < 4; j++)
    {
        OLED_Set_Pos(k, j);
        for (i = 0; i < 32; i++)
        {
            OLED_WrDat(F32x32[j][i]);
        }
    }
    k = 96;
    for (j = 0; j < 4; j++)
    {
        OLED_Set_Pos(k, j);
        for (i = 0; i < 32; i++)
        {
            OLED_WrDat(F32x32[j + 4][i]);
        }
    }
    k = 0;
    for (j = 0; j < 4; j++)
    {
        OLED_Set_Pos(k, j);
        for (i = 0; i < 32; i++)
        {
            OLED_WrDat(F32x32[j + 36][i]);
        }
    }
}

//========================================================================
// 函数: void OLED_3Figures(u16 num,u8 x,u8 y)
// 描述: 指南针显示角度数值，自动居中（好像有点问题，你们自己修一下笑死）
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================

void OLED_3Figures(u16 num, u8 x, u8 y)
{
    u8 a = 0;
    u8 b = 0;
    u8 c = 0;
    u8 i = 0;
    u8 p = 0;

    a = (u8)((num % 1000) / 100);
    b = (u8)((num % 100) / 10);
    c = (u8)(num % 10);
    p = numlongGet(num);

    if (p <= 2)
    {
        ZF_8X16(x, y, 10);
        ZF_8X16((u8)(x + 16), y, 10);
    }

    if (p == 3)
    {
        ZF_8X16(x, y, a);
    }
    if (p >= 2)
    {
        x = x + 8;
        if (p == 2)
            x = x + 4;
        ZF_8X16(x, y, b);
    }
    if (p >= 1)
    {
        x = x + 8;
        if (p == 1)
            x = x + 12;
        ZF_8X16(x, y, c);
    }
}

//========================================================================
// 函数: void OLED_Progress_Bar(u16 num,u16 jx)
// 描述: 三级菜单进度条
// 参数: num：当前值  jx：最大值
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_Progress_Bar(u16 num, u16 jx)
{
    u8 i;
    u8 j;
    u8 k;
    if (num <= jx)
    {
        j = ((u16)num * 60) / jx;
        OLED_Set_Pos(18, 5);
        OLED_WrDat(0xff);

        for (i = 0; i < j; i++)
        {
            OLED_WrDat(0xff);
        }
        k = 60 - j;
        for (i = 0; i < k; i++)
        {
            OLED_WrDat(0x81);
        }

        OLED_WrDat(0xff);
        k = (u8)((num % 1000) / 100);
        if (k != 0)
        {
            ZF_8X8(82, 5, k);
        }
        else
        {
            ZF_8X8(82, 5, 0x0A);
        }

        j = (u8)((num % 100) / 10);
        if (k == 0 && j == 0)
        {
            ZF_8X8(90, 5, 0x0A);
        }
        else
        {
            ZF_8X8(90, 5, j);
        }
        ZF_8X8(98, 5, (u8)(num % 10));
    }
}

//========================================================================
// 函数:void OLED_Region_Fill()
// 描述: 绘制一个带边的窗口，用于三级菜单
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_Region_Fill()
{
    u8 k;
    u8 q;
    u8 i;
    for (k = 0; k < 6; k++)
    {
        OLED_Set_Pos(16, (u8)(1 + k));
        for (i = 0; i < 96; i++)
        {
            if (k == 0)
            {
                q = 0x01;
            }
            else if (k == 5)
            {
                q = 0x80;
            }
            else
            {
                q = 0;
            }
            if ((i == 0) | (i == (95)))
            {
                q = 0xff;
            }
            OLED_WrDat(q);
        }
    }
}

//**********************************************************************
//
//		第二菜单功能区
//
//**********************************************************************

//========================================================================
// 函数:void OLED_FZQP(u8 x,u8 y)
// 描述: 清空一个区域（二级菜单滚动结束后可能如果图案定格的话会有残留）
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_FZQP(u8 x, u8 y)
{
    u8 i = 0;
    OLED_Set_Pos(x, y);
    for (i = 0; i < 64; i++)
    {
        OLED_WrDat(0x00);
    }
}

//========================================================================
// 函数: void OLED_16_16V(u8 (*dat)[16],u16 adde,u8 x,u8 y,bit p)
// 描述: (*dat)[16]：点阵  adde：二维数组第几行 x:x坐标 y:y坐标 p ：是否反色，用于做光标
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_16_16V(u8 (*dat)[16], u16 adde, u8 x, u8 y, bit p)
{
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;
    u8 *SPI_dat;
    j = adde;
    adde = adde + 8;

    k = 0;
    if (SPI_DMA_USE)
    {
        SPI_dat = SPI_DmaTxBuffer2;
    }
    else
    {
        SPI_dat = SPI_DmaTxBuffer;
    }
    for (; j < adde; j = j + 2)
    {
        for (i = 0; i < 16; i++)
        {
            if (p)
            {
                SPI_dat[k] = dat[j][i];
            }
            else
            {
                SPI_dat[k] = ~dat[j][i];
            }
            k++;
        }
    }
    OLED_Set_Pos(x, y);
    SPI_DMA_switch();
    SPI_DMA_TX((u8)(k - 1));
}

//========================================================================
// 函数: void OLED_16_16Vabc(u8 (*dat)[16],u16 adde,u8 x,u8 y,u8 c,bit jj,u8 Number)
// 描述: 显示半行（左右）二级菜单
// 参数: (*dat)[16]：点阵  adde：二维数组第几行 x:x坐标 y:y坐标 c:忘了，好像是0就不向上走顶出屏幕，用在第一个选项  jj：忘了 Number：我也忘了为什么一定要写8了
//                                                                                                           你们可以写不同的值试试效果
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_16_16Vabc(u8 (*dat)[16], u16 adde, u8 x, u8 y, u8 c, bit jj, u8 Number)
{
    u8 i = 0;
    u8 j = 0;
    u16 t = 0;

    y = (u8)(y / 8);
    j = adde;
    adde = adde + Number;
    if (jj)
        c = c % 8;
    if (y < 8)
    {
        OLED_Set_Pos(x, y);
        for (; j < adde; j = j + 2)
        {
            for (i = 0; i < 16; i++)
            {
                if (!jj)
                {
                    t = dat[j + 1][i] * 256 + dat[j][i];
                    OLED_WrDat((u8)(t >> c));
                }
                else
                {
                    t = dat[j][i];
                    OLED_WrDat((u8)(t << c));
                }
            }
        }
    }
    y = (u8)(y + 1);
    j = adde - (Number - 1);
    adde = adde + 1;
    if (y < 8)
    {
        OLED_Set_Pos(x, y);
        for (; j < adde; j = j + 2)
        {
            for (i = 0; i < 16; i++)
            {
                if (!jj)
                {
                    t = dat[j][i];
                    OLED_WrDat((u8)(t >> c));
                }
                else
                {
                    t = dat[j][i] * 256 + dat[j - 1][i];
                    OLED_WrDat((u8)(t >> (8 - c)));
                }
            }
        }
    }
    y = (u8)(y + 1);
    j = adde - Number;
    if (y < 8)
    {
        OLED_Set_Pos(x, y);
        for (; j < adde; j = j + 2)
        {
            for (i = 0; i < 16; i++)
            {
                if (!jj)
                {
                }
                else
                {
                    t = dat[j][i];
                    OLED_WrDat((u8)(t >> (8 - c)));
                }
            }
        }
    }

    // u8 i = 0;
    //     u8 j = 0;
    //     u16 t = 0;
    //		u8 k=0;
    //		u8 *SPI_dat;
    //     y = (u8)(y/8);
    //     j = adde;
    //     adde = adde + Number;
    //     if(jj) c = c % 8;
    //     if(y < 8)
    //     {
    //				k=0;
    //				if(SPI_DMA_USE)
    //				{
    //						SPI_dat = SPI_DmaTxBuffer2;
    //				}
    //				else
    //				{
    //						SPI_dat = SPI_DmaTxBuffer;
    //				}
    //         for(; j<adde; j = j + 2)
    //         {
    //             for(i=0; i<16; i++)
    //             {
    //                 if(!jj)
    //                 {
    //                     t = dat[j+1][i]*256+dat[j][i];
    //                     SPI_dat[k]=((u8)(t >> c));
    //                 }
    //                 else
    //                 {
    //                     t = dat[j][i];
    //                     SPI_dat[k]=((u8)(t << c));
    //                 }
    //								k++;
    //             }

    //        }
    //				OLED_Set_Pos(x,y);
    //		    SPI_DMA_switch();
    //		    SPI_DMA_TX((u8)(k-1));
    //
    //
    //
    //    }
    //    y = (u8)(y+1);
    //    j = adde  - (Number - 1);
    //    adde = adde + 1;
    //    if(y < 8)
    //    {
    //				k=0;
    //				if(SPI_DMA_USE)
    //				{
    //						SPI_dat = SPI_DmaTxBuffer2;
    //				}
    //				else
    //				{
    //						SPI_dat = SPI_DmaTxBuffer;
    //				}
    //        for(; j<adde; j = j + 2)
    //        {
    //            for(i=0; i<16; i++)
    //            {
    //                if(!jj)
    //                {
    //                    t = dat[j][i];
    //                    SPI_dat[k]=((u8)(t >> c));
    //                }
    //                else
    //                {
    //                    t = dat[j][i]*256+dat[j-1][i];
    //                    SPI_dat[k]=((u8)(t >> (8 - c)));
    //                }
    //								k++;
    //            }
    //
    //        }
    //				OLED_Set_Pos(x,y);
    //		    SPI_DMA_switch();
    //		    SPI_DMA_TX((u8)(k-1));
    //    }
    //    y = (u8)(y+1);
    //    j = adde  - Number;
    //    if(y < 8)
    //    {
    //				k=0;
    //				if(SPI_DMA_USE)
    //				{
    //						SPI_dat = SPI_DmaTxBuffer2;
    //				}
    //				else
    //				{
    //						SPI_dat = SPI_DmaTxBuffer;
    //				}
    //        for(; j<adde; j = j + 2)
    //        {
    //            for(i=0; i<16; i++)
    //            {
    //                if(!jj)
    //                {
    //											SPI_dat[k]=0x00;
    //                }
    //                else
    //                {
    //                    t = dat[j][i];
    //                    SPI_dat[k]=((u8)(t >> (8 - c)));
    //                }
    //								k++;
    //            }
    //
    //        }
    //				OLED_Set_Pos(x,y);
    //		    SPI_DMA_switch();
    //		    SPI_DMA_TX((u8)(k-1));

    //    }
}

//========================================================================
// 函数: u8** TVR(u8 i)
// 描述: 显示数组映射函数，用于直接用光标直接指示对应的点阵
// 参数:  i：光标的值
// 返回:  **pp 需要显示的点阵
// 版本: V1.0  2023.02.05
//========================================================================
u8 **TVR(u8 i)
{
    u8 **pp;

    switch (i)
    {
    case 0x0000:
        pp = TVMode_16; // 标准模式
        break;
    case 0x0001:
        pp = SleepMode_16; // 标准模式
        break;
    case 0x0002:
        pp = Environment; // 环境参数
        break;
    case 0x0003:
        pp = Brightness_setting; // 亮度设置
        break;
    case 0x0004:
        pp = CompassMenu_16; // 指南针
        break;

    case 0x0006:
        pp = time_set; // 时间设置
        break;

    case 0x000A:
        pp = Sound_setting; // 声音设置
        break;
    case 0x000C:
        pp = Power_Options; // 电源管理
        break;
    case 0x000E:
        pp = System_Information; // 系统信息
        break;

    case 0x0040:
        pp = System_Information_data; // 系统信息数据
        break;

    case 0x0060:
        pp = UI_dat; // 系统信息数据
        break;

    case 0x0080:
        pp = F8x16d;
        break;

    default:
        pp = TVMode_16;
        break;
    }
    return pp;
}

//========================================================================
// 函数: void OLED_16_16V2(u8 (*dat)[16],u16 adde,u8 x,u8 y,bit p)
// 描述: 没用到，好像之前是二级菜单显示的，后来为了能自定义图案就换了个方法实现
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================

void OLED_16_16V2(u8 (*dat)[16], u16 adde, u8 x, u8 y, bit p)
{

    OLED_16_16V(dat, adde, x, y, p);
    adde = adde + 1;
    OLED_16_16V(dat, adde, x, (u8)(y + 1), p);
    OLED_FZQP(0, 5);
}

//========================================================================
// 函数: void OLED_Mean2_fill(u8 p,u8 a)
// 描述: UI_dat数组里面全部填充一个数，平时用来清空
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================

void OLED_Mean2_fill(u8 p, u8 a)
{
    u8 i;
    u8 j;
    if (p <= 8)
    {
        for (j = 0; j < p; j++)
        {
            for (i = 0; i < 16; i++)
            {
                UI_dat[j][i] = a;
            }
        }
    }
}

//========================================================================
// 函数: void OLED_Mean2_fill2(u8 a)
// 描述: UI_dat数组里面全部填充一个开关图形，
// 参数: a：打开还是关闭
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_Mean2_fill2(u8 a)
{
    u8 i;
    u8 j;
    u8(*pa)[16];

    if (a > 0)
    {
        pa = KG_K;
    }
    else
    {
        pa = KG_G;
    }
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 16; i++)
        {
            UI_dat[j + 4][i] = pa[j][i];
        }
    }
    OLED_Mean2_fill(4, 0);
}

//========================================================================
// 函数: void ZQ8x8(u8 a,u8 dat)
// 描述: UI_dat数组里面全部填充一个8*8数字，
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void ZQ8x8(u8 a, u8 dat)
{
    u8 i = 0;
    u8 p = 0;
    u8 b = 0;
    p = dat / 10;
    for (i = 0; i < 8; i++)
    {
        UI_dat[a][i] = F8x8_2[p][i];
    }
    p = dat % 10;
    for (; i < 16; i++)
    {
        UI_dat[a][i] = F8x8_2[p][b];
        b++;
    }
}

//========================================================================
// 函数: u8 numlongGet(u16 a)
// 描述: 获取这个数据是几位的数据
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
u8 numlongGet(u16 a)
{
    if (a >= 10000)
    {
        a = 5;
    }
    else if (a >= 1000)
    {
        a = 4;
    }
    else if (a >= 100)
    {
        a = 3;
    }
    else if (a >= 10)
    {
        a = 2;
    }
    else
    {
        a = 1;
    }
    return a;
}

//========================================================================
// 函数: OLED_Mean2_fill_num(u8 a,u8 b,u8 (*dat)[16])
// 描述: 忘了，总之是填充一个8*16的数字到数组里面，供OLED_Mean2_fill_num2装数据的
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_Mean2_fill_num(u8 a, u8 b, u8 (*dat)[16])
{
    u8 i;
    u8 j;
    u8 c = 0;
    j = (a >> 1) << 1;
    c = (a % 2) << 3;

    for (i = 0; i < 8; i++)
    {
        UI_dat[j][i + c] = dat[b][i];
    }
    j = j + 1;
    for (i = 0; i < 8; i++)
    {
        UI_dat[j][i + c] = dat[b][i + 8];
    }
}

//========================================================================
// 函数: void OLED_Mean2_fill_num2(u16 a)
// 描述: 填充二级菜单字符，根据dat00_max查询到的类型自动填充开关、数字、单位、小数点等
// 参数:
// 返回:
// 版本: V1.0  2023.02.05
//========================================================================
void OLED_Mean2_fill_num2(u16 a)
{
    u8 i = 0;
    u8 k;
    u16 dat = 0;
    u8 xsd = 0;
    u8 fh = 0;
    u8 adde = 7;
    dat = dat00[a];
    xsd = (u8)(dat00_max[a] >> 14);
    fh = (u8)((dat00_max[a] >> 12) & 0x0003);
    k = (u8)((dat00_max[a] >> 8) & 0x000F);
    OLED_Mean2_fill(8, 0);
    if (fh > 0)
    {
        for (i = 0; i < fh; i++)
        {
            OLED_Mean2_fill_num(adde, (u8)(k - i), F8x16_unit);
            adde--;
        }
    }
    k = numlongGet(dat);
    if (xsd)
    {
        if (k <= xsd)
        {
            k = xsd + 2;
        }
        else
        {
            k = k + 1;
        }
    }
    for (i = 0; i < k; i++)
    {
        if (i == xsd && xsd != 0)
        {
            OLED_Mean2_fill_num(adde, 0x0B, F8x16);
            adde--;
        }
        else
        {
            OLED_Mean2_fill_num(adde, (u8)(dat % 10), F8x16);
            if (adde > 0)
                adde--;
            dat = dat / 10;
        }
    }
}

//========================================================================
// 函数: void shifen_dh_z(u8 t,u8 x,u8 yy,u16 y,u8  limit)
// 描述: 显示滚动的时分秒----其中一行
// 参数: t:每个字符的第几行（4行一个完整字符）  x:x坐标   yy:y坐标    y:偏移  limit:最大值，比如时的十位，最大值就是2，超过2跳回0
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void shifen_dh_z(u8 t, u8 x, u8 yy, u16 y, u8 limit) //
{
    //    u8 i;
    //    OLED_Set_Pos(x,yy);
    //    for(i=0; i<16; i++)
    //    {
    //        OLED_WrDat((F8x16d[(t+y/8) %  limit][i] >> (y % 8)) | (F8x16d[(t+1+y/8) %  limit][i] << 8-(y % 8)));
    //    }
    u8 i;
    u8 *SPI_dat;
    if (SPI_DMA_USE)
    {
        SPI_dat = SPI_DmaTxBuffer2;
    }
    else
    {
        SPI_dat = SPI_DmaTxBuffer;
    }
    for (i = 0; i < 16; i++)
    {
        SPI_dat[i] = (F8x16d[(t + y / 8) % limit][i] >> (y % 8)) | (F8x16d[(t + 1 + y / 8) % limit][i] << 8 - (y % 8));
    }
    OLED_Set_Pos(x, yy);
    SPI_DMA_switch();
    SPI_DMA_TX((u8)(i - 1));
}

//========================================================================
// 函数: void shifen_dh(u8 x,u8 yy,u16 y,u8  limit)
// 描述: 显示滚动的时分秒
// 参数: x:x坐标   yy:    y:y坐标 limit:强制显示，用于初始化
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void shifen_dh(u8 x, u8 yy, u16 y, u8 limit)
{
    unsigned char i;
    for (i = 0; i < 4; i++)
    {
        shifen_dh_z(i, x, (u8)(yy + i), y, limit);
    }
}

//========================================================================
// 函数: void IU_Battery_Icon_S_1(u8 x,u8 y)
// 描述: TV模式电池界面显示---大电池下面电量的显示
// 参数: x:x坐标 y:y坐标
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IU_Battery_Icon_S_1(u8 x, u8 y)
{
    u8 j, k;

    k = (u8)(dat00[0x00E5] / 1000);
    if (k != 0)
    {
        ZF_8X8((u8)(x + 24), y, k);
    }
    else
    {
        ZF_8X8((u8)(x + 24), y, 0x0A);
    }

    j = (u8)((dat00[0x00E5] % 1000) / 100);
    if (k == 0 && j == 0)
    {
        ZF_8X8((u8)(x + 32), y, 0x0A);
    }
    else
    {
        ZF_8X8((u8)(x + 32), y, j);
    }
    ZF_8X8((u8)(x + 40), y, (u8)((dat00[0x00E5] % 100) / 10));
}

//========================================================================
// 函数: IU_Battery_Icon_S(u8 x,u8 y,u8 p)
// 描述: 右上角电池UI显示
// 参数: x:x坐标 y:y坐标 p:强制显示，用于初始化
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IU_Battery_Icon_S(u8 x, u8 y, u8 p)
{
    u16 a = 0;
    u8 j = 0;
    u8 i;
    u8 k = 0;

    if ((dat00[0x00E5] != Battery_PE_Low) || p)
    {
        a = dat00[0x00E5] / 83;
        OLED_Set_Pos((u8)(x + 8), y);

        for (i = 0; i < 16; i++)
        {
            if (i == 0)
            {
                OLED_WrDat(0xFf);
            }
            else if (i == 1)
            {
                OLED_WrDat(0x81);
            }
            else if (i <= 12)
            {
                if ((a + 1) >= i)
                {
                    OLED_WrDat(0xBD);
                }
                else
                {
                    OLED_WrDat(0x81);
                }
            }
            else if (i <= 13)
            {
                if (a == 12)
                {
                    OLED_WrDat(0x99);
                }
                else
                {
                    OLED_WrDat(0x81);
                }
            }
            else if (i <= 14)
            {
                OLED_WrDat(0xC3);
            }
            else if (i <= 15)
            {
                OLED_WrDat(0x3C);
            }
        }

        IU_Battery_Icon_S_1(x, y);
        if (dat00[0x00E1])
        {
            ZF_8X8(x, y, 0x0B);
        }
        else
        {
            ZF_8X8(x, y, 0x0A);
        }
        Battery_PE_Low = dat00[0x00E5];
    }
}

//*************************电池界面UI********************

//========================================================================
// 函数: void IU_Battery_cheek()
// 描述: TV模式电池界面显示---画出大电池外壳
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IU_Battery_cheek()
{
    u8 i = 0;
    for (i = 2; i < 6; i++)
    {
        ZF_8X8(120, i, 16);
    }

    for (i = 2; i < 6; i++)
    {
        ZF_8X8(96, i, 16);
    }

    OLED_Set_Pos(104, 1);
    for (i = 0; i < 2; i++)
    {
        OLED_WrDat(0x3c);
    }
    for (i = 0; i < 12; i++)
    {
        OLED_WrDat(0x3f);
    }
    for (i = 0; i < 2; i++)
    {
        OLED_WrDat(0x3c);
    }

    OLED_Set_Pos(104, 6);
    for (i = 0; i < 16; i++)
    {
        OLED_WrDat(0x3c);
    }
    ZF_8X8(96, 1, 18);
    ZF_8X8(120, 1, 19);
    ZF_8X8(96, 6, 21);
    ZF_8X8(120, 6, 20);

    ZF_8X16(80, 5, 25);
    ZF_8X16(88, 5, 26);

    ZF_8X16(8, 5, 11);
    ZF_8X16(28, 5, 27);
}

//========================================================================
// 函数: void IU_16x8_Fill(u8 x,u8 y,u8 t)
// 描述: x:x坐标   y:y坐标  t：需要写入的数据
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IU_16x8_Fill(u8 x, u8 y, u8 t)
{
    u8 i;
    OLED_Set_Pos(x, y);
    for (i = 0; i < 16; i++)
    {
        OLED_WrDat(t);
    }
}

//========================================================================
// 函数: IU_Battery_cheek_renovate_count(u16 a,u8 x,u8 y,u8 t)
// 描述: TV模式电池界面显示---画出大电池里面的一层的电量
// 参数: a：电量  x:x坐标   y:y坐标  t：层数
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IU_Battery_cheek_renovate_count(u16 a, u8 x, u8 y, u8 t)
{
    u8 b, p;
    p = t + 8;
    if (a >= p)
    {
        b = 0xff;
    }
    else if (a > t && a <= p)
    {
        b = 0xff;
        b = b << (p - a);
    }
    else
    {
        b = 0;
    }
    IU_16x8_Fill(x, y, b);
}

//========================================================================
// 函数: IU_Battery_cheek_renovate()
// 描述: TV模式电池界面显示---其他部分的刷新
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void IU_Battery_cheek_renovate()
{
    u16 a = 0;
    u8 b = 0;
    u8 i;
    a = (u8)((u32)dat00[0x00E5] * 100 / 3125);

    // miao((u8)dat00[0x00E2]);

    if (dat00[0x00E2])
    {
        IU_Battery_cheek_renovate_count(a, 104, 2, 24);
        IU_Battery_cheek_renovate_count(a, 104, 3, 16);
        IU_Battery_cheek_renovate_count(a, 104, 4, 8);
        IU_Battery_cheek_renovate_count(a, 104, 5, 0);
    }
    else
    {
        IU_16x8_Fill(104, 2, 0);
        IU_16x8_Fill(104, 5, 0);
        OLED_Set_Pos(104, 3);
        ZF_8X8(104, 3, 22);
        ZF_8X8(112, 3, 23);
        ZF_8X8(104, 4, 23);
        ZF_8X8(112, 4, 22);
    }

    if (dat00[0x00E1])
    {

        if ((u8)(Time_New[0] % 6) == 0)
        {
            ZF_8X16(72, 3, 10);
            ZF_8X16(80, 3, 10);
        }
        ZF_8X16((u8)(24 + (Time_New[0] % 6) * 8), 3, 10);
        ZF_8X16((u8)(32 + (Time_New[0] % 6) * 8), 3, 23);
        ZF_8X16((u8)(40 + (Time_New[0] % 6) * 8), 3, 24);
    }
    else
    {
        if (dat00_low[0x00E1] != dat00[0x00E1])
        {
            OLED_Set_Pos(24, 3);
            for (i = 0; i < 56; i++)
            {
                OLED_WrDat(0x00);
            }
            OLED_Set_Pos(24, 4);
            for (i = 0; i < 56; i++)
            {
                OLED_WrDat(0x00);
            }
        }
    }

    if (dat00[0x00E4] >= 100)
    {
        ZF_8X16(54, 5, (u8)((dat00[0x00E4] % 1000) / 100));
    }
    else
    {
        ZF_8X16(54, 5, 10);
    }

    if (dat00[0x00E4] >= 10)
    {
        ZF_8X16(62, 5, (u8)((dat00[0x00E4] % 100) / 10));
    }
    else
    {
        ZF_8X16(62, 5, 10);
    }
    ZF_8X16(70, 5, (u8)(dat00[0x00E4] % 10));

    ZF_8X16(0, 5, (u8)((dat00[0x00E3] / 1000)));
    ZF_8X16(12, 5, (u8)((dat00[0x00E3] % 1000) / 100));
    ZF_8X16(20, 5, (u8)((dat00[0x00E3] % 100) / 10));

    IU_Battery_Icon_S_1(76, 7);
}

void OLED_16_16V2a()
{
    u8 i = 0;
    delay_ms(200);
    delay_ms(200);
    ;
    delay_ms(200);
    delay_ms(200);
    delay_ms(200);
    delay_ms(200);
    delay_ms(200);
    delay_ms(200);
}
