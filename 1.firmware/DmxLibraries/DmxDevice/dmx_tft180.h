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
 * @file       dmx_tft180.h
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

#ifndef _DMX_TFT180_H_
#define _DMX_TFT180_H_

#include "dmx_font.h"

// ͨ�ŷ�ʽѡ��,0:���SPI,1:Ӳ��SPI
#define TFT180_HARD_SPI   	1
// ������Ļ��ʾ����,0:����,1:����180��,2:����,3:����180��
#define TFT180_SHOW_DIR			0
// ����TFT1.8����Ļ���,����ģʽ�¿��Ϊ128x160,����ģʽ�¿��Ϊ160x128
#define TFT180_WIDTH       	128 // ����ģʽ�¿�Ϊ128,����ģʽ�¿�Ϊ160
#define TFT180_HEIGH      	160 // ����ģʽ�¸�Ϊ160,����ģʽ�¸�Ϊ128

//TFT1.8����Ļ�ܽź궨��
#if TFT180_HARD_SPI
// Ӳ��SPI����
#define TFT180_PIN 			SPI2_SCK_P25_MOSI_P23_MISO_P24
// Ӳ��SPI�ٶ�
#define TFT180_SPPED 		SPI_SPEED4
#else
// TFT1.8��,SCL�ܽ�
#define TFT180_SCL			P25
// TFT1.8��,SDA�ܽ�
#define TFT180_SDA  		P23
// SCL,SDA��ƽ����
#define TFT180_SCL_LEVEL(level)	(TFT180_SCL=level)
#define TFT180_SDA_LEVEL(level)	(TFT180_SDA=level)
#endif

// TFT180,RES�ܽ�
#define TFT180_RES  		P21
// TFT180,DC�ܽ�
#define TFT180_DC   		P24
// TFT180,CS�ܽ�
#define TFT180_CS   		P27
// ��TFT��Ļ�ܽŽ��иߵ͵�ƽ����
#define TFT180_RES_LEVEL(level)	(TFT180_RES=level)
#define TFT180_DC_LEVEL(level) 	(TFT180_DC=level)
#define TFT180_CS_LEVEL(level) 	(TFT180_CS=level)

// SPI��ʱ10ms
#define TFT180_DELAY_MS(time) 			(delay_ms(time))

/**
*
* @brief    TFT180��Ļ��ʼ��
* @param    void
* @return   void
* @notes    �޸ĹܽŶ������dmx_tft180.h�ļ���ĺ궨��ܽŽ����޸�
* Example:  Init_TFT180();
*
**/
void Init_TFT180(void);

/**
*
* @brief    TFT��Ļ������ʼ�ͽ�������
* @param    x1                x������ʼ��
* @param    y1                y������ʼ��
* @param    x2                x���������
* @param    y2                y���������
* @return   void
* @notes
* Example:  Set_Pos_TFT180(0,0,0,0);
*
**/
void Set_Pos_TFT180(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/**
*
* @brief    TFT��Ļ��ָ��λ�û���
* @param    x                   x��������
* @param    y                   y��������
* @param    color               �����ɫ(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Draw_Point_TFT180(0,0,0,0);
*
**/
void Draw_Point_TFT180(unsigned int x, unsigned int y, COLOR_enum color);

/**
*
* @brief    TFT��Ļ���ָ������
* @param    x1                  x������ʼ��
* @param    y1                  y������ʼ��
* @param    x2                  x���������
* @param    y2                  y���������
* @param    color               �����ɫ(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Fill_TFT180(0,0,128,160,WHITE);  // ��Ļȫ��
*
**/
void Fill_TFT180(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, COLOR_enum color);

/**
*
* @brief    TFT��Ļ����
* @param    color               �����ɫ(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Clean_TFT180(WHITE);   // TFT18��Ļȫ��
*
**/
void Clean_TFT180(COLOR_enum color);

/**
*
* @brief    TFT��Ļ��ʾ�����ַ�
* @param    x                   x��������
* @param    y                   y��������
* @param    data                �ַ�
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_Char_TFT180(0,0,'D',BLACK,WHITE,Show8x16);  // �����Ͻ���ʾ��ɫ������ɫ��8X16�ַ�'D'
*
**/
void Show_Char_TFT180(unsigned int x, unsigned int y, unsigned char dat, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT��Ļ��ʾ�ַ���
* @param    x                   x��������
* @param    y                   y��������
* @param    *p                  Ҫ��ʾ���ַ���
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_String_TFT180(0,0,"DMX",BLACK,WHITE,Show8x16);  // �����Ͻ���ʾ��ɫ������ɫ��8X16�ַ���'DMX'
*
**/
void Show_String_TFT180(unsigned int x, unsigned int y, char *p, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT1.8����Ļ��ʾint�ͱ���
* @param    x                   x��������
* @param    y                   y��������
* @param    data                Ҫ��ʾ��int�ͱ���
* @param    num                 Ҫ��ʾ��int�ͱ���λ��
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_Int_TFT180(0,0,521,3,BLACK,WHITE,Show8x16);     // �����Ͻ���ʾ��ɫ������ɫ��8X16���֡�521��
*
**/
void Show_Int_TFT180(unsigned short x, unsigned short y, const long dat, unsigned char num, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT1.8����Ļ��ʾfloat�ͱ���
* @param    x                   x��������
* @param    y                   y��������
* @param    data                Ҫ��ʾ��float�ͱ���
* @param    num                 Ҫ��ʾ��float�ͱ�������λ��
* @param    pointnum            Ҫ��ʾ��float�ͱ���С����λ��
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    fontsize            ����ߴ�(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Show_Float_TFT180(0,0,521.22,3,2,BLACK,WHITE,Show8x16);  // �����Ͻ���ʾ��ɫ������ɫ��8X16���֡�521.22��
*
**/
void Show_Float_TFT180(unsigned short x, unsigned short y, const double dat, unsigned char num, unsigned char pointnum, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT��Ļ��ʾ����
* @param    x                   x��������
* @param    y                   y��������
* @param    data                Ҫ��ʾ�ĺ�����������
* @param    startnumber         ��startnumber�����ֿ�ʼ
* @param    number              ��number�����ֽ���
* @param    fc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    bc                  ������ɫ(����dmx_font.h�ļ���鿴)
* @param    size                16
* @return   void
* @notes    ʹ��PCtoLCD2002���ȡģ,����,����ʽ,˳��,16*16
* Example:  Show_Chinese16x16_TFT180(0,0,Chinese16x16[0],0,3,BLACK,WHITE,16);    //�����Ͻ���ʾChinese16x16�����еĵ�0~3�����ּ�����������
*
**/
void Show_Chinese16x16_TFT180(unsigned int x, unsigned int y, const unsigned char *dat, unsigned char startnumber, unsigned char number, COLOR_enum word_color, COLOR_enum back_color, unsigned char fsize);

/**
*
* @brief    TFT��Ļ��ʾ��ɫͼƬ
* @param    x                   x��������
* @param    y                   y��������
* @param    width               ͼƬ���
* @param    heigh               ͼƬ�߶�
* @param    pic                 ͼƬ����
* @return   void
* @notes
* Example:  Show_Picture_TFT180(3,19,121,121,Earth121x121);     // ����Ļ�м���ʾ����ͼƬ
*
**/
void Show_Picture_TFT180(unsigned int x, unsigned int y, unsigned int width, unsigned int heigh, const unsigned char pic[]);

/**
*
* @brief    TFT��Ļ��ʾ����ͷ�Ҷ�ͼ��
* @param    x                   x��������
* @param    y                   y��������
* @param    image               ͼ������
* @param    width               ͼ������ʵ�ʿ��
* @param    heigh               ͼ������ʵ�ʸ߶�
* @param    show_width          ͼ��������ʾ���
* @param    show_height         ͼ��������ʾ�߶�
* @param    threshold           ��ֵ����ֵΪ0ʱ�رն�ֵ��
* @return   void
* @notes
* Example:  Show_Image_TFT180(0,0,OriginalImageData[0],188,120,188,120,0); // ��Ļ��ʾ����ͷ�Ҷ�ͼ��
*
**/
void Show_Image_TFT180(unsigned short x, unsigned short y, unsigned char *image, unsigned short width, unsigned short height, unsigned short show_width, unsigned short show_height, unsigned char threshold);


#endif /* DMXLIBRARIES_DMX_DEVICE_DMX_TFT180_H_ */
