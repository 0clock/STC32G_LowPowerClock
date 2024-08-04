#include "SleepMode.h"
#include "IO.h"
#include "OLED.h"
#include "IIC.h"
#include "DATA.h"
#include "ADC.h"
#include "time.h"
#include "INS5699S.h"
#include "TV_Mode.h"
#include "BMC050.h"



extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

extern u8 Time_New[7];
extern u8 Time_Low[7];


extern u8 OLED_InverseDisplay;







//========================================================================
// 函数: SleepMode_ON()
// 描述: 整机进入休眠模式
// 参数:
// 返回:
// 版本: V1.0 2024.02.12
//========================================================================
void SleepMode_ON()
{
	  EA = 0;
    if(dat00[0x0035] == 0x0001)//深度休眠
    {
        IIC_W2bit(0x64,0x0F,0x40);//禁用时间更新中断
        IIC_W2bit(0x64,0x0E,0x00);//清除警告
			  IIC_W2bit(BH1745_W,0x42,0x02);
    }
    OLED_WrCmd(0xa6);		  //OLED关闭
    Magnetometer_Close();//磁强计挂起
    if(dat00[0x0035] == 0x0001)//深度休眠
    {
        ADC_CONTR = 0x00;
        I2CCFG = 0x40+35;    //满速400K 35M 设定20
        P1PU = 0x00;         //IIC上拉电阻
        SPCTL &= 0xbf;
    }
//  BMC050 DRDYO中断+  KEYSET + KEY+ 设置为双向口 默认高电平
//
//	（双向）P07 SPL06_INT        （开漏）P06 OLEDV1       （开漏）P05 OLEDV2    （高阻）P04 BATTERY（电池ADC采样脚）   （高阻）P03 ADC6（充电电流）   （开漏）PP02 EN（锂电池充电EN）   （高阻）P01 GHRG   （开漏）PP00 VIN/2

    if(dat00[0x0036])//如果是休眠屏显模式，控制电压档位的两个脚不变
    {
			  dat00[0] |= 0x0080;
        P00 = 1;
        P01 = 1;
        P02 = 1;
        P03 = 1;
        P04 = 1;
        P07 = 1;
    }
    else
    {
        P0=0xff;
    }
    P0M0 = 0x65;
    P0M1 = 0x7f;



//*********************P4区*******************************
//	（开漏）P47 DRDYO        P46 KEYSET       P45 KEY+    P44 悬空   P43 悬空   P42 悬空   P41 悬空   P40 CCP0_EN（蜂鸣器控制）
    if(dat00[0x0035] == 0x0001)//深度休眠
    {
        P4=0xfe;
        P4M0 = 0x9f;
        P4M1 = 0x9f;
    }
    else
    {
			  if(dat00[0x0005])//如果蜂鸣器还在响，不关闭4K方波独立发生电路的供电
        {
            P4=0xFF;
            P4M0 = 0x1f;
            P4M1 = 0x1e;
        }
        else
        {
            P4=0xfe;
            P4M0 = 0x1f;
            P4M1 = 0x1f;
        }
    }




//*********************P1区*******************************
//	（双向）P17 INT3O        （开漏）P16 悬空       （双向）P15 SCL    （双向）P14 SDA   （开漏）P13 INT2O    （开漏）P12   （开漏）P11 BH_INT   （开漏）P10 悬空
    P1=0xff;
    P1M0 = 0x4d;
    P1M1 = 0x4d;

//*********************P3区*******************************
//	（双向）P37 INT3（BMC050）        （开漏）P36 悬空       （开漏）P35 悬空    （双向）P34 SW360   （开漏）P33 悬空    （开漏）P32 悬空     （开漏）P31 TX   （开漏）P30 RX
    P3=0xff;
    P3M0 = 0x03;
    P3M1 = 0x03;

//*********************P5区*******************************
//	（开漏）P57         （开漏）P56        （开漏）P55     （双向）P54 RSET   （开漏）P53 5VEN    （开漏）P52 BATTERY_ON     （开漏）P51 TEXT  （双向）P50 SQW（1Hz）
    if(dat00[0x0036])
    {
        P5=0xfB;
        P5M0 = 0x04;
        P5M1 = 0x04;
    }
    else
    {
        P5=0xf3;
        P5M0 = 0x0c;
        P5M1 = 0x0c;
    }



//*********************P2区*******************************
//	（双向）P27  KEY-       （双向）P26  KEYMODE      （推挽）P25 SCLK    （开漏）P24 悬空   （推挽）P23 MOSI    （开漏）P22 CS     （双向）P21 DC  （双向）P20 RES
    if(dat00[0x0036])
    {
        P2PU = 0x01;//打开OLED  RSET上拉电阻
        P2 = 0xFB;
        P2M0 = 0x3C;
        P2M1 = 0x14;
    }
    else
    {
        P2PU = 0x00;
        P2=0xC0;
        P2M0 = 0x3F;
        P2M1 = 0x3F;
    }







    P6=0xff;
    P6M0 = 0xff;
    P6M1 = 0xff;
    P7=0xff;
    P7M0 = 0xff;
    P7M1 = 0xff;




    P0IE=0x00;
    if(dat00[0x0035] == 0x0001)//深度休眠
    {
        P1IE=0x00;
    }
    else
    {

        P1IE=0x30;
    }
    P5IE=0x11;

    P2IE=0x00;
    P3IE=0x10;
    P4IE=0x00;




    P6IE=0x00;
    P7IE=0x00;






    P0INTE = 0x00;//允许端口中断
    P0WKUE = 0x00;//允许掉电唤醒

    P3IM1 &= ~0x10;//下降沿中断
    P3IM0 &= ~0x10;
    P3INTE = 0x10;//允许端口中断
    P3WKUE = 0x10;//允许掉电唤醒


    if(dat00[0x0035] == 0x0001)//深度休眠
    {
        P5INTE &= ~0x01;//允许端口中断
        P5WKUE = 0x00;//允许掉电唤醒
    }
    else
    {
        P5INTE |= 0x01;//允许端口中断
        P5WKUE |= 0x01;//允许掉电唤醒
    }



    P4INTE = 0x00;//允许端口中断
    P4WKUE = 0x00;//允许掉电唤醒
    P2INTE = 0x00;//允许端口中断
    P2WKUE = 0x00;//允许掉电唤醒
		OLED_InverseDisplay=0;
    EA = 1;


    _nop_();
    _nop_();
    _nop_();
    _nop_();
    PD = 1;                            //MCU进入掉电模式
    _nop_();
    _nop_();
    _nop_();
    _nop_();

}




//========================================================================
// 函数: SleepMode_OFF()
// 描述: 整机退出休眠模式
// 参数:
// 返回:
// 版本: V1.0 2024.02.12
//========================================================================
void SleepMode_OFF()
{
    P3WKUE &= ~0x10;
    P3INTE &= ~0x10;
    EA=0;
    P54RST = 1;
    P0=0xff;
    P1=0xff;
    P2=0xff;
    P3=0xff;
    P4=0xff;
    P5=0xff;
    P6=0xff;
    P7=0xff;
    IO_Init();
    OLED_POW(0);
    RESET54();
	  if(dat00[0x0036]) dat00[0] &= ~0x0080;
    ADC_init();
    Clock_INT();
    key_int();
    IIC_int();
    IIC_W2bit(0x64,0x21,0x80);//关闭芯片K2开关
    Delay10us();
    IIC_W2bit(0x64,0x0F,0x60);//配置开启1S定时 0x60
    Delay10us();
    IIC_W2bit(0x64,0x0D,0x0A);//FSEL 1Hz  定时器1Hz  时间更新：秒更新
    dat00[0x0006] &= ~0x0001;
    Delay10us();
    SPI_init();
    SPI_DMA_Config();
    P2PU = 0x01;//打开OLED  RSET上拉电阻
    if(dat00[0x0036] == 0) OLED_Init();
    P2PU = 0x01;//打开OLED  RSET上拉电阻
    delay_ms(1);
    INS5699_timeR();
    dat00[0] |= 0x0002;
    dat00[0x10]=0x0020;
    dat00_flag[0x10]=1;
    dat00[0x2D]=0xFFFF;
    SleepQueue_aheadW1(5,1,0);
    P0INTE |= 0x02;//允许端口中断
    P0IM1 |=  0xFD;//下降沿中断
    P0IM0 &=  0xFD;
    P0WKUE |= 0x02;//允许掉电唤醒
		OLED_InverseDisplay=0;
    EA = 1;
}



//========================================================================
// 函数: Sleep_task()
// 描述: 休眠任务，唤醒后执运行TV模式指定秒数的显示后再回到休眠模式
// 参数:
// 返回:
// 版本: V1.0 2024.02.12
//========================================================================
void Sleep_task()
{
    if(dat00[0] & 0x0020)
    {
        OLED_Fill(0x00); //初始清屏
        Sleep_Display_int();
        SleepMode_ON();
        dat00[0] &= ~0x0040;
        dat00[0] &= ~0x0020;
    }
    if(dat00[0] & 0x0010)
    {
        SleepMode_OFF();
        dat00[0] |= 0x0040;
        dat00[0] &= ~0x0010;
    }
}

//========================================================================
// 函数: Sleep_Display_int()
// 描述: 休眠屏显模式初始化
// 参数:
// 返回:
// 版本: V1.0 2024.02.12
//========================================================================
void Sleep_Display_int()
{

	  shifen_2(10,56);//显示两个点

    shifen_2((u8)(Time_New[2]/16),24);//显示时十位
    shifen_2((u8)(Time_New[2]%16),40);//显示时个位

    shifen_2((u8)(Time_New[1]/16),72);//显示分十位
    shifen_2((u8)(Time_New[1]%16),88);//显示分个位
    time_synchronization();//旧时间Time_Low[]更新
}



//========================================================================
// 函数: Sleep_Display_UI()
// 描述: 休眠屏显模式UI更新
// 参数:
// 返回:
// 版本: V1.0 2024.02.12
//========================================================================
void Sleep_Display_UI()
{
    u8 a=0;
    u8 b=0;

    if(Time_New[1] != Time_Low[1])
    {
        shifen_2((u8)(Time_New[1]%16),88);
        a = Time_New[1]/16;
        b = Time_Low[1]/16;
        if(a != b)
        {
            shifen_2((u8)(Time_New[1]/16),72);
        }
        a = Time_New[2]%16;
        b = Time_Low[2]%16;
        if(a != b)
        {
            shifen_2((u8)(Time_New[2]%16),40);
        }
        a = Time_New[2]/16;
        b = Time_Low[2]/16;
        if(a != b)
        {
            shifen_2((u8)(Time_New[2]/16),24);
        }
        time_synchronization();
    }


}









