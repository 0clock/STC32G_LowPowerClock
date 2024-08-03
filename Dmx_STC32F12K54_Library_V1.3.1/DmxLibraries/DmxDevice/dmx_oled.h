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
 * @file       dmx_oled.h
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

#ifndef _DMX_OLED_H_
#define _DMX_OLED_H_

#include "dmx_font.h"

// ͨ�ŷ�ʽѡ��,0:���SPI,1:Ӳ��SPI
#define OLED_HARD_SPI       1

// ��OLED��Ļ�����������
#define OLED_WIDTH          128
#define OLED_HEIGH          64

// SPIЭ��SCK,MOSI,MISO����
#if OLED_HARD_SPI
// Ӳ��SPI����
#define OLED_PIN 		SPI2_SCK_P25_MOSI_P23_MISO_P24
// Ӳ��SPI�ٶ�
#define OLED_SPPED 	SPI_SPEED4
#else
// ���SPI����
#define OLED_D0  			P25
#define OLED_D1 			P23
// ���SPI���ŵ�ƽ����
#define OLED_D0_LEVEL(level) (OLED_D0=level)
#define OLED_D1_LEVEL(level) (OLED_D1=level)
#endif
// SPI���ź궨��
#define OLED_RES 			P20
#define OLED_DC 			P21
#define OLED_CS 			P22
// SPI���ŵ�ƽ����
#define OLED_RES_LEVEL(level)	(OLED_RES=level)
#define OLED_DC_LEVEL(level) 	(OLED_DC=level)
#define OLED_CS_LEVEL(level) 	(OLED_CS=level)
// SPI��ʱ10ms
#define OLED_Delay10ms(time) 	delay_ms(time*10)
/**
*
* @brief    0.96��6��OLED��Ļ��ʼ��
* @param    void
* @return   void
* @notes    �޸ĹܽŶ������dmx_oled.h�ļ���ĺ궨��ܽŽ����޸�
* Example:  Init_OLED();
*
**/
void Init_OLED(void);

/**
*
* @brief    0.96��6��OLED��Ļ����
* @param    data
* @return   void
* @notes
* Example:  Draw_Point_OLED(0xff);
*
**/
void Draw_Point_OLED(unsigned char dat);

/**
*
* @brief    0.96��6��OLED��Ļ���õ�����
* @param    x           x����������(0~127)
* @param    y           y����������(0~7)
* @return   void
* @notes    ���Ͻ�����Ϊ(0,0)
* Example:  Set_Pos_OLED(0,0);
*
**/
void Set_Pos_OLED(unsigned char x, unsigned char y);

/**
*
* @brief    0.96��6��OLED��Ļ����
* @param
* @return   void
* @notes
* Example:  Fill_OLED();
*
**/
void Fill_OLED(void);

/**
*
* @brief    0.96��6��OLED��Ļ����
* @param
* @return   void
* @notes    ִ�к�Ч����ȫ��,����
* Example:  Clean_OLED();
*
**/
void Clean_OLED(void);

/**
*
* @brief    0.96��6��OLED��Ļ��ʾ�ַ���
* @param    x           x����������(0~127)
* @param    y           y����������(0~7)
* @param    biglow      ö���ַ���ʾ��С
* @param    str         ��Ҫ��ʾ���ַ���
* @return   void
* @notes
* Example:  Show_String_OLED(0,0,"123",Show8x16);    	// ����Ļ���Ͻ���ʾ8X16��С�ġ�123��
*
**/
void Show_String_OLED(unsigned char x, unsigned char y, char str[], SHOW_size_enum fontsize);

/**
*
* @brief    0.96��6��OLED��Ļ��ʾInt������
* @param    x                   x����������(0~127)
* @param    y                   y����������(0~7)
* @param    data                Ҫ��ʾ��int�ͱ���
* @param    num                 Ҫ��ʾ��int�ͱ���λ��
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_Int_OLED(0,0,a,10,Show8x16);    //����Ļ���Ͻ���ʾ8x16��С��a����������
*
**/
void Show_Int_OLED(char x, char y, const long dat, unsigned char num, SHOW_size_enum fontsize);

/**
*
* @brief    0.96��6��OLED��Ļ��ʾFloat������
* @param    x                   x����������(0~127)
* @param    y                   y����������(0~7)
* @param    data                Ҫ��ʾ��float�ͱ���
* @param    num                 Ҫ��ʾ��float�ͱ�������λ��
* @param    pointnum            Ҫ��ʾ��float�ͱ���С����λ��
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes    Float������С����������ʾ6λ
* Example:  Show_Float_OLED(64,0,3.1415926,1,6,Show8x16);    //����Ļ���Ͻ���ʾ8X16��С�ġ�3.142��
*
**/
void Show_Float_OLED(char x, char y, const double dat, unsigned char num, unsigned char pointnum, SHOW_size_enum fontsize);

/**
*
* @brief    0.96��6��OLED��Ļ��ʾ16X16���ĺ���
* @param    x           x����������(0~127)
* @param    y           y����������(0~7)
* @param    chinese     ����������׵�ַ
* @param    startnum    ��ʼ��ʾ�ĺ���λ��
* @param    endnum      ������ʾ�ĺ���λ��
* @return   void
* @notes    ���ȡģ�ڶ��庺���ֿ�ʱ��˵��
* Example:  Show_Chinese16x16_OLED(0,0,Chinese16x16,0,3);   // ����Ļ���Ͻ���ʾ16X16��С�ĺ��֡���������
*
**/
void Show_Chinese16x16_OLED(char x, char y, const unsigned char *chinese, char startnum, char endnum);

/**
*
* @brief    0.96��6��OLED��Ļ��ʾBMPͼƬ
* @param    x           x����������(0~127)
* @param    y           y����������(0~7)
* @param    wide        ͼƬʵ�����ؿ��(1~127)
* @param    high        ͼƬʵ�����ظ߶�(1~64)
* @param    bmp         ͼƬ�����׵�ַ
* @return   void
* @notes    ͼƬ�������Ϊ128��64
* Example:  OLED_ShowBMP(0, 0, 128, 54, OledDMXLOGO128X54);
*
**/
void Show_BMP_OLED(unsigned char x, unsigned char y, unsigned char width, unsigned char height, const unsigned char *bmp);

/**
*
* @brief    OLED��Ļ��ʾ����ͷ��ֵ��ͼ��
* @param    x                   x��������
* @param    y                   y��������
* @param    image               ͼ������
* @param    width               ͼ������ʵ�ʿ��
* @param    heigh               ͼ������ʵ�ʸ߶�
* @param    show_width          ͼ��������ʾ���
* @param    show_height         ͼ��������ʾ�߶�
* @param    threshold           ��ֵ����ֵ
* @return   void
* @notes
* Example:  Show_Image_Oled(0,0,OriginalImageData[0],188,120,80,60,80);
*
**/
void Show_Image_Oled (unsigned short x, unsigned short y, const unsigned char *image, unsigned short width, unsigned short height, unsigned short show_width, unsigned short show_height, unsigned char threshold);

#endif