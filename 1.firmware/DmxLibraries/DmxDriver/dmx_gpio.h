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
 * @file       dmx_gpio.h
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

#ifndef _DMX_GPIO_H_
#define _DMX_GPIO_H_

// GPIO����ö��
typedef enum
{
    // P0�˿�
    GPIO_P00,
    GPIO_P01,
    GPIO_P02,
    GPIO_P03,
    GPIO_P04,
    GPIO_P05,
    GPIO_P06,
    GPIO_P07,
    // P1�˿�
    GPIO_P10,
    GPIO_P11,
    GPIO_P12,
    GPIO_P13,
    GPIO_P14,
    GPIO_P15,
    GPIO_P16,
    GPIO_P17,
    // P2�˿�
    GPIO_P20,
    GPIO_P21,
    GPIO_P22,
    GPIO_P23,
    GPIO_P24,
    GPIO_P25,
    GPIO_P26,
    GPIO_P27,
    // P3�˿�
    GPIO_P30,
    GPIO_P31,
    GPIO_P32,
    GPIO_P33,
    GPIO_P34,
    GPIO_P35,
    GPIO_P36,
    GPIO_P37,
    // P4�˿�
    GPIO_P40,
    GPIO_P41,
    GPIO_P42,
    GPIO_P43,
    GPIO_P44,
    GPIO_P45,
    GPIO_P46,
    GPIO_P47,
    // P5�˿�
    GPIO_P50,
    GPIO_P51,
    GPIO_P52,
    GPIO_P53,
    GPIO_P54,
    GPIO_P55,
    GPIO_P56,
    GPIO_P57,
    // P6�˿�
    GPIO_P60,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P61,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P62,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P63,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P64,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P65,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P66,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P67,	// ����ռλʹ��,STC32F12K54�޴�����
    // P7�˿�
    GPIO_P70,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P71,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P72,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P73,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P74,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P75,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P76,	// ����ռλʹ��,STC32F12K54�޴�����
    GPIO_P77,	// ����ռλʹ��,STC32F12K54�޴�����
} GPIO_pin_enum;

// GPIO�˿�ģʽö��
typedef enum
{
    IO_PU,	// ׼˫���
    IN_HIZ,	// ��������
    OUT_OD, // ©����·
    OUT_PP, // �������
} GPIO_mode_enum;

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
void init_gpio(GPIO_pin_enum pin, GPIO_mode_enum mode, unsigned char up_enable);

#endif