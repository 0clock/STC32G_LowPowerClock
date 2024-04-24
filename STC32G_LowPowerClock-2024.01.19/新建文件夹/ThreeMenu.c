#include "ThreeMenu.h"
#include "OLED.h"
#include "DATA.h"
#include "SecondaryMenu.h"


extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;
extern u16 Sleep_Queue_task[Sleep_queue_num];


u16 jy;
u16 xxx;

void UI_ThreeMenu()
{
    OLED_Region_Fill();
    OLED_Progress_Bar(dat00[GET_SCdat3()],data_GetMax(GET_SCdat3()));
    OLED_16_16V(TVR((u8)dat00[0x11]),UI_S_F(0),32,2,1);
    OLED_16_16V(TVR((u8)dat00[0x11]),UI_S_F(0)+1,32,3,1);
}


void IU_Three()
{
    if(dat00[0x1A] == 1)
    {
        if(P45 && P27)
        {
            jy=0;
        }
        if(!P45)
        {
            data00_Change(GET_SCdat3(),1);
					  if(jy < 250)	jy = jy + 10;
        }
        if(!P27)
        {
            data00_Change(GET_SCdat3(),0);
					  if(jy < 250)	jy = jy + 10;
        }
        if(dat00[GET_SCdat3()] != dat00[0x1b] )
        {
            OLED_Progress_Bar(dat00[GET_SCdat3()],data_GetMax(GET_SCdat3()));
						//if(Sleep_Queue_task[0] != 0x0001) nian(Sleep_Queue_task[0],6);
					  //SleepQueue_behindW(350 - jy,1);
					  SleepQueue_aheadW1(350 - jy,1,0);
            dat00[0x1b] = dat00[GET_SCdat3()];
        }
        
				
    }
}


