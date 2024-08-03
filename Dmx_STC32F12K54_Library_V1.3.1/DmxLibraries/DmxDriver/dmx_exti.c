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
 * @file       dmx_exti.c
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
#include "dmx_exti.h"

/**
*
* @brief    �ⲿ�жϳ�ʼ��
* @param    EXTI_num_enum	ѡ����ⲿ�жϴ�������,����dmx_exti.h��ö�ٲ鿴
* @param    mode					�жϴ�����ʽ(0:�½��ش���;1:�����غ��½��ش���)
* @return   void
* @notes    �ⲿ�ж�2,3,4�жϴ�����ʽֻ��Ϊ�½��ش���
* Example:  init_exti(EXTI0_P32,0);	// STC32G��P32���ų�ʼ��Ϊ�ⲿ�ж�����,�½��ش���
*
**/
void init_exti(EXTI_num_enum num, unsigned char mode)
{
    switch(num)
    {
    case EXTI0_P32:
        IT0 = !mode;
        EX0 = 1;
        break;
    case EXTI1_P33:
        IT1 = !mode;
        EX1 = 1;
        break;
    case EXTI2_P36:
        EX2 = 1;
        break;
    case EXTI3_P37:
        EX3 = 1;
        break;
    case EXTI4_P30:
        EX4 = 1;
        break;
    }
}
