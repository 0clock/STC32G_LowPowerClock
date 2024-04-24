#include "time.h"
#include "DATA.h"
#include "OLED.h"

/****************************** 睡眠寄存器组 ***********************************/
extern u16 Sleep_Queue[Sleep_queue_num];


void Timer0Init(void)		//1毫秒@35MHz
{
    AUXR |= 0x80;		//定时器时钟1T模式
    TMOD &= 0xF0;		//设置定时器模式
    TL0 = 0x48;		//设置定时初始值
    TH0 = 0x77;		//设置定时初始值
    TF0 = 0;		//清除TF0标志
    ET0 = 1;    //使能定时器中断
    TR0 = 1;		//定时器0开始计时
}



//========================================================================
// 函数: void delay_ms(u8 ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注:
//========================================================================
void delay_ms(u8 ms)
{
    u16 i;
    do {
        i = MAIN_Fosc / 10000;
        while(--i);   //10T per loop
    } while(--ms);
}

void Delay()
{
    int i;

    for (i=0; i<100; i++)
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}



void Delay10us()		//@35MHz
{
	unsigned long i;

	_nop_();
	_nop_();
	_nop_();
	i = 173UL;
	while (i) i--;
}

void Delay50us()		//@35MHz
{
	unsigned long i;

	_nop_();
	i = 436UL;
	while (i) i--;
}


//========================================================================
// 函数: void Sleep_delay_us(u8 ms)
// 描述: 掉电唤醒延时函数。
// 参数: us
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注:
//========================================================================
void Sleep_delay_HalfMs(u16 ms)
{
	if(ms <= 32767)
	{
   ms |= 0x8000;   
	 WKTCL = (ms | 0x0001);
   WKTCH = ms>>8; 
	}
}








void Sleep_delay_Time(u16 sleep_dat)
{
	  u16 kk=0;
	  u16 uu = 0;
		while(sleep_dat)
		{	
			Sleep_delay_HalfMs(sleep_dat);
		_nop_();
		_nop_();
		_nop_();
		_nop_();		
			PCON = 0x02;                            //MCU进入掉电模式
		_nop_();
		_nop_();
		_nop_();
		_nop_();
			WKTCH &= 0x7f;
			kk = WKTCH;
			kk = kk*256;
			kk = kk + WKTCL;	
			if(kk == 0)
			{
				sleep_dat = 0;
			}				
			else
			{
				sleep_dat = sleep_dat - kk;
			}
			 //nian(uu++,6);
			//Delay50us();
			_nop_();
		}
		
}









void Sleep_Thread()
{
	  if(RFF() > 0)
		{
				Sleep_delay_Time(Sleep_Queue[0]);
				//SleepQueue_MoveLeft(1);
		}
    else
		{
					WKTCH &= 0x7f;
			    
			    
					_nop_();
					_nop_();
					_nop_();
					_nop_();			
					PCON = 0x02;                            //MCU进入掉电模式
					_nop_();
					_nop_();
					_nop_();
					_nop_();
		} 
}



















