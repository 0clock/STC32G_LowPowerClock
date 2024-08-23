#include "bms.h"
#include "dmx_adc.h"

bms_t battery;

void bms_init()
{
    init_adc(pin_battery_adc,ADC_CLK_2);
    init_adc(pin_current_adc,ADC_CLK_2);
}

void bms_main_task_500ms()
{
    battery.voltage = get_adc(pin_battery_adc,ADC_12BIT);
    battery.current = get_adc(pin_current_adc,ADC_12BIT);
}
#if 0

#include "ADC.h"
#include "time.h"
#include "IO.h"
#include "DATA.h"
#include "UART1.h"
#include "OLED.h"


extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];

u16 ADC_CK = 0;

u32 adc_rtf = 0;

u16 BV[8];

u8 pp=0;

u16 Battery_PE_Low = 0;

//========================================================================
// 函数: void VB_SET(u16 dat)
// 描述: 平均值数组赋初值
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void VB_SET(u16 dat)
{
    u8 i =0;
    for(i=0; i<8; i++)
    {
        BV[i] = dat;
    }
}


//========================================================================
// 函数: void VB_SET(u16 dat)
// 描述: 读取电池电压值
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
u16 Get_VB()
{
    u32 adc_dat = 0;
    u8 i = 0;
    Battery_ON_ON();//MOS打开
    for(i=0; i<4; i++)
    {
        adc_rtf = Get_ADC12bitResult(15);//读参考电压
    }
    for(i=0; i<4; i++)
    {
        adc_dat = Get_ADC12bitResult(0x0C);//读电池电压
    }
    Battery_ON_OFF();//MOS关闭
    for(i=0; i<4; i++)
    {
        adc_dat = Get_ADC12bitResult(0x0C);//再读一次读电池电压，MOS结电容有余电关不了这么快
    }
    adc_dat = (adc_dat<<16);
    adc_dat = (adc_dat/adc_rtf*ADC_CK)>>15;
    return  (u16)adc_dat;
}

//========================================================================
// 函数: u16 Get_VB_Average(u16 dat)
// 描述: 平均值滤波
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
u16 Get_VB_Average(u16 dat)
{
    u16 adc_dat = 0;
    u8 i = 0;
    for(i=0; i<7; i++)
    {
        BV[i] = BV[i+1];
    }
    BV[7] = dat;
    for(i=0; i<8; i++)
    {
        adc_dat += BV[i];
    }
    adc_dat = adc_dat>>3;

    return  adc_dat;
}





//========================================================================
// 函数: void ADC_init()
// 描述: ADC初始化
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void ADC_init()
{
    ADCEXCFG = 0x02;	//设置转换次数
    ADCTIM = 0x3f;		//设置 ADC 内部时序，ADC采样时间建议设最大值
    ADCCFG = 0x22;		//设置 ADC 时钟为系统时钟/2/16/16
    //ADC_CONTR = 0x80;
    //ADC_POWER=1; 			//ADC电源打开

    ADC_CONTR = 0x80;
    delay_ms(2);
    VB_SET(Get_VB());
    _nop_();
    ADC_CONTR = 0;
    ADC_CK = ((((u16)CHIPID7)<<8) | CHIPID8)+28;//读取内部参考源电压+修改
}


//========================================================================
// 函数: u16 Get_ADC12bitResult(u8 channel)
// 描述: ADC测量
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
u16 Get_ADC12bitResult(u8 channel) // channel = 0~15	0~7  P10~P17  8~14 P00~P06 15内部参考源
{
    u16 i = 100;
    ADC_RES = 0;
    ADC_RESL = 0;
    ADC_CONTR = 0x80 + channel;
    ADC_START = 1;//启动转换

    while((ADC_FLAG == 0) && (i--));   //等待ADC完成
    ADC_FLAG = 0;     //清除中断
    return  (((u16)ADC_RES << 8) | ADC_RESL);
}



//========================================================================
// 函数: void ADC_Get_V1()
// 描述: 检测是不是插有电池
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void ADC_Get_V1()
{
    if(P00)
    {
        if(dat00_low[0x00E2]>0)
        {
            dat00[0x00E2] = 0;
        }
        else
        {
            dat00[0x00E2] = 1;
        }
        dat00_low[0x00E2] = 0;
        P0INTE |= 0x02;//允许端口中断
        P0IM1 |=  0xFD;//下降沿中断
        P0IM0 &=  0xFD;
        P0WKUE |= 0x02;//允许掉电唤醒
    }
    else
    {
        dat00[0x00E2] = 1;
    }
    ADC_CONTR = 0x80;
    SleepQueue_aheadW1(16,0x0040,0);
}

/*
100%----4.20V
90%-----4.06V
80%-----3.98V
70%-----3.92V
60%-----3.87V
50%-----3.82V
40%-----3.79V
30%-----3.77V
20%-----3.74V
10%-----3.68V
5%------3.45V
0%------3.00V
*/






//========================================================================
// 函数: void Battery_percentage(u32 a)
// 描述: 电池电量判断（多少有点问题，到时候我再单独测软包电池的曲线看看）
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void Battery_percentage(u32 a)
{
    u16 b = 0;
    a = a*100;

    if(a>=417900)
    {
        b = 1000;
    }
    else if(a >= 406300)
    {
        b = 900 + (a - 406300)/116;
    }
    else if(a >= 394900)
    {
        b = 800 + (a - 394900)/114;
    }
    else if(a >= 384500)
    {
        b = 700 + (a - 384500)/104;
    }
    else if(a >= 376000)
    {
        b = 600 + (a - 376000)/85;
    }
    else if(a >= 369500)
    {
        b = 500 + (a - 369500)/65;
    }
    else if(a >= 364800)
    {
        b = 400 + (a - 364800)/47;
    }
    else if(a >= 361300)
    {
        b = 300 + (a - 361300)/35;
    }
    else if(a >= 357600)
    {
        b = 200 + (a - 357600)/37;
    }
    else if(a >= 352100)
    {
        b = 100 + (a - 352100)/55;
    }
    else if(a >= 348000)
    {
        b = 50 + (a - 348000)/41;
    }
    else if(a >= 342600)
    {
        b = (a - 342600)/54;
    }
    else
    {
        b = 0;
    }
    dat00[0x00E5] = b;
    if(!P00)
    {
			if(dat00[0x00E5] > (dat00_low[0x00E5] + 20) || dat00[0x00E5] < dat00_low[0x00E5])//不允许在2%只能的范围跳
        {
            dat00_low[0x00E5] = dat00[0x00E5];
        }
        else
        {
            dat00[0x00E5] = dat00_low[0x00E5];
        }
    }
}








//========================================================================
// 函数: void ADC_falg_0040()
// 描述: ADC使能之后要等1ms才能开始采集
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void ADC_falg_0040()
{
    u32 adc_dat = 0;
    if(Sleep_Queue_task[0] & 0x0040)
    {
        dat00_low[0x00E1] = dat00[0x00E1];
        dat00[0x00E1] = P00;

        adc_dat = Get_VB();



        if(adc_dat > (dat00[0x00E3]+200) || (adc_dat+200) < dat00[0x00E3] )
        {
            VB_SET((u16)adc_dat);
        }
        else
        {
            adc_dat = Get_VB_Average((u16)adc_dat);
        }
//			  //miao(pp++);
        dat00[0x00E3] = adc_dat;


        if(dat00[0x00E1])
        {
            adc_dat = Get_ADC12bitResult(0x0B);
            adc_dat = (adc_dat<<16);
            adc_dat = (((adc_dat/adc_rtf*ADC_CK)/25)*10)>>16;
            dat00[0x00E4] = adc_dat;
        }
        else
        {
            dat00[0x00E4] = 0;
        }
        Battery_percentage(dat00[0x00E3]);
        ADC_CONTR = 0;
        Sleep_Queue_task[0] &=~0x0040;
    }
}





#endif





