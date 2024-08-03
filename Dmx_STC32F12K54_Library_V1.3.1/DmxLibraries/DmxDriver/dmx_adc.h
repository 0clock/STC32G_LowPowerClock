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
 * @file       dmx_adc.h
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

#ifndef _DMX_ADC_H_
#define _DMX_ADC_H_

// ADC����ö��
typedef enum
{
    // P1�˿�ADC����
    ADC0_P10,
    ADC1_P11,
    ADCX_PXX,
    ADC3_P13,
    ADC4_P14,
    ADC5_P15,
    ADC6_P16,
    ADC7_P17,
    // P0�˿�ADC����
    ADC8_P00,
    ADC9_P01,
    ADC10_P02,
    ADC11_P03,
    ADC12_P04,
    ADC13_P05,
    ADC14_P06,
    ADC15,
} ADC_pin_enum;

// ADC����Ƶ��ö��
typedef enum
{
    ADC_CLK_2,
    ADC_CLK_4,
    ADC_CLK_6,
    ADC_CLK_8,
    ADC_CLK_10,
    ADC_CLK_12,
    ADC_CLK_14,
    ADC_CLK_16,
    ADC_CLK_18,
    ADC_CLK_20,
    ADC_CLK_22,
    ADC_CLK_24,
    ADC_CLK_26,
    ADC_CLK_28,
    ADC_CLK_30,
    ADC_CLK_32,
} ADC_clk_enum;

// ADC�ֱ���ö��
typedef enum
{
    ADC_8BIT,
    ADC_10BIT,
    ADC_12BIT,
} ADC_res_enum;

/**
*
* @brief    adc��ʼ��
* @param    adc_n     	ѡ����Ҫ��ʼ��adc����(dmx_adc.h�ļ�����ö�ٶ���)
* @return   void
* @notes    ���ô˺���ǰ�ɲ鿴dmx_adc.h�ļ���ö�ٵĿ�������
* Example:  init_adc(ADC0_P10,ADC_CLK_2);	// ��ʼ��ADC0_P10����,ʱ��Ƶ��Ϊ(ϵͳʱ��/2)
*
**/
void init_adc(ADC_pin_enum adc_n, ADC_clk_enum clk);

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
unsigned int get_adc(ADC_pin_enum adc_n, ADC_res_enum adc_res);

#endif