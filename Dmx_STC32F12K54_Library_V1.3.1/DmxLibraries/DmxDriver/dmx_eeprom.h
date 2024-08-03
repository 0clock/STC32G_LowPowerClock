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
 * @file       dmx_eeprom.h
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

#ifndef _DMX_EEPROM_H_
#define _DMX_EEPROM_H_

/**
*
* @brief    ��ָ��eeprom�׵�ַ����д��n���ֽ�
* @param    addr				eeprom��ַ					
* @param    dat					д�����ݵ�������׵�ַ				
* @param    num					д���ֽڸ���		
* @return   void
* @notes   	dat�������ͱ���Ϊunsigned char
* Example:  write_eeprom(0x00,write_data,1); // ��write_data��1������д��eeprom�е�0x00��ַ����
*
**/
void write_eeprom(unsigned long addr,unsigned char *dat,unsigned int num);

/**
*
* @brief    ��ָ��eeprom�׵�ַ��������n���ֽ�
* @param    addr				eeprom��ַ					
* @param    dat					������ݵ�������׵�ַ				
* @param    num					��ȡ�ֽڸ���		
* @return   void
* @notes   	dat�������ͱ���Ϊunsigned char
* Example:  read_eeprom(0x00,read_data,1); // ��eeprom�е�0x00��ַ��������1�����ݴ����read_data����
*
**/
void read_eeprom(unsigned long addr,unsigned char *dat,unsigned int num);

/**
*
* @brief    ɾ��ָ��eeprom��ַ������
* @param    addr				eeprom��ַ					
* @return   void
* @notes   	dat�������ͱ���Ϊunsigned char
* Example:  erase_sector(0x00); // ��ָ����ַ��EEPROM��������
*
**/
void erase_sector(unsigned long addr);

#endif