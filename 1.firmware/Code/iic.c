#include "iic.h"

void iic_init()
{
    P_SW2 |= 0x80 + 0x00;
    I2CCFG = 0xC0 + 35; // 满速400K 35M 设定20
    I2CMSST = 0x00;
}

bit iic_wait()
{
    bit k = 0;
    u16 i = 100;
    while ((!(I2CMSST & 0x40)) & (i > 0))
    {
        i--;
    }
    if (I2CMSST & 0x02)
    {
        k = 1;
    }
    I2CMSST &= ~0x40;
    return k;
}

void iic_start()
{
    I2CMSCR = 0x01; // 发送START命令
    iic_wait();
}

bit iic_senddata(char dat)
{
    I2CTXD = dat;   // 写数据到数据缓冲区
    I2CMSCR = 0x02; // 发送SEND命令
    return iic_wait();
}

void iic_stop()
{
    I2CMSCR = 0x06; // 发送STOP命令
    iic_wait();
}

bit RecvACK()
{
    bit p = 0;
    I2CMSCR = 0x03; // 发送读ACK命令
    p = iic_wait();
    return p;
}

u8 RecvData()
{
    I2CMSCR = 0x04; // 发送RECV命令
    iic_wait();
    return I2CRXD;
}



void SendACK()
{
    I2CMSST = 0x00; // 设置ACK信号
    I2CMSCR = 0x05; // 发送ACK命令
    iic_wait();
}

void SendNAK()
{
    I2CMSST = 0x01; // 设置NAK信号
    I2CMSCR = 0x05; // 发送ACK命令
    iic_wait();
}

bit iic_write2bit(u8 addre, u8 MSB, u8 LSB)
{
    bit ACK = 0;

    iic_start();         // 发送起始命令
    iic_senddata(addre); // 发送设备地址+写命令
    ACK |= RecvACK();
    iic_senddata(MSB); // 发送设备地址+写命令
    ACK |= RecvACK();
    iic_senddata(LSB); // 发送设备地址+写命令
    ACK |= RecvACK();
    iic_stop();
    return ACK;
}

bit iic_write1bit(u8 addre, u8 LSB)
{
    bit ACK = 0;
    iic_start();         // 发送起始命令
    iic_senddata(addre); // 发送设备地址+写命令
    ACK |= RecvACK();
    iic_senddata(LSB); // 发送设备地址+写命令
    ACK |= RecvACK();
    iic_stop();
    return ACK;
}

u8 iic_read1bit(u8 nam, u8 adds)
{
    u8 a = 0;
    iic_start();       // 发送起始命令
    iic_senddata(nam); // 发送设备地址+写命令
    RecvACK();
    iic_senddata(adds); // 发送设备地址+写命令
    RecvACK();
    iic_start();                 // 发送起始命令
    ((u8)(nam + 1)); // 发送设备地址+写命令
    RecvACK();
    a = RecvData(); // 读取数据2
    SendNAK();
    iic_stop();
    return a;
}

#if 0

//========================================================================
// 函数: u8 IIC_Query_1(u8 adds)
// 描述: 检测传感器是否应答
// 参数: adds ： 从机地址
// 返回: 返回0正常，1异常
// 版本: V1.0 2023.02.05
//========================================================================
u8 IIC_Query_1(u8 adds)
{
    u8 num = 0;
    iic_start();        // 发送起始命令
    iic_senddata(adds); // 发送设备地址+写命令
    num = RecvACK();
    iic_stop();
    Delay10us();
    return num;
}

//========================================================================
// 函数: u8 IIC_Query()
// 描述: 扫描传感器，看是否应答，判断焊接是否正确
// 参数:
// 返回: 返回0则全部支持
// 版本: V1.0 2023.02.05
//========================================================================

u8 IIC_Query()
{
    u8 num = 0;
    num |= IIC_Query_1(0x22);
    num = num << 1;
    num |= IIC_Query_1(0x32);
    num = num << 1;
    num |= IIC_Query_1(INS5699_W);
    num = num << 1;
    num |= IIC_Query_1(SPL06_W);
    num = num << 1;
    num |= IIC_Query_1(BH1745_W);
    return num;
}

#endif