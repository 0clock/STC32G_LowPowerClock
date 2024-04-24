#include "IIC.h"
#include "SHT41.h"
#include "time.h"
#include "DATA.h"
#include "OLED.h"
#include "UART1.h"

extern u8 xdata DMABuffer[32];
extern u32 xdata JCQ06_00[10];


extern u16 Sleep_Queue_task[Sleep_queue_num];

u8 uuy = 0;
void SHT41_Init()
{
		IIC_W1bit(SHT41_W,0x94);
}

u16 SHT41_R_16bit(bit k)
{
		  u16 dat = 0;
	    dat += RecvData();                            //读取数据2
      SendACK();
	    dat = dat <<8;
			dat += RecvData();                            //读取数据2
      SendACK();
	    RecvData();                            //读取数据2
      if(!k) SendACK();
	    if(k) SendNAK();
      return dat;	
}

void SHT41_R_High()
{

		  IIC_W1bit(SHT41_W,0xFD);
	    SleepQueue_aheadW1(28,4,0);
//	    delay_ms(12);
//	
//	    Start(); //发送起始命令
//      SendData(SHT41_R);                             //发送设备地址+写命令
//      RecvACK();
//			a = SHT41_R_16bit(0);
//	    b = SHT41_R_16bit(1);
//			Stop();
//		  a = a*17500/65535-4500;
//		  b = b*12500/65535-600;
//	    JCQ06_00[0] = a;
//	    JCQ06_00[1] = b;
}


void SHT41_R_High1()
{
      u16 a;
	    u16 b;
		  IIC_W1bit(SHT41_W,0xFD);
	    delay_ms(12);
	
	    Start(); //发送起始命令
      SendData(SHT41_R);//发送设备地址+写命令
      RecvACK();
			a = SHT41_R_16bit(0);
	    b = SHT41_R_16bit(1);
			Stop();

	
}












void SHT41_falg_0004()
{
		  u32 a = 0;
	    u32 b = 0;
	   if(Sleep_Queue_task[0] & 0x0004)
		 {			 
	    Start(); //发送起始命令
      SendData(SHT41_R);                             //发送设备地址+写命令
      RecvACK();
			a = SHT41_R_16bit(0);
	    b = SHT41_R_16bit(1);
			Stop();
		  a = a*17500/65535-4500;
		  b = b*12500/65535-600;	
	    JCQ06_00[0] = a;
	    JCQ06_00[1] = b;
			Sleep_Queue_task[0] &=~0x0004;
		 }    
		 
		 
		 
		 
}