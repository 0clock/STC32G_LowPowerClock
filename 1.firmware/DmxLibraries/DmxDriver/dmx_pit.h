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
 * @file       dmx_pit.h
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

#ifndef _DMX_PIT_H_
#define _DMX_PIT_H_

// ��ʱ���ж�ͨ��ö��
typedef enum
{
    PIT0,
    PIT1,
    PIT2,
    PIT3,
    PIT4,
} PIT_num_enum;

/**
*
* @brief    ��ʱ���жϳ�ʼ��
* @param    PIT_num_enum	ѡ��Ķ�ʱ���ж�ͨ��,����dmx_pit.h��ö�ٲ鿴
* @param    time_ms				�ж�ʱ��(��λ:ms)
* @return   void
* @notes    ��ʱ���ж�2,3,4ģʽ�̶�Ϊ16λ�Զ���װ��ֵģʽ,Ӳ���Զ�����
* Example:  init_pit_ms(PIT1,5);	// STC32G�Ķ�ʱ��1��ʼ��,�ж�ʱ��Ϊ5ms
*
**/
void init_pit_ms(PIT_num_enum num, unsigned int time_ms);

#endif