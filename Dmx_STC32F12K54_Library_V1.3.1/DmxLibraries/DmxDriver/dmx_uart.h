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
 * @file       dmx_uart.h
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

#ifndef _DMX_UART_H_
#define _DMX_UART_H_

#include "dmx_pit.h"

// DEBUG���ں궨��
#define DEBUG_UART_N UART1
#define DEBUG_UART_TIM PIT2
#define DEBUG_UART_PIN UART1_RX_P30_TX_P31

// ���ں�ö��
typedef enum
{
    UART1,
    UART2,
    UART3,
    UART4,
} UART_num_enum;

// ��������ö��
typedef enum
{
    UART1_RX_P30_TX_P31,
    UART1_RX_P36_TX_P37,
    UART1_RX_P16_TX_P17,
    UART1_RX_P43_TX_P44,

    UART2_RX_P10_TX_P11,
    UART2_RX_P46_TX_P47,

    UART3_RX_P00_TX_P01,
    UART3_RX_P50_TX_P51,

    UART4_RX_P02_TX_P03,
    UART4_RX_P52_TX_P53,
} UART_pin_enum;

// �����ı���
extern char TX_BUSY[5];

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
void init_uart(UART_num_enum uart_x, PIT_num_enum pit_x, UART_pin_enum pin, unsigned long baud);

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
void send_char_uart(UART_num_enum uart_x, char dat);

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
void send_string_uart(UART_num_enum uart_x, char *str);

#endif