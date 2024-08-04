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


















