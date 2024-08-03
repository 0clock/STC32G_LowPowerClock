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
 * @file       dmx_isr.h
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

#ifndef _ISR_H_
#define _ISR_H_

// �ж���ö��
typedef enum
{
    INT0_PRIORITY,
    INT1_PRIORITY,
    INT4_PRIORITY,

    TIME0_PRIORITY,
    TIME1_PRIORITY,

    UART1_PRIORITY,
    UART2_PRIORITY,
    UART3_PRIORITY,
    UART4_PRIORITY,
} ISR_nvic_enum;

/**
*
* @brief    �ж����ȼ�����
* @param    isr_nvic				�ж���,����isr.h��ö�ٲ鿴
* @param    priority				���ȼ�(0~3,3��Ϊ��߼�)
* @return   void
* @notes    ���ڷ�������ʱ,�����ж����ȼ�Ҫ���ڶ�ʱ���ж����ȼ�
* Example:  init_nvic(TIME1_PRIORITY,3);	// ��ʱ��1�ж����ȼ�����Ϊ��߼���3��
*
**/
void init_nvic(ISR_nvic_enum isr_nvic, unsigned char priority);

#endif