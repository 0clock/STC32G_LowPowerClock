#include "BH1745.h"
#include "IIC.h"
#include "time.h"
#include "DATA.h"
extern u8 xdata DMABuffer[32];
extern u32 xdata JCQ06_00[10];


extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];


u16 Brightness_num[7] = {0};






//========================================================================
// 函数: void BH1745_Init()
// 描述: BH1745初始化
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void BH1745_Init()
{
    IIC_W2bit(BH1745_W,0x40,0xCB);
    delay_ms(1);
    IIC_W2bit(BH1745_W,0x41,0x00);
    delay_ms(1);
    IIC_W2bit(BH1745_W,0x42,0x02);		//0x10 活动 0x00 休眠
}
//========================================================================
// 函数: void BH1745_Rdat(u8 adds)
// 描述: BH1745读数据
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void BH1745_Rdat(u8 adds)
{
    u16 a= 0;
    u16 b= 0;
    Start(); //发送起始命令
    SendData(BH1745_W);                             //发送设备地址+写命令
    RecvACK();
    SendData(adds);                             //发送设备地址+写命令
    RecvACK();
    Start(); //发送起始命令
    SendData(BH1745_R);                             //发送设备地址+写命令
    RecvACK();
    a = RecvData();                            //读取数据2
    SendACK();
    b = RecvData();                            //读取数据2
    SendACK();
    dat00[0x0045] = ((b<<8) + a);
    if(dat00[0x0045] & 0x8000)
    {
        dat00[0x0045] = (u16)(65536 - dat00[0x0045]);
    }


    //Stop();


    a = RecvData();                            //读取数据2
    SendACK();
    b = RecvData();                            //读取数据2
    SendACK();
    dat00[0x0046] = ((b<<8) + a);
    if(dat00[0x0046] & 0x8000)
    {
        dat00[0x0046] = (u16)(65536 - dat00[0x0046]);
    }

    a = RecvData();                            //读取数据2
    SendACK();
    b = RecvData();                            //读取数据2
    SendACK();
    dat00[0x0047] = ((b<<8) + a);
    if(dat00[0x0047] & 0x8000)
    {
        dat00[0x0047] = (u16)(65536 - dat00[0x0047]);
    }

    a = RecvData();                            //读取数据2
    SendACK();
    b = RecvData();                            //读取数据2
    SendNAK();
    dat00[0x0044] = ((b<<8) + a);
    if(dat00[0x0044] & 0x8000)
    {
        dat00[0x0044] = (u16)(65536 - dat00[0x0044]);
    }
    //发送停止命令
}


//========================================================================
// 函数: void BH1745_RdatX4()
// 描述: BH1745读数据光强、红光、蓝光、绿光分量
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void BH1745_RdatX4()
{
    //IIC_W2bit(BH1745_W,0x42,0x10);
    //delay_ms(200);
    BH1745_Rdat(0x50);
    BH1745_Rdat(0x52);
    BH1745_Rdat(0x54);
    BH1745_Rdat(0x56);
    //IIC_W2bit(BH1745_W,0x42,0x00);
}












//========================================================================
// 函数: void BH1745R_flag0002()
// 描述: BH1745任务，读取光强
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void BH1745R_flag0002()
{
    if(Sleep_Queue_task[0] & 0x0002)
    {
        BH1745_Rdat(0x50);
        IIC_W2bit(BH1745_W,0x42,0x02);
        Sleep_Queue_task[0] &=~0x0002;
    }
}



















