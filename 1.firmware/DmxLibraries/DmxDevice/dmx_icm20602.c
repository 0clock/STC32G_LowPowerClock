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
 * @file       dmx_icm20602.c
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
#include "dmx_icm20602.h"
#include "dmx_delay.h"

#include "dmx_oled.h"
#include "dmx_ips.h"
#include "dmx_tft180.h"

// ICM20602���ٶȼ�����
float icm20602_acc_x  = 0, icm20602_acc_y  = 0, icm20602_acc_z  = 0;
// ICM20602�Ǽ��ٶ�����
float icm20602_gyro_x = 0, icm20602_gyro_y = 0, icm20602_gyro_z = 0;

// ��̬��������,���º�����Ϊ��.c�ļ��ڲ�����
static void Write_Data_ICM20602(unsigned char reg, unsigned char dat);
static void Read_Datas_ICM20602(unsigned char reg, unsigned char *dat, unsigned int num);
// ����ת��Ϊʵ���������ݵ�ת��ϵ��
static float icm20602_acc_inv = 1, icm20602_gyro_inv = 1;

/**
*
* @brief    ICM20602�����ǳ�ʼ��
* @param
* @return   void
* @notes    �û�����
* Example:  Init_ICM20602();
*
**/
void Init_ICM20602(void)
{
		// ��ʼ����ʱ
		char time = 50;
    // ��ȡ�������ͺ��������Լ�
    unsigned char model = 0xff;

    while(1)
    {
        Read_Datas_ICM20602(WHO_AM_I, &model, 1);
        if(model == 0x12)
        {
            // ICM20602
            break;
        }
        else
        {
            ICM20602_DELAY_MS(10);
            time--;
            if(time < 0)
            {
                switch(output_device_flag)
                {
                    case 0:printf("ICM20602 Init Error!\r\n");break;
                    case 1:Show_String_OLED(0,0,"ICM20602 Init Error!",Show6x8);break;
                    case 2:Show_String_IPS(0,0,"ICM20602 Init Error!",BLACK,WHITE,Show8x16);break;
                    case 3:Show_String_TFT180(0,0,"ICM20602 Init Error!",BLACK,WHITE,Show8x16);break;
                    default:printf("ICM20602 Init Error!\r\n");break;
                }
                while(1);
                // ��������ԭ�������¼���
                // ICM20602����,������µĸ��ʼ���
                // ���ߴ������û�нӺ�
            }
        }
    }
    Write_Data_ICM20602(PWR_MGMT_1, 0x80); // ��λ�豸
    ICM20602_DELAY_MS(10);

    Write_Data_ICM20602(PWR_MGMT_1, 0x01);
    ICM20602_DELAY_MS(10);
    Write_Data_ICM20602(PWR_MGMT_2, 0x00);
    ICM20602_DELAY_MS(10);
		
    Write_Data_ICM20602(SMPLRT_DIV, 0x07);

    // ����ICM20602�����ǵ�ͨ�˲������������,ICM20602��MPU6050��Ϊͬһ�Ҳ�Ʒǰ�߿ɿ������ߵ�������
    Set_LowpassFilter_Range_ICM20602(MPU_ABW_218, MPU_GBW_176, MPU_FS_4G, MPU_FS_2000DPS);
}

/**
*
* @brief    ���ICM20602�����Ǽ��ٶ�
* @param
* @return   void
* @notes    ��λ:g(m/s^2),�û�����
* Example:  Get_Acc_ICM20602();
*
**/
void Get_Acc_ICM20602(void)
{
    unsigned char dat[6];
    Read_Datas_ICM20602(ACCEL_XOUT_H, dat, 6);
    icm20602_acc_x = icm20602_acc_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    icm20602_acc_y = icm20602_acc_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    icm20602_acc_z = icm20602_acc_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}

/**
*
* @brief    ���ICM20602�����ǽǼ��ٶ�
* @param
* @return   void
* @notes    ��λΪ:��/s,�û�����
* Example:  Get_Gyro_ICM20602();
*
**/
void Get_Gyro_ICM20602(void)
{
    unsigned char dat[6];
    Read_Datas_ICM20602(GYRO_XOUT_H, dat, 6);
    icm20602_gyro_x = icm20602_gyro_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    icm20602_gyro_y = icm20602_gyro_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    icm20602_gyro_z = icm20602_gyro_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}

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
void Set_LowpassFilter_Range_ICM20602(enum mpu_acc_bw abw, enum mpu_gyro_bw gbw, enum mpu_acc_fs afs, enum mpu_gyro_fs gfs)
{
    Write_Data_ICM20602(MPU6050_CONFIG, (unsigned char)gbw);
    Write_Data_ICM20602(ACCEL_CONFIG_2, (unsigned char)abw);
    Write_Data_ICM20602(GYRO_CONFIG, (unsigned char)(gfs << 3));
    Write_Data_ICM20602(ACCEL_CONFIG, (unsigned char)(afs << 3));
    switch(afs)
    {
    case MPU_FS_2G:
        icm20602_acc_inv = (float)2.0 * 9.8 / 32768.0;   // ���ٶȼ�����Ϊ:��2g
        break;
    case MPU_FS_4G:
        icm20602_acc_inv = (float)4.0 * 9.8 / 32768.0;   // ���ٶȼ�����Ϊ:��4g
        break;
    case MPU_FS_8G:
        icm20602_acc_inv = (float)8.0 * 9.8 / 32768.0;   // ���ٶȼ�����Ϊ:��8g
        break;
    case MPU_FS_16G:
        icm20602_acc_inv = (float)16.0 * 9.8 / 32768.0;  // ���ٶȼ�����Ϊ:��16g
        break;
    default:
        icm20602_acc_inv = 1;                            // ��ת��Ϊʵ������
        break;
    }
    switch(gfs)
    {
    case MPU_FS_250DPS:
        icm20602_gyro_inv = (float)250.0 / 32768.0;      // ����������Ϊ:��250dps
        break;
    case MPU_FS_500DPS:
        icm20602_gyro_inv = (float)500.0 / 32768.0;      // ����������Ϊ:��500dps
        break;
    case MPU_FS_1000DPS:
        icm20602_gyro_inv = (float)1000.0 / 32768.0;     // ����������Ϊ:��1000dps
        break;
    case MPU_FS_2000DPS:
        icm20602_gyro_inv = (float)2000.0 / 32768.0;     // ����������Ϊ:��2000dps
        break;
    default:
        icm20602_gyro_inv = 1;                           // ��ת��Ϊʵ������
        break;
    }
}

/**
*
* @brief    ICM20602������д8bit����
* @param    data                ����
* @return   void
* @notes    ICM20602.c�ļ��ڲ�����,�û�������ó���
* Example:  Write_8bit_ICM20602(0x00);
*
**/
static void Write_8bit_ICM20602(unsigned char dat)
{
    unsigned char temp = 0;
    for(temp = 8; temp > 0; temp --)
    {
        ICM20602_SPC_LEVEL(0);
        if(0x80 & dat)
        {
            ICM20602_SDI_LEVEL(1);
        }
        else
        {
            ICM20602_SDI_LEVEL(0);
        }
        ICM20602_SPC_LEVEL(1);
        dat <<= 1;
    }
}

/**
*
* @brief    ICM20602�����Ƕ�8bit����
* @param    data                ����
* @return   void
* @notes    ICM20602.c�ļ��ڲ�����,�û�������ó���
* Example:  Read_8bit_ICM20602(dat);
*
**/
static void Read_8bit_ICM20602(unsigned char *dat)
{
    unsigned char temp = 0;
    for(temp = 8; temp > 0; temp --)
    {
        ICM20602_SPC_LEVEL(0);
        *dat = *dat << 1;
        *dat |= ICM20602_SDO;
        ICM20602_SPC_LEVEL(1);
    }
}

/**
*
* @brief    ICM20602������д����
* @param    reg                 �Ĵ���
* @param    data                ��Ҫд���üĴ���������
* @return   void
* @notes    ICM20602.c�ļ��ڲ�����,�û�������ó���
* Example:  Write_Data_ICM20602(0x00,0x00);
*
**/
static void Write_Data_ICM20602(unsigned char reg, unsigned char dat)
{
    ICM20602_CS_LEVEL(0);
    Write_8bit_ICM20602(reg);
    Write_8bit_ICM20602(dat);
    ICM20602_CS_LEVEL(1);
}

/**
*
* @brief    ICM20602�����Ƕ�����
* @param    reg                 �Ĵ���
* @param    data                �Ѷ��������ݴ���data
* @param    num                 ���ݸ���
* @return   void
* @notes    ICM20602.c�ļ��ڲ�����,�û�������ó���
* Example:  Read_Datas_ICM20602(0x00,0x00,1);
*
**/
static void Read_Datas_ICM20602(unsigned char reg, unsigned char *dat, unsigned int num)
{
    ICM20602_CS_LEVEL(0);
    Write_8bit_ICM20602(reg|0x80);
    while(num--)
        Read_8bit_ICM20602(dat++);
    ICM20602_CS_LEVEL(1);
}
