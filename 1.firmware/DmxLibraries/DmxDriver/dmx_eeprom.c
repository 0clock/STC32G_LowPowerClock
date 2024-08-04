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

#include "dmx_delay.h"
#include "dmx_board.h"
#include "dmx_eeprom.h"
#include "intrins.h"

/**
*
* @brief    eeprom��������
* @param    void					
* @return   void
* @notes    dmx_eeprom.c�ļ��ڲ�����
* Example:  enable_eeprom();
*
**/
static void enable_eeprom(void)
{
		IAPEN = 1; 
		IAP_TPS = MAIN_FOSC / 1000000;
}

/**
*
* @brief    eeprom��ֹ����
* @param    void					
* @return   void
* @notes    dmx_eeprom.c�ļ��ڲ�����
* Example:  disable_eeprom();
*
**/
static void disable_eeprom(void)
{
		IAP_CONTR = 0;			
		IAP_CMD   = 0;			
		IAP_TRIG  = 0;			
		IAP_ADDRE = 0xff;   
		IAP_ADDRH = 0xff;   
		IAP_ADDRL = 0xff;
}

/**
*
* @brief    ����eeprom����.
* @param    void					
* @return   void
* @notes    dmx_eeprom.c�ļ��ڲ�����
* Example:  trig_eeprom();
*
**/
static void trig_eeprom(void)
{
		F0 = EA;    
		EA = 0;     
		IAP_TRIG = 0x5A;
		IAP_TRIG = 0xA5;                    												
		_nop_();   
		_nop_();
		_nop_();
		_nop_();
		EA = F0;
}

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
void write_eeprom(unsigned long addr,unsigned char *dat,unsigned int num)
{
		close_icache();
		enable_eeprom();                       
		IAP_CMD = 2;                        
		while(num--)
		{
			IAP_ADDRE = (unsigned char)(addr >> 16); 
			IAP_ADDRH = (unsigned char)(addr >> 8);  
			IAP_ADDRL = (unsigned char)addr;        
			IAP_DATA  = *dat++;       
			trig_eeprom(); 
			addr++;                 
		}
		disable_eeprom();    
		open_icache();
}

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
void read_eeprom(unsigned long addr,unsigned char *dat,unsigned int num)
{
		close_icache();
		enable_eeprom();                           
		IAP_CMD = 1;                           
		while(num--)
		{
			IAP_ADDRE = (unsigned char)(addr >> 16); 
			IAP_ADDRH = (unsigned char)(addr >> 8);  
			IAP_ADDRL = (unsigned char)addr;         
			trig_eeprom();                      
			*dat++ = IAP_DATA;            
			addr++;
		}
		disable_eeprom();    
		open_icache();
}

/**
*
* @brief    ɾ��ָ��eeprom��ַ������
* @param    addr				eeprom��ַ					
* @return   void
* @notes   	dat�������ͱ���Ϊunsigned char
* Example:  erase_sector(0x00); // ��ָ����ַ��EEPROM��������
*
**/
void erase_sector(unsigned long addr)
{
		close_icache();
		enable_eeprom();                      
		IAP_CMD = 3;                    
		IAP_ADDRE = (unsigned char)(addr >> 16);
		IAP_ADDRH = (unsigned char)(addr >> 8);
		IAP_ADDRL = (unsigned char)addr;         
		trig_eeprom();                     
		disable_eeprom();      
		open_icache();
}
