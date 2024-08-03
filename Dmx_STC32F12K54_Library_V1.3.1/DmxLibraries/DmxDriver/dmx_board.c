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
 * @file       dmx_board.c
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
#include "intrins.h"
#include "stdio.h"

/**
 *
 * @brief    оƬ��ʼ��
 * @param    void
 * @return   void
 * @notes    ���ô˺���ǰ�ɲ鿴dmx_adc.h�ļ���ö�ٵĿ�������
 * Example:  init_chip();
 *
 **/
void init_chip(void)
{
	// ʹ�ܷ���XFR
	EAXFR = 1;

	// �����ڲ�IRCƵ��
	switch (MAIN_FOSC)
	{
	case 22118400UL:
		CLKDIV = 0x04;
		IRTRIM = T22M_ADDR;
		VRTRIM = VRT24M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x01;
		CLKDIV = 0x00;
		WTST = 0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 24000000UL:
		CLKDIV = 0x04;
		IRTRIM = T24M_ADDR;
		VRTRIM = VRT24M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x01;
		CLKDIV = 0x00;
		WTST = 0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 40000000UL:
		CLKDIV = 0x04;
		IRTRIM = T40M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 45158400UL:
		CLKDIV = 0x04;
		IRTRIM = T45M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 48000000UL:
		CLKDIV = 0x04;
		IRTRIM = T48M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 50803200UL:
		CLKDIV = 0x04;
		IRTRIM = T50M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 52000000UL:
		CLKDIV = 0x04;
		IRTRIM = T52M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 56000000UL:
		CLKDIV = 0x04;
		IRTRIM = T56M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 2;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 60000000UL:
		CLKDIV = 0x04;
		IRTRIM = T60M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 2;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	case 64000000UL:
		CLKDIV = 0x04;
		IRTRIM = T64M_ADDR;
		VRTRIM = VRT72M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 2;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	default:
		CLKDIV = 0x04;
		IRTRIM = T56M_ADDR;
		VRTRIM = VRT44M_ADDR;
		IRCBAND &= ~0x03;
		IRCBAND |= 0x02;
		CLKDIV = 0x00;
		WTST = 2;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		break;
	}

	// ����P54���Ź���Ϊ��λ
	RSTCFG = 0x10;

	//	��˫��P07 SPL06_INT        ����©��P06 OLEDV1       ����©��P05 OLEDV2    �����裩P04 BATTERY�����ADC�����ţ�   �����裩P03 ADC6����������   ����©��PP02 EN��﮵�س��EN��   �����裩P01 GHRG   ����©��PP00 VIN/2
	P0M0 = 0x65;// P0M0 = 0x00;
    P0M1 = 0x7f;// P0M1 = 0x00;
	//	��˫��P17 INT3O        ����©��P16 ����       ��˫��P15 SCL    ��˫��P14 SDA   ��˫��P13 INT2O    ����©��P12   ����©��P11 BH_INT   ����©��P10 ����
    P1M0 = 0x47;
    P1M1 = 0x47;
	P1PU = 0x30; // IIC��������
	//	��˫��P27  KEY-       ��˫��P26  KEYMODE      �����죩P25 SCLK    ����©��P24 ����   �����죩P23 MOSI    ����©��P22 CS     ��˫��P21 DC  ��˫��P20 RES
    P2M0 = 0x3c;
    P2M1 = 0x14;
	P2SR = 0xd7;
    P2DR = 0xd7;
	//	��˫��P37 INT3��BMC050��        ����©��P36 ����       ����©��P35 ����    ��˫��P34 SW360   ����©��P33 ����    ����©��P32 ����     ����©��P31 TX   ����©��P30 RX
    P3M0 = 0x6f;
    P3M1 = 0x6f;
	//	P47 DRDYO        P46 KEYSET       P45 KEY+    P44 ����   P43 ����   P42 ����   P41 ����   P40 CCP0_EN�����������ƣ�
    P4M0 = 0x1f;
    P4M1 = 0x1f;
	P5M1 = 0x00;
	P5M0 = 0x00;

	P6M0 = 0xff;
    P6M1 = 0xff;

    P7M0 = 0xff;
    P7M1 = 0xff;

	// // ����Ĵ���ʹ��
    // P0IE = 0x1b;
    // P1IE = 0x30;
    // P2IE = 0xc0;
    // P3IE = 0x13; // 0x10
    // P4IE = 0x60;
    // P5IE = 0x11;
    // P6IE = 0x00;
    // P7IE = 0x00;

	// ����ICACHE����
	open_icache();

	// ��ʼ���Ĵ���
	ADCCFG = 0;
	AUXR = 0;
	SCON = 0;
	S2CON = 0;
	S3CON = 0;
	S4CON = 0;
	P_SW1 = 0;
	IE2 = 0;
	TMOD = 0;

	// ��ʼ��ȫ���ж�
	EA = 1;

	// ��ʼ��DEBUG����
	init_uart(DEBUG_UART_N, DEBUG_UART_TIM, DEBUG_UART_PIN, 115200);
	printf("Hello World!\r\n");
}

/**
 *
 * @brief    ����ICACHE����
 * @param    void
 * @return   void
 * @notes    ����洢�����ٻ��漼��,�ֲ��ڲ�����洢�����ٶȸ������ڲ�Ӳ���߼���������ٶ�
 * Example:  open_icache();
 *
 **/
void open_icache(void)
{
	unsigned char  tEA;
	if (WTST > 0)
	{
		tEA = EA;
		EA = 0;
		_nop_();
		_nop_();
		TA = 0xaa;
		TA = 0x55;
		ICHECR = 0x01;
		_nop_();
		_nop_();
		EA = tEA;
	}
}

/**
 *
 * @brief    �ر�ICACHE����
 * @param    void
 * @return   void
 * @notes    ����洢�����ٻ��漼��,�ֲ��ڲ�����洢�����ٶȸ������ڲ�Ӳ���߼���������ٶ�
 * Example:  close_icache();
 *
 **/
void close_icache(void)
{
	unsigned char  tEA;
	tEA = EA;
	EA = 0;
	_nop_();
	_nop_();
	TA = 0xaa;
	TA = 0x55;
	ICHECR = 0x00;
	_nop_();
	_nop_();
	EA = tEA;
}