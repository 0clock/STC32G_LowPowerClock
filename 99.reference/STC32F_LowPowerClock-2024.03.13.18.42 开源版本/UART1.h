#ifndef __UART1_H
#define __UART1_H
#include "STC32F.h"
#define Baudrate1           115200L


void UART1_DMA();
void UART1_config();
void DMA_Config(void);
void UART1_TX(u8 num);

void UART1_TX_task();

void UART1_TX_task2();

void UART1_bit(u8 dat);


#endif