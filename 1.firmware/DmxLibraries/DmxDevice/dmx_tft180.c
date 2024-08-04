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
 * @file       dmx_tft180.c
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
#include "dmx_tft180.h"
#include "dmx_delay.h"
#include "dmx_hard_spi.h"
#include "dmx_function.h"

// ��̬��������,���º�����Ϊ��.c�ļ��ڲ�����
static void Write_Cmd_TFT180(unsigned char cmd);
static void Write_8bit_TFT180(unsigned char dat);
static void Write_16bit_TFT180(unsigned short dat);

/**
*
* @brief    TFT1.8����Ļ��ʼ��
* @param    void
* @return   void
* @notes    �޸ĹܽŶ������dmx_tft180.h�ļ���ĺ궨��ܽŽ����޸�
* Example:  Init_TFT180();
*
**/
void Init_TFT180(void)
{
    // SPI��ʼ��
#if TFT180_HARD_SPI
    init_hard_spi(TFT180_PIN, TFT180_SPPED, 0, 0);
#endif

    TFT180_RES_LEVEL(0);
    TFT180_DELAY_MS(100);
    TFT180_RES_LEVEL(1);
    TFT180_DELAY_MS(100);
    Write_Cmd_TFT180(0x11);
    TFT180_DELAY_MS(100);

    Write_Cmd_TFT180(0x3A);
    Write_8bit_TFT180(0x55);
    Write_Cmd_TFT180(0x26);
    Write_8bit_TFT180(0x04);
    Write_Cmd_TFT180(0xF2);
    Write_8bit_TFT180(0x01);
    Write_Cmd_TFT180(0xE0);
    Write_8bit_TFT180(0x3F);
    Write_8bit_TFT180(0x25);
    Write_8bit_TFT180(0x1c);
    Write_8bit_TFT180(0x1e);
    Write_8bit_TFT180(0x20);
    Write_8bit_TFT180(0x12);
    Write_8bit_TFT180(0x2a);
    Write_8bit_TFT180(0x90);
    Write_8bit_TFT180(0x24);
    Write_8bit_TFT180(0x11);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_Cmd_TFT180(0xe1);
    Write_8bit_TFT180(0x20);
    Write_8bit_TFT180(0x20);
    Write_8bit_TFT180(0x20);
    Write_8bit_TFT180(0x20);
    Write_8bit_TFT180(0x05);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x15);
    Write_8bit_TFT180(0xa7);
    Write_8bit_TFT180(0x3d);
    Write_8bit_TFT180(0x18);
    Write_8bit_TFT180(0x25);
    Write_8bit_TFT180(0x2a);
    Write_8bit_TFT180(0x2b);
    Write_8bit_TFT180(0x2b);
    Write_8bit_TFT180(0x3a);
    Write_Cmd_TFT180(0xb1);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_Cmd_TFT180(0xb4);
    Write_8bit_TFT180(0x07);
    Write_Cmd_TFT180(0xc0);
    Write_8bit_TFT180(0x0a);
    Write_8bit_TFT180(0x02);
    Write_Cmd_TFT180(0xc1);
    Write_8bit_TFT180(0x02);
    Write_Cmd_TFT180(0xc5);
    Write_8bit_TFT180(0x4f);
    Write_8bit_TFT180(0x5a);
    Write_Cmd_TFT180(0xc7);
    Write_8bit_TFT180(0x40);
    Write_Cmd_TFT180(0x2a);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0xa8);
    Write_Cmd_TFT180(0x2b);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0x00);
    Write_8bit_TFT180(0xb3);
    Write_Cmd_TFT180(0x36);
    switch(TFT180_SHOW_DIR)
    {
    case 0:
        Write_8bit_TFT180(0xC0);
        break;
    case 1:
        Write_8bit_TFT180(0X00);
        break;
    case 2:
        Write_8bit_TFT180(0xA0);
        break;
    case 3:
        Write_8bit_TFT180(0x60);
        break;
    }
    Write_Cmd_TFT180(0xb7);
    Write_8bit_TFT180(0x00);
    Write_Cmd_TFT180(0x29);
    Write_Cmd_TFT180(0x2c);

    Clean_TFT180(WHITE);

    output_device_flag = 3;
}

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
void Set_Pos_TFT180(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    Write_Cmd_TFT180(0x2A);                      	//Colulm addRSTs set
    Write_8bit_TFT180(0x00);                     	//����ʼ�����8λ,ʼ��Ϊ��
    Write_8bit_TFT180((unsigned char)(x1 + 2)); 	//����ʼ�����8λ
    Write_8bit_TFT180(0x00);                     	//����ֹ�����8λ,ʼ��Ϊ��
    Write_8bit_TFT180((unsigned char)(x2 + 2)); 	//����ֹ�����8λ
    Write_Cmd_TFT180(0x2B);                      	//Colulm addRSTs set
    Write_8bit_TFT180(0x00);                     	//����ʼ�����8λ,ʼ��Ϊ��
    Write_8bit_TFT180((unsigned char)(y1 + 1)); 	//����ʼ�����8λ
    Write_8bit_TFT180(0x00);                     	//����ֹ�����8λ,ʼ��Ϊ��
    Write_8bit_TFT180((unsigned char)(y2 + 1));		//����ֹ�����8λ
    Write_Cmd_TFT180(0x2C);                      	//GRAM����MCU���ݻ�����
}

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
void Draw_Point_TFT180(unsigned int x, unsigned int y, COLOR_enum color)
{
    Set_Pos_TFT180(x, y, x, y);
    Write_16bit_TFT180(color);
}

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
* Example:  Fill_TFT180(0,0,128,160,WHITE);    //IPS114��Ļȫ��
*
**/
void Fill_TFT180(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, COLOR_enum color)
{
    unsigned int i, j;
    Set_Pos_TFT180(x1, y1, x2 - 1, y2 - 1); //������ʾ��Χ
    for(i = y1; i < y2; i++)
    {
        for(j = x1; j < x2; j++)
        {
            Write_16bit_TFT180(color);
        }
    }
}

/**
*
* @brief    TFT��Ļ����
* @param    color               �����ɫ(����dmx_font.h�ļ���鿴)
* @return   void
* @notes
* Example:  Clean_TFT180(WHITE);   // TFT18��Ļȫ��
*
**/
void Clean_TFT180(COLOR_enum color)
{
    Fill_TFT180(0, 0, TFT180_WIDTH, TFT180_HEIGH, color);
}

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
void Show_Char_TFT180(unsigned int x, unsigned int y, unsigned char dat, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize)
{
    unsigned char i, j, temp_start, temp_end;
    switch(fontsize)
    {
    case Show6x8:
        for(i = 0; i < 6; i ++)
        {
            Set_Pos_TFT180(x + i, y, x + i, y + 8);
            temp_start = Char6x8[dat - 32][i];
            for(j = 0; j < 8; j ++)
            {
                if(temp_start & 0x01)
                {
                    Write_16bit_TFT180(fc);
                }
                else
                {
                    Write_16bit_TFT180(bc);
                }
                temp_start >>= 1;
            }
        }
        break;
    case Show8x16:
        for(i = 0; i < 8; i ++)
        {
            Set_Pos_TFT180(x + i, y, x + i, y + 15);
            temp_start = Char8x16[dat - 32][i];
            temp_end = Char8x16[dat - 32][i + 8];
            for(j = 0; j < 8; j ++)
            {
                if(temp_start & 0x01)
                {
                    Write_16bit_TFT180(fc);
                }
                else
                {
                    Write_16bit_TFT180(bc);
                }
                temp_start >>= 1;
            }
            for(j = 0; j < 8; j ++)
            {
                if(temp_end & 0x01)
                {
                    Write_16bit_TFT180(fc);
                }
                else
                {
                    Write_16bit_TFT180(bc);
                }
                temp_end >>= 1;
            }
        }
        break;
    }
}

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
* Example:  Show_String_TFT180(0,0,"DMX",BLACK,WHITE,Show8x16);   // �����Ͻ���ʾ��ɫ������ɫ��8X16�ַ���'DMX'
*
**/
void Show_String_TFT180(unsigned int x, unsigned int y, char *p, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize)
{
    while(*p != '\0')
    {
        Show_Char_TFT180(x, y, *p, fc, bc, fontsize);
        if(fontsize == Show6x8)
            x += 6;
        else if(fontsize == Show8x16)
            x += 8;
        p++;
    }
}

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
void Show_Int_TFT180(unsigned short x, unsigned short y, const long dat, unsigned char num, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize)
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
    Show_String_TFT180(x, y, buff, fc, bc, fontsize);
}

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
void Show_Float_TFT180(unsigned short x, unsigned short y, const double dat, unsigned char num, unsigned char pointnum, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize)
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
    Show_String_TFT180(x, y, buff, fc, bc, fontsize);
}

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
* Example:  Show_Chinese16x16_TFT180(0,0,Chinese16x16[0],0,3,BLACK,WHITE,16);    //�����Ͻ���ʾIPSChinese16x16�����еĵ�0~3�����ּ�����������
*
**/
void Show_Chinese16x16_TFT180(unsigned int x, unsigned int y, const unsigned char *dat, unsigned char startnumber, unsigned char number, COLOR_enum fc, COLOR_enum bc, unsigned char fsize)
{
    unsigned char i, j, k;
    unsigned char temp;
    const unsigned char *pdat;

    Set_Pos_TFT180(x, y, (number - startnumber) * fsize - 1 + x, y + fsize - 1);

    for(i = 0; i < fsize; i ++)
    {
        temp = number;
        pdat = dat + startnumber * 32 + i * (fsize / 8);
        for(; temp > startnumber; temp--)
        {
            for(k = 0; k < (fsize / 8); k ++)
            {
                for(j = 8; j > 0; j --)
                {
                    if((*pdat >> (j - 1)) & 0x01)
                    {
                        Write_16bit_TFT180(fc);
                    }
                    else
                    {
                        Write_16bit_TFT180(bc);
                    }
                }
                pdat ++;
            }
            pdat = pdat - (fsize / 8) + (fsize / 8) * fsize;
        }
    }
}

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
* Example:  Show_Picture_TFT180(3,19,121,121,Earth121x121);  //����Ļ�м���ʾ����ͼƬ
*
**/
void Show_Picture_TFT180(unsigned int x, unsigned int y, unsigned int width, unsigned int heigh, const unsigned char pic[])
{
    unsigned int i, j;
    unsigned long k = 0;
    Set_Pos_TFT180(x, y, x + width - 1, y + heigh - 1);
    for(i = 0; i < width; i++)
    {
        for(j = 0; j < heigh; j++)
        {
            Write_8bit_TFT180(pic[k * 2]);
            Write_8bit_TFT180(pic[k * 2 + 1]);
            k++;
        }
    }
}

/**
*
* @brief    TFT��Ļ��ʾ����ͷ�Ҷ�ͼ��
* @param    x                   x��������
* @param    y                   y��������
* @param    image               ͼ������
* @param    width               ͼ������ʵ�ʿ��
* @param    heigh               ͼ������ʵ�ʸ߶�
* @param    show_width           ͼ��������ʾ���
* @param    show_height          ͼ��������ʾ�߶�
* @param    threshold           ��ֵ����ֵΪ0ʱ�رն�ֵ��
* @return   void
* @notes
* Example:  Show_Image_TFT180(0,0,OriginalImageData[0],188,120,188,120,0); // ��Ļ��ʾ����ͷ�Ҷ�ͼ��
*
**/
void Show_Image_TFT180(unsigned short x, unsigned short y, unsigned char *image, unsigned short width, unsigned short height, unsigned short show_width, unsigned short show_height, unsigned char threshold)
{
    unsigned int i, j;
    unsigned short color, temp;
    unsigned int width_ratio, height_ratio;

    Set_Pos_TFT180(x, y, x + show_width - 1, y + show_height - 1);

    for(j = 0; j < show_height; j ++)
    {
        height_ratio = j * height / show_height;
        for(i = 0; i < show_width; i ++)
        {
            width_ratio = i * width / show_width;
            temp = *(image + height_ratio * width + width_ratio);
            if(threshold == 0)
            {
                color = (0x001f & ((temp) >> 3)) << 11;
                color = color | (((0x003f) & ((temp) >> 2)) << 5);
                color = color | (0x001f & ((temp) >> 3));
                Write_16bit_TFT180(color);
            }
            else if(temp < threshold)
            {
                Write_16bit_TFT180(BLACK);
            }
            else
            {
                Write_16bit_TFT180(WHITE);
            }

        }
    }
}

/**
*
* @brief    TFT��Ļд8bit����
* @param    data                8bit����
* @return   void
* @notes
* Example:  Write_8bit_TFT180(0x23);
*
**/
static void Write_8bit_TFT180(unsigned char dat)
{
    unsigned char temp = 0;
    TFT180_CS_LEVEL(0);
#if TFT180_HARD_SPI
    write_8bit_hard_spi(dat);
#else
    for(temp = 8; temp > 0; temp --)
    {
        TFT180_SCL_LEVEL(0);
        if(0x80 & dat)
        {
            TFT180_SDA_LEVEL(1);
        }
        else
        {
            TFT180_SDA_LEVEL(0);
        }
        TFT180_SCL_LEVEL(1);
        dat <<= 1;
    }
#endif
    TFT180_CS_LEVEL(1);
}

/**
*
* @brief    TFT��Ļд����
* @param    cmd             ����
* @return   void
* @notes
* Example:  Write_Cmd_TFT180(0x21);
*
**/
static void Write_Cmd_TFT180(unsigned char cmd)
{
    TFT180_DC_LEVEL(0);
    Write_8bit_TFT180(cmd);
    TFT180_DC_LEVEL(1);
}

/**
*
* @brief    TFT��Ļд16bit����
* @param    data                16bit����
* @return   void
* @notes
* Example:  Write_16bit_TFT180(0x0000);
*
**/
static void Write_16bit_TFT180(unsigned short dat)
{
    unsigned char temp = 0;
    TFT180_CS_LEVEL(0);
#if TFT180_HARD_SPI
    write_8bit_hard_spi((unsigned char)(dat >> 8));
    write_8bit_hard_spi((unsigned char)dat);
#else
    for(temp = 16; temp > 0; temp --)
    {
        TFT180_SCL_LEVEL(0);
        if(0x8000 & dat)
        {
            TFT180_SDA_LEVEL(1);
        }
        else
        {
            TFT180_SDA_LEVEL(0);
        }
        TFT180_SCL_LEVEL(1);
        dat <<= 1;
    }
#endif
    TFT180_CS_LEVEL(1);
}
