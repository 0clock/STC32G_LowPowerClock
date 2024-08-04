#include "CompassMenu.h"
#include "OLED.h"
#include "DATA.h"
#include "BMC050.h"

extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;


u8 Compass_Scale[5]= {0,0,0,0,0};

u16 Compass_data[5]= {0,0,0,0,0};

u16 start_X_low = 0;

u8 test1=0;

void UI_Compass_Animation_1(u8 a,bit clear)
{
    if(Compass_Scale[a] >127) return;
    OLED_Set_Pos(Compass_Scale[a],5);
    if(clear)
    {
				OLED_WrDat(0x18);
        if((Compass_data[a] % 90) == 0)
				{
					OLED_16X16_MOV((u8)(Compass_Scale[a]-8),3,4,0,(u8)(135-Compass_Scale[a]));				
				}
				else
				{
					OLED_8X8X3_MOV((u8)(Compass_Scale[a]-12),4,0xFFFF,0,(u8)(139-Compass_Scale[a]));
				}
    }
    else
    {
        if((Compass_data[a] % 90) == 0)
        {				 
            OLED_WrDat(0xFF);
					  OLED_16X16_MOV((u8)(Compass_Scale[a]-8),3,(u8)((Compass_data[a]%360)/90),0,(u8)(135-Compass_Scale[a]));//  135是127+8
        }
        else
        { 
            OLED_WrDat(0x3C);
					  OLED_8X8X3_MOV((u8)(Compass_Scale[a]-12),4,Compass_data[a],0,(u8)(139-Compass_Scale[a]));//  139是127+12
        }
    }
}
















void UI_CompassMenu_int()
{
    u8 i = 0;
    Magnetometer_OPEN();
    OLED_Set_Pos(0,5);
    for(i=0; i<128; i++)
    {
        OLED_WrDat(0x18);
    }
    OLED_16X16(56,6,65);
    SleepQueue_aheadW1(5,0x0080,0);



}


void UI_Compass_Animation()//FF  3C
{
    u8 i = 0;
    u16 start_X = 0;
    u16 b = 0;
    u16 c = 0;

    u8 a1= 0;
    u8 b1= 0;
    u8 c1= 0;
    u8 d1= 0;


    if(dat00_low[0x0068] != dat00[0x0068])
    {
        for(i=0; i<5; i++)
        {
            UI_Compass_Animation_1(i,1);
        }

        if(dat00[0x0068] < 64)
        {
            start_X = dat00[0x0068] + 296;
        }
        else
        {
            start_X = dat00[0x0068] - 64;
        }	
        i = start_X/30;
        i = i+1;
        b = i*30;
        for(i=0; i<5; i++)
        {
            Compass_Scale[i] = b- start_X;
            Compass_data[i] = (b % 360);
            b += 30;
        }
        for(i=0; i<5; i++)
        {
            UI_Compass_Animation_1(i,0);
        }
        dat00_low[0x0068] = dat00[0x0068];
    }
}








void UI_CompassMenu_time()
{
    if(dat00[0x10] == 0x0005)
    {
//	  IU_Battery_Icon_S(80,0);
//    time_show_s();
    }
}



void UI_CompassMenu()
{
//    debug_8x8(dat00_low[0x006A],0,3);
//    debug_8x8(dat00_low[0x006B],0,4);
//	 debug_8x8(dat00[0x0067],64,3);
//	 debug_8x8(dat00_low[0x0067],64,4);
    UI_Compass_Animation();
	  //debug_8x8(test1++,0,1);
    debug_8x8(dat00[0x0068],64,1);
}









