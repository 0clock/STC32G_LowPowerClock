#include "SQW.h"
#include "time.h"
#include "IIC.h"
#include "UART1.h"
#include "SHT41.h"
#include "SPL06.h"
#include "IO.h"
#include "BH1745.h"
#include "OLED.h"
#include "UI.h"
#include "DATA.h"
#include "logic.h"
#include "SecondaryMenu.h"
#include "ThreeMenu.h"
#include "flag.h"
#include "TV_Mode.h"
#include "INS5699S.h"
#include "ADC.h"


extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];

extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u32 xdata JCQ06_00[10];




void SQW_flag()
{
				u8 i = 0;
				 if(dat00[0] & 0x0002)
        {
            i = IIC_R1bit(0x64,0x0e);
            if(i & 0x20)
            {
                INS5699_timeR();
                if(dat00[0x0010] == 0x0020 | dat00[0x0010] == 0x0021)
                {
                    dat00[0x002E]  |= 0x0001;//秒 刷新标志位
                    dat00[0x0004]  |= 0x0001;
                }
            }
            i = i & 0xdf;
            IIC_W2bit(0x64,0x0e,i);

            if((dat00[0x0010] == 3 && Sleep_Queue_task[0] != 1) | dat00[0x0010] == 0x0021)//
            {
                SHT41_R_High();
                dat00[0x0041] = JCQ06_00[0];
                dat00[0x0042] = JCQ06_00[1];
                SPL06_PR_R();
                dat00[0x0043] = JCQ06_00[4]/100;
                IIC_W2bit(BH1745_W,0x42,0x10);
                SleepQueue_aheadW1(380,2,0);

                if(dat00[0x0010] == 3)
                {
                    UI_SecondaryMenu_G();
                }
            }
            if((dat00[0x0010] == 0x000D && Sleep_Queue_task[0] != 1) | dat00[0x0010] == 0x0022)
            {

                ADC_Get_V1();
                UI_SecondaryMenu_G();
            }
            SleepQueue_aheadW1(6,1,0);
            dat00[0] &= ~0x0002;
            //aaa();
        }
}



