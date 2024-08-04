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
 * @file       dmx_encoder.c
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
#include "dmx_encoder.h"

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
void init_encoder(ENCODER_pin_enum count_pin, GPIO_pin_enum dir_pin)
{
    init_gpio(dir_pin, IN_HIZ, 0);
    switch(count_pin)
    {
    case ENCODER0_P34:
        init_gpio(GPIO_P34, IN_HIZ, 0);
        T0_CT = 1;
        TL0 = 0xFF;
        TH0 = 0xFF;
        TR0 = 1;
        ET0 = 1;
        break;
    case ENCODER1_P35:
        init_gpio(GPIO_P35, IN_HIZ, 0);
        T1_CT = 1;
        TL1 = 0xFF;
        TH1 = 0xFF;
        TR1 = 1;
        ET1 = 1;
        break;
    case ENCODER2_P12:
        init_gpio(GPIO_P12, IN_HIZ, 0);
        T2_CT = 1;
        T2L = 0xFF;
        T2H = 0xFF;
        T2R = 1;
        ET2 = 1;
        break;
    case ENCODER3_P04:
        init_gpio(GPIO_P04, IN_HIZ, 0);
        T4T3M |= 0x0C;
        T3L = 0xFF;
        T3H = 0xFF;
        ET3 = 1;
        break;
    case ENCODER4_P06:
        init_gpio(GPIO_P06, IN_HIZ, 0);
        T4T3M |= 0xC0;
        T4L = 0xFF;
        T4H = 0xFF;
        ET4 = 1;
        break;
    }
}

/**
*
* @brief    ������������������ݻ�ȡ
* @param    count_pin			��׽������������,����dmx_encoder.h��ö�ٲ鿴
* @return   unsigned int	���ر�������������,
* @notes    �����ͨ���������ŵĵ�ƽ�ж�
* Example:  get_encoder_count(ENCODER0_P34);	// ���ر�����ENCODER0_P34��������
*
**/
unsigned int get_encoder_count(ENCODER_pin_enum count_pin)
{
    unsigned int count;
    switch(count_pin)
    {
    case ENCODER0_P34:
        count = TL0;
        count = TH0 << 8 | count;
        break;
    case ENCODER1_P35:
        count = TL1;
        count = TH1 << 8 | count;
        break;
    case ENCODER2_P12:
        count = T2L;
        count = T2H << 8 | count;
        break;
    case ENCODER3_P04:
        count = T3L;
        count = T3H << 8 | count;
        break;
    case ENCODER4_P06:
        count = T4L;
        count = T4H << 8 | count;
        break;
    }
    return count;
}

/**
*
* @brief    �������������������ֵ���
* @param    count_pin			��׽������������,����dmx_encoder.h��ö�ٲ鿴
* @return   void
* @notes    �ɼ����������������Ҫ���ô˺���
* Example:  get_encoder_count(ENCODER0_P34);
*
**/
void clean_encoder_count(ENCODER_pin_enum count_pin)
{
    switch(count_pin)
    {
    case ENCODER0_P34:
        TR0 = 0;
        TL0 = 0;
        TH0 = 0;
        TR0 = 1;
        break;
    case ENCODER1_P35:
        TR1 = 0;
        TL1 = 0;
        TH1 = 0;
        TR1 = 1;
        break;
    case ENCODER2_P12:
        T2R = 0;
        T2L = 0;
        T2H = 0;
        T2R = 1;
        break;
    case ENCODER3_P04:
        T3R = 0;
        T3L = 0;
        T3H = 0;
        T3R = 1;
        break;
    case ENCODER4_P06:
        T4R = 0;
        T4L = 0;
        T4H = 0;
        T4R = 1;
        break;
    }
}