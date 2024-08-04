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
 * @file       dmx_adc.c
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
#include "dmx_adc.h"

/**
*
* @brief    adc��ʼ��
* @param    adc_n     	ѡ����Ҫ��ʼ��adc����(dmx_adc.h�ļ�����ö�ٶ���)
* @return   void
* @notes    ���ô˺���ǰ�ɲ鿴dmx_adc.h�ļ���ö�ٵĿ�������
* Example:  init_adc(ADC0_P10,ADC_CLK_2);	// ��ʼ��ADC0_P10����,ʱ��Ƶ��Ϊ(ϵͳʱ��/2)
*
**/
void init_adc(ADC_pin_enum adc_n, ADC_clk_enum clk)
{
    ADC_POWER = 1;
    ADC_CONTR &= 0xF0;
    ADC_CONTR |= adc_n;
    if(adc_n / 8 == 1)
    {
        P0M1 |=  (0x01 << adc_n % 8);
        P0M0 &= ~(0x01 << adc_n % 8);
    }
    else if(adc_n / 8 == 0)
    {
        P1M1 |=  (0x01 << adc_n % 8);
        P1M0 &= ~(0x01 << adc_n % 8);
    }
    ADCCFG |= 0xF0;
    ADCCFG |= clk;
}

/**
*
* @brief    adcת��һ�μ���ȡһ��adֵ
* @param    adc_n     	ѡ����Ҫ��ȡ��adc����(dmx_adc.h�ļ�����ö�ٶ���)
* @param    adc_res     ѡ��adc����(8bit,10bit,12bit)
* @return   void
* @notes
* Example:  get_adc(ADC0_P10, ADC_12BIT);  // ��ȡһ��ADC0_P10���ŵ�adֵ,����Ϊ12bit���ú�������ֵ���Ϊ2��ʮ���η�
*
**/
unsigned int get_adc(ADC_pin_enum adc_n, ADC_res_enum adc_res)
{
    ADC_CONTR &= 0xF0;
    ADC_CONTR |= adc_n;
    ADC_RES = 0;
    ADC_RESL = 0;
    ADC_START = 1;
    while(ADC_FLAG == 0);
    ADC_FLAG = 0;
    return (((unsigned int)ADC_RES << 8) | ADC_RESL) >> (4 - (adc_res * 2));
}