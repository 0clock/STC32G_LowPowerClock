#include "INS5699S.h"
#include "IIC.h"
#include "time.h"
#include "DATA.h"

extern u16 xdata dat00[System_data_num];
extern u8 xdata Time_New[7];
extern u8 xdata Time_int[7];



void INS5699_timeR()
{
	    u8 i = 0;
		  Start(); //发送起始命令
      SendData(INS5699_W);                             //发送设备地址+写命令
      RecvACK();
			SendData(0x00);                             //发送设备地址+写命令
      RecvACK();
			Start(); //发送起始命令
      SendData(INS5699_R);                             //发送设备地址+写命令
      RecvACK();
	
	   for(i=0;i<7;i++)
		 {
			Time_New[i] = RecvData();                            //读取数据2
			if(i == 6)
			{
					SendNAK();	
			}
			else
			{
			   SendACK();
			}
		 }
		 Stop();
}




void INS5699_timeW(u8 *a)
{
	    u8 i = 0;
		  Start(); //发送起始命令
      SendData(INS5699_W);                             //发送设备地址+写命令
      RecvACK();
			SendData(0x00);                             //发送设备地址+写命令
      RecvACK();
	   for(i=0;i<7;i++)
		 {
			SendData(a[i]);                             //发送设备地址+写命令
      RecvACK();
		 }
		 Stop();
}



void time_set_thread()
{
	  if(dat00[0x0006] & 0x0001)
		{
	   SET_time_c();
		 INS5699_timeW(Time_int);
		 dat00[0x0006] &=~0x0001;
		}
}




void INS5699_timer_clean()
{
    IIC_W2bit(0x64,0x0D,0x02);//关闭定时器
		IIC_W2bit(0x64,0x0E,0x00);//清除警告
    IIC_W2bit(0x64,0x0B,0x01);//配置定时器
		IIC_W2bit(0x64,0x0C,0x00);//配置定时器
    IIC_W2bit(0x64,0x0D,0x10);//打开定时器，配置4096Hz清零	

		Sleep_delay_Time(2);	
	  if(!(0x08 & IIC_R1bit(INS5699_W,0x0e)))
		{
			Sleep_delay_Time(2);
		}
	  //delay_ms(1);
		IIC_W2bit(0x64,0x0D,0x02);//关闭定时器，配置1Hz
		IIC_W2bit(0x64,0x0E,0x00);//清除警告
		IIC_W2bit(0x64,0x0D,0x12);//打开定时器，配置1Hz
}





















