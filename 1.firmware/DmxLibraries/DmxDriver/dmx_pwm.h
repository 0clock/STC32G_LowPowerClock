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
 * @file       dmx_pwm.h
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

#ifndef _DMX_PWM_H_
#define _DMX_PWM_H_

#define PWM_DUTY_MAX 10000

typedef enum
{
    // ��һ��PWM:PWMA
    // PWMA�ĵ�PWM��ͬͨ�������ͬʱ���,��ÿ��ͨ��ֻ�����һ��PWM
    // ͨ��1
    PWMA1_P_P10, PWMA1_N_P11,
    PWMA1_P_P20, PWMA1_N_P21,
    PWMA1_P_P60, PWMA1_N_P61,	// ����ռλʹ��,STC32F12K54�޴�����
    // ͨ��2
    PWMA2_P_P54, PWMA2_N_P13,
    PWMA2_P_P22, PWMA2_N_P23,
    PWMA2_P_P62, PWMA2_N_P63,	// ����ռλʹ��,STC32F12K54�޴�����
    // ͨ��3
    PWMA3_P_P14, PWMA3_N_P15,
    PWMA3_P_P24, PWMA3_N_P25,
    PWMA3_P_P64, PWMA3_N_P65,	// ����ռλʹ��,STC32F12K54�޴�����
    // ͨ��4
    PWMA4_P_P16, PWMA4_N_P17,
    PWMA4_P_P26, PWMA4_N_P27,
    PWMA4_P_P66, PWMA4_N_P67,	// ����ռλʹ��,STC32F12K54�޴�����
    PWMA4_P_P34, PWMA4_N_P33,

    // �ڶ���PWM:PWMB
    // PWMB��PWM��ͬͨ�������ͬʱ���,��ÿ��ͨ��ֻ�����һ��PWM
    // ͨ��1
    PWMB1_P20,
    PWMB1_P17,
    PWMB1_P00,
    PWMB1_P74,	// ����ռλʹ��,STC32F12K54�޴�����
    // ͨ��2
    PWMB2_P21,
    PWMB2_P54,
    PWMB2_P01,
    PWMB2_P75,	// ����ռλʹ��,STC32F12K54�޴�����
    // ͨ��3
    PWMB3_P22,
    PWMB3_P33,
    PWMB3_P02,
    PWMB3_P76,	// ����ռλʹ��,STC32F12K54�޴�����
    // ͨ��4
    PWMB4_P23,
    PWMB4_P34,
    PWMB4_P03,
    PWMB4_P77,	// ����ռλʹ��,STC32F12K54�޴�����
} PWM_pin_enum;

/**
*
* @brief    pwm��ʼ��
* @param    pin     		ѡ����Ҫ��ʼ��pwm����(dmx_pwm.h�ļ�����ö�ٶ���)
* @param    freq     		��ʼ��pwm���ŵ�Ƶ��
* @param    duty     		��ʼ��pwm���ŵĳ�ʼռ�ձ�
* @return   void
* @notes    ���ô˺���ǰ�ɲ鿴dmx_pwm.h�ļ���ö�ٵĿ�������
* Example:  init_pwm(PWMA1_P_P20,100,1000);	// ��ʼ��PWMA1_P_P20��,Ƶ��Ϊ100HZ,ռ�ձ�Ϊ1000,��ռ�ձ�Ϊ10%
*
**/
void init_pwm(PWM_pin_enum pin, unsigned int freq, unsigned int duty);

/**
*
* @brief    pwm����ռ�ձ�
* @param    pin     		������Ҫ����ռ�ձȵ�pwm����(dmx_pwm.h�ļ�����ö�ٶ���)
* @param    duty     		����ռ�ձ�
* @return   void
* @notes    ���ô˺���ǰpwm�������ʼ��
* Example:  set_duty_pwm(PWMA1_P_P20,1600);	// ����PWMA1_P_P20��,ռ�ձ�Ϊ1600,��ռ�ձ�Ϊ16%
*
**/
void set_duty_pwm(PWM_pin_enum pin, unsigned int duty);

#endif