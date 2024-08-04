#include "TV_Mode.h"
#include "OLED.h"
#include "DATA.h"
#include "ADC.h"

extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

extern u8 xdata Time_New[7];
extern u8 xdata Time_Low[7];

extern u32 xdata JCQ06_00[10];

u16 Low_wendu = 0;


void time_show_s()
{
    ZF_8X16(0,0,(u8)(Time_New[2]/16));
    ZF_8X16(8,0,(u8)(Time_New[2]%16));
    ZF_8X16(16,0,20);
    ZF_8X16(24,0,(u8)(Time_New[1]/16));
    ZF_8X16(32,0,(u8)(Time_New[1]%16));
    ZF_8X16(40,0,20);
    ZF_8X16(48,0,(u8)(Time_New[0]/16));
    ZF_8X16(56,0,(u8)(Time_New[0]%16));
}






void UI_humidity()
{
    ZF_8X16(88,2,(u8)(dat00[0x0042]/1000));
    ZF_8X16(96,2,(u8)((dat00[0x0042]%1000)/100));
    ZF_8X16(112,2,(u8)((dat00[0x0042]%100)/10));
}


void UI_pressure()
{
    u16 k = 0;

    k = JCQ06_00[4]/10;


    ZF_8X8(120,5,(u8)(k % 10));

    ZF_8X8(108,5,(u8)((k % 100)/10));
    ZF_8X8(100,5,(u8)((k % 1000)/100));
    ZF_8X8(92,5,(u8)((k % 10000)/1000));
    k = k / 10000;
    if(k)
    {
        ZF_8X8(84,5,(u8)(k));
    }
    else
    {
        ZF_8X8(84,5,10);
    }
}




u8 Week_conversion()
{
    u8 p = 0;
    if(Time_New[3] & 0x01)
    {
        p=13;
    }
    else if(Time_New[3] & 0x02)
    {
        p=14;
    }
    else if(Time_New[3] & 0x04)
    {
        p=15;
    }
    else if(Time_New[3] & 0x08)
    {
        p=16;
    }
    else if(Time_New[3] & 0x10)
    {
        p=17;
    }
    else if(Time_New[3] & 0x20)
    {
        p=18;
    }
    else if(Time_New[3] & 0x40)
    {
        p=19;
    }
    return p;



}





u16 Time_comparison()
{
    u16 j = 0;

    if(Time_New[0] != Time_Low[0])
    {
        if(Time_New[0]/16 != Time_Low[0]/16)  j |= 0x0002;
        if(Time_New[0]%16 != Time_Low[0]%16)  j |= 0x0001;
    }

    if(Time_New[1] != Time_Low[1])
    {
        if(Time_New[1]/16 != Time_Low[1]/16)  j |= 0x0008;
        if(Time_New[1]%16 != Time_Low[1]%16)  j |= 0x0004;
    }

    if(Time_New[2] != Time_Low[2])
    {
        if(Time_New[2]/16 != Time_Low[2]/16)  j |= 0x0020;
        if(Time_New[2]%16 != Time_Low[2]%16)  j |= 0x0010;
    }
    if(Time_New[3] != Time_Low[3]) j |= 0x0040;
    if(Time_New[4] != Time_Low[4]) j |= 0x0080;
    if(Time_New[5] != Time_Low[5]) j |= 0x0100;
    if(Time_New[6] != Time_Low[6]) j |= 0x0200;

    return j;
}



void time_synchronization()
{
    u8 i = 0;
    for(i=0; i<7; i++)
    {
        Time_Low[i] = Time_New[i];
    }
}


void TVMode_int()
{
    //ADC_Get_V1();
    IU_Battery_Icon_S(80,0);
	
	
	  yue_ri(0,20,0);//
    yue_ri(16,(u8)((Time_New[6]/16*10) + (Time_New[6]%16)),0);//

    ZF_8X16(32,0,22);

    yue_ri(37,(u8)((Time_New[5]/16*10) + (Time_New[5]%16)),0);//

    ZF_8X16(53,0,22);

    yue_ri(59,(u8)((Time_New[4]/16*10) + (Time_New[4]%16)),0);//

    ZF_8X16(112,1,Week_conversion());
	
	  ZF_8X16(100,1,29);
	
	  ZF_8X16(88,1,28);
	

    shifen(12,40);
    shifen(12,80);

    shifen_dh(8,3,(Time_New[2]/16)<<5,40);
    shifen_dh(24,3,(Time_New[2]%16)<<5,40);
    shifen_dh(48,3,(Time_New[1]/16)<<5,40);
    shifen_dh(64,3,(Time_New[1]%16)<<5,40);
    shifen_dh(88,3,(Time_New[0]/16)<<5,40);
    shifen_dh(104,3,(Time_New[0]%16)<<5,40);
    //shifen_dh(8,3,13<<5,100);
    time_synchronization();
}




void TVMode2_int()
{
    //ADC_Get_V1();
    IU_Battery_Icon_S(80,0);
    time_show_s();
    shifen_dh(4,3,(dat00[0x0041]/1000)<<5,40);
    shifen_dh(20,3,((dat00[0x0041]%1000)/100)<<5,40);
    shifen_dh(36,3,11<<5,100);
    shifen_dh(44,3,((dat00[0x0041]%100)/10)<<5,40);
    shifen_dh(60,3,13<<5,100);

    UI_humidity();
    ZF_8X16(104,2,11);
    ZF_8X16(120,2,21);
    ZF_8X8(116,5,15);
    ZF_8X8(120,6,14);
    ZF_8X8(112,6,13);
    ZF_8X8(104,6,12);
    UI_pressure();

    time_synchronization();
}



void TVMode3_int()
{
    //ADC_Get_V1();
    time_show_s();
    IU_Battery_cheek();
//		UI_humidity();
//	  ZF_8X16(104,2,11);
//		ZF_8X16(120,2,21);
//	  ZF_8X8(116,5,15);
//	  ZF_8X8(120,6,14);
//	  ZF_8X8(112,6,13);
//	  ZF_8X8(104,6,12);
//		UI_pressure();
    IU_Battery_cheek_renovate();
    time_synchronization();
}
































void TVMode_1()
{
    u16 dat = 0;
    dat  = Time_comparison();
    if(dat & 0x0001)
    {
        //秒刷新
        shifen_dh(104,3,(33 - dat00[0x12] + (Time_Low[0]%16)*32),40);
        if(dat00[0x0004] & 0x0001)
        {
            //ADC_Get_V1();
            IU_Battery_Icon_S(80,0);
            dat00[0x0004] &= ~0x0001;
        }

    }
    if(dat & 0x0002)
    {
        shifen_dh(88,3,(33 - dat00[0x12] + (Time_Low[0]/16)*32),24);
    }
    if(dat & 0x0004)
    {
        shifen_dh(64,3,(33 - dat00[0x12] + (Time_Low[1]%16)*32),40);
    }
    if(dat & 0x0008)
    {
        shifen_dh(48,3,(33 - dat00[0x12] + (Time_Low[1]/16)*32),24);
    }
    if(dat & 0x0010)
    {
        if((u8)(Time_Low[2]/16) == 2)
        {
            shifen_dh(24,3,(33 - dat00[0x12] + (Time_Low[2]%16)*32),16);
        }
        else
        {
            shifen_dh(24,3,(33 - dat00[0x12] + (Time_Low[2]%16)*32),40);
        }
    }
    if(dat & 0x0020)
    {
            shifen_dh(8,3,(33 - dat00[0x12] + (Time_Low[2]/16)*32),12);
    }
    if(dat & 0x0040)
    {
        ZF_8X16(112,1,Week_conversion());
    }
    if(dat & 0x0080)
    {
        yue_ri(59,(u8)((Time_New[4]/16*10) + (Time_New[4]%16)),0);//
    }
    if(dat & 0x0100)
    {
        yue_ri(37,(u8)((Time_New[5]/16*10) + (Time_New[5]%16)),0);//
    }
    if(dat & 0x0200)
    {
        yue_ri(16,(u8)((Time_New[6]/16*10) + (Time_New[6]%16)),0);//
    }
		//time_synchronization();
}






void TVMode_2()
{
    u16 dat = 0;
    dat  = Time_comparison();
    //ADC_Get_V1();
    IU_Battery_Icon_S(80,0);
    if(dat & 0x0001)//秒刷新
    {
        ZF_8X16(56,0,(u8)(Time_New[0]%16));
    }
    if(dat & 0x0002)
    {
        ZF_8X16(48,0,(u8)(Time_New[0]/16));
    }


    if(dat & 0x0004)
    {
        ZF_8X16(32,0,(u8)(Time_New[1]%16));
    }
    if(dat & 0x0008)
    {
        ZF_8X16(24,0,(u8)(Time_New[1]/16));
    }


    if(dat & 0x0010)
    {
        ZF_8X16(8,0,(u8)(Time_New[2]%16));

    }
    if(dat & 0x0020)
    {
        ZF_8X16(0,0,(u8)(Time_New[2]/16));
    }


    dat = dat00[0x0041]/1000;
    if(dat != (Low_wendu/1000))
    {
        shifen_dh(4,3,dat<<5,40);
    }


    dat = (dat00[0x0041]%1000)/100;
    if(dat != (Low_wendu%1000)/100)
    {
        shifen_dh(20,3,((dat00[0x0041]%1000)/100)<<5,40);
    }


    dat = (dat00[0x0041]%100)/10;
    if(dat != (Low_wendu % 100)/10)
    {
        shifen_dh(44,3,dat<<5,40);
    }
    //**************温湿度*****************************

    UI_humidity();
    UI_pressure();

    Low_wendu = dat00[0x0041];
    time_synchronization();
}



void TVMode_3()
{
    u16 dat = 0;
    dat  = Time_comparison();
    //ADC_Get_V1();
    if(dat & 0x0001)//秒刷新
    {
        ZF_8X16(56,0,(u8)(Time_New[0]%16));
    }
    if(dat & 0x0002)
    {
        ZF_8X16(48,0,(u8)(Time_New[0]/16));
    }


    if(dat & 0x0004)
    {
        ZF_8X16(32,0,(u8)(Time_New[1]%16));
    }
    if(dat & 0x0008)
    {
        ZF_8X16(24,0,(u8)(Time_New[1]/16));
    }


    if(dat & 0x0010)
    {
        ZF_8X16(8,0,(u8)(Time_New[2]%16));

    }
    if(dat & 0x0020)
    {
        ZF_8X16(0,0,(u8)(Time_New[2]/16));
    }

    IU_Battery_cheek_renovate();






    time_synchronization();
}



//    dat00[0x10]=0x0020;
//    dat00_flag[0x10]=1;


void IU_TVMode()//线程处理
{
	  u8 a = 0;
    if(dat00[0x10] == 0x0020 && dat00[0x1A] == 0 && dat00[0x002E] > 0)//TV时钟模式
    {
        if((dat00[0x002E] & 0x0001) && !(dat00[0x002E] & 0x0002))
        {
            dat00[0x002E] |= 0x0002;
        }
        if((dat00[0x002E] & 0x0001) && (dat00[0x002E] & 0x0002))
        {
            if(dat00[0x12] == 0x0000 && !(dat00[0x002E] & 0x0004))
            {
                dat00[0x12] = 32;
                fast_SleepQueue_aheadW1(8,1,32);
                dat00[0x002E] |= 0x0004;
            }
            if(dat00[0x12] != 0x0000 && (dat00[0x002E] & 0x0004))
            {
                TVMode_1();
                dat00[0x12]--;
            }
            if(dat00[0x12] == 0x0000 && (dat00[0x002E] & 0x0004))
            {
                dat00[0x002E] = 0;
                time_synchronization();
            }
        }


    }
    else if(dat00[0x10] == 0x0021 && dat00[0x1A] == 0 && dat00[0x002E] > 0)//TV时钟模式
    {
        TVMode_2();
        dat00[0x002E] = 0;
        time_synchronization();
    }
    else if(dat00[0x10] == 0x0022 && dat00[0x1A] == 0 && dat00[0x002E] > 0)//TV时钟模式
    {
        TVMode_3();
        dat00[0x002E] = 0;
        time_synchronization();
    }


    a = (u8)P45 + (u8)P46 + (u8)P27;

    if((a == 3) && (dat00[0x0010] >= 0x0020 &&  dat00[0x0010] <= 0x0022) && dat00[0x1A] == 0 && (dat00[0x0005] & 0x0001))
    {
			  dat00[0x002D]++;
        if(dat00[0x0010] == 0x0020)
        {
            if(dat00[0x002D] >= dat00[0x0022])
            {
                if(dat00[0x0023] > 0)
                {
                    dat00[0x10]=0x0021;
                    dat00_flag[0x10]=1;
                }
                else if(dat00[0x0024] > 0)
                {
                    dat00[0x10]=0x0022;
                    dat00_flag[0x10]=1;
                }
                dat00[0x002D]=0;
            }
        }
        else if(dat00[0x0010] == 0x0021)
        {
            if(dat00[0x002D] >= dat00[0x0023])
            {
                if(dat00[0x0024] > 0)
                {
                    dat00[0x10]=0x0022;
                    dat00_flag[0x10]=1;
                }
                else if(dat00[0x0022] > 0)
                {
                    dat00[0x10]=0x0020;
                    dat00_flag[0x10]=1;
                }
                dat00[0x002D]=0;
            }
        }
        else if(dat00[0x0010] == 0x0022)
        {
            if(dat00[0x002D] >= dat00[0x0024])
            {
                dat00[0x10]=0x0020;
                dat00_flag[0x10]=1;
                dat00[0x002D]=0;
            }
        }
				dat00[0x0005] &= ~0x0001;
    }

    












}






















