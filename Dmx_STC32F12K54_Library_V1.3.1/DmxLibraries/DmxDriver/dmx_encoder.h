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
 * @file       dmx_encoder.h
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

#ifndef _DMX_ENCODER_H_
#define _DMX_ENCODER_H_

#include "dmx_gpio.h"

// ����������������ö��
typedef enum
{
    ENCODER0_P34,
    ENCODER1_P35,
    ENCODER2_P12,
    ENCODER3_P04,
    ENCODER4_P06,
} ENCODER_pin_enum;

/**
*
* @brief    �����������������ʼ��
* @param    count_pin			��׽������������,����dmx_encoder.h��ö�ٲ鿴
* @param    dir_pin				��׽���������,��ͨIO�ڼ���
* @return   void
* @notes    һ����׽������������ռ��һ����ʱ��������Դ,������ö�ʱ��0�Ͳ�����ENCODER0_P34�ɼ�����������
* @notes    ENCODER0_P34��Ӧ��ʱ��0,ENCODER1_P35��Ӧ��ʱ��1,ENCODER2_P12��Ӧ��ʱ��2
* @notes    ENCODER3_P04��Ӧ��ʱ��3,ENCODER4_P06��Ӧ��ʱ��4
* Example:  init_encoder(ENCODER0_P34,GPIO_P35);	// ��������ʼ��,��������׽����ΪENCODER0_P34,��������ΪP35
*
**/
void init_encoder(ENCODER_pin_enum count_pin, GPIO_pin_enum dir_pin);

/**
*
* @brief    ������������������ݻ�ȡ
* @param    count_pin			��׽������������,����dmx_encoder.h��ö�ٲ鿴
* @return   unsigned int	���ر�������������,
* @notes    �����ͨ���������ŵĵ�ƽ�ж�
* Example:  get_encoder_count(ENCODER0_P34);	// ���ر�����ENCODER0_P34��������
*
**/
unsigned int get_encoder_count(ENCODER_pin_enum count_pin);

/**
*
* @brief    �������������������ֵ���
* @param    count_pin			��׽������������,����dmx_encoder.h��ö�ٲ鿴
* @return   void
* @notes    �ɼ����������������Ҫ���ô˺���
* Example:  get_encoder_count(ENCODER0_P34);
*
**/
void clean_encoder_count(ENCODER_pin_enum count_pin);

#endif