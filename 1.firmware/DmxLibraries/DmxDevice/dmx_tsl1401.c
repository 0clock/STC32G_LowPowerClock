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
 * @file       dmx_tsl1401.c
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

#include "stc32g.h"
#include "dmx_tsl1401.h"
#include "dmx_pit.h"
#include "dmx_adc.h"
#include "isr.h"

// CCD����(һά���鼴һ��ͼ��)
unsigned char ccd0_data[128];
unsigned char ccd1_data[128];

/**
*
* @brief    TSL1401����CCD��ʼ��
* @param
* @return   void
* @notes    �û�����
* Example:  Init_TSL1401();
*
**/
void Init_TSL1401(void)
{
    init_adc(TCL1401_AO_PIN0, ADC_CLK_2);
    init_adc(TCL1401_AO_PIN1, ADC_CLK_2);
    init_pit_ms(PIT1, 1);
    init_nvic(TIME1_PRIORITY, 3);
}

/**
*
* @brief    TSL1401����CCD���ݻ�ȡ
* @param
* @return   void
* @notes    �û�����
* Example:  Get_TSL1401();
*
**/
void Get_TSL1401(void)
{
    unsigned int i = 0;
    TCL1401_CLK_LEVEL(0);
	
    TCL1401_CLK_LEVEL(1);
    TCL1401_SI_LEVEL(0);
	
    TCL1401_SI_LEVEL(1);
    TCL1401_CLK_LEVEL(0);
	
    TCL1401_CLK_LEVEL(1);
    TCL1401_SI_LEVEL(0);
	
    for(i = 0; i < 128; i++)
    {
        TCL1401_CLK_LEVEL(0);
        ccd0_data[i] = get_adc(TCL1401_AO_PIN0, ADC_8BIT);
        ccd1_data[i] = get_adc(TCL1401_AO_PIN1, ADC_8BIT);
        TCL1401_CLK_LEVEL(1);
    }
}