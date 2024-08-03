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
 * @file       dmx_tsl1401.h
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

#ifndef _DMX_TSL1401_H_
#define _DMX_TSL1401_H_

// CCD�ع�ʱ��,��λMS
#define TCL1401_EXPOSURE_TIME		10

// CCD��CLK����
#define TCL1401_CLK_PIN					P26
// CCD��SI����
#define TCL1401_SI_PIN     			P32
// CCD��AO����0
#define TCL1401_AO_PIN0     		ADC4_P14
// CCD��AO����1
#define TCL1401_AO_PIN1     		ADC0_P10

// CCD�ܽŵ�ƽ����
#define TCL1401_CLK_LEVEL(level) 	(TCL1401_CLK_PIN = (level))
#define TCL1401_SI_LEVEL(level) 	(TCL1401_SI_PIN  = (level))

// ����CCD����(һά���鼴һ��ͼ��)
extern unsigned char ccd0_data[128];
extern unsigned char ccd1_data[128];

/**
*
* @brief    TSL1401����CCD��ʼ��
* @param
* @return   void
* @notes    �û�����
* Example:  Init_TSL1401();
*
**/
void Init_TSL1401(void);

/**
*
* @brief    TSL1401����CCD���ݻ�ȡ
* @param
* @return   void
* @notes    �û�����
* Example:  Get_TSL1401();
*
**/
void Get_TSL1401(void);

#endif