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
 * @file       dmx_gpio.c
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
#include "dmx_gpio.h"

/**
*
* @brief    GPIO��ʼ��
* @param    pin					ѡ�������,����dmx_gpio.h��ö������
* @param    mode				���Ź���ģʽ
* @param    up_enable 	��������(0:������ֹ;0:����ʹ��)
* @return   void
* @notes
* Example:  init_gpio(GPIO_P00,OUT_PP ,0);	// STC32F��P00���ų�ʼ��Ϊ�������ģʽ,������ֹ
*
**/
void init_gpio(GPIO_pin_enum pin, GPIO_mode_enum mode, unsigned char up_enable)
{
    switch(mode)
    {
    // ����׼˫���
    case IO_PU:
        switch(pin / 8)
        {
        case 0:
            P0M1 &= ~(0x01 << pin % 8);
            P0M0 &= ~(0x01 << pin % 8);
            if(up_enable) P0PU |= (0x01 << pin % 8);
            else P0PU &= ~(0x01 << pin % 8);
            break;
        case 1:
            P1M1 &= ~(0x01 << pin % 8);
            P1M0 &= ~(0x01 << pin % 8);
            if(up_enable) P1PU |= (0x01 << pin % 8);
            else P1PU &= ~(0x01 << pin % 8);
            break;
        case 2:
            P2M1 &= ~(0x01 << pin % 8);
            P2M0 &= ~(0x01 << pin % 8);
            if(up_enable) P2PU |= (0x01 << pin % 8);
            else P2PU &= ~(0x01 << pin % 8);
            break;
        case 3:
            P3M1 &= ~(0x01 << pin % 8);
            P3M0 &= ~(0x01 << pin % 8);
            if(up_enable) P3PU |= (0x01 << pin % 8);
            else P3PU &= ~(0x01 << pin % 8);
            break;
        case 4:
            P4M1 &= ~(0x01 << pin % 8);
            P4M0 &= ~(0x01 << pin % 8);
            if(up_enable) P4PU |= (0x01 << pin % 8);
            else P4PU &= ~(0x01 << pin % 8);
            break;
        case 5:
            P5M1 &= ~(0x01 << pin % 8);
            P5M0 &= ~(0x01 << pin % 8);
            if(up_enable) P5PU |= (0x01 << pin % 8);
            else P5PU &= ~(0x01 << pin % 8);
            break;
        case 6:
            P6M1 &= ~(0x01 << pin % 8);
            P6M0 &= ~(0x01 << pin % 8);
            if(up_enable) P6PU |= (0x01 << pin % 8);
            else P6PU &= ~(0x01 << pin % 8);
            break;
        case 7:
            P7M1 &= ~(0x01 << pin % 8);
            P7M0 &= ~(0x01 << pin % 8);
            if(up_enable) P7PU |= (0x01 << pin % 8);
            else P7PU &= ~(0x01 << pin % 8);
            break;
        }
        break;
    // ��������
    case IN_HIZ:
        switch(pin / 8)
        {
        case 0:
            P0M1 |=  (0x01 << pin % 8);
            P0M0 &= ~(0x01 << pin % 8);
            if(up_enable) P0PU |= (0x01 << pin % 8);
            else P0PU &= ~(0x01 << pin % 8);
            break;
        case 1:
            P1M1 |=  (0x01 << pin % 8);
            P1M0 &= ~(0x01 << pin % 8);
            if(up_enable) P1PU |= (0x01 << pin % 8);
            else P1PU &= ~(0x01 << pin % 8);
            break;
        case 2:
            P2M1 |=  (0x01 << pin % 8);
            P2M0 &= ~(0x01 << pin % 8);
            if(up_enable) P2PU |= (0x01 << pin % 8);
            else P2PU &= ~(0x01 << pin % 8);
            break;
        case 3:
            P3M1 |=  (0x01 << pin % 8);
            P3M0 &= ~(0x01 << pin % 8);
            if(up_enable) P3PU |= (0x01 << pin % 8);
            else P3PU &= ~(0x01 << pin % 8);
            break;
        case 4:
            P4M1 |=  (0x01 << pin % 8);
            P4M0 &= ~(0x01 << pin % 8);
            if(up_enable) P4PU |= (0x01 << pin % 8);
            else P4PU &= ~(0x01 << pin % 8);
            break;
        case 5:
            P5M1 |=  (0x01 << pin % 8);
            P5M0 &= ~(0x01 << pin % 8);
            if(up_enable) P5PU |= (0x01 << pin % 8);
            else P5PU &= ~(0x01 << pin % 8);
            break;
        case 6:
            P6M1 |=  (0x01 << pin % 8);
            P6M0 &= ~(0x01 << pin % 8);
            if(up_enable) P6PU |= (0x01 << pin % 8);
            else P6PU &= ~(0x01 << pin % 8);
            break;
        case 7:
            P7M1 |=  (0x01 << pin % 8);
            P7M0 &= ~(0x01 << pin % 8);
            if(up_enable) P7PU |= (0x01 << pin % 8);
            else P7PU &= ~(0x01 << pin % 8);
            break;
        }
        break;
    // ��©���
    case OUT_OD:
        switch(pin / 8)
        {
        case 0:
            P0M1 |=  (0x01 << pin % 8);
            P0M0 |=  (0x01 << pin % 8);
            if(up_enable) P0PU |= (0x01 << pin % 8);
            else P0PU &= ~(0x01 << pin % 8);
            break;
        case 1:
            P1M1 |=  (0x01 << pin % 8);
            P1M0 |=  (0x01 << pin % 8);
            if(up_enable) P1PU |= (0x01 << pin % 8);
            else P1PU &= ~(0x01 << pin % 8);
            break;
        case 2:
            P2M1 |=  (0x01 << pin % 8);
            P2M0 |=  (0x01 << pin % 8);
            if(up_enable) P2PU |= (0x01 << pin % 8);
            else P2PU &= ~(0x01 << pin % 8);
            break;
        case 3:
            P3M1 |=  (0x01 << pin % 8);
            P3M0 |=  (0x01 << pin % 8);
            if(up_enable) P3PU |= (0x01 << pin % 8);
            else P3PU &= ~(0x01 << pin % 8);
            break;
        case 4:
            P4M1 |=  (0x01 << pin % 8);
            P4M0 |=  (0x01 << pin % 8);
            if(up_enable) P4PU |= (0x01 << pin % 8);
            else P4PU &= ~(0x01 << pin % 8);
            break;
        case 5:
            P5M1 |=  (0x01 << pin % 8);
            P5M0 |=  (0x01 << pin % 8);
            if(up_enable) P5PU |= (0x01 << pin % 8);
            else P5PU &= ~(0x01 << pin % 8);
            break;
        case 6:
            P6M1 |=  (0x01 << pin % 8);
            P6M0 |=  (0x01 << pin % 8);
            if(up_enable) P6PU |= (0x01 << pin % 8);
            else P6PU &= ~(0x01 << pin % 8);
            break;
        case 7:
            P7M1 |=  (0x01 << pin % 8);
            P7M0 |=  (0x01 << pin % 8);
            if(up_enable) P7PU |= (0x01 << pin % 8);
            else P7PU &= ~(0x01 << pin % 8);
            break;
        }
        break;
    // �������
    case OUT_PP:
        switch(pin / 8)
        {
        case 0:
            P0M1 &= ~(0x01 << pin % 8);
            P0M0 |=  (0x01 << pin % 8);
            if(up_enable) P0PU |= (0x01 << pin % 8);
            else P0PU &= ~(0x01 << pin % 8);
            break;
        case 1:
            P1M1 &= ~(0x01 << pin % 8);
            P1M0 |=  (0x01 << pin % 8);
            if(up_enable) P1PU |= (0x01 << pin % 8);
            else P1PU &= ~(0x01 << pin % 8);
            break;
        case 2:
            P2M1 &= ~(0x01 << pin % 8);
            P2M0 |=  (0x01 << pin % 8);
            if(up_enable) P2PU |= (0x01 << pin % 8);
            else P2PU &= ~(0x01 << pin % 8);
            break;
        case 3:
            P3M1 &= ~(0x01 << pin % 8);
            P3M0 |=  (0x01 << pin % 8);
            if(up_enable) P3PU |= (0x01 << pin % 8);
            else P3PU &= ~(0x01 << pin % 8);
            break;
        case 4:
            P4M1 &= ~(0x01 << pin % 8);
            P4M0 |=  (0x01 << pin % 8);
            if(up_enable) P4PU |= (0x01 << pin % 8);
            else P4PU &= ~(0x01 << pin % 8);
            break;
        case 5:
            P5M1 &= ~(0x01 << pin % 8);
            P5M0 |=  (0x01 << pin % 8);
            if(up_enable) P5PU |= (0x01 << pin % 8);
            else P5PU &= ~(0x01 << pin % 8);
            break;
        case 6:
            P6M1 &= ~(0x01 << pin % 8);
            P6M0 |=  (0x01 << pin % 8);
            if(up_enable) P6PU |= (0x01 << pin % 8);
            else P6PU &= ~(0x01 << pin % 8);
            break;
        case 7:
            P7M1 &= ~(0x01 << pin % 8);
            P7M0 |=  (0x01 << pin % 8);
            if(up_enable) P7PU |= (0x01 << pin % 8);
            else P7PU &= ~(0x01 << pin % 8);
            break;
        }
        break;
    }
}