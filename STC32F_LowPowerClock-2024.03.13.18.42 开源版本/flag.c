#include "flag.h"
#include "OLED.h"
#include "IO.h"
#include "DATA.h"
#include "UI.h"
#include "time.h"
#include "UART1.h"
#include "INS5699S.h"
#include "SleepMode.h"




extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];

u8 OLED_InverseDisplay = 0;

extern u8 Time_New[7];
extern u8 Time_Low[7];
extern u8 xdata Time_int[7];



//========================================================================
// 函数: void flag_handle()
// 描述: 当检测到数据被修改了，判断是否要作出相应动作
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void flag_handle()
{

    if((dat00[0x0053]) && (dat00_flag[0x14])&& (dat00[0x10] == 0x0000) && (dat00[0x1A] == 0)) SleepQueue_aheadW1(6,1,0);//好像是开了自动调光之后主菜单又是反应慢一点，这里直接在前面插入一个UI任务缓解

    if(dat00_flag[0x14] == 0x01)//主菜单界面移动触发
    {
        dat00[0x11] = dat00[0x14] % 16;
        dat00_flag[0x14]	= 0;
    }
    if(dat00_flag[0x17] == 0x01)//
    {
        dat00[GET_SC() + 0x000f] = dat00[0x17] % GET_SCdat();
        dat00_flag[0x17]	= 0;
    }


    if(dat00[0] & 0x0200)//烧屏保护相关
    {

        if(dat00_flag[0x0021])
        {
            if((dat00[0x0021] == 0x00) && (OLED_InverseDisplay))
            {
                OLED_WrCmd(0xa6);
                OLED_InverseDisplay = 0;
            }
            dat00_flag[0x0021]	= 0;
        }

        if(dat00_flag[0x37] == 0x01)//休眠
        {
            OLED_Fill(0x00); //初始清屏
            Sleep_Display_int();
            SleepMode_ON();
            dat00_flag[0x37] = 0;
        }
        dat00[0] &= ~0x0200;
    }

    if(dat00_flag[0x0051])//调节屏幕亮度
    {
        OLED_WrCmd(0x81);//--set contrast control register	 设置对比度控制寄存器
        OLED_WrCmd((u8)dat00[0x0051]); // Set SEG Output Current Brightness	设置SEG输出电流亮度
        dat00_flag[0x0051] = 0;
    }
    if(dat00_flag[0x0052])//调节电压
    {

        OLED_POW((u8)dat00[0x0052]);
        dat00_flag[0x0052] = 0;
    }
}



