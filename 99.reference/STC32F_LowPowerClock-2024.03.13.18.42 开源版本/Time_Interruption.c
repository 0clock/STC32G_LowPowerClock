#include "Time_Interruption.h"
#include "IIC.h"
#include "SHT41.h"
#include "SPL06.h"
#include "BH1745.h"
#include "time.h"
#include "DATA.h"
#include "OLED.h"
#include "INS5699S.h"
#include "ADC.h"
#include "SecondaryMenu.h"

#include "SleepMode.h"
#include "UART1.h"
#include "logic.h"
#include "TV_Mode.h"



extern u8 xdata DMABuffer[96];
extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];



/*************  本地变量声明  功能寄存器  **************/
extern u8 xdata DMABuffer[96];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];

extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u8 Time_New[7];
extern u8 Time_Low[7];
extern u8 xdata Time_int[7];

extern u32 xdata JCQ06_00[5];

u8 yya=0;

extern u8 New_time_num[7];

extern u16 Sleep36;

//========================================================================
// 函数: void TVMode_2()
// 描述: 秒中断/分中断处理
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void Time_Interruption_1S()
{
    u8 i = 0;
    u8 UI_flag = 0;
    u8 ADC_flag = 0;
    u8 BH_flag = 0;

    if(dat00[0] & 0x0002)
    {
        i = IIC_R1bit(INS5699_W,0x0e);//读标志位寄存器
        if(i & 0x20)
        {
            INS5699_timeR();//读取当前时间
					if((dat00[0] & 0x0040) ||  (dat00[0x37] == 0))//休眠模式唤醒后 或 不为休眠模式
            {
                dat00[0x0000]  |= 0x000C;
							  if(dat00[0x0053]) BH_flag = 1;//自动调光打开，则标志需要测量光强
                if(dat00[0x0010] == 0x0020 || dat00[0x0010] == 0x0021 || dat00[0x0010] == 0x0022 || dat00[0x0010] == 0x0005)//TV模式或者指南针
                {
                    dat00[0x002E]  |= 0x0001;//秒 刷新标志位
									  dat00[0] |= 0x0100;
									  dat00[0x0009]  = Time_comparison();
                    ADC_flag = 1;//申请ADC测量
                }

                //IIC_W2bit(INS5699_W,0x0e,i);
                if((dat00[0x0010] == 3 && Sleep_Queue_task[0] != 1) || dat00[0x0010] == 0x0021)//如果在环境信息内或TV模式电源显示页面时，开启所有传感器的检测
                {
                    SHT41_R_High();
                    dat00[0x0041] = JCQ06_00[0];
                    dat00[0x0042] = JCQ06_00[1];
                    SPL06_PR_R();
                    dat00[0x0043] = JCQ06_00[4]/100;
                    BH_flag = 1;
                    if(dat00[0x0010] == 3)
                    {
                        UI_flag = 1;//更新二级菜单右边UI数据
                    }
                }

                if((dat00[0x0010] == 0x000D && Sleep_Queue_task[0] != 1) || dat00[0x0010] == 0x0022)//如果在电源检测或在TV模式温度湿度气压显示时，开启所有传感器的检测
                {
                    if(dat00[0x0010] == 0x000D)
                    {
                        ADC_flag = 1;
                        UI_flag = 1;//更新二级菜单右边UI数据
                    }
                }
                
                SleepQueue_aheadW1(6,1,0);//申请一个UI刷新任务

            }
            else if((dat00[0x37] == 1) && (dat00[0x36] == 1) && ((dat00[0] & 0x0040) == 0x00) )//如果在休眠屏显
            {
                Sleep_Display_UI();
                //if(dat00[0x0053]) BH_flag = 1;
            }
            if(dat00[0x0010] == 12)	 BH_flag = 0;


            if(BH_flag)
            {
                IIC_W2bit(BH1745_W,0x42,0x12);
                SleepQueue_aheadW1(400,2,0);
            }
            if(UI_flag)
            {
                UI_SecondaryMenu_G();
            }
            if(ADC_flag)
            {
                ADC_Get_V1();
            }
						
						//无动作60秒退回休眠模式或者TV模式
						
            if((dat00[0x0010] == 0x0020 || dat00[0x0010] == 0x0021 || dat00[0x0010] == 0x0022) || ((dat00[0] & 0x0080) && (dat00[0x0036])) || (dat00[0x0006] & 0x0006) )
            {
                dat00[0x0008] = 60;
            }
            else
            {
                dat00[0x0008]--;
                if(dat00[0x0008] <= 2)
                {
									if(dat00[0x0008] == 2)
									{
									   ESC_key();
										 dat00[0x0008] = 2;
									}
									else if(dat00[0x0008] == 1)
									{
									   ESC_key();
										 dat00[0x0008] = 1;
									}
									else if(dat00[0x0008] == 0)
									{
                    if(dat00[0x26])
                    {
                        dat00[0x10] = 0x0020;
                        dat00[0x002D] = 0;
                        dat00[0x37] = 0;
                        dat00[0x26] = 1;
											  dat00[0x1A] = 0;
                        dat00[0] &= ~0x0040;
                        dat00[0] &= ~0x0080;
                        dat00_flag[0x10] = 1;
                    }
										else
										{
												
											dat00_flag[0x37] = 1;
											dat00[0] &= ~0x0040;
										
										}
									}
                }

            }
            i = i & 0xdf;
            IIC_W2bit(INS5699_W,0x0e,i);

        }
        dat00[0] &= ~0x0002;
    }
}








