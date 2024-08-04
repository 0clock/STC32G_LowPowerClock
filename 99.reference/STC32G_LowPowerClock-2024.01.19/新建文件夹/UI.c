#include "UI.h"
#include "OLED.h"
#include "DATA.h"
#include "Main_Menu.h"
#include "TV_Mode.h"
#include "SecondaryMenu.h"
#include "ThreeMenu.h"
#include "CompassMenu.h"
#include "BMC050.h"

extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;
extern u16 Sleep_Queue_task[Sleep_queue_num];

//******************************************************************************
//
//		用于选择UI界面
//
//******************************************************************************
void UI_switch()
{

	if (dat00_flag[0x0010])
	{

		if (dat00[0x0010] == 0x0000 && dat00[0x1A] == 0)
		{
			if (dat00_low[0x0010] == 0x0005)
				Magnetometer_Close(); // 如果从指南针退出，挂起传感器
			OLED_Fill(0x00);		  // 初始清屏
			IU_Main_Menu_Switch();
			UI_main(1);
		}
		else if (dat00[0x0010] >= 0x0001 && dat00[0x1A] == 0 && dat00[0x0010] <= 0x0010 && dat00[0x0010] != 0x0005)
		{
			OLED_Fill(0x00); // 初始清屏
			IU_Main_Menu_Switch();
			UI_SecondaryMenu(1);
		}
		else if (dat00[0x1A] == 1)
		{
			IU_Main_Menu_Switch();
			dat00[0x1B] = dat00_low[GET_SCdat3()];
			UI_ThreeMenu();
		}
		else if (dat00[0x0010] == 0x0020 && dat00[0x1A] == 0)
		{
			OLED_Fill(0x00); // 初始清屏
			IU_Main_Menu_Switch();
			TVMode_int();
		}
		else if (dat00[0x0010] == 0x0021 && dat00[0x1A] == 0)
		{
			OLED_Fill(0x00); // 初始清屏
			IU_Main_Menu_Switch();
			TVMode2_int();
		}
		else if (dat00[0x0010] == 0x0022 && dat00[0x1A] == 0)
		{
			OLED_Fill(0x00); // 初始清屏
			IU_Main_Menu_Switch();
			TVMode3_int();
		}
		else if (dat00[0x1A] == 0 && dat00[0x0010] == 0x0005)
		{
			OLED_Fill(0x00); // 初始清屏
			UI_CompassMenu_int();
		}

		dat00_flag[0x0010] = 0;
	}
}

//******************************************************************************
//
//		UI线程
//
//******************************************************************************

void IU_Thread()
{
	if (Sleep_Queue_task[0] & 0x0001)
	{

		IU_Main_Menu();
		IU_Secondary();
		IU_Three();
		IU_TVMode();
		UI_CompassMenu_time();

		Sleep_Queue_task[0] &= ~0x0001;
	}
}
