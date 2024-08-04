#include "UART1.h"
#include "time.h"

u8 xdata DMABuffer[96];
u8 xdata DMABufferRX[32];

/*************  本地变量声明  功能寄存器  **************/
extern u16 xdata JCQ03_00[32];
extern u16 Sleep_Queue_task[Sleep_queue_num];
extern u16 Sleep_Queue[Sleep_queue_num];

//========================================================================
//
//		UART1超时变量
//
//========================================================================



void DMA_Config(void)
{
    P_SW2 |= 0x80;
    DMA_UR1T_CFG = 0x80; 							// bit7 1:启用中断
    DMA_UR1T_STA = 0x00;							//清空中断
    DMA_UR1T_AMT = 31; 								// 自动发送字节数 = n+1
	  DMA_UR1T_TXAH = (u8)((u16)&DMABuffer >> 8);
	  DMA_UR1T_TXAL = (u8)((u16)&DMABuffer);	
	  DMA_UR1T_CR = 0x80; 

    DMA_UR1R_CFG = 0x80; 							// bit7 1:启用中断
    DMA_UR1R_STA = 0x00; 							//清空中断
    DMA_UR1R_AMT = 31; 								// 自动接收字节数 = n+1
		DMA_UR1R_RXAH = (u8)((u16)&DMABufferRX >> 8);
	  DMA_UR1R_RXAL = (u8)((u16)&DMABufferRX);
    DMA_UR1R_CR = 0xa1; 							// 自动接收
}

void UART1_config() // ѡձҨ͘Ê, 2: ʹԃTimer2ضҨ͘Ê, Ǥֵ̼: ʹԃTimer1ضҨ͘Ê.
{
    TR1 = 0;
    AUXR &= ~0x01;	   // S1 BRT Use Timer1;
    AUXR |= (1 << 6);  // Timer1 set as 1T mode
    TMOD &= ~(1 << 6); // Timer1 set As Timer
    TMOD &= ~0x30;	   // Timer1_16bitAutoReload;
    TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate1) / 256);
    TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate1) % 256);
    ET1 = 0;		  //޻ֹא׏
    INTCLKO &= ~0x02; //һˤԶʱד
    TR1 = 1;
    SCON = (SCON & 0x3f) | 0x40; // UART1ģʽ, 0x00: ͬҽӆλˤԶ, 0x40: 8λ˽ߝ,ࠉҤҨ͘Ê, 0x80: 9λ˽ߝ,ڌ֨Ҩ͘Ê, 0xc0: 9λ˽ߝ,ࠉҤҨ͘Ê
    //  PS  = 1;    //ٟԅЈܶא׏
    //	ES  = 1;    //Պѭא׏
    REN = 1; //Պѭޓ˕
    P_SW1 &= 0x3f;
    P_SW1 |= 0x00; // UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
}


void UART1_TX(u8 num)
{
		DMA_UR1T_AMT = num-1;
    DMA_UR1T_CR = 0xc0;

	 
}






void UART1_DMA()
{
  if (DMA_UR1T_STA & 0x01)	//发送完成
	{
		DMA_UR1T_STA &= ~0x01;


	}
	if (DMA_UR1T_STA & 0x04)	//数据覆盖
	{
		DMA_UR1T_STA &= ~0x04;
	}
	
	if (DMA_UR1R_STA & 0x01)	//接收完成
	{
		DMA_UR1R_STA &= ~0x01;
		DMA_UR1R_CR = 0x00;
		DMA_UR1R_CR = 0xa1;	

	}
	if (DMA_UR1R_STA & 0x02)	//数据丢弃
	{
		DMA_UR1R_STA &= ~0x02;
	}
}


void UART1_TX_task()
{
	u8 i = 0;
	DMA_UR1T_AMT = 95;
	for(i=0;i<96;i++)
	{
     DMABuffer[i] = (u8)Sleep_Queue_task[i];
	}		
	  DMABuffer[95]  = 0xff;
    DMA_UR1T_CR = 0xc0;
	  delay_ms(250);
	  delay_ms(250);
}

void UART1_TX_task2()
{
	u8 i = 0;
	DMA_UR1T_AMT = 95;
	for(i=0;i<96;i++)
	{
     DMABuffer[i] = (u8)Sleep_Queue[i];
	}		
	  DMABuffer[95]  = 0xBB;
    DMA_UR1T_CR = 0xc0;
	  delay_ms(250);
	  delay_ms(250);
}


void UART1_bit(u8 dat)
{
	  DMA_UR1T_AMT = 0;
	  DMABuffer[0]  = dat;
	 DMA_UR1T_CR = 0xc0;
}







