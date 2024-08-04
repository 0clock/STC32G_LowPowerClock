#include "flag.h"
#include "OLED.h"
#include "IO.h"

extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];



void flag_handle()
{
   if(dat00_flag[0x0051])
	 {
			OLED_WrCmd(0x81);//--set contrast control register	 设置对比度控制寄存器
      OLED_WrCmd((u8)dat00[0x0051]); // Set SEG Output Current Brightness	设置SEG输出电流亮度
	    dat00_flag[0x0051] = 0;
	 }
   if(dat00_flag[0x0052])
	 {

		  OLED_POW((u8)dat00[0x0052]);
	    dat00_flag[0x0052] = 0;
	 }	
}



