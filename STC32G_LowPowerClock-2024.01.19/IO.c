#include "IO.h"
#include "time.h"
#include "DATA.h"
#include "logic.h"
#include "OLED.h"
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
//			P5M0 &= 0xfe; //p54双向
//			P5M1 &= 0xfe;
	    P50 = 1;
		  P5INTE |= 0x01;//允许端口中断
	    P5IM1 =  0x00;//下降沿中断
	    P5IM0 =  0x00;	
	    P5WKUE |= 0x01;//允许掉电唤醒 

			
//			P5M0 &= 0xfe; //p54双向
//			P5M1 &= 0xfe;

//			P5M0 &= 0xFB; //初始化电池电压检测MOS控制开关
//			P5M1 &= 0xFB; // 
}








void key_int()
{
	//+
//	P4M0 &= 0x9f; 
//	P4M1 &= 0x9f; 

	P4INTE |= 0x60;//允许端口中断
	P4IM1 &=  0x9f;//下降沿中断
	P4IM0 &=  0x9f;
	
	P4WKUE |= 0x60;//允许掉电唤醒 
	
//	P2M0 &= 0x3f; 
//	P2M1 &= 0x3f; 	
	
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






void IO_Init(void)
{
		
//*********************P0区*******************************	
//	
//  BMC050 DRDYO中断+  KEYSET + KEY+ 设置为双向口 默认高电平
//	
//	（双向）P07 SPL06_INT        （开漏）P06 OLEDV1       （开漏）P05 OLEDV2    （高阻）P04 BATTERY（电池ADC采样脚）   （高阻）P03 ADC6（充电电流）   （开漏）PP02 EN（锂电池充电EN）   （高阻）P01 GHRG   （开漏）PP00 VIN/2 
		  
    P0M0 = 0x65;
    P0M1 = 0x7f;	
	
//*********************P4区*******************************	
//	P47 DRDYO        P46 KEYSET       P45 KEY+    P44 悬空   P43 悬空   P42 悬空   P41 悬空   P40 CCP0_EN（蜂鸣器控制）   
    P4M0 = 0x1f; 
	  P4M1 = 0x1f; 
    Buzzer_Close();
	
	
//*********************P1区*******************************		
//	（双向）P17 INT3O        （开漏）P16 悬空       （双向）P15 SCL    （双向）P14 SDA   （双向）P13 INT2O    （开漏）P12   （开漏）P11 BH_INT   （开漏）P10 悬空
    P1M0 = 0x47;
    P1M1 = 0x47;
//		SCK = 0;
    P1PU = 0x30;//IIC上拉电阻
		
		
		
//*********************P3区*******************************		
//	（双向）P37 INT3（BMC050）        （开漏）P36 悬空       （开漏）P35 悬空    （双向）P34 SW360   （开漏）P33 悬空    （开漏）P32 悬空     （开漏）P31 TX   （开漏）P30 RX
    P3M0 = 0x6f; 
		P3M1 = 0x6f; 

//*********************P5区*******************************		
//	（开漏）P57         （开漏）P56        （开漏）P55     （双向）P54 RSET   （开漏）P53 5VEN    （开漏）P52 BATTERY_ON     （开漏）P51 TEXT  （双向）P50 SQW（1Hz）
    P5M0 = 0xee; 
		P5M1 = 0xee; 
		
		
//*********************P2区*******************************		
//	（双向）P27  KEY-       （双向）P26  KEYMODE      （推挽）P25 SCLK    （开漏）P24 悬空   （推挽）P23 MOSI    （开漏）P22 CS     （双向）P21 DC  （双向）P20 RES
    CS =0;
    P2M0 = 0x3c; 
		P2M1 = 0x14; 


    P6M0 = 0xff; 
		P6M1 = 0xff; 

    P7M0 = 0xff; 
		P7M1 = 0xff; 
		
		P2SR = 0xd7; 
    P2DR = 0xd7; 


	  P0IE=0x1b;  P1IE=0x30;
	  P2IE=0xc0;  P3IE=0x13;//0x10
  	P4IE=0x60;  P5IE=0x11;
	  P6IE=0x00;  P7IE=0x00;




		  

}













