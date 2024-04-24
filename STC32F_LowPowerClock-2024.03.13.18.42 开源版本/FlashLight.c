#include "FlashLight.h"
#include "OLED.h"
#include "IIC.h"
#include "DATA.h"
#include "IO.h"
#include "INS5699S.h"

extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];

extern u8 Time_New[7];
extern u8 Time_Low[7];





//========================================================================
// 函数: FlashLight_int()
// 描述: 手电筒控制器
// 参数:
// 返回:
// 版本: V1.0 2024.03.08
//========================================================================
void FlashLight_int()
{
   OLED_Fill(0xFF);//屏幕全亮
   dat00_low[0x0051] = dat00[0x0051];//保存进入前的亮度
   dat00_low[0x0052] = dat00[0x0052];//保存进入前的电压档位
   
   dat00[0x0051] = 255;//亮度255
	 dat00_flag[0x0051] = 1;//变更标志位
}



