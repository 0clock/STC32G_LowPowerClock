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
 * @file       dmx_icm20602.h
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

#ifndef _DMX_ICM20602_H_
#define _DMX_ICM20602_H_

#include "dmx_mpu.h"

// IPS��Ļ�ܽź궨��
// ICM20602,SPC�ܽ�
#define ICM20602_SPC     		P40
// ICM20602,SD1�ܽ�
#define ICM20602_SDI     		P41
// ICM20602,SD0�ܽ�
#define ICM20602_SDO    		P42
// ICM20602,CS�ܽ�
#define ICM20602_CS      		P43

// �ܽŵ�ƽ����
#define ICM20602_SPC_LEVEL(level)	(ICM20602_SPC=level)
#define ICM20602_SDI_LEVEL(level)	(ICM20602_SDI=level)
#define ICM20602_CS_LEVEL(level)	(ICM20602_CS=level)

// ICM20602��ʱ
#define ICM20602_DELAY_MS(time)	(delay_ms(time))

extern float icm20602_acc_x, icm20602_acc_y, icm20602_acc_z  ;       // ����ICM20602���ٶȼ�����
extern float icm20602_gyro_x, icm20602_gyro_y, icm20602_gyro_z ;     // ����ICM20602�Ǽ��ٶ�����

/**
*
* @brief    ICM20602�����ǳ�ʼ��
* @param
* @return   void
* @notes    �û�����
* Example:  Init_ICM20602();
*
**/
void Init_ICM20602(void);

/**
*
* @brief    ���ICM20602�����Ǽ��ٶ�
* @param
* @return   void
* @notes    ��λ:g(m/s^2),�û�����
* Example:  Get_Acc_ICM20602();
*
**/
void Get_Acc_ICM20602(void);

/**
*
* @brief    ���ICM20602�����ǽǼ��ٶ�
* @param
* @return   void
* @notes    ��λΪ:��/s,�û�����
* Example:  Get_Gyro_ICM20602();
*
**/
void Get_Gyro_ICM20602(void);

/**
*
* @brief    ����ICM20602�����ǵ�ͨ�˲������������
* @param    abw                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @param    gbw                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @param    afs                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @param    gfs                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @return   void
* @notes    ICM20602.c�ļ��ڲ�����,�û�������ó���
* Example:  Set_LowpassFilter_Range_ICM20602(MPU_ABW_218, MPU_GBW_176,MPU_FS_4G, MPU_FS_2000DPS);
*
**/
void Set_LowpassFilter_Range_ICM20602(enum mpu_acc_bw abw, enum mpu_gyro_bw gbw, enum mpu_acc_fs afs, enum mpu_gyro_fs gfs);

#endif
