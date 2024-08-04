#include "IIC.h"

void IIC_int()
{
	  P_SW2 |= 0x80 + 0x00;
    I2CCFG = 0xC0+35;    //满速400K 35M 设定20
	  I2CMSST = 0x00;

}

bit Wait()
{
	  bit k=0;
	  u16 i =100;
    while ((!(I2CMSST & 0x40))&(i>0))
		{
				i--;
		}
	  if(I2CMSST & 0x02)
		{
			k=1;
		}
    I2CMSST &= ~0x40;
	  return k;
}

void Start()
{
    I2CMSCR = 0x01;                             //发送START命令
    Wait();
}

void SendData(char dat)
{
    I2CTXD = dat;                               //写数据到数据缓冲区
    I2CMSCR = 0x02;                             //发送SEND命令
    Wait();
}

bit RecvACK()
{
	  bit p=0;
    I2CMSCR = 0x03;                             //发送读ACK命令
    p = Wait();
	  return p;
}

char RecvData()
{
    I2CMSCR = 0x04;                             //发送RECV命令
    Wait();
    return I2CRXD;
}

void SendACK()
{
    I2CMSST = 0x00;                             //设置ACK信号
    I2CMSCR = 0x05;                             //发送ACK命令
    Wait();
}

void SendNAK()
{
    I2CMSST = 0x01;                             //设置NAK信号
    I2CMSCR = 0x05;                             //发送ACK命令
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06;                             //发送STOP命令
    Wait();
}







/*

void    I2C_Delay(void) //for normal MCS51, delay (2 * dly + 4) T, for STC12Cxxxx delay (4 * dly + 10) T
{
	unsigned long i;
	_nop_();
	_nop_();
	i = 7UL;
	while (i) i--;
}

bit RecvACK()
{
	  bit p=0;
    SDA = 1;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    p  = SDA;
    SCL = 0;
    I2C_Delay();
	  return p;
}




void Start()               //start the I2C, SDA High-to-low when SCL is high
{
    SDA = 1;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
}       


void Stop()                 //STOP the I2C, SDA Low-to-high when SCL is high
{
    SDA = 0;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    SDA = 1;
    I2C_Delay();
}

void SendACK()              //Send ACK (LOW)
{
    SDA = 0;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
}

void SendNAK()           //Send No ACK (High)
{
    SDA = 1;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
}
        
void I2C_Check_ACK(void)         //Check ACK, If F0=0, then right, if F0=1, then error
{
    SDA = 1;
    I2C_Delay();
    SCL = 1;
    I2C_Delay();
    F0  = SDA;
    SCL = 0;
    I2C_Delay();
}


void SendData(u8 dat)     //write a byte to I2C
{
    u8 i;
    i = 8;
    do
    {
        if(dat & 0x80)  SDA = 1;
        else            SDA = 0;
        dat <<= 1;
        I2C_Delay();
        SCL = 1;
        I2C_Delay();
        SCL = 0;
        I2C_Delay();
    }
    while(--i);
}

u8 RecvData()          //read A byte from I2C
{
    u8 i,dat;
    i = 0;
    SDA = 1;
    for(i=0;i<8;i++)
	{
		    dat = dat<<1;  
		    I2C_Delay();
        SCL = 1;
        I2C_Delay();
		    if(SDA)
				{
				   dat |= 0x01;
			  }
        
        SCL  = 0;
        I2C_Delay();
   }
    return(dat);
}


*/










bit IIC_W2bit(u8 addre,u8 MSB,u8 LSB)
{
	    bit ACK = 0;
	
			Start(); //发送起始命令
      SendData(addre);//发送设备地址+写命令
      ACK |= RecvACK();
			SendData(MSB);//发送设备地址+写命令
      ACK |= RecvACK();
			SendData(LSB);//发送设备地址+写命令
      ACK |= RecvACK();
			Stop(); 
	    return ACK;	
}

bit IIC_W1bit(u8 addre,u8 LSB)
{
	    bit ACK = 0;
			Start(); //发送起始命令
      SendData(addre);//发送设备地址+写命令
      ACK |= RecvACK();
			SendData(LSB);//发送设备地址+写命令
      ACK |= RecvACK();
			Stop(); 
	    return ACK;	
}


u8 IIC_R1bit(u8 nam,u8 adds)
{
	    u16 a= 0;
	    u16 b= 0;
		  Start(); //发送起始命令
      SendData(nam);                             //发送设备地址+写命令
      RecvACK();
			SendData(adds);                             //发送设备地址+写命令
      RecvACK();
			Start(); //发送起始命令
      SendData((u8)(nam + 1));                             //发送设备地址+写命令
      RecvACK();
			a = RecvData();                            //读取数据2
      SendNAK();	
      Stop(); 
	    return a;	
	
}










