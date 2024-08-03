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
 * @file       dmx_board.h
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

#ifndef _DMX_BOARD_H_
#define _DMX_BOARD_H_

#include "stc32g.h"

// ��Ƭ����Ƶ,���ǿ�������ڲ�IRC��Ƶ,STC-ISP����ѡ��Ч
// ��ѡ 22.1184MHz,24MHz,40MHz,45.1584MHz,48MHz,50.8032MHz,52MHz,56MHz,60MHz,64MHz
#define MAIN_FOSC	56000000UL

#define     T22M_ADDR			CHIPID11 	// 22.1184MHz
#define     T24M_ADDR 		CHIPID12 	// 24MHz
#define     T40M_ADDR 		CHIPID13 	// 40MHz
#define     T45M_ADDR 		CHIPID14 	// 45.1584MHz
#define     T48M_ADDR 		CHIPID15 	// 48MHz
#define     T50M_ADDR 		CHIPID16 	// 50.8032MHz
#define     T52M_ADDR 		CHIPID17 	// 52MHz
#define     T56M_ADDR 		CHIPID18 	// 56MHz
#define     T60M_ADDR 		CHIPID19 	// 60MHz
#define     T64M_ADDR 		CHIPID20 	// 64MHz
#define     VRT20M_ADDR 	CHIPID21 	// VRTRIM_20M
#define     VRT24M_ADDR 	CHIPID22 	// VRTRIM_24M
#define     VRT44M_ADDR 	CHIPID23 	// VRTRIM_44M
#define     VRT72M_ADDR 	CHIPID24 	// VRTRIM_64M

/**
*
* @brief    оƬ��ʼ��
* @param    void
* @return   void
* @notes    ���ô˺���ǰ�ɲ鿴dmx_adc.h�ļ���ö�ٵĿ�������
* Example:  init_chip();
*
**/
void init_chip(void);

/**
*
* @brief    ����ICACHE����
* @param    void
* @return   void
* @notes    ����洢�����ٻ��漼��,�ֲ��ڲ�����洢�����ٶȸ������ڲ�Ӳ���߼���������ٶ�
* Example:  open_icache();
*
**/
void open_icache(void);

/**
*
* @brief    �ر�ICACHE����
* @param    void
* @return   void
* @notes    ����洢�����ٻ��漼��,�ֲ��ڲ�����洢�����ٶȸ������ڲ�Ӳ���߼���������ٶ�
* Example:  close_icache();
*
**/
void close_icache(void);
    
#endif