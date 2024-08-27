/*
 * @Author: 0clock
 * @Date: 2024-08-21 22:08:41
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-28 00:18:18
 * @FilePath: \STC32_LowPowerClock\1.firmware\Code\IIC.h
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#ifndef __IIC_H
#define __IIC_H
#include "stc32g.h"
// 39H
#define BH1745_W 0x72
#define BH1745_R 0x73

// 44H
#define SHT41_W 0x88
#define SHT41_R 0x89

// 77H
#define SPL06_W 0xEE
#define SPL06_R 0xEF

// 32H
#define INS5699_W 0x64
#define INS5699_R 0x65

sbit SDA = P1 ^ 4; // 定义SDA
sbit SCL = P1 ^ 5; // 定义SCL

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

void iic_init();
void iic_start();
bit RecvACK();
u8 RecvData();
void SendACK();
void SendNAK();
void iic_stop();
bit iic_senddata(char dat);
bit iic_write2bit(u8 addre, u8 MSB, u8 LSB);
bit iic_write1bit(u8 addre, u8 LSB);

u8 iic_read1bit(u8 nam, u8 adds);

#endif