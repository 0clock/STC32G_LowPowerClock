#ifndef __OLED_H
#define __OLED_H
#include "STC32F.h"

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xff
#define X_WIDTH 	128
#define Y_WIDTH 	64


/*************  TTF相关变量声明   **************/
sbit    DC  = P2^1;     //PIN6
sbit    RST = P2^0;     //PIN6
sbit    CS  = P2^2;     //PIN6
sbit    SI  = P2^3;     //PIN5
sbit    SCK = P2^5;     //PIN6

void SPI_init(void);

void OLED_WrCmd(u8 dat1);

void OLED_Fill(u8 bmp_dat);

void OLED_Init(void);

void miao(unsigned char ee);

void shifen(unsigned char t,unsigned char y);

void OLED_GD();

void ZF_8X8(u8 x,u8 y,u8 ee);

void ZF_8X16(u8 x,u8 y,u8 ee);

void OLED_32X32(u8 x,u8 y,u8 addr,u8 devia1,u8 devia2);

void nian(u16 ee,u8 y);

void yue_ri(u8 x,u8 ee,bit g);//


void OLED_Region_Fill();

void OLED_Progress_Bar(u16 num,u16 jx);

void OLED_ED();

void OLED_16X16(u8 x,u8 y,u8 adde);

void OLED_16_16V(u8 (*dat)[16],u16 adde,u8 x,u8 y,bit p);

void OLED_16_16V2(u8 (*dat)[16],u16 adde,u8 x,u8 y,bit p);

void OLED_16_16V2a();

u8** TVR(u8 i);

void OLED_16_16Vabc(u8 (*dat)[16],u16 adde,u8 x,u8 y,u8 c,bit jj,u8 Number);

void OLED_Mean2_fill_num(u8 a,u8 b,u8 (*dat)[16]);

void OLED_Mean2_fill_num2(u16 a);

void OLED_Mean2_fill(u8 p,u8 a);

void OLED_Mean2_fill2(u8 a);

void shifen_dh(u8 x,u8 yy,u16 y,u8  limit);

void IU_Battery_Icon_S(u8 x,u8 y,u8 p);

void shifen_2(unsigned char t,unsigned char y);

void IU_Battery_cheek();

void IU_Battery_cheek_renovate();

void OLED_Set_Pos(u8 x, u8 y);

void OLED_WrDat(u8 dat);

//void OLED_16X16(u8 x,u8 y,u8 adde);

void debug_8x8(int k,u8 x,u8 y);

void OLED_16X16_MOV(u8 x,u8 y,u8 adde,u8 devia1,u8 devia2);

void OLED_8X8X3_MOV(u8 x,u8 y,u16 adde,u8 devia1,u8 devia2);



void SPI_DMA();

void SPI_DMA_Config(void);


void SPI_DMA_delay();

void OLED_Frames();

void OLED_InverseDisplay2();

void debug_8x8_3(u16 k,u8 x,u8 y);

void OLED_3Figures(u16 num,u8 x,u8 y);

u8 numlongGet(u16 a);

#endif