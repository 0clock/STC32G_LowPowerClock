#include "BMC050.h"
#include "time.h"
#include "UART1.h"
#include "IIC.h"
#include "CompassMenu.h"
#include "DATA.h"


extern u8 xdata DMABuffer[96];

const int code angle[15] = {11520,6801,3593,1824,916,458,229,115,57,29,14,7,4,2,1};


extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_task[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;



int Magnetism_X[7] = {0};
int Magnetism_X1[7] = {0};

int Magnetism_Y[7] = {0};
int Magnetism_Y1[7] = {0};


int Magnetism_JD[5] = {0};
int Magnetism_JD1[5] = {0};

u16 JD_low = 0;
u8 JD_low_num = 0;

int Get_Magnetism_dat(int *dat1,u16 dat,u8 num)
{
    int adc_dat = 0;
    u8 i = 0;
    num = num-1;
    for(i=0; i<num; i++)
    {
        dat1[i] = dat1[i+1];
    }
    dat1[num] = (int)dat;
    num++;
    for(i=0; i<num; i++)
    {
        adc_dat += dat1[i];
    }
    adc_dat = adc_dat/num;

    return  adc_dat;
}

int convert_1(u16 dat1)
{
    int adc_dat=0;


    if(dat1 & 0x1000)
    {
        adc_dat= 0x1fff - dat1 + 1;
        adc_dat = adc_dat*(-1);
    }
    else
    {
        adc_dat = dat1;
    }

    return  adc_dat;
}










void Magnetometer_initi()
{
    IIC_W2bit(0x22,0x4b,0x01);// 0x01 休眠模式 0x80 软复位
	  Sleep_delay_Time(20);
    IIC_W2bit(0x22,0x51,0x0A);//XY 重复21次
	  IIC_W2bit(0x22,0x4C,0x00);
    IIC_W2bit(0x22,0x4b,0x00);//挂起
}


void  Magnetometer_OPEN()
{
    IIC_W2bit(0x22,0x4b,0x01);//切换到休眠模式
    IIC_W2bit(0x22,0x4C,0x02);//采样速度10Hz,强制采样模式
    Sleep_delay_Time(20);
}

void  Magnetometer_Close()
{
    IIC_W2bit(0x22,0x4b,0x00);//切换到休眠模式
}





















void ggg(u8 addre,long num)
{

    if(num >= 0)
    {
        DMABuffer[addre] = 43;
    }
    else
    {
        DMABuffer[addre] = 45;
        num = num*(-1);
    }

    DMABuffer[addre+1] = ((num % 100000) /10000) + 48;
    DMABuffer[addre+2] = ((num % 10000) / 1000) + 48;
    DMABuffer[addre+3] = ((num % 1000) / 100) + 48;
    DMABuffer[addre+4] = ((num % 100) / 10) + 48;
    DMABuffer[addre+5] = (num % 10) + 48;





}








u16 atan(int x1,int y1)//结果需要/256
{
    int i =0;
    long x_new = 0;
    long y_new = 0;
    long xx,yy;
    long angleSum =0;
    long x =x1;
    long y =y1;

    if(x<0) x = x*(-1);
    if(y<0) y = y*(-1);

    x *= 1024;
    y *= 1024;
    for(i=0; i<15; i++)
    {
        yy = (y>>i);
        xx = (x>>i);
        if(y>0)
        {
            x_new = (x + yy);
            y_new = (y - xx);
            angleSum += angle[i];
        }
        else
        {
            x_new = x - yy;
            y_new = y + xx;
            angleSum -= angle[i];
        }
        x = x_new;
        y = y_new;
    }

    return  (u16)angleSum;
}


void Angle_calculation(int x1,int y1)
{
    u8 a=0;
    u8 b=0;
    u16 Angle = 0;

    if(x1<0) a=1;
    if(y1<0) b=1;

    if(x1 == 0)
    {
        if(y1 > 0)
        {
            Angle =90;
        }
        else
        {
            Angle =270;
        }
    }
    else if(y1 == 0)
    {
        if(x1 > 0)
        {
            Angle =360;
        }
        else
        {
            Angle =180;
        }
    }
    else
    {
        Angle = (atan(y1,x1))/256;

        if(!a && b)
        {
            Angle = Angle +270;
        }
        else if(a && b)
        {
            Angle = 270 -Angle;
        }
        else if(a && !b)
        {
            Angle = Angle +90;
        }
        else
        {
            Angle = 90 -Angle;
        }

    }

    dat00[0x0068] = Angle;






}











u16 Magnetometer_measure_1(u8 aa,u8 b)
{
    u8 a = 0;
    u16 c = 0;
    a = RecvData();                             //读取数据2
    SendACK();
    c = RecvData();                             //读取数据2
    if(b)
    {
        SendACK();
    }
    else
    {
        SendNAK();
        Stop();
    }
    c = (((c<<8)+a)>>aa);
    return c;

}


void Magnetometer_measure()
{
//    u8 a = 0;
    u16 c = 0;
    u16 b=0;

    IIC_W2bit(0x22,0x4C,0x02);//采样速度10Hz,正常采样模式


    Start(); //发送起始命令
    SendData(0x22);                         //发送设备地址+写命令
    DMABuffer[0] = RecvACK();
    SendData(0x42);                             //发送设备地址+写命令
    RecvACK();
    // Stop();
    Start(); //发送起始命令
    SendData(0x23);                         //发送设备地址+写命令
    RecvACK();

    dat00[0x0060] = Magnetometer_measure_1(3,1);
    dat00[0x0061] = Magnetometer_measure_1(3,1);
    dat00[0x0062] = Magnetometer_measure_1(1,1);
    dat00[0x0063] = Magnetometer_measure_1(2,0);



    dat00[0x0060] = (u16)convert_1(dat00[0x0060]);
    dat00[0x0061] = (u16)convert_1(dat00[0x0061]);



    dat00_low[0x0060] = (u16)Get_Magnetism_dat(Magnetism_X,dat00[0x0060],7);
    dat00_low[0x0060] = (u16)Get_Magnetism_dat(Magnetism_X1,dat00_low[0x0060],7);

    dat00_low[0x0061] = (u16)Get_Magnetism_dat(Magnetism_Y,dat00[0x0061],7);
    dat00_low[0x0061] = (u16)Get_Magnetism_dat(Magnetism_Y1,dat00_low[0x0061],7);






//    dat00_low[0x0060] = (u16)Get_Magnetism_dat(Magnetism_X,convert_1(dat00[0x0060]));
//    dat00_low[0x0061] = (u16)Get_Magnetism_dat(Magnetism_Y,convert_1(dat00[0x0061]));




    if( (int)dat00[0x0060] > (int)dat00[0x0064])
    {
        dat00[0x0064] = dat00_low[0x0060];
    }
    if((int)dat00[0x0060] < (int)dat00_low[0x0064])
    {
        dat00_low[0x0064] = dat00_low[0x0060];
    }
    if( (int)dat00[0x0061] > (int)dat00[0x0065])
    {
        dat00[0x0065] = dat00_low[0x0061];
    }
    if((int)dat00[0x0061] < (int)dat00_low[0x0065])
    {
        dat00_low[0x0065] = dat00_low[0x0061];
    }


    dat00[0x0066] = (u16)(((int)dat00[0x0064] + (int)dat00_low[0x0064])/2);
    dat00_low[0x0066] = (u16)(((int)dat00[0x0065] + (int)dat00_low[0x0065])/2);


    dat00[0x0067] = (u16)(((int)dat00[0x0066] - (int)dat00_low[0x0060]));
    dat00_low[0x0067] = (u16)(((int)dat00_low[0x0066] - (int)dat00_low[0x0061]));

    Angle_calculation((int)dat00[0x0067],(int)dat00_low[0x0067]);


}
































void Magnetometer_falg_0080()
{


    if(Sleep_Queue_task[0] & 0x0080)
    {
        Sleep_Queue_task[0] &=~0x0080;
        if(dat00[0x10] == 0x0005)
        {
            Magnetometer_measure();
            UI_CompassMenu();
            SleepQueue_aheadW1(250,0x0080,0);
        }
    }
}












