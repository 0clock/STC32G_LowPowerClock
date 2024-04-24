#include "SecondaryMenu.h"
#include "OLED.h"
#include "DATA.h"

extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u8  UI_dat[8][16];


u8 p=0;

u16 UI_S_F(char a)
{
	  u16 dat = 0;
		dat = ((dat00_low[0x17] + a) % GET_SCdat()) * 8;
	  return dat;
}



u16 UI_S_F2(char a)
{
	  u16 dat = 0;
		dat = ((dat00_low[0x17] + a) % GET_SCdat());
	  dat = dat + 1 + GET_SC();
	  return dat;
}


void UIData_fill(u16 a)
{
	if(a == (GET_SC() + GET_SCdat()) && (dat00[0x11] == 0 | dat00[0x11] == 6))
	{
		OLED_Mean2_fill(8,0);//不显示数值，比如说设置
	}
	else if((dat00_max[a] % 0x0080) == 1)
	{
	    OLED_Mean2_fill2((u8)dat00[a]);//填充开关符号
	}
	else 
	{
	    OLED_Mean2_fill_num2(a);//填充数值
	}
	
}







void UI_SeMenu_Cursor_num(u8 yy1,u8 yy2,char num,u8 a)
{
	
	    if(dat00[0x11] == 0 | dat00[0x11] == 3 | dat00[0x11] == 2 | dat00[0x11] == 0x0A | dat00[0x11] == 0x0C | dat00[0x11] == 0x06)//设置哪个可以进入二级菜单
			{
				UIData_fill(UI_S_F2(num));
        OLED_16_16Vabc(UI_dat,0,64,yy1,yy2,a,8);//填充数字
			}
	    else if(dat00[0x11] == 14)
			{
				OLED_16_16Vabc(TVR(0x0040),UI_S_F(num),64,yy1,yy2,a,8);	//填充信息图案
			}
}



//****************************系统信息************************************





//************************************************************************
void UI_SecondaryMenu_G()
{

	      u8 yy = 0;
				UI_SeMenu_Cursor_num(0,yy,-1,0);
			  yy =  24;
			  UI_SeMenu_Cursor_num(yy,yy,0,1);
			  yy = 48 ;
        UI_SeMenu_Cursor_num(yy,yy,1,1);		
}






void UI_SecondaryMenu(u8 a)
{
	u8 dat1 = 0;
	u8 dat2 = 0;
	u8 yy = 0;
	

		if(a)
		{		
			if(dat00[0x12] > 8 | dat00[0x13] == 0)
			{
				yy = 24 - (dat00[0x12] - 1) ;
        if(dat00[0x13] == 0) yy = 0;				
			  OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(-1),0,0,yy,0,8);
				UI_SeMenu_Cursor_num(0,yy,-1,0);
        				
			}
				
			yy =  (dat00[0x12] - 1) + dat2;
			if(dat00[0x13] == 0) yy = 24;	
			OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(0),0,yy,yy,1,8);
			
			UI_SeMenu_Cursor_num(yy,yy,0,1);
			

			yy = 24 + (dat00[0x12] - 1) + dat2;	
      if(dat00[0x13] == 0) yy = 48;				
			OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(1),0,yy,yy,1,8);	
			
      UI_SeMenu_Cursor_num(yy,yy,1,1);	

			
			if(dat00[0x13] != 0)
			{	
				yy = 48 + (dat00[0x12] - 1);		 
				OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(2),0,yy,yy,1,8);	
        UI_SeMenu_Cursor_num(yy,yy,2,1);			
			}

	
		}
		else
		{
			if(dat00[0x12] <16)
			{
				yy =(dat00[0x12] - 1);
			  OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(-2),0,0,yy,0,8);	
				
        UI_SeMenu_Cursor_num(0,yy,-2,0);				
			}
	
				yy =  24 - (dat00[0x12] - 1);				
			  OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(-1),0,yy,yy,1,8);
			
        UI_SeMenu_Cursor_num(yy,yy,-1,1);
			
			
			
				yy = 48 - (dat00[0x12] - 1);		
			  OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(0),0,yy,yy,1,8);
			
        UI_SeMenu_Cursor_num(yy,yy,0,1);

				
				yy = 72 - (dat00[0x12] - 1);			
			  OLED_16_16Vabc(TVR((u8)dat00[0x11]),UI_S_F(1),0,yy,yy,1,8);
			
        UI_SeMenu_Cursor_num(yy,yy,1,1);
		
		}
		if(dat00[0x11] != 0x000E)
		{
		
			if((dat00[0x13] == 0 | dat00[0x12] == 1) && (P45 && P27))
			{
				dat1  = ((dat00[0x17] ) % GET_SCdat()) * 8;
			  OLED_16_16V2(TVR((u8)dat00[0x11]),dat1,0,3,0);
				
			}	
		}
		
}



void IU_Secondary()
{
	if(dat00[0x10] > 0 && dat00[0x10] < 16 && dat00[0x1A] == 0)
	{
		if(dat00[0x17] != dat00_low[0x17] && ((dat00[0x13] & 0x0004) == 0))//主菜单界面移动触发
    {
			dat00[0x18]	= dat00[0x17];
			dat00[0x13] |= 0x0004;
		}
		if(dat00[0x13] & 0x0004)//主菜单界面移动触发
    {
				if(dat00[0x12] == 0x0000)
        {
            if((dat00[0x13] & 0x0001)== 0x0000)
            {
							  dat00[0x12] = 24;					  
								fast_SleepQueue_aheadW1(15,1,24);
							  dat00[0x13] |= 0x0001;
            }		
        }
				if((dat00[0x12] != 0x0000) && (dat00[0x13] & 0x0001))
				{
					
					  if(dat00[0x18] > dat00_low[0x17])
						{
						   UI_SecondaryMenu(1);
							 dat00[0x13] |= 0x0002;
						}
					  else if(dat00[0x18] < dat00_low[0x17])
						{
						  UI_SecondaryMenu(0);
							dat00[0x13] &= 0xFFFD;
							 
						}	
				    dat00[0x12]--;
					  if(dat00[0x12] == 0x0000)
						{
							  dat00[0x13] &= 0xfffe;
							  dat00[0x13] &= 0xfffB;
								SleepQueue_behindW(15,1);
							
							dat00[GET_SC() + 0x0f] = dat00[0x17] % GET_SCdat();
							if(dat00[0x18] > dat00_low[0x17])
							{
								dat00_low[0x17]++;

								if((dat00_low[0x17] > (dat00[GET_SC()] << 2)) && (dat00[0x17] > (dat00[GET_SC()] << 2)))	
								{
									dat00_low[0x17]=(dat00_low[0x17] % dat00[GET_SC()]) + (dat00[GET_SC()] << 1);
									dat00[0x17]=(dat00[0x17] % dat00[GET_SC()]) + (dat00[GET_SC()] << 1);
								}							
							}
							else if(dat00[0x18] < dat00_low[0x17])
							{
								dat00_low[0x17]--;
								if(dat00_low[0x17] < dat00[GET_SC()] && dat00[0x17] < dat00[GET_SC()])	
								{
									dat00_low[0x17]=(dat00_low[0x17] % dat00[GET_SC()]) + (dat00[GET_SC()] << 1);
									dat00[0x17]=(dat00[0x17] % dat00[GET_SC()]) + (dat00[GET_SC()] << 1);
								}	
							}
							if(dat00[0x17] == dat00_low[0x17])
							{
									if(!P45)
									{
										data00_Change(0x17,1);
									}
									if(!P27)
									{
										data00_Change(0x17,0);
									}  
							}		
						}
						
				}
			
			

		}
		
		
	}
	
}