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
 * @file       dmx_delay.c
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
#include "dmx_delay.h"

/**
*
* @brief    �����ʱ����
* @param    ms					��Ҫ��ʱ��ʱ��
* @return   void
* @notes    ��λ:ms
* Example:  delay_ms(10);	// �����ʱ10ms
*
**/
void delay_ms(int ms)
{
    unsigned int i;
    while(ms--)
    {
        i = MAIN_FOSC / 6030;
        while(--i);
    }
}

/**
 * @brief    �����ʱ������΢�뼶��
 * 
 * @param    us ��Ҫ��ʱ��ʱ�䣬��λ:΢��
 * @return   void
 * @notes    ʹ�����ѭ��ʵ�ֵ�΢�뼶��ʱ����ʱʱ����ܻ���������Ż���CPUƵ�ʱ仯������
 * @example  delay_us(100);  // �����ʱ100΢��
 */
void delay_us(int us)
{
    unsigned int i;
    while(us--)
    {
        i = MAIN_FOSC / 6030 / 1000;  // ������Ƶ��Ŀ����ʱ����ѭ������
        while(--i);
    }
}
