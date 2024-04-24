#include "SleepMode.h"
#include "IO.h"
#include "OLED.h"
#include "IIC.h"
#include "ADC.h"
#include "time.h"
#include "DATA.h"
#include "INS5699S.h"


extern u16 xdata dat00[System_data_num];
extern u16 xdata dat00_low[System_data_num];
extern u8 xdata dat00_flag[System_data_num];








void SleepMode_ON()
{
	  IIC_W2bit(0x64,0x0E,0x00);//清除警告	
	  IIC_W2bit(0x64,0x0F,0x40);//关闭定时器
	
	
	
	
	
	
	
	
	  EAXFR = 1;
	  ADC_CONTR = 0x00;
	  I2CCFG = 0x40+25;    //满速400K 35M 设定20
	  SPCTL &= 0xbf; 
//	  HSSPI_CFG2 &=~0x20;
//	  DMA_SPI_CR = 0x00;
		P2PU = 0x00;
	  P1PU = 0x00;//IIC上拉电阻
	
	  Battery_ON_OFF();


//*********************P0区*******************************	
//	
//  BMC050 DRDYO中断+  KEYSET + KEY+ 设置为双向口 默认高电平
//	
//	（双向）P07 SPL06_INT        （开漏）P06 OLEDV1       （开漏）P05 OLEDV2    （高阻）P04 BATTERY（电池ADC采样脚）   （高阻）P03 ADC6（充电电流）   （开漏）PP02 EN（锂电池充电EN）   （高阻）P01 GHRG   （开漏）PP00 VIN/2 

    P0=0xff;
    P0M0 = 0x65;
    P0M1 = 0x7f;	
	
//*********************P4区*******************************	
//	P47 DRDYO        P46 KEYSET       P45 KEY+    P44 悬空   P43 悬空   P42 悬空   P41 悬空   P40 CCP0_EN（蜂鸣器控制）  
    P4=0xfe;
    P4M0 = 0x1f; 
	  P4M1 = 0x1f; 
	
	
//*********************P1区*******************************		
//	（双向）P17 INT3O        （开漏）P16 悬空       （双向）P15 SCL    （双向）P14 SDA   （双向）P13 INT2O    （开漏）P12   （开漏）P11 BH_INT   （开漏）P10 悬空
    P1=0xff;
    P1M0 = 0x45;
    P1M1 = 0x45;
	
//*********************P3区*******************************		
//	（双向）P37 INT3（BMC050）        （开漏）P36 悬空       （开漏）P35 悬空    （双向）P34 SW360   （开漏）P33 悬空    （开漏）P32 悬空     （开漏）P31 TX   （开漏）P30 RX
    P3=0xff;
    P3M0 = 0xef; 
		P3M1 = 0xef; 

//*********************P5区*******************************		
//	（开漏）P57         （开漏）P56        （开漏）P55     （双向）P54 RSET   （开漏）P53 5VEN    （开漏）P52 BATTERY_ON     （开漏）P51 TEXT  （双向）P50 SQW（1Hz）
		P5=0xf3;
    P5M0 = 0xef; 
		P5M1 = 0xef; 
		
		
//*********************P2区*******************************		
//	（双向）P27  KEY-       （双向）P26  KEYMODE      （推挽）P25 SCLK    （开漏）P24 悬空   （推挽）P23 MOSI    （开漏）P22 CS     （双向）P21 DC  （双向）P20 RES
		P2=0xff;
    P2M0 = 0x3f; 
		P2M1 = 0x3f; 

		

    P6=0xff;
    P6M0 = 0xff; 
		P6M1 = 0xff; 
    P7=0xff;
    P7M0 = 0xff; 
		P7M1 = 0xff; 






	  P0IE=0x00;  P1IE=0x00;
	  P2IE=0x00;  P3IE=0x10;//0x10
  	P4IE=0x00;  P5IE=0x10;
	  P6IE=0x00;  P7IE=0x00;

		P2SR = 0xff; 
    P2DR = 0xff; 
		
    P3IM1 &= ~0x10;//下降沿中断
    P3IM0 &= ~0x10;		
    P3WKUE |= 0x10;
		P3INTE |= 0x10;
    EA=1;




	  _nop_();
		_nop_();
		_nop_();
		_nop_();		
		 PD = 1;                            //MCU进入掉电模式
		_nop_();
		_nop_();
		_nop_();
		_nop_();

	
}

void SleepMode_OFF()
{
		P3WKUE &= ~0x10;
		P3INTE &= ~0x10;	
    EA=0;
		P54RST = 1;
	  P0=0xff;
	  P1=0xff;	  
	  P2=0xff;	
	  P3=0xff;	
	  P4=0xff;	
	  P5=0xff;	
	  P6=0xff;	
	  P7=0xff;	
		IO_Init();
    OLED_POW(0);
    RESET54();	
    ADC_init();
    Clock_INT();
    key_int();
    IIC_int();
    IIC_W2bit(0x64,0x21,0x80);//关闭芯片K2开关
    Delay10us();
		IIC_W2bit(0x64,0x0F,0x71);//配置开启1S定时 0x60
		Delay10us();
    SPI_init();
    SPI_DMA_Config(); 
    P2PU = 0x01;//打开OLED  RSET上拉电阻
    OLED_Init();
    P2PU = 0x01;//打开OLED  RSET上拉电阻
    delay_ms(1);
    INS5699_timeR();
		dat00[0] |= 0x0002;
    dat00[0x10]=0x0020;
    dat00_flag[0x10]=1;
    SleepQueue_aheadW1(5,1,0);
    P0INTE |= 0x02;//允许端口中断
    P0IM1 |=  0xFD;//下降沿中断
    P0IM0 &=  0xFD;
    P0WKUE |= 0x02;//允许掉电唤醒
    EA = 1;
	

}