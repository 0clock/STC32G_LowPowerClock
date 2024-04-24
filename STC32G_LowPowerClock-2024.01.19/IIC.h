#ifndef __IIC_H
#define __IIC_H	
#include "STC32G.h"


//39H
#define BH1745_W          0x72 
#define BH1745_R          0x73 



//44H
#define SHT41_W          0x88
#define SHT41_R          0x89


//77H
#define SPL06_W          0xEE
#define SPL06_R          0xEF

//32H
#define INS5699_W        0x64
#define INS5699_R 			 0x65



sbit    SDA = P1^4; //定义SDA
sbit    SCL = P1^5; //定义SCL












void IIC_int();
void Start();
void SendData(char dat);
u8 RecvACK();
u8 RecvData();
void SendACK();
void SendNAK();
void Stop();


u8 IIC_W2bit(u8 addre,u8 MSB,u8 LSB);
u8 IIC_W1bit(u8 addre,u8 LSB);

u8 IIC_R1bit(u8 nam,u8 adds);

u8 IIC_ResponseText();

#endif