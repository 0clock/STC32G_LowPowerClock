#include "CompassMenu.h"
#include "OLED.h"
#include "DATA.h"
#include "BMC050.h"
#include "TV_Mode.h"

extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u8  UI_dat[8][16];
extern u8 Time_New[7];
extern u8 Time_Low[7];


u8 Compass_Scale[5]= {0,0,0,0,0};

u16 Compass_data[5]= {0,0,0,0,0};

u16 start_X_low = 0;

u8 test1=0;




//========================================================================
// 函数: void UI_Compass_Animation_1(u8 a,bit clear)
// 描述: 有点忘了，大概是将90/180/270/0(360)度在屏幕上的位置存到数组里，然后把标尺和字符打到屏幕上
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_Compass_Animation_1(u8 a,bit clear)
{
    if(Compass_Scale[a] >127) return;
    OLED_Set_Pos(Compass_Scale[a],5);
    if(clear)
    {
        OLED_WrDat(0x18);
        if((Compass_data[a] % 90) == 0)
        {
            OLED_16X16_MOV((u8)(Compass_Scale[a]-8),3,4,0,(u8)(135-Compass_Scale[a]));
        }
        else
        {
            OLED_8X8X3_MOV((u8)(Compass_Scale[a]-12),4,0xFFFF,0,(u8)(139-Compass_Scale[a]));
        }
    }
    else
    {
        if((Compass_data[a] % 90) == 0)
        {
            OLED_WrDat(0xFF);
            OLED_16X16_MOV((u8)(Compass_Scale[a]-8),3,(u8)((Compass_data[a]%360)/90),0,(u8)(135-Compass_Scale[a]));//  135是127+8（127是屏幕宽度，8是一个汉字的一半）
        }
        else
        {
            OLED_WrDat(0x3C);
            OLED_8X8X3_MOV((u8)(Compass_Scale[a]-12),4,Compass_data[a],0,(u8)(139-Compass_Scale[a]));//  139是127+12（127是屏幕宽度，12是三个8*8数字的一半）
        }
    }
}











//========================================================================
// 函数: u16 Time_comparison_4()
// 描述: 返回时间更新标志位
// 参数: 
// 返回: j时间更新标志位
// 版本: V1.0 2023.02.05
//========================================================================
u16 Time_comparison_4()
{
    u16 j = 0;

    if(Time_New[0] != Time_Low[0])
    {
        if(Time_New[0]/16 != Time_Low[0]/16)  j |= 0x0002;
        if(Time_New[0]%16 != Time_Low[0]%16)  j |= 0x0001;
    }

    if(Time_New[1] != Time_Low[1])
    {
        if(Time_New[1]/16 != Time_Low[1]/16)  j |= 0x0008;
        if(Time_New[1]%16 != Time_Low[1]%16)  j |= 0x0004;
    }
		if(Time_New[2] != Time_Low[2])
    {
        if(Time_New[2]/16 != Time_Low[2]/16)  j |= 0x0020;
        if(Time_New[2]%16 != Time_Low[2]%16)  j |= 0x0010;
    }
		return j;
}

//========================================================================
// 函数: void UI_8x8time(u16 dat)
// 描述: 显示8*8大小的时钟
// 参数: dat更新标志位
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_8x8time(u16 dat)
{

	  if(dat == 0) return;
		if(dat & 0x0001)//秒刷新
    {
        ZF_8X8(48,7,(u8)(Time_New[0]%16));
				if((dat00[0x0069]) && (dat00[0x006A]))
				{
					dat00[0x006A]++;		
				}
    }
    if(dat & 0x0002)
    {
        ZF_8X8(40,7,(u8)(Time_New[0]/16));
    }
    if(dat & 0x0004)
    {
        ZF_8X8(28,7,(u8)(Time_New[1]%16));
    }
    if(dat & 0x0008)
    {
        ZF_8X8(20,7,(u8)(Time_New[1]/16));
    }
    if(dat & 0x0010)
    {
        ZF_8X8(8,7,(u8)(Time_New[2]%16));
    }
    if(dat & 0x0020)
    {
        ZF_8X8(0,7,(u8)(Time_New[2]/16));
    }	
}



//========================================================================
// 函数: void UI_CompassMenu_int()
// 描述: 指南针UI初始化
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_CompassMenu_int()
{
    u8 i = 0;
    Magnetometer_OPEN();//传感器打开
    OLED_Set_Pos(0,5);
    for(i=0; i<128; i++)//画一条线
    {
        OLED_WrDat(0x18);
    }
    OLED_16X16(56,6,65);//画箭头
		IU_Battery_Icon_S(80,7,1);//电池显示
		ZF_8X8(34,7,25);//显示时间之间的“-”
		ZF_8X8(14,7,25);
		ZF_8X16(76,0,30);	 //显示“°”
		UI_8x8time(0x003f);//显示时间

    SleepQueue_aheadW1(5,0x0080,0);//申请指南针测量任务



}






//========================================================================
// 函数: void UI_Compass_Animation()
// 描述: 指南针UI进程
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_Compass_Animation()//FF  3C
{
    u8 i = 0;
    u16 start_X = 0;
    u16 b = 0;
    u16 c = 0;

    u8 a1= 0;
    u8 b1= 0;
    u8 c1= 0;
    u8 d1= 0;

	  UI_8x8time(dat00[0x0009]);//时间更新

	
	   //第一次进指南针会显示转三圈校准，清除校准数据之后会显示 数据已经清除啥的 + 转三圈校准
    if((dat00[0x0069] == 0) && (dat00[0x006A] == 0))
		{
			OLED_16_16Vabc(TVR(0x0004),0,0,0,0,1,8);
			OLED_16_16Vabc(TVR(0x0004),8,64,0,0,1,8);
			dat00[0x0069] = 1;
			dat00[0x006A] = 1;
		}
		else if((dat00[0x0069] == 1) && (dat00[0x006A] >= 6))
		{
			OLED_16_16Vabc(TVR(0x0004),16,0,0,0,1,8);
			OLED_16_16Vabc(TVR(0x0004),24,64,0,0,1,8);
			dat00[0x0069] = 2;
			dat00[0x006A] = 1;			
		}	
		else if((dat00[0x0069] == 2) && (dat00[0x006A] >= 6))
		{
			OLED_Mean2_fill(8,0);
			OLED_16_16Vabc(UI_dat,0,0,0,0,1,8);
			OLED_16_16Vabc(UI_dat,0,64,0,0,1,8);	
		  ZF_8X16(76,0,30);				
			dat00[0x0069] = 0;			
		}			
	
		//角度更新了才刷新UI
    if(dat00_low[0x0068] != dat00[0x0068])
    {
        for(i=0; i<5; i++)//128个像素内最多存在5个标尺
        {
            UI_Compass_Animation_1(i,1);
        }

        if(dat00[0x0068] < 64)
        {
            start_X = dat00[0x0068] + 296;
        }
        else
        {
            start_X = dat00[0x0068] - 64;
        }
        i = start_X/30;
        i = i+1;
        b = i*30;
        for(i=0; i<5; i++)
        {
            Compass_Scale[i] = b- start_X;
            Compass_data[i] = (b % 360);
            b += 30;
        }
        for(i=0; i<5; i++)//128个像素内最多存在5个标尺
        {
            UI_Compass_Animation_1(i,0);
        }
        dat00_low[0x0068] = dat00[0x0068];
    }
		
		if(dat00[0x0069] == 0)  OLED_3Figures(dat00[0x0068],52,0);
		
		
		time_synchronization();
}








void UI_CompassMenu_time()
{
    if(dat00[0x10] == 0x0005)
    {
//	  IU_Battery_Icon_S(80,0);
//    time_show_s();
    }
}


//========================================================================
// 函数: void UI_CompassMenu()
// 描述: 指南针UI任务
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_CompassMenu()
{
    UI_Compass_Animation();//UI刷新
	  IU_Battery_Icon_S(80,7,0);//电池UI更新
	
	
    //debug_8x8(dat00[0x0068],64,1);
}









