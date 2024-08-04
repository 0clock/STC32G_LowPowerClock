/****************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (C) 2023,AS DAIMXA
 *     copyright Copyright (C) ������DAIMXA,2023
 *     All rights reserved.
 *     ��������QQȺ��710026750
 *
 *     ��ע�������⣬�����������ݰ�Ȩ�������������ܿƼ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣�������������Ƽ��İ�Ȩ������
 *      ____    _    ___ __  ____  __    _    
 *     |  _ \  / \  |_ _|  \/  \ \/ /   / \   
 *     | | | |/ _ \  | || |\/| |\  /   / _ \  
 *     | |_| / ___ \ | || |  | |/  \  / ___ \ 
 *     |____/_/   \_\___|_|  |_/_/\_\/_/   \_\
 *
 * @file       dmx_uart.c
 * @brief      ������STC32F12K54��Դ��
 * @company    �Ϸʴ��������ܿƼ����޹�˾
 * @author     �������Ƽ���QQ��2453520483��
 * @MCUcore    STC32F12K54
 * @Software   Keil5 C251
 * @version    �鿴˵���ĵ���version�汾˵��
 * @Taobao     https://daimxa.taobao.com/
 * @Openlib    https://gitee.com/daimxa
 * @date       2023-11-10
****************************************************************************************/

#include "dmx_board.h"
#include "dmx_uart.h"

// TXD���ͱ�־λ
char TX_BUSY[5];

/**
*
* @brief    ���ڳ�ʼ��
* @param    uart_x		ѡ��Ĵ���ͨ����,����dmx_uart.h��ö��
* @param    pit_x			ѡ��Ķ�ʱ��ͨ����,����dmx_pit.h��ö��
* @param    pin				ѡ��Ĵ�������,����dmx_uart.h��ö��
* @param    baud 			������(����:9600,115200)
* @return   void
* @notes    ����1ֻ�����ö�ʱ��1��ʱ��2;����2ֻ�����ö�ʱ��2;����3ֻ�����ö�ʱ��2��ʱ��3;����4ֻ�����ö�ʱ��2��ʱ��4;
* Example:  init_uart(UART1,PIT2,UART1_RX_P36_TX_P37,115200);	// STC32G�Ĵ���1���ö�ʱ��2,RXD����ΪP36,TX����ΪP37,������Ϊ115200
*
**/
void init_uart(UART_num_enum uart_x, PIT_num_enum pit_x, UART_pin_enum pin, unsigned long baud)
{
    unsigned int time = (unsigned int)(MAIN_FOSC / baud / 4);
    switch(uart_x)
    {
    // ����1
    case UART1:
        // ��ʱ��1
        if(pit_x == PIT1)
        {
            TR1 = 0;
            S1BRT = 0;
            SCON = 0x50;
            TMOD |= 0x00;
            TL1 = (65536 - time) % 256;
            TH1 = (65536 - time) / 256;
            T1x12 = 1;
            TR1 = 1;
        }
        // ��ʱ��2
        else if(pit_x == PIT2)
        {
            T2R = 0;
            S1BRT = 1;
            SCON = 0x50;
            T2L = (65536 - time) % 256;
            T2H = (65536 - time) / 256;
            T2x12 = 1;
            T2R = 1;
        }
        ES = 1;
        P_SW1 &= 0x3f;
        switch(pin)
        {
        case UART1_RX_P30_TX_P31:
            P_SW1 |= 0x00;
            break;
        case UART1_RX_P36_TX_P37:
            P3M0 &= ~(0xC0);
            P3M1 &= ~(0xC0);
            P_SW1 |= 0x40;
            break;
        case UART1_RX_P16_TX_P17:
            P1M0 &= ~(0xC0);
            P1M1 &= ~(0xC0);
            P_SW1 |= 0x80;
            break;
        case UART1_RX_P43_TX_P44:
            P4M0 |=  (0x18);
            P4M1 |=  (0x18);
            P_SW1 |= 0xC0;
            break;
        }
        TX_BUSY[1] = 0;
        break;
    // ����2
    case UART2:
        // ��ʱ��2
        if(pit_x == PIT2)
        {
            T2R = 0;
            S2CFG |= 0x01;
            S2CON = 0x50;
            T2L = (65536 - time) % 256;
            T2H = (65536 - time) / 256;
            T2x12 = 1;
            T2R = 1;
        }
        ES2 = 1;
        switch(pin)
        {
        case UART2_RX_P10_TX_P11:
            P1M0 &= ~(0x03);
            P1M1 &= ~(0x03);
            S2_S = 0;
            break;
        case UART2_RX_P46_TX_P47:
            P4M0 &= ~(0xC0);
            P4M1 &= ~(0xC0);
            S2_S = 1;
            break;
        }
        TX_BUSY[2] = 0;
        break;
    // ����3
    case UART3:
        // ��ʱ��2
        if(pit_x == PIT2)
        {
            T2R = 0;
            S3CON = 0x10;
            T2L = (65536 - time) % 256;
            T2H = (65536 - time) / 256;
            T2x12 = 1;
            T2R = 1;
        }
        // ��ʱ��3
        else if(pit_x == PIT3)
        {
            T3R = 0;
            S3CON = 0x50;
            T3L = (65536 - time) % 256;
            T3H = (65536 - time) / 256;
            T3x12 = 1;
            T3R = 1;
        }
        ES3 = 1;
        switch(pin)
        {
        case UART3_RX_P00_TX_P01:
            P0M0 &= ~(0x03);
            P0M1 &= ~(0x03);
            S3_S = 0;
            break;
        case UART3_RX_P50_TX_P51:
            P5M0 &= ~(0x03);
            P5M1 &= ~(0x03);
            S3_S = 1;
            break;
        }
        TX_BUSY[3] = 0;
        break;
    // ����4
    case UART4:
        // ��ʱ��2
        if(pit_x == PIT2)
        {
            T2R = 0;
            S4CON = 0x10;
            T2L = (65536 - time) % 256;
            T2H = (65536 - time) / 256;
            T2x12 = 1;
            T2R = 1;
        }
        // ��ʱ��4
        else if(pit_x == PIT4)
        {
            T4R = 0;
            S4CON = 0x50;
            T4L = (65536 - time) % 256;
            T4H = (65536 - time) / 256;
            T4x12 = 1;
            T4R = 1;
        }
        ES4 = 1;
        switch(pin)
        {
        case UART4_RX_P02_TX_P03:
            P0M0 &= ~(0x0C);
            P0M1 &= ~(0x0C);
            S4_S = 0;
            break;
        case UART4_RX_P52_TX_P53:
            P5M0 |=  (0x0C);
            P5M1 |=  (0x0C);
            S4_S = 1;
            break;
        }
        TX_BUSY[4] = 0;
        break;
    }
}

/**
*
* @brief    ���ڷ����ֽ�
* @param    uart_x		ѡ��Ĵ���ͨ����,����dmx_uart.h��ö��
* @param    dat 			��Ҫ���͵��ֽ�
* @return   void
* @notes
* Example:  send_char_uart(UART1,'A');	// STC32Gͨ������1�����ŷ����ֽ�'A',���ż��ǳ�ʼ��ʱѡ�������
*
**/
void send_char_uart(UART_num_enum uart_x, char dat)
{
    while(TX_BUSY[uart_x + 1]);
    TX_BUSY[uart_x + 1] = 1;
    switch(uart_x)
    {
    case UART1:
        SBUF = dat;
        break;
    case UART2:
        S2BUF = dat;
        break;
    case UART3:
        S3BUF = dat;
        break;
    case UART4:
        S4BUF = dat;
        break;
    }
}

/**
*
* @brief    ���ڷ����ַ���
* @param    uart_x		ѡ��Ĵ���ͨ����,����dmx_uart.h��ö��
* @param    str 			��Ҫ���͵��ַ���
* @return   void
* @notes
* Example:  send_string_uart(UART1,"UART1 TEST!\n");	// STC32Gͨ������1�����ŷ����ַ���"UART1 TEST!\n",���ż��ǳ�ʼ��ʱѡ�������
*
**/
void send_string_uart(UART_num_enum uart_x, char *str)
{
    while(*str)
    {
        send_char_uart(uart_x, *str++);
    }
}

/**
*
* @brief    ��дputchar�����ض���printf����
* @param    c 		
* @return   char
* @notes
* Example:  
*
**/
char putchar(char c)
{
	send_char_uart(DEBUG_UART_N,(unsigned char)c);
	return c;
}