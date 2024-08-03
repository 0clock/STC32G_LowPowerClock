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
 * @file       dmx_icm42688.c
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
#include "dmx_delay.h"
#include "dmx_icm42688.h"

#include "dmx_oled.h"
#include "dmx_ips.h"
#include "dmx_tft180.h"

// ICM42688���ٶȼ�����
float icm42688_acc_x  = 0, icm42688_acc_y  = 0, icm42688_acc_z  = 0;
// ICM42688�Ǽ��ٶ�����
float icm42688_gyro_x = 0, icm42688_gyro_y = 0, icm42688_gyro_z = 0;    

// ��̬��������,���º�����Ϊ��.c�ļ��ڲ�����
static void Write_Data_ICM42688(unsigned char reg, unsigned char dat);
static void Read_Datas_ICM42688(unsigned char reg, unsigned char *dat, unsigned int num);
// ����ת��Ϊʵ���������ݵ�ת��ϵ��
static float icm42688_acc_inv = 1, icm42688_gyro_inv = 1; 

/**
*
* @brief    ICM42688�����ǳ�ʼ��
* @param
* @return   void
* @notes    �û�����
* Example:  Init_ICM42688();
*
**/
void Init_ICM42688(void)
{
    // ��ʼ����ʱ
    char time = 50;
    // ��ȡ�������ͺ��������Լ�
    unsigned char model = 0xff;
	
    while(1)
    {
        Read_Datas_ICM42688(ICM42688_WHO_AM_I, &model, 1);
        if(model == 0x47)
        {  
            // ICM42688
            break;
        }
        else
        {
            ICM42688_DELAY_MS(10);
            time--;
            if(time < 0)
            {
                switch(output_device_flag)
                {
                    case 0:printf("ICM42688 Init Error!\r\n");break;
                    case 1:Show_String_OLED(0,0,"ICM42688 Init Error!",Show6x8);break;
                    case 2:Show_String_IPS(0,0,"ICM42688 Init Error!",BLACK,WHITE,Show8x16);break;
                    case 3:Show_String_TFT180(0,0,"ICM42688 Init Error!",BLACK,WHITE,Show8x16);break;
                    default:printf("ICM42688 Init Error!\r\n");break;
                }
                while(1);
                // ��������ԭ�������¼���
                // ICM42688����,������µĸ��ʼ���
                // ���ߴ������û�нӺ�
            }
        }
    }
		Write_Data_ICM42688(ICM42688_PWR_MGMT0,0x00);				// ��λ�豸
		ICM42688_DELAY_MS(10);															// ������PWR��MGMT0�Ĵ�����200us�ڲ������κζ�д�Ĵ����Ĳ���

    // ����ICM42688���ٶȼƺ������ǵ����̺��������
    Set_LowpassFilter_Range_ICM42688(ICM42688_AFS_16G, ICM42688_AODR_32000HZ, ICM42688_GFS_2000DPS, ICM42688_GODR_32000HZ);

    Write_Data_ICM42688(ICM42688_PWR_MGMT0, 0x0f);      // ����GYRO_MODE,ACCEL_MODEΪ������ģʽ
    ICM42688_DELAY_MS(10);
}

/**
*
* @brief    ���ICM42688�����Ǽ��ٶ�
* @param
* @return   void
* @notes    ��λ:g(m/s^2),�û�����
* Example:  Get_Acc_ICM42688();
*
**/
void Get_Acc_ICM42688(void)
{
    unsigned char dat[6];
    Read_Datas_ICM42688(ICM42688_ACCEL_DATA_X1, dat, 6);
    icm42688_acc_x = icm42688_acc_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    icm42688_acc_y = icm42688_acc_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    icm42688_acc_z = icm42688_acc_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}

/**
*
* @brief    ���ICM42688�����ǽǼ��ٶ�
* @param
* @return   void
* @notes    ��λΪ:��/s,�û�����
* Example:  Get_Gyro_ICM42688();
*
**/
void Get_Gyro_ICM42688(void)
{
    unsigned char dat[6];
    Read_Datas_ICM42688(ICM42688_GYRO_DATA_X1, dat, 6);
    icm42688_gyro_x = icm42688_gyro_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    icm42688_gyro_y = icm42688_gyro_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    icm42688_gyro_z = icm42688_gyro_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}

/**
*
* @brief    ����ICM42688�����ǵ�ͨ�˲������������
* @param    afs                 // ���ٶȼ�����,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @param    aodr                // ���ٶȼ��������,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @param    gfs                 // ����������,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @param    godr                // �������������,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @return   void
* @notes    ICM42688.c�ļ��ڲ�����,�û�������ó���
* Example:  Set_LowpassFilter_Range_ICM42688(ICM42688_AFS_16G,ICM42688_AODR_32000HZ,ICM42688_GFS_2000DPS,ICM42688_GODR_32000HZ);
*
**/
void Set_LowpassFilter_Range_ICM42688(enum icm42688_afs afs, enum icm42688_aodr aodr, enum icm42688_gfs gfs, enum icm42688_godr godr)
{
    Write_Data_ICM42688(ICM42688_ACCEL_CONFIG0, (afs << 5) | (aodr + 1));   // ��ʼ��ACCEL���̺��������(p77)
    Write_Data_ICM42688(ICM42688_GYRO_CONFIG0, (gfs << 5) | (godr + 1));    // ��ʼ��GYRO���̺��������(p76)

    switch(afs)
    {
    case ICM42688_AFS_2G:
        icm42688_acc_inv = 2000 / 32768.0f;             // ���ٶȼ�����Ϊ:��2g
        break;
    case ICM42688_AFS_4G:
        icm42688_acc_inv = 4000 / 32768.0f;             // ���ٶȼ�����Ϊ:��4g
        break;
    case ICM42688_AFS_8G:
        icm42688_acc_inv = 8000 / 32768.0f;             // ���ٶȼ�����Ϊ:��8g
        break;
    case ICM42688_AFS_16G:
        icm42688_acc_inv = 16000 / 32768.0f;            // ���ٶȼ�����Ϊ:��16g
        break;
    default:
        icm42688_acc_inv = 1;                           // ��ת��Ϊʵ������
        break;
    }
    switch(gfs)
    {
    case ICM42688_GFS_15_625DPS:
        icm42688_gyro_inv = 15.625f / 32768.0f;         // ����������Ϊ:��15.625dps
        break;
    case ICM42688_GFS_31_25DPS:
        icm42688_gyro_inv = 31.25f / 32768.0f;          // ����������Ϊ:��31.25dps
        break;
    case ICM42688_GFS_62_5DPS:
        icm42688_gyro_inv = 62.5f / 32768.0f;           // ����������Ϊ:��62.5dps
        break;
    case ICM42688_GFS_125DPS:
        icm42688_gyro_inv = 125.0f / 32768.0f;          // ����������Ϊ:��125dps
        break;
    case ICM42688_GFS_250DPS:
        icm42688_gyro_inv = 250.0f / 32768.0f;          // ����������Ϊ:��250dps
        break;
    case ICM42688_GFS_500DPS:
        icm42688_gyro_inv = 500.0f / 32768.0f;          // ����������Ϊ:��500dps
        break;
    case ICM42688_GFS_1000DPS:
        icm42688_gyro_inv = 1000.0f / 32768.0f;         // ����������Ϊ:��1000dps
        break;
    case ICM42688_GFS_2000DPS:
        icm42688_gyro_inv = 2000.0f / 32768.0f;         // ����������Ϊ:��2000dps
        break;
    default:
        icm42688_gyro_inv = 1;                          // ��ת��Ϊʵ������
        break;
    }
}              

/**
*
* @brief    ICM42688������д8bit����
* @param    data                ����
* @return   void
* @notes    ICM42688.c�ļ��ڲ�����,�û�������ó���
* Example:  Write_8bit_ICM42688(0x00);
*
**/
static void Write_8bit_ICM42688(unsigned char dat)  
{
    unsigned char temp = 0;
    for(temp = 8; temp > 0; temp --)
    {
        ICM42688_SPC_LEVEL(0);
        if(0x80 & dat)
        {
            ICM42688_SDI_LEVEL(1);
        }
        else
        {
            ICM42688_SDI_LEVEL(0);
        }
        ICM42688_SPC_LEVEL(1);
        dat <<= 1;
    }
}

/**
*
* @brief    ICM42688�����Ƕ�8bit����
* @param    data                ����
* @return   void
* @notes    ICM42688.c�ļ��ڲ�����,�û�������ó���
* Example:  Read_8bit_ICM42688(dat);
*
**/
static void Read_8bit_ICM42688(unsigned char *dat)
{
    unsigned char temp = 0;
    for(temp = 8; temp > 0; temp --)
    {
        ICM42688_SPC_LEVEL(0);
        *dat = *dat << 1;
        *dat |= ICM42688_SDO;
        ICM42688_SPC_LEVEL(1);
    }
}

/**
*
* @brief    ICM42688������д����
* @param    reg                 �Ĵ���
* @param    data                ��Ҫд���üĴ���������
* @return   void
* @notes    ICM42688.c�ļ��ڲ�����,�û�������ó���
* Example:  Write_Data_ICM42688(0x00,0x00);
*
**/
static void Write_Data_ICM42688(unsigned char reg, unsigned char dat)
{
    ICM42688_CS_LEVEL(0);
    Write_8bit_ICM42688(reg);
    Write_8bit_ICM42688(dat);
    ICM42688_CS_LEVEL(1);
}

/**
*
* @brief    ICM42688�����Ƕ�����
* @param    reg                 �Ĵ���
* @param    data                �Ѷ��������ݴ���data
* @param    num                 ���ݸ���
* @return   void
* @notes    ICM42688.c�ļ��ڲ�����,�û�������ó���
* Example:  Read_Datas_ICM42688(0x00,0x00,1);
*
**/
static void Read_Datas_ICM42688(unsigned char reg, unsigned char *dat, unsigned int num)
{
    ICM42688_CS_LEVEL(0);
		reg |= 0x80;
    Write_8bit_ICM42688(reg);
    while(num--)
        Read_8bit_ICM42688(dat++);
    ICM42688_CS_LEVEL(1);
}