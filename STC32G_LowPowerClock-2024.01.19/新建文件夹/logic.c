#include "logic.h"
#include "DATA.h"
#include "OLED.h"
#include "IO.h"
#include "SecondaryMenu.h"


extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];






u8 aa=0;


void jump_MainMenu()     //跳到主菜单
{
    dat00[0x10] = 0;
    dat00_low[0x17] = dat00[0x17];
    dat00[GET_SC() + 0x0f] = dat00[0x17] % GET_SCdat();
    dat00_flag[0x10] = 1;
}

void SET_key()     //主菜单 按下确认
{
    if(dat00[0x10] == 0x0000 && dat00[0x1A] == 0)
    {
        dat00[0x10] = dat00[0x11]+1;
        dat00_flag[0x10] = 1;
        dat00[0x17] = (dat00[GET_SC()] << 1)+ dat00[GET_SC() + 0x0f];
        dat00[0x18] = dat00[0x17];
        dat00_low[0x17] = dat00[0x17];
        if(dat00[0x11] == 0x06) GET_time_c();
    }
    else if(dat00[0x10] == 0x000F && dat00[0x1A] == 0)
    {
        jump_MainMenu();
    }
    else if((dat00_max[GET_SCdat3()] & 0x0080 ) && dat00[0x1A] == 0)
    {
        if(dat00[0x10] == 0x0001 && dat00[0x2F] == 0x0005)//进入标准TV模式
        {
            dat00[0x10] = 0x0020;
            dat00_flag[0x10] = 1;
        }
        else if(dat00[0x11] == 0x0006 && dat00[0x8F] == 0x0007)//设置时间
        {
            jump_MainMenu();
            dat00[0x0006] |=0x0001;
        }
    }
    else if(dat00[0x10] >= 0x0001 && dat00[0x10] <= 0x0010 && dat00[0x1A] == 0 && dat00[0x10] != 0x0005)
    {
        dat00[0x1A] = 1;
        dat00_flag[0x10] = 1;
    }
    else if(dat00[0x10] >= 0x0001 && dat00[0x10] <= 0x0010 && dat00[0x1A] == 1 && dat00[0x10] != 0x0005)
    {
        dat00[0x1A] = 0;
        dat00_flag[0x10] = 1;
        dat00_low[GET_SCdat3()] = dat00[GET_SCdat3()];
    }
		else if(dat00[0x0010] == 0x0020 | dat00[0x0010] == 0x0021 | dat00[0x0010] == 0x0022)
    {
				dat00[0x10]=0x0022;
				dat00_flag[0x10]=1;
    }	
    else
    {
        jump_MainMenu();
    }


}



void ESC_key()     //二级菜单 按下退出
{
    if((dat00[0x10] >= 0x0001 && dat00[0x10] <= 0x0010 && dat00[0x1A] == 0) | (dat00[0x10] >= 0x0020 && dat00[0x10] <= 0x0022))
    {
			
			  //if(dat00[0x0010] == 0x0005) Magnetometer_Close();
			  dat00_low[0x0010] = dat00[0x0010];
        jump_MainMenu();
			  
    }
    else if(dat00[0x10] >= 0x0001 && dat00[0x10] <= 0x0010 && dat00[0x1A] == 1)
    {
        dat00[0x1A] = 0;
        dat00_flag[0x10] = 1;
        dat00[GET_SCdat3()] = dat00_low[GET_SCdat3()];
        dat00_flag[GET_SCdat3()] = 1;
    }

    else
    {
        jump_MainMenu();

    }

}


void Add_key(u8 a)   //主菜单 按下+
{





    if(dat00[0x10] == 0x0005)
    {

        dat00[0x0064] = 0;
        dat00_low[0x0064] = 0;
        dat00[0x0065] = 0;
        dat00_low[0x0065] = 0;
        dat00[0x0066] = 0;
        dat00_low[0x0066] = 0;
        dat00[0x0067] = 0;
        dat00_low[0x0067] = 0;
    }	
    else if(dat00[0x10] == 0x0000 && dat00[0x1A] == 0)
    {
        data00_Change(0x14,a);
    }
    else if(dat00[0x10] > 0 && dat00[0x10] < 16 && dat00[0x1A] == 0)
    {
        data00_Change(0x17,a);
    }
    else if(dat00[0x1A] == 1)
    {
        //data00_Change((u8)GET_SCdat3(),a);
    }
		
		
		if(dat00[0x0010] == 0x0020 | dat00[0x0010] == 0x0021 | dat00[0x0010] == 0x0022)
    {
			if(a)
			{	
				dat00[0x10]=0x0021;
				dat00_flag[0x10]=1;
			}
			else
			{
				dat00[0x10]=0x0020;
				dat00_flag[0x10]=1;					
			}
    }	
		
		
		
		
		
		
}


void key_thread1_new()
{
    if(dat00[0] & 0x0001)
    {

        SleepQueue_aheadW1(6,1,0);


        if(dat00[0xC3] == 1)
        {
            if(dat00[0xC1] == 1)
            {
                Buzzer_Open();
                SleepQueue_aheadW1(dat00[0xC2],0x0020,0);
            }
        }



        dat00[0] &= ~0x0001;
    }
}
