#include "UI.h"
#include "OLED.h"
#include "DATA.h"
#include "Main_Menu.h"
#include "TV_Mode.h"
#include "SecondaryMenu.h"
#include "ThreeMenu.h"
#include "CompassMenu.h"
#include "BMC050.h"
#include "SleepMode.h"
#include "FlashLight.h"


extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];


/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;
extern u16 Sleep_Queue_task[Sleep_queue_num];

//========================================================================
// 函数: UI_switch()
// 描述: UI界面切换
// 参数:
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void UI_switch()
{

    if((dat00[0] & 0x0040) ||  (dat00[0x37] == 0))//界面更新 + 休眠运行允许1S任务运行标志位
    {
        if(dat00_flag[0x0010])//界面更新
        {
            
            if(dat00[0x0010] == 0x0000 && dat00[0x1A] == 0)//进入主菜单
            {
                if(dat00_low[0x0010] == 0x0005)//从指南针退出来
								{
								   Magnetometer_Close();//挂起传感器
									 dat00_low[0x0010] = dat00[0x0010];
								}
								else if(dat00_low[0x0010] == 12)//从手电筒退出
								{
									 dat00[0x0051] = dat00_low[0x0051];
									 dat00[0x0052] = dat00_low[0x0052];	
									 dat00_flag[0x0051] = 1;
									 dat00_flag[0x0052] = 1;
									 dat00_low[0x0010] = dat00[0x0010];									
								}
                OLED_Fill(0x00); //初始清屏
                IU_Main_Menu_Switch();
                UI_main(1);
            }
            else if((dat00[0x0010] >= 0x0001 && dat00[0x1A] == 0 && dat00[0x0010] <= 0x0010 && (dat00[0x0010] != 0x0005) && (dat00[0x0010] != 12)))//在主菜单选中非指南针和手电筒
            {
                OLED_Fill(0x00); //初始清屏
                IU_Main_Menu_Switch();
                UI_SecondaryMenu(1);
            }
            else if(dat00[0x1A] == 1)//在三级菜单
            {
                IU_Main_Menu_Switch();
                dat00[0x1B] = dat00_low[GET_SCdat3()];
                UI_ThreeMenu();

            }
            else if(dat00[0x0010] == 0x0020 && dat00[0x1A] == 0)//在TV模式-时间显示
            {
                //dat00[0x002D] = 0;
                OLED_Fill(0x00); //初始清屏
                IU_Main_Menu_Switch();
                TVMode_int();

            }
            else if(dat00[0x0010] == 0x0021 && dat00[0x1A] == 0)//在TV模式-温湿度气压
            {

                OLED_Fill(0x00); //初始清屏
                IU_Main_Menu_Switch();
                TVMode2_int();

            }
            else if(dat00[0x0010] == 0x0022 && dat00[0x1A] == 0)//在TV模式-电量显示
            {
                OLED_Fill(0x00); //初始清屏
                IU_Main_Menu_Switch();
                TVMode3_int();

            }
            else if(dat00[0x1A] == 0 &&  dat00[0x0010] == 0x0005)//进入指南针
            {
                OLED_Fill(0x00); //初始清屏
                UI_CompassMenu_int();
            }
            else if(dat00[0x1A] == 0 &&  dat00[0x0010] == 12)//进入手电筒
            {
                FlashLight_int();	
            }						
						
						
            dat00_flag[0x0010] = 0;
        }
    }

}



//========================================================================
// 函数: IU_Thread()
// 描述: UI局部刷新任务
// 参数:
// 返回:
// 版本: V1.0 2024.02.12
//========================================================================
void IU_Thread()
{
    if((dat00[0] & 0x0040) ||  (dat00[0x37] == 0))
    {
        if(Sleep_Queue_task[0] & 0x0001)
        {
            IU_Main_Menu();
            IU_Secondary();
            IU_TVMode();
            UI_CompassMenu_time();
            UI_SecondaryMenu_G_1();
            Sleep_Queue_task[0] &= ~0x0001;
        }
    }

}






