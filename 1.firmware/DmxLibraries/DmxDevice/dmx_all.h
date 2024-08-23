/*
 * @Author: 0clock
 * @Date: 2024-08-03 00:48:43
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-24 00:48:15
 * @FilePath: \STC32_LowPowerClock\1.firmware\DmxLibraries\DmxDevice\dmx_all.h
 * @Description: 
 * 
 * Copyright (c) 2024 by 0clock, All Rights Reserved. 
 */
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
 * @file       dmx_all.h
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

#ifndef _DMX_ALL_H_
#define _DMX_ALL_H_

// C���Կ��ļ�
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

// ���������η�װ����ͷ�ļ�
#include "dmx_adc.h"
#include "dmx_board.h"
#include "dmx_delay.h"
#include "dmx_eeprom.h"
#include "dmx_encoder.h"
#include "dmx_exti.h"
#include "dmx_gpio.h"
#include "dmx_hard_spi.h"
#include "dmx_pit.h"
#include "dmx_pwm.h"
#include "dmx_uart.h"

// ��������Χ�豸ͷ�ļ�
#include "dmx_icm20602.h"
#include "dmx_icm42688.h"
#include "dmx_ips.h"
#include "dmx_mpu.h"
#include "dmx_oled.h"
#include "dmx_tft180.h"
#include "dmx_tof400c.h"
#include "dmx_tsl1401.h"

// USERͷ�ļ�
#include "main.h"
#include "isr.h"

// CODE
#include "buzzer.h"
#include "dmx_oled.h"
#include "myoled.h"
#include "BMP.h"
#include "bms.h"
#include "task.h"
#include "ins5699s.h"
#include "iic.h"

#endif