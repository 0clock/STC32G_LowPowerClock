#include "ThreeMenu.h"
#include "OLED.h"
#include "DATA.h"
#include "SecondaryMenu.h"


extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;
extern u16 Sleep_Queue_task[Sleep_queue_num];

u16 jy=0;
u16 xxx;

//========================================================================
// 函数: UI_ThreeMenu()
// 描述: 三级菜单初始化
// 参数:
// 返回:
// 版本: V1.0 2024.02.12
//========================================================================
void UI_ThreeMenu()
{
    OLED_Region_Fill();//画一个带边长方形
    OLED_Progress_Bar(dat00[GET_SCdat3()],data_GetMax(GET_SCdat3()));//画进度条 + 显示进度条的数值
	OLED_16_16V(TVR((u8)dat00[0x11]),UI_S_F(0),32,2,1);								//显示选项的文本上半
    OLED_16_16V(TVR((u8)dat00[0x11]),UI_S_F(0)+1,32,3,1);						//显示选项的文本下半

}




//========================================================================
// 函数: IU_Three()
// 描述: 三级菜单更新UI处理
// 参数:
// 返回:  更新dat00[0x1b]（三级菜单设置的旧值暂存寄存器）
// 版本: V1.0 2024.02.12
//========================================================================
void IU_Three()
{
    if(dat00[0x1A] == 1)
    {
        if(dat00[GET_SCdat3()] != dat00[0x1b] )
        {
            OLED_Progress_Bar(dat00[GET_SCdat3()],data_GetMax(GET_SCdat3()));//画进度条 + 显示进度条的数值
            dat00[0x1b] = dat00[GET_SCdat3()];
        }
    }
}



//========================================================================
// 函数: key_falg_0100()
// 描述: 三级菜单加减任务，独立出一个任务，避免按一次加两次
// 参数:
// 返回:  
// 版本: V1.0 2024.02.12
//========================================================================
void key_falg_0100()
{
    u8 a=0;
    if(Sleep_Queue_task[0] & 0x0100)
    {
        if(dat00[0x1A] == 1)
        {
            if(P45 && P27)
            {
                jy=0;
            }
            if(!P45)//长按加
            {
                data00_Change(GET_SCdat3(),1);
                if(jy < 250)	jy = jy + 10;
							  dat00[0x0008] = 60;
            }
            if(!P27)//长按减
            {
                data00_Change(GET_SCdat3(),0);
                if(jy < 250)	jy = jy + 10;
							  dat00[0x0008] = 60;
            }
            if(dat00[GET_SCdat3()] != dat00[0x1b] )
            {
                a =1;
                //dat00[0x1b] = dat00[GET_SCdat3()];
            }
        }
				
				IU_Three();
        Sleep_Queue_task[0] &=~0x0100;
        if(a) SleepQueue_aheadW1(350 - jy,0x0101,0);
    }
}