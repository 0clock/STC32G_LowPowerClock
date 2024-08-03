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
 * @file       dmx_function.h
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

#ifndef _DMX_FUNCTION_H_
#define _DMX_FUNCTION_H_

/**
*
* @brief    ����ֵ����
* @param    int                 ����
* @return   int                 ����ֵ
* @notes
* Example:  abs_func(-2);
*
**/
int abs_func(int x);

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
int limit_func(int x,int min,int max);

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

void int_to_str_func(long dat,char *str);
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
void float_to_str_func(double dat,unsigned char pointnum,char *str);

#endif