/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#ifndef	__STC32G_CLOCK_H
#define	__STC32G_CLOCK_H

#include	"config.h"

//========================================================================
//                              ʱ������
//========================================================================

#define		MainClockSel(n)	CLKSEL = (CLKSEL & ~0x0f) | (n)				/* ϵͳʱ��ѡ�� */
#define		PLLClockSel(n)	CLKSEL = (CLKSEL & ~0x80) | (n<<7)		/* PLLʱ��ѡ�� */
#define		HSIOClockSel(n)	CLKSEL = (CLKSEL & ~0x40) | (n<<6)		/* ����IOʱ��ѡ�� */
#define		PLLClockIn(n)		USBCLK = (USBCLK & ~0x60) | (n<<4)		/* ϵͳʱ�� n ��Ƶ��ΪPLLʱ��Դ,ȷ����Ƶ��Ϊ12M */
#define		PLLEnable(n)		USBCLK = (USBCLK & ~0x80) | (n<<7)		/* PLL��Ƶʹ�� */
#define		HSClockDiv(n)		HSCLKDIV = (n)		/* ����IOʱ�ӷ�Ƶϵ�� */

//========================================================================
//                              ��������
//========================================================================

/* ϵͳʱ��ѡ����� */
#define MCLKSEL_HIRC       0x00
#define MCLKSEL_XIRC       0x01
#define MCLKSEL_X32K       0x02
#define MCLKSEL_I32K       0x03
#define MCLKSEL_PLL        0x04
#define MCLKSEL_PLL2       0x08
#define MCLKSEL_I48M       0x0c

/* PLLʱ��ѡ����� */
#define PLL_96M         0
#define PLL_144M        1

/* ����IOʱ��ѡ����� */
#define HSCK_MCLK       0
#define HSCK_PLL        1

/* ϵͳʱ�� n ��Ƶ��ΪPLLʱ��Դ����,ȷ����Ƶ��Ϊ12M */
#define ENCKM           0x80
#define PCKI_MSK        0x60
#define PCKI_D1         0x00
#define PCKI_D2         0x20
#define PCKI_D4         0x40
#define PCKI_D8         0x60

//========================================================================
//                              �ⲿ����
//========================================================================

void HSPllClkConfig(u8 clksrc, u8 pllsel, u8 div);

#endif