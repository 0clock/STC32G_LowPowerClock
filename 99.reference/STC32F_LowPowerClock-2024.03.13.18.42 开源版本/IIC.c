#include "IIC.h"
#include "time.h"

void IIC_int()
{
    P_SW2 |= 0x80 + 0x00;
    I2CCFG = 0xC0+35;    //满速400K 35M 设定20
    I2CMSST = 0x00;

}

bit Wait()
{
    bit k=0;
    u16 i =100;
    while ((!(I2CMSST & 0x40))&(i>0))
    {
        i--;
    }
    if(I2CMSST & 0x02)
    {
        k=1;
    }
    I2CMSST &= ~0x40;
    return k;
}

void Start()
{
    I2CMSCR = 0x01;                             //发送START命令
    Wait();
}

void SendData(char dat)
{
    I2CTXD = dat;                               //写数据到数据缓冲区
    I2CMSCR = 0x02;                             //发送SEND命令
    Wait();
}

bit RecvACK()
{
    bit p=0;
    I2CMSCR = 0x03;                             //发送读ACK命令
    p = Wait();
    return p;
}

u8 RecvData()
{
    I2CMSCR = 0x04;                             //发送RECV命令
    Wait();
    return I2CRXD;
}

void SendACK()
{
    I2CMSST = 0x00;                             //设置ACK信号
    I2CMSCR = 0x05;                             //发送ACK命令
    Wait();
}

void SendNAK()
{
    I2CMSST = 0x01;                             //设置NAK信号
    I2CMSCR = 0x05;                             //发送ACK命令
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06;                             //发送STOP命令
    Wait();
}






bit IIC_W2bit(u8 addre,u8 MSB,u8 LSB)
{
    bit ACK = 0;

    Start(); //发送起始命令
    SendData(addre);//发送设备地址+写命令
    ACK |= RecvACK();
    SendData(MSB);//发送设备地址+写命令
    ACK |= RecvACK();
    SendData(LSB);//发送设备地址+写命令
    ACK |= RecvACK();
    Stop();
    return ACK;
}

bit IIC_W1bit(u8 addre,u8 LSB)
{
    bit ACK = 0;
    Start(); //发送起始命令
    SendData(addre);//发送设备地址+写命令
    ACK |= RecvACK();
    SendData(LSB);//发送设备地址+写命令
    ACK |= RecvACK();
    Stop();
    return ACK;
}


u8 IIC_R1bit(u8 nam,u8 adds)
{
    u8 a= 0;
    Start(); //发送起始命令
    SendData(nam);                             //发送设备地址+写命令
    RecvACK();
    SendData(adds);                             //发送设备地址+写命令
    RecvACK();
    Start(); //发送起始命令
    SendData((u8)(nam + 1));                             //发送设备地址+写命令
    RecvACK();
    a = RecvData();                            //读取数据2
    SendNAK();
    Stop();
    return a;

}

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
    Start(); //发送起始命令
    SendData(adds);//发送设备地址+写命令
    num = RecvACK();
    Stop();
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
    num = num<<1;
    num |= IIC_Query_1(0x32);
    num = num<<1;
    num |= IIC_Query_1(INS5699_W);
    num = num<<1;
    num |= IIC_Query_1(SPL06_W);
    num = num<<1;
    num |= IIC_Query_1(BH1745_W);	
    return num;
}





