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
 * @file       dmx_ips.h
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

#ifndef _DMX_IPS_H_
#define _DMX_IPS_H_

#include "dmx_font.h"

// ͨ�ŷ�ʽѡ��,0:���SPI,1:Ӳ��SPI
#define IPS_HARD_SPI    1
// ��Ļ�ͺ�ѡ��,0:IPS114,1:IPS130,2:IPS154,3:IPS200
#define IPS_MODEL       0
// ������Ļ��ʾ����,0:����,1:����180��,2:����,3:����180��
#define IPS_SHOW_DIR    2

// IPS��Ļ�ܽź궨��
#if IPS_HARD_SPI
// Ӳ��SPI����
#define IPS_PIN 				SPI2_SCK_P25_MOSI_P23_MISO_P24
// Ӳ��SPI�ٶ�
#define IPS_SPPED 			SPI_SPEED4
#else
// IPS,SCL�ܽ�
#define IPS_SCL  				P25
// IPS,SDA�ܽ�
#define IPS_SDA  				P23
// SCL,SDA��ƽ����
#define IPS_SCL_LEVEL(level)	(IPS_SCL=level)
#define IPS_SDA_LEVEL(level)	(IPS_SDA=level)
#endif
// IPS,RES�ܽ�
#define IPS_RES  				P21
// IPS,DC�ܽ�
#define IPS_DC					P24
// IPS,CS�ܽ�
#define IPS_CS   				P27

// ��IPS��Ļ�ܽŽ��иߵ͵�ƽ����
#define IPS_RES_LEVEL(level)    (IPS_RES=level)
#define IPS_DC_LEVEL(level)     (IPS_DC=level)
#define IPS_CS_LEVEL(level)     (IPS_CS=level)

// IPS��Ļ��ʱ
#define IPS_DELAY_MS(time) 			(delay_ms(time))

/**
*
* @brief    ��ʼ��IPS��Ļ
* @param    void
* @return   void
* @notes    �޸Ĺܽſ���dmx_ips.h�ļ���ĺ궨��ܽŽ����޸�
* Example:  Init_IPS();
*
**/
void Init_IPS(void);

/**
*
* @brief    IPS��Ļ������ʼ�ͽ�������
* @param    x1                x������ʼ��(0~240)
* @param    y1                y������ʼ��(0~135)
* @param    x2                x���������(0~240)
* @param    y2                y���������(0~135)
* @return   void
* @notes
* Example:  Set_Pos_IPS(0,0,0,0);
*
**/
void Set_Pos_IPS(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/**
*
* @brief    IPS��Ļ��ָ��λ�û���
* @param    x                   x��������(0~240)
* @param    y                   y��������(0~135)
* @param    color               �����ɫ(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Draw_Point_IPS(0,0,BLACK);
*
**/
void Draw_Point_IPS(unsigned int x, unsigned int y, COLOR_enum color);

/**
*
* @brief    IPS��Ļ���ָ������
* @param    x1                  x������ʼ��(0~240)
* @param    y1                  y������ʼ��(0~135)
* @param    x2                  x���������(0~240)
* @param    y2                  y���������(0~135)
* @param    color               �����ɫ(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Fill_IPS(0,0,240,135,WHITE);    // IPS114��Ļȫ��
*
**/
void Fill_IPS(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, COLOR_enum color);

/**
*
* @brief    IPS��Ļ����
* @param    color               �����ɫ(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Clean_IPS(WHITE);   // IPS114��Ļȫ��
*
**/
void Clean_IPS(COLOR_enum color);

/**
*
* @brief    IPS��Ļ��ʾ�����ַ�
* @param    x                   x��������(0~240)
* @param    y                   y��������(0~135)
* @param    data                �ַ�
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_Char_IPS(0,0,'D',BLACK,WHITE,Show8x16);    // �����Ͻ���ʾ��ɫ������ɫ��8X16�ַ�'D'
*
**/
void Show_Char_IPS(unsigned int x, unsigned int y, unsigned char dat, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    IPS��Ļ��ʾ�ַ���
* @param    x                   x��������(0~240)
* @param    y                   y��������(0~135)
* @param    *p                  Ҫ��ʾ���ַ���
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_String_IPS(0,0,"DMX",BLACK,WHITE,Show8x16);    // �����Ͻ���ʾ��ɫ������ɫ��8X16�ַ���'DMX'
*
**/
void Show_String_IPS(unsigned int x, unsigned int y, char *p, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    IPS��Ļ��ʾInt�ͱ���
* @param    x                   x��������(IPS1.14�����ģʽ0~240)
* @param    y                   y��������(IPS1.14�����ģʽ0~135)
* @param    data                Ҫ��ʾ��int�ͱ���
* @param    num                 Ҫ��ʾ��int�ͱ���λ��
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes    "data = 1234,num = 3"����Ļ��ʾΪ"234"
* Example:  Show_Int_IPS(0,0,521,3,BLACK,WHITE,Show8x16);   // �����Ͻ���ʾ��ɫ������ɫ�����8x16���֡�521��
*
**/
void Show_Int_IPS (unsigned short x, unsigned short y, const long dat, unsigned char num, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    IPS��Ļ��ʾfloat�ͱ���
* @param    x                   x��������(IPS1.14�����ģʽ0~240)
* @param    y                   y��������(IPS1.14�����ģʽ0~135)
* @param    data                Ҫ��ʾ��float�ͱ���
* @param    num                 Ҫ��ʾ��float�ͱ�������λ��
* @param    pointnum            Ҫ��ʾ��float�ͱ���С����λ��
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_Float_IPS(0,0,521.22,3,2,BLACK,WHITE,Show8x16);    // �����Ͻ���ʾ��ɫ������ɫ�����8x16���֡�521.22��
*
**/
void Show_Float_IPS (unsigned short x, unsigned short y, const double dat, unsigned char num, unsigned char pointnum, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    IPS��Ļ��ʾ����
* @param    x                   x��������(0~240)
* @param    y                   y��������(0~135)
* @param    data                Ҫ��ʾ�ĺ�����������
* @param    startnumber         ��startnumber�����ֿ�ʼ
* @param    number              ��number�����ֽ���
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    size                16
* @return   void
* @notes    ʹ��PCtoLCD2002���ȡģ,����,����ʽ,˳��,16*16
* Example:  Show_Chinese16x16_IPS(0,0,Chinese16x16[0],0,3,BLACK,WHITE,16);   // �����Ͻ���ʾIPSChinese16x16�����еĵ�0~3�����ּ�����������
*
**/
void Show_Chinese16x16_IPS(unsigned int x, unsigned int y, const unsigned char *dat, unsigned char startnumber, unsigned char number, COLOR_enum fc, COLOR_enum bc, unsigned char fsize);

/**
*
* @brief    IPS��Ļ��ʾ��ɫͼƬ
* @param    x                   x��������(0~240)
* @param    y                   y��������(0~135)
* @param    width               ͼƬ���
* @param    heigh               ͼƬ�߶�
* @param    pic                 ͼƬ����
* @return   void
* @notes
* Example:  Show_Picture_IPS(60,7,121,121,Earth121x121); // ��ʾ����ͼƬ
*
**/
void Show_Picture_IPS(unsigned int x, unsigned int y, unsigned int width, unsigned int heigh, const unsigned char pic[]);

/**
*
* @brief    IPS��Ļ��ʾ����ͷ�Ҷ�ͼ��
* @param    x                   x��������(0~240)
* @param    y                   y��������(0~135)
* @param    image               ͼ������
* @param    width               ͼ������ʵ�ʿ��
* @param    heigh               ͼ������ʵ�ʸ߶�
* @param    show_width          ͼ��������ʾ���
* @param    show_height         ͼ��������ʾ�߶�
* @param    threshold           Ϊ0ʱ�رն�ֵ��
* @return   void
* @notes
* Example:  Show_Image_IPS(0,0,OriginalImageData[0],188,120,188,135,0);
*
**/
void Show_Image_IPS(unsigned short x, unsigned short y, unsigned char *image, unsigned short width, unsigned short height, unsigned short show_width, unsigned short show_height, unsigned char threshold);

#endif
