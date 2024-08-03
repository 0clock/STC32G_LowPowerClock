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
 * @file       dmx_pit.c
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
#include "dmx_pit.h"

/**
*
* @brief    ��ʱ���жϳ�ʼ��
* @param    PIT_num_enum	ѡ��Ķ�ʱ���ж�ͨ��,����dmx_pit.h��ö�ٲ鿴
* @param    time_ms				�ж�ʱ��(��λ:ms)
* @return   void
* @notes    ��ʱ���ж�2,3,4ģʽ�̶�Ϊ16λ�Զ���װ��ֵģʽ,Ӳ���Զ�����
* Example:  init_pit_ms(PIT1,5);		// STC32G�Ķ�ʱ��1��ʼ��,�ж�ʱ��Ϊ5ms
*
**/
void init_pit_ms(PIT_num_enum num, unsigned int time_ms)
{
    unsigned int time = (unsigned int)(MAIN_FOSC / 12 / 1000) * time_ms;
    switch(num)
    {
    case PIT0:
        TMOD |= 0x00;
        TL0 = (65536 - time) % 256;
        TH0 = (65536 - time) / 256;
        TR0 = 1;
        ET0 = 1;
        break;
    case PIT1:
        TMOD |= 0x00;
        TL1 = (65536 - time) % 256;
        TH1 = (65536 - time) / 256;
        TR1 = 1;
        ET1 = 1;
        break;
    case PIT2:
        T2L = (65536 - time) % 256;
        T2H = (65536 - time) / 256;
        T2R = 1;
        ET2 = 1;
        break;
    case PIT3:
        T3L = (65536 - time) % 256;
        T3H = (65536 - time) / 256;
        T3R = 1;
        ET3 = 1;
        break;
    case PIT4:
        T4L = (65536 - time) % 256;
        T4H = (65536 - time) / 256;
        T4R = 1;
        ET4 = 1;
        break;
    }
}