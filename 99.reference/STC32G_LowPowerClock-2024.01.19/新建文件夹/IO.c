#include "IO.h"
#include "time.h"
#include "DATA.h"
#include "logic.h"
//******************************************************************************
//*
//*			OLED_POW  打开OLED屏幕供电，00-5V 01-5.5V 02-6V 03-6.5V
//*
//******************************************************************************
extern u16 Sleep_Queue_task[Sleep_queue_num];
extern u16 xdata dat00[System_data_num];

void OLED_POW(u8 dat)
{
	    P53 = 1;
			P5M0 &= 0xf7; //p54双向
			P5M1 &= 0xf7;
	    P05 = ~(dat & 0x01);
	    P06 = ~(dat & 0x02);
}

void OLED_POW_LOW()
{
	    P53 = 0;
			P5M0 |= 0x08; 
			P5M1 |= 0x08;
	    P05 = 1;
	    P06 = 1;
}

void RESET54()
{
			P5M0 &= 0xed; //p54双向
			P5M1 &= 0xed;
	    P54RST = 1;
}

void Clock_INT()
{
			P5M0 &= 0xfe; //p54双向
			P5M1 &= 0xfe;
	    P50 = 1;
		  P5INTE |= 0x01;//允许端口中断
	    P5IM1 |=  0xFE;//下降沿中断
	    P5IM0 &=  0xFE;	
	    P5DR |= 0x01;
	    P5WKUE |= 0x01;//允许掉电唤醒 

			
			P5M0 &= 0xfe; //p54双向
			P5M1 &= 0xfe;

			P5M0 &= 0xFB; //初始化电池电压检测MOS控制开关
			P5M1 &= 0xFB; // 
}








void key_int()
{
	//+
	P4M0 &= 0x9f; 
	P4M1 &= 0x9f; 

	P4INTE |= 0x60;//允许端口中断
	P4IM1 &=  0x9f;//下降沿中断
	P4IM0 &=  0x9f;
	
	P4WKUE |= 0x60;//允许掉电唤醒 
	
	P2M0 &= 0x3f; 
	P2M1 &= 0x3f; 	
	
	P2INTE |= 0xC0;//允许端口中断
	P2IM1 &=  0x3f;//下降沿中断
	P2IM0 &=  0x3f;	
	
	P2WKUE |= 0xC0;//允许掉电唤醒 	
	
}


void Buzzer_Open()
{
		P4M0 |= 0x01;    
    P4M1 &= 0xFE;
}

void Buzzer_Close()
{
		P4M0 |= 0x01;    
    P4M1 |= 0x01;
}



void Buzzer_falg_0020()
{
	   if(Sleep_Queue_task[0] & 0x0020)
		 {			 
			Buzzer_Close();
			Sleep_Queue_task[0] &=~0x0020;
		 }    
}


void Battery_ON_ON()
{ 
	  P52 = 1;
		P5M0 |= 0x04; //初始化电池电压检测MOS控制开关
		P5M1 &= ~0x04; //
}

void Battery_ON_OFF()
{ 

		P5M0 |= 0x04; //初始化电池电压检测MOS控制开关
		P5M1 |= 0x04; //
		P52 = 0;	
}






void ICacheOn(void)     //打开ICACHE功能
{
    bit fEA;
    
    if(WTST > 0)
    {
        fEA = EA;
        EA = 0;         //关闭中断，防止写触发命令序列中途产生中断
        _nop_();
        _nop_();
			  NOP40();
        TA = 0xaa;      //写入触发命令序列1
                        //此处不能有其他任何指令
        TA = 0x55;      //写入触发命令序列2
                        //此处不能有其他任何指令
        ICHECR = 0x01;  //写保护暂时关闭，可以修改ICHECR中的EN位
                        //EN为再次进入写保护状态
        _nop_();
        _nop_();
			  NOP40();
        EA = fEA;
    }
}

void ICacheOff(void)//关闭ICACHE功能
{
    bit fEA;

    fEA = EA;
    EA = 0;         //关闭中断，防止写触发命令序列中途产生中断
    _nop_();
    _nop_();
	  NOP40();
    TA = 0xaa;      //写入触发命令序列1
                    //此处不能有其他任何指令
    TA = 0x55;      //写入触发命令序列2
                    //此处不能有其他任何指令
    ICHECR = 0x00;  //写保护暂时关闭，可以修改ICHECR中的EN位
                    //EN位再次进入写保护状态
    _nop_();
    _nop_();
	  NOP40();
    EA = fEA;
}


















