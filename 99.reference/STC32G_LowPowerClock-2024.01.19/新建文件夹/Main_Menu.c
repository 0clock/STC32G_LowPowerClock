#include "Main_Menu.h"
#include "OLED.h"
#include "DATA.h"

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
//		显示主界面
//
//******************************************************************************
void UI_main(u8 num1)
{
	u8 num = 0;
	u8 yyy = 0;
	yyy = (u8)(49 - dat00[0x12]);
	if (dat00[0x13] == 0x0000)
		yyy = 0;
	if (num1)
	{

		OLED_32X32(0, 0, (u8)((dat00_low[0x14] - 1) % 16), yyy, 32);
		OLED_32X32((u8)(48 - yyy), 0, (u8)(dat00_low[0x14] % 16), 0, 32);
		OLED_32X32((u8)(96 - yyy), 0, (u8)((dat00_low[0x14] + 1) % 16), 0, 32);
		OLED_32X32((u8)(144 - yyy), 0, (u8)((dat00_low[0x14] + 2) % 16), 0, (u8)(yyy - 16));
	}
	else
	{
		OLED_32X32(0, 0, (u8)((dat00_low[0x14] - 2) % 16), (u8)(47 - yyy), 32);
		OLED_32X32(yyy, 0, (u8)((dat00_low[0x14] - 1) % 16), 0, 32);
		OLED_32X32((u8)(48 + yyy), 0, (u8)(dat00_low[0x14] % 16), 0, 32);
		OLED_32X32((u8)(96 + yyy), 0, (u8)((dat00_low[0x14] + 1) % 16), 0, (u8)(32 - yyy));
	}
	if (dat00[0x12] == dat00[0x13])
	{
		num = (u8)((dat00[0x11] % 16) << 2);
		OLED_16X16(32, 5, num);
		OLED_16X16(48, 5, (u8)(num + 1));
		OLED_16X16(64, 5, (u8)(num + 2));
		OLED_16X16(80, 5, (u8)(num + 3));
	}
}

void IU_Main_Menu()
{
	if (dat00[0x10] == 0x0000 && dat00[0x1A] == 0)
	{
		if (dat00[0x14] != dat00_low[0x14] && ((dat00[0x13] & 0x0004) == 0)) // 主菜单界面移动触发
		{
			dat00[0x15] = dat00[0x14];
			dat00[0x13] |= 0x0004;
		}
		if (dat00[0x13] & 0x0004) // 主菜单界面移动触发
		{

			if (dat00[0x12] == 0x0000)
			{
				if ((dat00[0x13] & 0x0001) == 0x0000)
				{
					dat00[0x12] = 48;

					fast_SleepQueue_aheadW1(6, 1, 48);

					dat00[0x13] |= 0x0001;
				}
			}
			// dat00_flag[0x01]	= 0;
			if ((dat00[0x12] != 0x0000) && (dat00[0x13] & 0x0001))
			{
				if (dat00[0x15] > dat00_low[0x14])
				{
					UI_main(1);
					dat00[0x13] |= 0x0002;
				}
				else if (dat00[0x15] < dat00_low[0x14])
				{
					UI_main(0);
					dat00[0x13] &= 0xFFFD;
				}
				dat00[0x12]--;
				if (dat00[0x12] == 0x0000)
				{
					dat00[0x13] &= 0xfffe;
					dat00[0x13] &= 0xfffB;

					SleepQueue_behindW(6, 1);

					if (dat00[0x15] > dat00_low[0x14])
					{
						dat00_low[0x14]++;

						if (dat00_low[0x14] > 0x8010 && dat00[0x14] > 0x8010)
						{
							dat00_low[0x14] = (dat00_low[0x14] % 16) + 0x8000;
							dat00[0x14] = (dat00[0x14] % 16) + 0x8000;
						}
					}
					else if (dat00[0x15] < dat00_low[0x14])
					{
						dat00_low[0x14]--;
						if (dat00_low[0x14] < 0x7FF0 && dat00[0x14] < 0x7FF0)
						{
							dat00_low[0x14] = (dat00_low[0x14] % 16) + 0x8000;
							dat00[0x14] = (dat00[0x14] % 16) + 0x8000;
						}
					}
					if (dat00[0x14] == dat00_low[0x14])
					{
						if (!P45)
						{
							data00_Change(0x14, 1);
						}
						if (!P27)
						{
							data00_Change(0x14, 0);
						}
					}
				}
			}
		}
	}
}

void IU_Main_Menu_Switch()
{
	dat00[0x12] = 0;
	dat00[0x13] = 0;
	//	  Sleep_Queue_count = 0;
	//    Sleep_Queue_count_target = 0;
	dat00_low[0x14] = dat00[0x11] + 0x8000;
	dat00[0x14] = dat00_low[0x14];
	dat00[0x15] = dat00[0x14];
}
