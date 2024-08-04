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
 * @file       dmx_oled.c
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
#include "dmx_oled.h"
#include "dmx_delay.h"
#include "dmx_hard_spi.h"
#include "dmx_function.h"

// ��̬��������,���º�����Ϊ��.c�ļ��ڲ�����
static void Write_Data_OLED(unsigned char dat);
static void Write_Commond_OLED(unsigned char cmd);

/**
*
* @brief    0.96��6��OLED��Ļ��ʼ��
* @param    void
* @return   void
* @notes    �޸ĹܽŶ������dmx_oled.h�ļ���ĺ궨��ܽŽ����޸�
* Example:  Init_OLED();
*
**/
void Init_OLED(void)
{
#if OLED_HARD_SPI
    init_hard_spi(OLED_PIN, OLED_SPPED, 0, 0);
#endif

    OLED_RES_LEVEL(0);
    OLED_Delay10ms(8);
    OLED_RES_LEVEL(1);

    Write_Commond_OLED(0xae);   
    Write_Commond_OLED(0x00);   
    Write_Commond_OLED(0x10);   
    Write_Commond_OLED(0x40);   
    Write_Commond_OLED(0x81);   
    Write_Commond_OLED(0xff);  
    Write_Commond_OLED(0xa0);   
    Write_Commond_OLED(0xc0);  
    Write_Commond_OLED(0xa6);   
    Write_Commond_OLED(0xa8);   
    Write_Commond_OLED(0x3f);   
    Write_Commond_OLED(0xd3);   
    Write_Commond_OLED(0x00);   
    Write_Commond_OLED(0xd5);  
    Write_Commond_OLED(0x80);   
    Write_Commond_OLED(0xd9);   
    Write_Commond_OLED(0xf1);   
    Write_Commond_OLED(0xda);  
    Write_Commond_OLED(0x12);
    Write_Commond_OLED(0xdb); 
    Write_Commond_OLED(0x40);  
    Write_Commond_OLED(0x20);   
    Write_Commond_OLED(0x02);
    Write_Commond_OLED(0x8d);  
    Write_Commond_OLED(0x10);   
    Write_Commond_OLED(0xa4);   
    Write_Commond_OLED(0xa6);   
    Write_Commond_OLED(0xaf);  
    Clean_OLED();
    Set_Pos_OLED(0, 0);

    output_device_flag = 1;
}

/**
*
* @brief    0.96��6��OLED��Ļ����
* @param    data
* @return   void
* @notes
* Example:  Draw_Point_OLED(0xff);
*
**/
void Draw_Point_OLED(unsigned char dat)
{
    OLED_DC_LEVEL(1);
    Write_Data_OLED(dat);
    OLED_DC_LEVEL(0);
}

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
void Set_Pos_OLED(unsigned char x, unsigned char y)
{
    Write_Commond_OLED((unsigned char)(0xb0 + y));
    Write_Commond_OLED(((x & 0xf0) >> 4) | 0x10);
    Write_Commond_OLED((x & 0x0f));
}

/**
*
* @brief    0.96��6��OLED��Ļ����
* @param
* @return   void
* @notes
* Example:  Fill_OLED();
*
**/
void Fill_OLED(void)
{
    unsigned char y, x;

    for(y = 0; y < 8; y++)
    {
        Write_Commond_OLED((unsigned char)(0xb0 + y));
        Write_Commond_OLED(0x00);
        Write_Commond_OLED(0x10);
        for(x = 0; x < OLED_WIDTH; x++)
            Draw_Point_OLED(0xff);
    }
}

/**
*
* @brief    0.96��6��OLED��Ļ����
* @param
* @return   void
* @notes    ִ�к�Ч����ȫ��,����
* Example:  Clean_OLED();
*
**/
void Clean_OLED(void)
{
    unsigned char y, x;
    for(y = 0; y < 8; y++)
    {
        Write_Commond_OLED((unsigned char)(0xb0 + y));
        Write_Commond_OLED(0x00);
        Write_Commond_OLED(0x10);
        for(x = 0; x < OLED_WIDTH; x++)
            Draw_Point_OLED(0);
    }
}

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
void Show_String_OLED(unsigned char x, unsigned char y, char str[], SHOW_size_enum fontsize)
{
    unsigned char c = 0, i = 0, j = 0;
    while (str[j] != '\0')
    {
        c = str[j] - 32;
        if(fontsize == Show6x8)
        {
            if(x > 126)
            {
                x = 0;
                y++;
            }
            Set_Pos_OLED(x, y);
            for(i = 0; i < 6; i++)
                Draw_Point_OLED(Char6x8[c][i]);
            x += 6;
        }
        else if(fontsize == Show8x16)
        {
            if(x > 120)
            {
                x = 0;
                y++;
            }
            Set_Pos_OLED(x, y);
            for(i = 0; i < 8; i++)
                Draw_Point_OLED(Char8x16[c][i]);
            Set_Pos_OLED(x, (unsigned char)(y + 1));
            for(i = 0; i < 8; i++)
                Draw_Point_OLED(Char8x16[c][i + 8]);
            x += 8;
        }
        j++;
    }
}

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
void Show_Int_OLED(char x, char y, const long dat, unsigned char num, SHOW_size_enum fontsize)
{
    long multiple = 1;
    long remainder = dat;
    char buff[12] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

    if(num > 10)
        num = 10;
    if(num < 1)
        num = 1;

    buff[num + 1] = '\0';

    if(num<10)
    {
        while(num > 0)
        {
            num --;
            multiple *= 10;
        }
        remainder %= multiple;
    }

    int_to_str_func(remainder,buff);
    Show_String_OLED(x, y, buff, fontsize);
}

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
void Show_Float_OLED(char x, char y, const double dat, unsigned char num, unsigned char pointnum, SHOW_size_enum fontsize)
{
    double multiple = 1;
    double remainder = dat;
    char buff[17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

    if(num > 8)
        num = 8;
    if(num < 1)
        num = 1;
    if(pointnum > 6)
        pointnum = 6;
    if(pointnum < 1)
        pointnum = 1;

    buff[num + pointnum + 2] = '\0';

    while(num > 0)
    {
        num --;
        multiple *= 10;
    }
    remainder = remainder - ((long)remainder/(long)multiple)* multiple;

    float_to_str_func(remainder,pointnum,buff);
    Show_String_OLED(x, y, buff, fontsize);
}

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
void Show_Chinese16x16_OLED(char x, char y, const unsigned char *chinese, char startnum, char endnum)
{
    unsigned int i, j, k;

    for(j = startnum; j < endnum; j++)
    {
        Set_Pos_OLED((unsigned char)(x + (j - startnum) * 16), y);
        for(i = 0; i < 16; i++)
        {
            Draw_Point_OLED(chinese[startnum * 16 * 2 + k]);
            k++;
        }
        Set_Pos_OLED((unsigned char)(x + (j - startnum) * 16), (unsigned char)(y + 1));
        for(i = 0; i < 16; i++)
        {
            Draw_Point_OLED(chinese[startnum * 16 * 2 + k]);
            k++;
        }
    }
}

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
void Show_BMP_OLED(unsigned char x, unsigned char y, unsigned char width, unsigned char height, const unsigned char *bmp)
{
    unsigned int i = 0;
    unsigned char x0, y0, x1, y1;
    y1 = (y + height - 1) / 8;
    x1 = x + width;
    for(y0 = y / 8; y0 <= y1; y0++)
    {
        Set_Pos_OLED(x, y0);
        for(x0 = x; x0 < x1; x0++)
        {
            Draw_Point_OLED(bmp[i++]);
        }
    }
}

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
void Show_Image_Oled (unsigned short x, unsigned short y, const unsigned char *image, unsigned short width, unsigned short height, unsigned short show_width, unsigned short show_height, unsigned char threshold)
{
    unsigned short i, j;
    unsigned char dat;
    unsigned int width_ratio = 0, height_ratio = 0;

    show_height = show_height - show_height % 8;
    for(i = 0; i < show_height; i += 8) 
    {
        Set_Pos_OLED((unsigned char)(x + 0), (unsigned char)(y + i / 8));
        height_ratio = i * height / show_height;
        for(j = 0; j < show_width; j ++)
        {
            width_ratio = j * width / show_width;
            dat = 0;
            if(*(image + height_ratio * width + width_ratio + width * 0) > threshold)
                dat |= 0x01;
            if(*(image + height_ratio * width + width_ratio + width * 1) > threshold)
                dat |= 0x02;
            if(*(image + height_ratio * width + width_ratio + width * 2) > threshold)
                dat |= 0x04;
            if(*(image + height_ratio * width + width_ratio + width * 3) > threshold)
                dat |= 0x08;
            if(*(image + height_ratio * width + width_ratio + width * 4) > threshold)
                dat |= 0x10;
            if(*(image + height_ratio * width + width_ratio + width * 5) > threshold)
                dat |= 0x20;
            if(*(image + height_ratio * width + width_ratio + width * 6) > threshold)
                dat |= 0x40;
            if(*(image + height_ratio * width + width_ratio + width * 7) > threshold)
                dat |= 0x80;
            Draw_Point_OLED(dat);
        }
    }
}

/**
*
* @brief    0.96��6��OLED��Ļд����
* @param    dat         ����
* @return   void
* @notes    �ڲ�����
* Example:  Write_Data_OLED(0xff);
*
**/
static void Write_Data_OLED(unsigned char dat)
{
    unsigned char temp = 0;
    OLED_CS_LEVEL(0);          
#if OLED_HARD_SPI
    write_8bit_hard_spi(dat);
#else
    for(temp = 8; temp > 0; temp --)
    {
        OLED_D0_LEVEL(0);
        if(0x80 & dat)
        {
            OLED_D1_LEVEL(1);
        }
        else
        {
            OLED_D1_LEVEL(0);
        }
        OLED_D0_LEVEL(1);
        dat <<= 1;
    }
#endif
    OLED_CS_LEVEL(1);
}

/**
*
* @brief    0.96��6��OLED��Ļд����
* @param    cmd         ����
* @return   void
* @notes    �ڲ�����
* Example:  Write_Commond_OLED(0xff);
*
**/
static void Write_Commond_OLED(unsigned char cmd)
{
    OLED_DC_LEVEL(0);
    Write_Data_OLED(cmd);
    OLED_DC_LEVEL(1);
}
