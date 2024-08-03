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
 * @file       dmx_function.c
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

#include "dmx_function.h"
#include "string.h"

/**
*
* @brief    ����ֵ����
* @param    int                 ����
* @return   int                 ����ֵ
* @notes
* Example:  abs_func(-2);
*
**/
int abs_func(int x)
{
    return (x >= 0) ? x : -x;
}

/**
*
* @brief    �޷�����
* @param    x                   ��Ҫ�޷�������
* @param    min                 �޷�����Сֵ
* @param    max                 �޷������ֵ
* @return   int                 �޷�ֵ
* @notes
* Example:  limit_func(110,-100,100);   // ����ֵΪ100
*
**/
int limit_func(int x,int min,int max)
{
    return ((x) > (max) ? (max) : ((x) < -(min) ? -(min) : (x)));
}

/**
*
* @brief    ����ת�ַ���
* @param    dat                 ����
* @param    str                 �ַ���ָ��
* @return   void
* @notes
* Example:  int_to_str_func(-521,str);
*
**/
void int_to_str_func(long dat,char *str)
{
    unsigned char dat_buff[12];
    unsigned char num = 0;
    if (str == NULL)
    {
        return;
    }

    if (dat < 0)
    {
        *str++ = '-';
        dat = -dat;
    }
    else if (dat == 0)
    {
        *str++ = '0';
        return;
    }

    while (dat != 0)
    {
        dat_buff[num++] = (dat % 10);
        dat /= 10;
    }

    while (num != 0)
    {
        *str++ = (dat_buff[--num] + '0');
    }
}

/**
*
* @brief    ������ת�ַ���
* @param    dat                 ����
* @param    pointnum            С����λ��
* @param    str                 �ַ���ָ��
* @return   void
* @notes
* Example:  float_to_str_func(-521.22,2,str);
*
**/
void float_to_str_func(double dat,unsigned char pointnum,char *str)
{
    long dat_temp = (long)dat;
    long point_dat_temp = 0;
    unsigned char dat_buff[12];
    unsigned char point_dat_buff[9];
    unsigned char num = 0;
    unsigned char point_num = pointnum;

    if (str == NULL)
    {
        return;
    }

    // ����Ԥ����
    dat = dat - dat_temp;
    if (dat_temp < 0 || dat < 0)
    {
        *str++ = '-';
        dat_temp = -dat_temp;
        dat = -dat;
    }
    else if (dat == 0.0 && dat_temp == 0)
    {
        *str++ = '0';
        *str++ = '.';
        return;
    }

    // ת������������������
    do
    {
        dat_buff[num++] = (dat_temp % 10);
        dat_temp /= 10;
    }while (dat_temp != 0);
    while (num != 0)
    {
        *str++ = (dat_buff[--num] + '0');
    }

    // ת����������С������
    while(pointnum--)
    {
        dat *= 10;
    }
    point_dat_temp = (long)dat;
    num = 0;
    *str++ = '.';
    if(point_dat_temp == 0)
    {
        *str = '0';
    }
    else
    {
        while (point_num-- != 0)
        {
            point_dat_buff[num++] = (point_dat_temp % 10);
            point_dat_temp /= 10;
        }
        while (num != 0)
        {
            *str++ = (point_dat_buff[--num] + '0');
        }
    }
}