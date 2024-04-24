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

extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];



/*************  本地变量声明  功能寄存器  **************/
extern u8 xdata DMABuffer[96];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];

extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u8 xdata Time_New[7];
extern u8 xdata Time_Low[7];
extern u8 xdata Time_int[7];

extern u32 xdata JCQ06_00[5];

u8 yya=0;

void Time_Interruption_1S()
{
	  u8 i = 0;
	 
//	  if(!P26)
//		{
//		 _nop_();
//		 IAP_CONTR |=0x60;		
//		 _nop_();
//		}
    //debug_8x8(WDT_CONTR,64,1);
	
		if(dat00[0] & 0x0002)
		{
				i = IIC_R1bit(INS5699_W,0x0e);//读标志位寄存器 
			  
			//debug_8x8(i,64,1);
			
				if(i & 0x20)
				{
						dat00[0x0004]  |= 0x0001;
						dat00[0x0005]  |= 0x0001;
						INS5699_timeR();
						if(dat00[0x0010] == 0x0020 | dat00[0x0010] == 0x0021 | dat00[0x0010] == 0x0022 | dat00[0x0010] == 0x0005)
						{
								dat00[0x002E]  |= 0x0001;//秒 刷新标志位
								ADC_Get_V1();
						 }
				i = i & 0xdf;
				IIC_W2bit(INS5699_W,0x0e,i);
				if((dat00[0x0010] == 3 && Sleep_Queue_task[0] != 1) | dat00[0x0010] == 0x0021)//
				{
						SHT41_R_High();
						dat00[0x0041] = JCQ06_00[0];
						dat00[0x0042] = JCQ06_00[1];
						SPL06_PR_R();
						dat00[0x0043] = JCQ06_00[4]/100;
						IIC_W2bit(BH1745_W,0x42,0x12);
						SleepQueue_aheadW1(400,2,0);

						if(dat00[0x0010] == 3)
						{
								UI_SecondaryMenu_G();
						}
				}

				if((dat00[0x0010] == 0x000D && Sleep_Queue_task[0] != 1) | dat00[0x0010] == 0x0022)
				{
						if(dat00[0x0010] == 0x000D)
						{
								ADC_Get_V1();
								UI_SecondaryMenu_G();
						}
				}
				SleepQueue_aheadW1(6,1,0);
				dat00[0] &= ~0x0002;
		}
				
		if(i & 0x10)
		{
			  _nop_();
//			  yya++;

			  i = i & 0xEF;
				
////			  IIC_W2bit(0x64,0x0F,0x71);//配置开启1S定时 0x60
//			  _nop_();
//			  IIC_W2bit(0x64,0x0B,0x0B+yya);//配置开启1S定时 0x60
////			  _nop_();
//		    IIC_W2bit(0x64,0x0C,0x00);//配置开启1S定时 0x60
////				_nop_();

			  IIC_W2bit(INS5699_W,0x0e,i);
		}
		
		
	}
}			
			
			
			
			
			
			
			
			
			