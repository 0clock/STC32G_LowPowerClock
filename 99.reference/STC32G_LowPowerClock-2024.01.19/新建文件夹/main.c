#include "STC32F.h"
#include "stdio.h"
#include "intrins.h"
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
#include "BMC050.h"
#include "Time_Interruption.h"
//REMOVEUNUSED

/****************************** 用户定义宏 ***********************************/


/*****************************************************************************/

/*************  本地常量声明    **************/
//u16 xdata JCQ03_00[32];
u32 xdata JCQ06_00[5];

//**************JCQ06_00***********************
//00 温度 ×100 ℃
//01 湿度 ×100 RH
//02 SPL06气压 原始值
//03 SPL06温度 原始值
//04 气压 ×1   Pa
//*************************************




extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];



/*************  本地变量声明  功能寄存器  **************/
extern u8 xdata DMABuffer[96];

/*************  中断任务 标志位 专用变量（妈的别优化了）  **************/
volatile u32 Interrupt_flag=0;



/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];

extern u16 Sleep_Queue_task[Sleep_queue_num];

extern u16 Sleep_Queue_count;
extern u16 Sleep_Queue_count_target;



extern u8 xdata Time_New[7];
extern u8 xdata Time_Low[7];
extern u8 xdata Time_int[7];



/********************* 主函数 *************************/

u32 dat1 = 0;
u32 dat2 = 0;
u32 dat3 = 0;
u32 dat4 = 0;
u32 dat5 = 0;

u32 dat6 = 0;
u32 dat7 = 0;
u8 uu=0;
u16 sleep_dat=0;

u16 gg = 0;










void Buzzer_task()
{
    if(dat00[2] & 0x0001)
    {
        if(dat00[0xC3] == 1)
        {
            if(dat00[0xC1] == 1)
            {
                Buzzer_Open();
                SleepQueue_aheadW1(dat00[0xC2],0x0020,0);
            }
        }
        dat00[2] &= ~0x0001;
    }
}




void main(void)
{
    u8 i = 0;
    u8 a = 0;
    u8 b = 0;
    u16 c = 0;
    u32 d = 0;
    u8 ii = 0;
    u8 datac=0;
    u16 kk=0;

	  
//    P_SW2 |=0x80;	
	  EAXFR = 1;
    WTST = 2;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	  ELVD = 0;
	  ENLVR = 0;
	  while((HIRCCR & 0x01) == 0);
	  
	
	
	
    delay_ms(1);
    SCK = 0;
    WDT_FLAG = 0;
    P4M0 = 0x1f; 
	  P4M1 = 0x1f; 
    Buzzer_Close();
	
	
    P0M0 = 0x65;
    P0M1 = 0xff;

		P2PU |= 0x01;//打开OLED  RSET上拉电阻
    P1PU = 0x30;//IIC上拉电阻

    P1M0 = 0x47;
    P1M1 = 0x47;

    P3M0 = 0x6c; 
		P3M1 = 0x6c; 


    P5M0 = 0xfb;                                //设置P0.0~P0.7为双向口模式
    P5M1 = 0xfb;

    //P5PU = 0x11;


    //OLED_POW_LOW();
    OLED_POW(0);
    RESET54();
		ENLVR = 0;
		
    
    P3M0 &= 0xFC;                                //设置P0.0~P0.7为双向口模式
    P3M1 &= 0xFC; 
		P2PU = 0x03;		
		delay_ms(50);
		P2PU = 0x00;
	
		
    ADC_init();
    Clock_INT();
    key_int();
    IIC_int();
		
		
    Delay10us();
		
		
//		_nop_();
    
		
    IIC_W2bit(0x64,0x21,0x80);//关闭芯片K2开关
    Delay10us();
		IIC_W2bit(0x64,0x0F,0x71);//配置开启1S定时 0x60
		Delay10us();
		
		


    //IIC_W2bit(0x64,0x18,0x03);


//    UART1_config();
//    DMA_Config();
//    delay_ms(100);

    BH1745_Init();
    delay_ms(1);
    SHT41_Init();
    delay_ms(1);
    SPL06_Init();
    delay_ms(1);
    delay_ms(1);
    SPI_init();


//    P2PU = 0x28;
//    P2DR = 0x2f;
//    P2SR = 0x2f;




    P2M0 = 0x10;//SPI推挽
    P2M1 = 0x10;
    P2M0 |= 0x28;
    P2M1 &= ~0x28;		
		
    CS =0;
		
		Magnetometer_initi();
    delay_ms(1);
    IIC_W2bit(0x32,0x11,0x5E);//低功耗模式
    delay_ms(1);
    IIC_W2bit(0x32,0x11,0x9E);//从低功耗切换到挂起模式
    delay_ms(1);






    OLED_Init();
		
    OLED_Fill(0x00);
    P2PU &= ~0x01;//打开OLED  RSET上拉电阻

//    P5M0 &= 0xFD;
//    P5M1 &= 0xFD;

    //P0IE = 0x02;

    INS5699_timeW(Time_int);
		
		INS5699_timer_clean();	

    IIC_W2bit(0x64,0x0B,0x0B);//配置开启1S定时 0x60
		Delay10us();
		IIC_W2bit(0x64,0x0C,0x00);//配置开启1S定时 0x60
		Delay10us();	
//		
//    IIC_W2bit(0x64,0x0E,0x00);//配置开启1S定时 0x60
//		Delay10us();
//    IIC_W2bit(0x64,0x0F,0x71);//配置开启1S定时 0x60
		
		
    delay_ms(1);
    INS5699_timeR();
    dat00[0x10]=0x0020;
    dat00_flag[0x10]=1;
    SleepQueue_aheadW1(5,1,0);

//			dat00[0x01] = 1;
//		  Sleep_Queue[0]=20;
//			Sleep_Queue_count = 0;
//		  Sleep_Queue_count_target = 1;

//***********************指南针启动***************
//		  delay_ms(1);	
//			IIC_W2bit(0x22,0x4b,0x01);
//			delay_ms(1);
//      IIC_W2bit(0x22,0x4C,0x06);
//		  delay_ms(1);	
//			IIC_W2bit(0x22,0x4C,0x07);
//			delay_ms(10);
//			IIC_W2bit(0x22,0x4C,0x02);
//			delay_ms(10);
//			IIC_W2bit(0x22,0x51,0x0A);	
//******************************************************
    //P0IE &= 0xe7;


    P0INTE |= 0x02;//允许端口中断
    P0IM1 |=  0xFD;//下降沿中断
    P0IM0 &=  0xFD;
    P0WKUE |= 0x02;//允许掉电唤醒
    ICacheOn();//打开ICACHE功能
    EA = 1;

    //P2PU = 0x00;
		//SPL06_C0R();
    //Delay10us();
		//IIC_W2bit(INS5699_W,0x0e,0x00);
		
		
	  delay_ms(1);
		//a++;
		Delay10us();	
		Delay10us();
		
    while (1)
    {

         
        Time_Interruption_1S();
        flag_handle();
        dat00_Change();
        IU_Thread();
				time_set_thread();
        key_thread1_new();
        //Buzzer_task();
        if(Sleep_Queue_task[0] == 0)
        {
            SleepQueue_MoveLeft(1);
        }

				
				
				
				//a++;
        Sleep_Thread();


        UI_switch();				//1号任务
        SHT41_falg_0004();
        SPL06_PR_R_flag0008();
        SPL06_PR_P_flag0010();
        BH1745R_flag0002();
        Buzzer_falg_0020();
        ADC_falg_0040();
				Magnetometer_falg_0080();

				

    }
}




void INT0_Isr() interrupt 0
{

}

void INT1_Isr() interrupt 2
{

}

void KZ_isr() interrupt 13
{
    u8 intf = 0;



    intf = P0INTF;
    if(intf)
    {
        //dat00[0] |= 0x0001;
        P0INTF = 0;
        if(intf & 0x02)
        {
            dat00_low[0x00E2]++;
            if(dat00_low[0x00E2]>=2)
            {
                P0INTE &= ~0x02;//允许端口中断
                P0WKUE &= ~0x02;//允许掉电唤醒
            }

        }
    }











    intf = P4INTF;
    if(intf)
    {
        //dat00[0] |= 0x0001;
        P4INTF = 0;
        if(intf & 0x20)
        {
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            //Delay10us();
            Add_key(1);
            dat00[1] &= 0x0001;
        }
        if(intf & 0x40)
        {
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            //Delay10us();
            SET_key();     //主菜单 按下确认件
            dat00[1] = 0;
        }
    }

    intf = P2INTF;
    if(intf)
    {

        //miao(P2INTF);
        P2INTF = 0;
        if(intf & 0x80)
        {
					  
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            //Delay10us();
            Add_key(0);
            dat00[1] &= 0x0002;
        }
        if(intf & 0x40)
        {
            dat00[0] |= 0x0001;
            dat00[2] |= 0x0001;
            //Delay10us();
            ESC_key();     //二级菜单 按下退出
            dat00[1] = 0;
        }
    }
    intf = P5INTF;
    if(intf)
    {
        P5INTF = 0;
        if(intf & 0x01)
        {
            dat00[0] |= 0x0002;
        }
    }
    P2INTF=0;
    P4INTF=0;
    P5INTF=0;
    P0INTF=0;
    P1INTF=0;
    P3INTF=0;
    P6INTF=0;
    P7INTF=0;


//    UART1_DMA();

    //nian(gg++,6);


}

