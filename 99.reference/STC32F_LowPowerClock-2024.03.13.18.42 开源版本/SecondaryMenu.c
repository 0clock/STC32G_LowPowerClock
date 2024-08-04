#include "SecondaryMenu.h"
#include "OLED.h"
#include "DATA.h"

extern u16 dat00[System_data_num];
extern u16 dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];
extern u16 xdata dat00_max[System_data_num];

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];
extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;

extern u8  UI_dat[8][16];


u8 p=0;



//========================================================================
// 函数: u16 UI_S_F(char a)
// 描述: 返回光标选中的 菜单选项字符 在点阵数组中的位置
// 参数: a ：菜单光标增加或者减少的数量
// 返回: dat
// 版本: V1.0 2023.02.05
//========================================================================
u16 UI_S_F(char a)
{
    u16 dat = 0;
    dat = ((dat00_low[0x17] + a) % GET_SCdat()) * 8;
    return dat;
}

//========================================================================
// 函数: UI_S_F2(char a)
// 描述: 获取当前光标指向dat00数组中的值的数组标号
// 参数: a ：菜单光标增加或者减少的数量
// 返回: dat
// 版本: V1.0 2023.02.05
//========================================================================
u16 UI_S_F2(char a)
{
    u16 dat = 0;
    dat = ((dat00_low[0x17] + a) % GET_SCdat());
    dat = dat + 1 + GET_SC();
    return dat;
}

//========================================================================
// 函数: void UIData_fill(u16 a)
// 描述: 二级菜单选项 数值的装填
// 参数: a:光标指向哪个值 b：闹钟相关，开源代码中已移除
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UIData_fill(u16 a)
{
    if(dat00_max[a] & 0x0040)
    {
        OLED_Mean2_fill(8,0);//不显示数值，比如说设置
    }
    else if((dat00_max[a] & 0x003F) == 1)
    {
        OLED_Mean2_fill2((u8)dat00[a]);//填充开关符号
    }
    else
    {
        OLED_Mean2_fill_num2(a);//填充数值
    }



}

//========================================================================
// 函数: void UI_SeMenu_Cursor_num(char num,u8 x,u8 yy1,u8 yy2,u8 a)
// 描述: 二级菜单选项 填充/中文显示或者数值显示
// 参数: num:光标偏移  x：x坐标
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_SeMenu_Cursor_num(char num,u8 x,u8 yy1,u8 yy2,u8 a)
{
    u8 address = 0;
    u16 address2 = 0;
    u16 address3 = 0;
    address3 = UI_S_F(num);
    if(x==0)//显示左边的中文选项
    {
        if(dat00[0x10] == 0x0008 && dat00[0x009E] == 1)//闹钟相关
        {
            UIData_fill(address3/8);
            address = 0x60;
            address2 = 0;
        }
        else
        {
            address = dat00[0x11];
            address2 = address3;
        }
    }
    else
    {
        if(dat00[0x10] == 1 || dat00[0x10] == 2 || dat00[0x10] == 4 || dat00[0x10] == 3 || dat00[0x10] == 0x0B || dat00[0x10] == 0x0D || dat00[0x10] == 0x07)//设置哪个可以进入二级菜单
        {
            UIData_fill(UI_S_F2(num));
            address2 = 0;
            address = 0x60;
        }
        else if(dat00[0x10] == 15)
        {
            address = 0x40;
            address2 = address3;
        }
    }
    OLED_16_16Vabc(TVR(address),address2,x,yy1,yy2,a,8);	//填充信息图案

}


//========================================================================
// 函数: void UI_SeMenu_Cursor_num_2(char num,u8 yy1,u8 yy2,u8 a)
// 描述: 二级菜单显示一行
// 参数: num：光标偏移 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_SeMenu_Cursor_num_2(char num,u8 yy1,u8 yy2,u8 a)
{
    UI_SeMenu_Cursor_num(num,0,yy1,yy2,a);//左边一半
    UI_SeMenu_Cursor_num(num,64,yy1,yy2,a);//右边一半
}




//========================================================================
// 函数: void UI_SecondaryMenu_G_1()
// 描述: 开关型变量变化刷新右边的值
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_SecondaryMenu_G_1()
{
    if(dat00[0]	& 0x0400)
    {
        UI_SeMenu_Cursor_num(0,64,24,24,1);
        if(dat00[0x10] == 0x0002)
        {
            if(dat00[0x3F] == 0x0004)
            {
                dat00[0x0036] ^= 0x0001;
                UI_SeMenu_Cursor_num(1,64,48,48,1);
            }
            else if(dat00[0x3F] == 0x0005)
            {
                dat00[0x0035] ^= 0x0001;
                UI_SeMenu_Cursor_num(-1,64,0,0,0);
            }
        }
        dat00[0] &= ~0x0400;
    }

}



//========================================================================
// 函数: void UI_SecondaryMenu_G()
// 描述: 二级菜单右侧刷新函数，查看值的时候刷新数值
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void UI_SecondaryMenu_G()
{
    u8 yy = 0;
    UI_SeMenu_Cursor_num(-1,64,0,yy,0);//当前光标选中-1行
    yy =  24;
    UI_SeMenu_Cursor_num(0,64,yy,yy,1);//当前光标选中行
    yy = 48 ;
    UI_SeMenu_Cursor_num(1,64,yy,yy,1);//当前光标选中+1行

}


//========================================================================
// 函数: void UI_SecondaryMenu(u8 a)
// 描述: 二级菜单滚动刷新函数
// 参数: a：向哪个方向滚
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
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

            UI_SeMenu_Cursor_num_2(-1,0,yy,0);

        }

        yy =  (dat00[0x12] - 1) + dat2;
        if(dat00[0x13] == 0) yy = 24;

        UI_SeMenu_Cursor_num_2(0,yy,yy,1);

        yy = 24 + (dat00[0x12] - 1) + dat2;
        if(dat00[0x13] == 0) yy = 48;

        UI_SeMenu_Cursor_num_2(1,yy,yy,1);

        if(dat00[0x13] != 0)
        {
            yy = 48 + (dat00[0x12] - 1);

            UI_SeMenu_Cursor_num_2(2,yy,yy,1);
        }
    }
    else
    {
        if(dat00[0x12] <16)
        {
            yy =(dat00[0x12] - 1);
            UI_SeMenu_Cursor_num_2(-2,0,yy,0);
        }

        yy =  24 - (dat00[0x12] - 1);
        UI_SeMenu_Cursor_num_2(-1,yy,yy,1);


        yy = 48 - (dat00[0x12] - 1);
        UI_SeMenu_Cursor_num_2(0,yy,yy,1);

        yy = 72 - (dat00[0x12] - 1);
        UI_SeMenu_Cursor_num_2(1,yy,yy,1);

    }
    if(dat00[0x10] != 0x000F && !(dat00[0x10] == 0x0008 && dat00[0x009E] == 1))
    {

        if((dat00[0x13] == 0 || dat00[0x12] == 1) && (P45 && P27))
        {
            dat1  = ((dat00[0x17] ) % GET_SCdat()) * 8;
            OLED_16_16V2(TVR((u8)(dat00[0x10]-1)),dat1,0,3,0);
        }
    }

}


//========================================================================
// 函数: void IU_Secondary()
// 描述: 同主菜单，参考主菜单，懒得写了
// 参数: 
// 返回: 
// 版本: V1.0 2023.02.05
//========================================================================
void IU_Secondary()
{
    if(dat00[0x10] > 0 && dat00[0x10] < 16 && dat00[0x1A] == 0)
    {
        if(dat00[0x17] != dat00_low[0x17] && ((dat00[0x13] & 0x0004) == 0))//二级菜单移动触发
        {
            dat00[0x18]	= dat00[0x17];
            dat00[0x13] |= 0x0004;
        }
        if(dat00[0x13] & 0x0004)//二级菜单移动触发
        {
            if(dat00[0x12] == 0x0000)
            {
                if((dat00[0x13] & 0x0001)== 0x0000)
                {
                    dat00[0x12] = 24;
                    fast_SleepQueue_aheadW1(15,1,25);//申请24+1个UI移动事件
                    dat00[0x13] |= 0x0001;
                    dat00[0x0006] |= 0x0002;
									  dat00[0] |= 0x0100;

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
                    dat00[0x0006] &= ~0x0002;
									  dat00[0] |= 0x0100;
                    dat00[0x13] &= 0xfffe;
                    dat00[0x13] &= 0xfffB;
                    //SleepQueue_behindW(15,1);

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