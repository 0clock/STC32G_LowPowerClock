/****************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (C) 2023,AS DAIMXA
 *     copyright Copyright (C) 呆萌侠DAIMXA,2023
 *     All rights reserved.
 *     技术讨论QQ群：710026750
 *
 *     除注明出处外，以下所有内容版权均属呆萌侠智能科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留呆萌智能侠科技的版权声明。
 *      ____    _    ___ __  ____  __    _    
 *     |  _ \  / \  |_ _|  \/  \ \/ /   / \   
 *     | | | |/ _ \  | || |\/| |\  /   / _ \  
 *     | |_| / ___ \ | || |  | |/  \  / ___ \ 
 *     |____/_/   \_\___|_|  |_/_/\_\/_/   \_\
 *
 * @file       dmx_tft180.h
 * @brief      呆萌侠STC32F12K54开源库
 * @company    合肥呆萌侠智能科技有限公司
 * @author     呆萌侠科技（QQ：2453520483）
 * @MCUcore    STC32F12K54
 * @Software   Keil5 C251
 * @version    查看说明文档内version版本说明
 * @Taobao     https://daimxa.taobao.com/
 * @Openlib    https://gitee.com/daimxa
 * @date       2023-11-10
****************************************************************************************/

#ifndef _DMX_TFT180_H_
#define _DMX_TFT180_H_

#include "dmx_font.h"

// 通信方式选择,0:软件SPI,1:硬件SPI
#define TFT180_HARD_SPI   	1
// 设置屏幕显示方向,0:竖屏,1:竖屏180度,2:横屏,3:横屏180度
#define TFT180_SHOW_DIR			0
// 设置TFT1.8寸屏幕宽高,竖屏模式下宽高为128x160,横屏模式下宽高为160x128
#define TFT180_WIDTH       	128 // 竖屏模式下宽为128,横屏模式下宽为160
#define TFT180_HEIGH      	160 // 竖屏模式下高为160,横屏模式下高为128

//TFT1.8寸屏幕管脚宏定义
#if TFT180_HARD_SPI
// 硬件SPI引脚
#define TFT180_PIN 			SPI2_SCK_P25_MOSI_P23_MISO_P24
// 硬件SPI速度
#define TFT180_SPPED 		SPI_SPEED4
#else
// TFT1.8寸,SCL管脚
#define TFT180_SCL			P25
// TFT1.8寸,SDA管脚
#define TFT180_SDA  		P23
// SCL,SDA电平操作
#define TFT180_SCL_LEVEL(level)	(TFT180_SCL=level)
#define TFT180_SDA_LEVEL(level)	(TFT180_SDA=level)
#endif

// TFT180,RES管脚
#define TFT180_RES  		P21
// TFT180,DC管脚
#define TFT180_DC   		P24
// TFT180,CS管脚
#define TFT180_CS   		P27
// 对TFT屏幕管脚进行高低电平操作
#define TFT180_RES_LEVEL(level)	(TFT180_RES=level)
#define TFT180_DC_LEVEL(level) 	(TFT180_DC=level)
#define TFT180_CS_LEVEL(level) 	(TFT180_CS=level)

// SPI延时10ms
#define TFT180_DELAY_MS(time) 			(delay_ms(time))

/**
*
* @brief    TFT180屏幕初始化
* @param    void
* @return   void
* @notes    修改管脚定义可在dmx_tft180.h文件里的宏定义管脚进行修改
* Example:  Init_TFT180();
*
**/
void Init_TFT180(void);

/**
*
* @brief    TFT屏幕设置起始和结束坐标
* @param    x1                x方向起始点
* @param    y1                y方向起始点
* @param    x2                x方向结束点
* @param    y2                y方向结束点
* @return   void
* @notes
* Example:  Set_Pos_TFT180(0,0,0,0);
*
**/
void Set_Pos_TFT180(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/**
*
* @brief    TFT屏幕在指定位置画点
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    color               点的颜色(可在dmx_font.h文件里查看)
* @return   void
* @notes
* Example:  Draw_Point_TFT180(0,0,0,0);
*
**/
void Draw_Point_TFT180(unsigned int x, unsigned int y, COLOR_enum color);

/**
*
* @brief    TFT屏幕填充指定区域
* @param    x1                  x方向起始点
* @param    y1                  y方向起始点
* @param    x2                  x方向结束点
* @param    y2                  y方向结束点
* @param    color               填充颜色(可在dmx_font.h文件里查看)
* @return   void
* @notes
* Example:  Fill_TFT180(0,0,128,160,WHITE);  // 屏幕全白
*
**/
void Fill_TFT180(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, COLOR_enum color);

/**
*
* @brief    TFT屏幕清屏
* @param    color               填充颜色(可在dmx_font.h文件里查看)
* @return   void
* @notes
* Example:  Clean_TFT180(WHITE);   // TFT18屏幕全白
*
**/
void Clean_TFT180(COLOR_enum color);

/**
*
* @brief    TFT屏幕显示单个字符
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    data                字符
* @param    fc                  字体颜色(可在dmx_font.h文件里查看)
* @param    bc                  背景颜色(可在dmx_font.h文件里查看)
* @param    fontsize            字体尺寸(可在dmx_font.h文件里查看)
* @return   void
* @notes
* Example:  Show_Char_TFT180(0,0,'D',BLACK,WHITE,Show8x16);  // 在左上角显示白色背景黑色的8X16字符'D'
*
**/
void Show_Char_TFT180(unsigned int x, unsigned int y, unsigned char dat, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT屏幕显示字符串
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    *p                  要显示的字符串
* @param    fc                  字体颜色(可在dmx_font.h文件里查看)
* @param    bc                  背景颜色(可在dmx_font.h文件里查看)
* @param    fontsize            字体尺寸(可在dmx_font.h文件里查看)
* @return   void
* @notes
* Example:  Show_String_TFT180(0,0,"DMX",BLACK,WHITE,Show8x16);  // 在左上角显示白色背景黑色的8X16字符串'DMX'
*
**/
void Show_String_TFT180(unsigned int x, unsigned int y, char *p, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT1.8寸屏幕显示int型变量
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    data                要显示的int型变量
* @param    num                 要显示的int型变量位数
* @param    fc                  字体颜色(可在dmx_font.h文件里查看)
* @param    bc                  背景颜色(可在dmx_font.h文件里查看)
* @param    fontsize            字体尺寸(可在dmx_font.h文件里查看)
* @return   void
* @notes
* Example:  Show_Int_TFT180(0,0,521,3,BLACK,WHITE,Show8x16);     // 在左上角显示白色背景黑色的8X16数字“521”
*
**/
void Show_Int_TFT180(unsigned short x, unsigned short y, const long dat, unsigned char num, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT1.8寸屏幕显示float型变量
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    data                要显示的float型变量
* @param    num                 要显示的float型变量整数位数
* @param    pointnum            要显示的float型变量小数点位数
* @param    fc                  字体颜色(可在dmx_font.h文件里查看)
* @param    bc                  背景颜色(可在dmx_font.h文件里查看)
* @param    fontsize            字体尺寸(可在dmx_font.h文件里查看)
* @return   void
* @notes
* Example:  Show_Float_TFT180(0,0,521.22,3,2,BLACK,WHITE,Show8x16);  // 在左上角显示白色背景黑色的8X16数字“521.22”
*
**/
void Show_Float_TFT180(unsigned short x, unsigned short y, const double dat, unsigned char num, unsigned char pointnum, COLOR_enum fc, COLOR_enum bc, SHOW_size_enum fontsize);

/**
*
* @brief    TFT屏幕显示汉字
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    data                要显示的汉字所在数组
* @param    startnumber         第startnumber个汉字开始
* @param    number              第number个汉字结束
* @param    fc                  字体颜色(可在dmx_font.h文件里查看)
* @param    bc                  背景颜色(可在dmx_font.h文件里查看)
* @param    size                16
* @return   void
* @notes    使用PCtoLCD2002软件取模,阴码,逐行式,顺向,16*16
* Example:  Show_Chinese16x16_TFT180(0,0,Chinese16x16[0],0,3,BLACK,WHITE,16);    //在左上角显示Chinese16x16数组中的第0~3个汉字即“呆萌侠”
*
**/
void Show_Chinese16x16_TFT180(unsigned int x, unsigned int y, const unsigned char *dat, unsigned char startnumber, unsigned char number, COLOR_enum word_color, COLOR_enum back_color, unsigned char fsize);

/**
*
* @brief    TFT屏幕显示彩色图片
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    width               图片宽度
* @param    heigh               图片高度
* @param    pic                 图片数组
* @return   void
* @notes
* Example:  Show_Picture_TFT180(3,19,121,121,Earth121x121);     // 在屏幕中间显示地球图片
*
**/
void Show_Picture_TFT180(unsigned int x, unsigned int y, unsigned int width, unsigned int heigh, const unsigned char pic[]);

/**
*
* @brief    TFT屏幕显示摄像头灰度图像
* @param    x                   x方向坐标
* @param    y                   y方向坐标
* @param    image               图像数组
* @param    width               图像数组实际宽度
* @param    heigh               图像数组实际高度
* @param    show_width          图像数组显示宽度
* @param    show_height         图像数组显示高度
* @param    threshold           二值化阈值为0时关闭二值化
* @return   void
* @notes
* Example:  Show_Image_TFT180(0,0,OriginalImageData[0],188,120,188,120,0); // 屏幕显示摄像头灰度图像
*
**/
void Show_Image_TFT180(unsigned short x, unsigned short y, unsigned char *image, unsigned short width, unsigned short height, unsigned short show_width, unsigned short show_height, unsigned char threshold);


#endif /* DMXLIBRARIES_DMX_DEVICE_DMX_TFT180_H_ */
