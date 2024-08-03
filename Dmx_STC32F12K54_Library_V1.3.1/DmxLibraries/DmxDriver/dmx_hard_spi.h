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
 * @file       dmx_hard_spi.h
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

#ifndef _DMX_HARD_SPI_H_
#define _DMX_HARD_SPI_H_

// Ӳ��SPI����ö��
typedef enum
{
    SPI1_SCK_P15_MOSI_P13_MISO_P14,
    SPI2_SCK_P25_MOSI_P23_MISO_P24,
    SPI3_SCK_P43_MOSI_P40_MISO_P41,
    SPI4_SCK_P32_MOSI_P34_MISO_P33,
} SPI_pin_enum;

// Ӳ��SPI����ö��
typedef enum
{
    SPI_SPEED4,
    SPI_SPEED8,
    SPI_SPEED16,
    SPI_SPEED2,
} SPI_speed_enum;

/**
*
* @brief    Ӳ��SPI��ʼ��
* @param    pin     	��ʼ����Ӳ��SPI����(dmx_hard_spi.h�ļ�����ö�ٶ���)
* @param    speed    	��ʼ����Ӳ��SPI�ٶ�(dmx_hard_spi.h�ļ�����ö�ٶ���)
* @param    mode     	Ӳ��SPIģʽѡ��
* @param    mstr     	Ӳ��SPI����ģʽѡ��(����ģʽ:0,�ӻ�ģʽ:1)
* @return   void
* @notes
* Example:  init_hard_spi(SPI2_SCK_P25_MOSI_P23_MISO_P24,SPI_SPEED16,0,0);	// ��ʼ��Ӳ��SPI:SCK_P25,MOSI_P23,MISO_P24����,�ٶ�ΪSPI_SPEED16,����ģʽ
*
**/
void init_hard_spi(SPI_pin_enum pin, SPI_speed_enum speed, unsigned char mode, unsigned char mstr);

/**
*
* @brief    Ӳ��SPIд8bit����
* @param    dat             ����
* @return   void
* @notes
* Example:  write_8bit_hard_spi(0x11);
*
**/
void write_8bit_hard_spi(unsigned char dat);

/**
*
* @brief    Ӳ��SPI��8bit����
* @return   unsigned char  	����
* @notes
* Example:  read_8bit_hard_spi();
*
**/
unsigned char read_8bit_hard_spi(void);

#endif