#include "logic.h"
#include "DATA.h"
#include "OLED.h"
#include "IO.h"
#include "SecondaryMenu.h"


extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];

extern u32 AlarmClock[16];



void jump_MainMenu()     //跳到主菜单
{
    dat00[0x10] = 0;
    dat00_low[0x17] = dat00[0x17];
    dat00[GET_SC() + 0x0f] = dat00[0x17] % GET_SCdat();
    dat00_flag[0x10] = 1;
}

void jump_SecondaryMenu()     //跳到二级菜单
{
				dat00[0x10] = dat00[0x11]+1;
        dat00_flag[0x10] = 1;
        dat00[0x17] = (dat00[GET_SC()] << 1)+ dat00[GET_SC() + 0x0f];
        dat00[0x18] = dat00[0x17];
        dat00_low[0x17] = dat00[0x17];
        if(dat00[0x11] == 0x06) GET_time_c();
}

void test_10()     //跳到二级菜单
{
					  dat00[0x009F] = AlarmClock[0x0f];
					  dat00[0x0090] = 10;
						dat00[0x009E] =	1;	
            				
}




void SET_key()     //主菜单 按下确认
{
	
	
		

	
	
//******************************************************************************
//*
//*			主菜单下，且不在三级菜单，按下确定，进入相应的二级菜单
//*
//******************************************************************************
    if((dat00[0x10] == 0x0000 | (dat00[0x11]==0x0007 && dat00[0x009E]<2)) && dat00[0x1A] == 0 )
    {
			  if(dat00[0x11]==0x0007)  // dat00[0x009E]标志进入诱导闹钟菜单
				{
					 if(dat00[0x009E] == 0)
					 {
					  test_10();						 
					 }
						else
						{
							AlarmClock[0x0f] = dat00[0x009F];
							Alarm_conversion_32TO16((u8)AlarmClock[0x0f]);
							dat00[0x009F] = 0x00;
							dat00[0x009E] =	2;
							dat00[0x0090] = 9;		
						}
				}	
				
				jump_SecondaryMenu();
    }
//******************************************************************************
//*
//*			系统信息界面，按下确认也是返回主菜单
//*
//******************************************************************************
    else if(dat00[0x10] == 0x000F && dat00[0x1A] == 0)
    {
        jump_MainMenu();
    }
//******************************************************************************
//*
//*			在二级菜单（且你在三级菜单）不可修改值按下确定
//*
//******************************************************************************
    else if((dat00_max[GET_SCdat3()] & 0x0080 ) && dat00[0x1A] == 0)
    {
        if(dat00[0x10] == 0x0001 && dat00[0x2F] == 0x0005)//进入标准TV模式
        {
            dat00[0x10] = 0x0020;
            dat00_flag[0x10] = 1;
        }
				 else if(dat00[0x10] == 0x0002 && dat00[0x3F] == 0x0005)//设置时间
        {

            dat00_flag[0x10] = 1;
					  dat00_flag[0x36] = 1;
        }
        else if(dat00[0x10] == 0x0007 && dat00[0x008F] == 0x0000)//设置时间
        {
            jump_MainMenu();
            dat00_flag[0x81] = 1;
        }
    }	
//******************************************************************************
//*
//*			在二级菜单（且你在三级菜单、不是指南针菜单）按下确定，进入三级菜单
//*
//******************************************************************************
    else if(dat00[0x10] >= 0x0001 && dat00[0x10] <= 0x0010 && dat00[0x1A] == 0 && dat00[0x10] != 0x0005)
    {
        dat00[0x1A] = 1;
        dat00_flag[0x10] = 1;
    }
//******************************************************************************
//*
//*			在三级菜单（非指南针菜单）按下确定，回到二级菜单
//*
//******************************************************************************
    else if(dat00[0x10] >= 0x0001 && dat00[0x10] <= 0x0010 && dat00[0x1A] == 1 && dat00[0x10] != 0x0005)
    {
        dat00[0x1A] = 0;
        dat00_flag[0x10] = 1;
        dat00_low[GET_SCdat3()] = dat00[GET_SCdat3()];
    }
//******************************************************************************
//*
//*			在TV模式时，按下确定按键支持快速查询
//*
//******************************************************************************
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
	
	
	
	  if(dat00[0x11]==0x0007 && dat00[0x1A] == 0)
		{
			  if(dat00[0x009E] == 1)  // 退出诱导闹钟菜单，回到主菜单
				{
					  AlarmClock[0x0f] = dat00[0x009F];
						dat00[0x009E] =	0;
						dat00_low[0x0010] = dat00[0x0010];
						jump_MainMenu();					
				}
			  else if(dat00[0x009E] == 2)  // 退出闹钟菜单，回到诱导闹钟菜单
				{
					  Alarm_conversion_16TO32((u8)(AlarmClock[0x0f]));
					  test_10();						 
						jump_SecondaryMenu();				
				}	
		}
    else if((dat00[0x10] >= 0x0001 && dat00[0x10] <= 0x0010 && dat00[0x1A] == 0) | (dat00[0x10] >= 0x0020 && dat00[0x10] <= 0x0022))
    {
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
