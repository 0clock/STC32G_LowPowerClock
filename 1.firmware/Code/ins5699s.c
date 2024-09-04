#include "stc32g.h"
#include "ins5699s.h"
#include "iic.h"
#include "dmx_delay.h"
#include "stdio.h"
#include "task.h"
// Function to initialize the INS5699S RTC

void rtc_init()
{
    iic_write2bit(0x64, 0x21, 0x80); // 关闭芯片K2开关
    delay_us(10);
    iic_write2bit(0x64, 0x0F, 0x60); // 配置开启1S时间更新 0x60
    delay_us(10);
    iic_write2bit(0x64, 0x0D, 0x0A); // FSEL 1Hz  定时器1Hz  时间更新：秒更新
    delay_us(10);
    iic_write2bit(0x64, 0x0E, 0x00); // 清除警告
    delay_ms(1);

    ins5699_read_time(&current_time);
}

/**
 * @brief BCD码转换为十进制
 * @param bcd 需要转换的BCD码
 * @return 返回转换后的十进制数
 */
unsigned char bcd_to_dec(unsigned char bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/**
 * @brief 十进制转BCD
 * @param dec 需要转换的十进制数
 * @return 返回转换后的BCD码
 */
unsigned char dec_to_bcd(unsigned char dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}


//========================================================================
// 函数: void ins5699_read_time(time_t *time)
// 描述: 读取时间，并将秒、分、时、星期、日、月、年存储到time_t结构体中
// 参数: time - 指向time_t结构体的指针，用于存储读取的时间数据
// 返回: void
// 版本: V1.0 2023.02.05
//========================================================================
void ins5699_read_time(time_t *time)
{
    u8 i = 0;
    
    iic_start(); // 发送起始命令
    iic_senddata(INS5699_W); // 发送设备地址+写命令
    RecvACK();
    iic_senddata(0x00); // 设置起始寄存器地址为0x00
    RecvACK();
    iic_start(); // 发送重复起始命令
    iic_senddata(INS5699_R); // 发送设备地址+读命令
    RecvACK();

    // 读取7字节的数据并存入time结构体的相应字段
    time->sec   = bcd_to_dec(RecvData()); SendACK(); // 秒
    time->min   = bcd_to_dec(RecvData()); SendACK(); // 分
    time->hour  = bcd_to_dec(RecvData()); SendACK(); // 时
    time->week  = RecvData(); SendACK(); // 星期
    time->day   = bcd_to_dec(RecvData()); SendACK(); // 日
    time->month = bcd_to_dec(RecvData()); SendACK(); // 月
    time->year  = bcd_to_dec(RecvData()); SendNAK(); // 年


    iic_stop(); // 发送停止命令
}

//========================================================================
// 函数: void ins5699_write_time(const time_t *time)
// 描述: 将秒、分、时、星期、日、月、年写入到INS5699芯片
// 参数: time - 指向time_t结构体的指针，其中包含需要写入的时间数据
// 返回: void
// 版本: V1.0 2024.09.04
//========================================================================
void ins5699_write_time(const time_t *time)
{
    // 开始I2C通信
    iic_start();                  // 发送起始命令
    iic_senddata(INS5699_W);      // 发送设备地址+写命令
    RecvACK();
    
    // 发送寄存器地址
    iic_senddata(0x00);           // 设置起始寄存器地址为0x00
    RecvACK();
    
    // 发送7字节的时间数据
    iic_senddata(dec_to_bcd(time->sec));   // 秒
    RecvACK();
    iic_senddata(dec_to_bcd(time->min));   // 分
    RecvACK();
    iic_senddata(dec_to_bcd(time->hour));  // 时
    RecvACK();
    iic_senddata(time->week);  // 星期
    RecvACK();
    iic_senddata(dec_to_bcd(time->day));   // 日
    RecvACK();
    iic_senddata(dec_to_bcd(time->month)); // 月
    RecvACK();
    iic_senddata(dec_to_bcd(time->year));  // 年
    SendNAK();  // 不需要再等待ACK，直接发送NAK
    
    // 结束I2C通信
    iic_stop();                   // 发送停止命令
}

#if 0
#include "INS5699S.h"
#include "IIC.h"
#include "time.h"
#include "DATA.h"

extern u16 dat00[System_data_num];
extern u8 Time_New[7];
extern u8 xdata Time_int[7];


//========================================================================
// 函数: void INS5699_timeR()
// 描述: 读取时间，秒、分、时、星期、日、月、年
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void INS5699_timeR()
{
    u8 i = 0;
    Start(); //发送起始命令
    SendData(INS5699_W);                             //发送设备地址+写命令
    RecvACK();
    SendData(0x00);                             //发送设备地址+写命令
    RecvACK();
    Start(); //发送起始命令
    SendData(INS5699_R);                             //发送设备地址+写命令
    RecvACK();

    for(i=0; i<7; i++)
    {
        Time_New[i] = RecvData();                            //读取数据2
        if(i == 6)
        {
            SendNAK();
        }
        else
        {
            SendACK();
        }
    }
    Stop();
}


//========================================================================
// 函数: void INS5699_timeW(u8 *a)
// 描述: 写入时间，秒、分、时、星期、日、月、年
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================

void INS5699_timeW(u8 *a)
{
    u8 i = 0;
    Start(); //发送起始命令
    SendData(INS5699_W);                             //发送设备地址+写命令
    RecvACK();
    SendData(0x00);                             //发送设备地址+写命令
    RecvACK();
    for(i=0; i<7; i++)
    {
        SendData(a[i]);                             //发送设备地址+写命令
        RecvACK();
    }
    Stop();
}


//========================================================================
// 函数: void INS5699_Initialization()
// 描述: 芯片初始化
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void INS5699_Initialization()
{
    IIC_W2bit(0x64,0x21,0x80);//关闭芯片K2开关
    Delay10us();
    IIC_W2bit(0x64,0x0F,0x60);//配置开启1S时间更新 0x60
    Delay10us();
    IIC_W2bit(0x64,0x0D,0x0A);//FSEL 1Hz  定时器1Hz  时间更新：秒更新
    Delay10us();
    IIC_W2bit(0x64,0x0E,0x00);//清除警告
}




//========================================================================
// 函数: void Interrupt_Transmission(u8 position)
// 描述: RTC芯片中断时间切换
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void Interrupt_Transmission(u8 position)
{
    if(position)
    {
        IIC_W2bit(0x64,0x0D,0x2A);//FSEL 1Hz  定时器1Hz  时间更新：分更新
    }
    else
    {
        IIC_W2bit(0x64,0x0D,0x0A);//FSEL 1Hz  定时器1Hz  时间更新：秒更新
    }
}

#endif
