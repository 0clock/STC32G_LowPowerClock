#include "IIC.h"
#include "SPL06.h"
#include "time.h"
#include "DATA.h"
#include "UART1.h"

extern u8 xdata DMABuffer[96];
extern u32 xdata JCQ06_00[10];
extern u16 Sleep_Queue_task[Sleep_queue_num];



void SPL06_Init()
{
		 IIC_W2bit(SPL06_W,0x0C,0x09);//09初始化  80：从FIFO中读出所有数据后，写入'1'以清除所有旧数据。	
		 delay_ms(10);
		IIC_W2bit(SPL06_W,0x08,0x00);//待机模式		 
		 IIC_W2bit(SPL06_W,0x06,0x00);//配置气压采样
		 IIC_W2bit(SPL06_W,0x07,0x00);//配置温度采样
		 IIC_W2bit(SPL06_W,0x09,0x00);// 		

	
}



void SPL06_C0R()
{
	  u8 i = 0;
	  Start(); //发送起始命令
    SendData(SPL06_W);                             //发送设备地址+写命令
    RecvACK();
    SendData(0x10);                             //发送设备地址+写命令
    RecvACK();
	  delay_ms(1);
    Start(); //发送起始命令
    SendData(SPL06_R);                             //发送设备地址+写命令
    RecvACK();
		for(i=0;i<18;i++)
		{
		   DMABuffer[i] = RecvData(); 
		   if(i != 17) SendACK();	
		   if(i == 17) SendNAK();		
		}	

    UART1_TX(18);
	 delay_ms(10);
		
}



void SPL06_PR(u8 cmd,u8 dat)// 0x01,0x00 气压，0x02,0x03 温度
{
	  u32 dat1 = 0;
		IIC_W2bit(SPL06_W,0x08,cmd);//测量温度
    delay_ms(6);
    Start(); //发送起始命令
    SendData(SPL06_W);                             //发送设备地址+写命令
    RecvACK();
    SendData(dat);                             //发送设备地址+写命令
    RecvACK();
    Start(); //发送起始命令
    SendData(SPL06_R);                             //发送设备地址+写命令
    RecvACK();
	  
		dat1 = RecvData();                           //发送设备地址+写命令
    SendACK();
	  dat1 = dat1<<8;
		dat1 += RecvData();                           //发送设备地址+写命令
    SendACK();	
    dat1 = dat1<<8;	
		dat1 += RecvData();                           //发送设备地址+写命令
    SendNAK();
	  Stop();
	  JCQ06_00[cmd+1] = dat1;
}


void SPL06_Math_P(u32 dat3,u32 dat2)//dat3气压
{
	    u32 dat4;
	
//	  DMABuffer[0] =dat3>>24;
//    DMABuffer[1] =dat3>>16;
//    DMABuffer[2] =dat3>>8;
//    DMABuffer[3] =dat3;
// 
//	  DMABuffer[4] =dat2>>24;
//    DMABuffer[5] =dat2>>16;
//    DMABuffer[6] =dat2>>8;
//    DMABuffer[7] =dat2;	
//	
//    UART1_TX(8);
//	 delay_ms(10);
//	
	
	
	
	
	
	
	
	
	    dat2 = (dat2<<12) / k_pt_1; 
      dat3 = ((0x00ffffff - (dat3 & 0x007fffff)+1)<<12)/ k_pt_1;
      dat4 =c00 + ((dat3 *(c10 - ((dat3 *(c20 - ((dat3 * c30)>>12)))>>12)))>>12) - ((dat2 * c01)>>12) - ((((dat3 * dat2)>>2) * (c11 + ((dat3*c21)>>12)))>>22);
	    JCQ06_00[4] = dat4;
	
	
	
	
	
	
	
}





//      SPL06_PR(0x02,0x03);
//			delay_ms(20);
//      SPL06_PR(0x01,0x00);
//			SPL06_Math_P(JCQ06_00[2],JCQ06_00[3]);





void SPL06_R2(u8 cmd,u8 dat)
{
	  u32 dat1 = 0;
    Start(); //发送起始命令
    SendData(SPL06_W);                             //发送设备地址+写命令
    RecvACK();
    SendData(dat);                             //发送设备地址+写命令
    RecvACK();
    Start(); //发送起始命令
    SendData(SPL06_R);                             //发送设备地址+写命令
    RecvACK();
	  
		dat1 = RecvData();                           //发送设备地址+写命令
    SendACK();
	  dat1 = dat1<<8;
		dat1 += RecvData();                           //发送设备地址+写命令
    SendACK();	
    dat1 = dat1<<8;	
		dat1 += RecvData();                           //发送设备地址+写命令
    SendNAK();
	  Stop();
	  JCQ06_00[cmd] = dat1;
}







void SPL06_PR_R()
{
	 IIC_W2bit(SPL06_W,0x08,0x02);//测量温度
	 SleepQueue_aheadW1(15,0x0008,0);
}


void SPL06_PR_R_flag0008()
{

		   if(Sleep_Queue_task[0] & 0x0008)
		 {			 
      SPL06_R2(0x03,0x03);
			Sleep_Queue_task[0] &=~0x0008;
			IIC_W2bit(SPL06_W,0x08,0x01);//测量气压
	    SleepQueue_aheadW1(15,0x0010,0);
		 } 
}

void SPL06_PR_P_flag0010()
{
		   if(Sleep_Queue_task[0] & 0x0010)
		 {			 
			SPL06_R2(0x02,0x00);
			SPL06_Math_P(JCQ06_00[2],JCQ06_00[3]);
			Sleep_Queue_task[0] &=~0x0010;
		 } 
}




















