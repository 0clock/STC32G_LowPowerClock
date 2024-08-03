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
 * @file       dmx_mpu.c
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
#include "dmx_mpu.h"

#include "dmx_oled.h"
#include "dmx_ips.h"
#include "dmx_tft180.h"

// MPU6050���ٶȼ�����
float mpu_acc_x  = 0, mpu_acc_y  = 0, mpu_acc_z  = 0;
// MPU6050����������
float mpu_gyro_x = 0, mpu_gyro_y = 0, mpu_gyro_z = 0;

// ��̬��������,���º�����Ϊ��.c�ļ��ڲ�����
static void MPU_Write_Data(unsigned char addr, unsigned char reg, unsigned char dat);
static void MPU_Read_Datas(unsigned char addr, unsigned char reg, unsigned char *dat, unsigned char num);
// ����ת��Ϊʵ���������ݵ�ת��ϵ��
static float mpu_acc_inv = 1, mpu_gyro_inv = 1;
// �����ǵ�ַ
static unsigned char GYROSCOPE_DEV_ADDR;

/**
*
* @brief    MPU6050�����ǳ�ʼ��
* @param
* @return   void
* @notes    �û�����
* Example:  Init_MPU();
*
**/
void Init_MPU(void)
{
    // ��ʼ����ʱ
    char time = 50;
    // ��ȡ�������ͺ��������Լ�
    unsigned char model = 0xff;
    unsigned char slave_addr = 0x68;
	
    while(1)
    {
        MPU_Read_Datas(slave_addr, WHO_AM_I, &model, 1);
        if(model == 0x68)
        {
            // MPU6050,104
            GYROSCOPE_DEV_ADDR = 0x68;
            break;
        }
        else if(model == 0x12)
        {
            // ICM20602
            GYROSCOPE_DEV_ADDR = 0x69;
            break;
        }
        else
        {
            slave_addr = slave_addr ^ 0x01; // ��ת�����ǵ�ַ���λ(0x69,0x68)
            MPU_DELAY_MS(10);
            time--;
            if(time < 0)
            {
                switch(output_device_flag)
                {
                    case 0:printf("MPU Init Error!\r\n");break;
                    case 1:Show_String_OLED(0,0,"MPU Init Error!",Show6x8);break;
                    case 2:Show_String_IPS(0,0,"MPU Init Error!",BLACK,WHITE,Show8x16);break;
                    case 3:Show_String_TFT180(0,0,"MPU Init Error!",BLACK,WHITE,Show8x16);break;
                    default:printf("MPU Init Error!\r\n");break;
                }
                while(1);
                // ��������ԭ�������¼���
                // MPU6050����ICM20602����,������µĸ��ʼ���
                // ���ߴ������û�нӺ�
                // ������Ҫ�����������,������3.3V
            }
        }
    }
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, PWR_MGMT_1, 0x80);
    MPU_DELAY_MS(20);

    MPU_Write_Data(GYROSCOPE_DEV_ADDR, PWR_MGMT_1, 0x00);                 // �������״̬
    MPU_DELAY_MS(20);
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, PWR_MGMT_2, 0x00);
    MPU_DELAY_MS(20);
		
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, SMPLRT_DIV, 0x07);                 // 125HZ������

    // ���������ǵ�ͨ�˲������������
    Set_LowpassFilter_Range_MPU(MPU_ABW_218, MPU_GBW_176, MPU_FS_4G, MPU_FS_2000DPS);

    MPU_Write_Data(GYROSCOPE_DEV_ADDR, USR_CONTROL, 0x00);                // �ر�MPU6050�Ը���IIC�豸�Ŀ���
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, INT_PIN_CFG, 0x02);
}

/**
*
* @brief    ���MPU6050�����Ǽ��ٶ�
* @param
* @return   void
* @notes    ��λ:g(m/s^2),�û�����
* Example:  Get_Acc_MPU();
*
**/
void Get_Acc_MPU(void)
{
    unsigned char dat[6];
    MPU_Read_Datas(GYROSCOPE_DEV_ADDR, ACCEL_XOUT_H, dat, 6);
    mpu_acc_x = mpu_acc_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    mpu_acc_y = mpu_acc_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    mpu_acc_z = mpu_acc_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}

/**
*
* @brief    ���MPU6050�����ǽǼ��ٶ�
* @param
* @return   void
* @notes    ��λΪ:��/s,�û�����
* Example:  Get_Gyro_MPU();
*
**/
void Get_Gyro_MPU(void)
{
    unsigned char dat[6];
    MPU_Read_Datas(GYROSCOPE_DEV_ADDR, GYRO_XOUT_H, dat, 6);
    mpu_gyro_x = mpu_gyro_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    mpu_gyro_y = mpu_gyro_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    mpu_gyro_z = mpu_gyro_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}

/**
*
* @brief    ����MPU6050�����ǵ�ͨ�˲������������
* @param    abw                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @param    gbw                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @param    afs                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @param    gfs                 // ����dmx_mpu.h�ļ���ö�ٶ����в鿴
* @return   void
* @notes    dmx_mpu.c�ļ��ڲ�����,�û�������ó���
* Example:  Set_LowpassFilter_Range_MPU(MPU_ABW_218, MPU_GBW_176,MPU_FS_4G, MPU_FS_2000DPS);
*
**/
void Set_LowpassFilter_Range_MPU(enum mpu_acc_bw abw, enum mpu_gyro_bw gbw, enum mpu_acc_fs afs, enum mpu_gyro_fs gfs)
{
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, MPU6050_CONFIG, (unsigned char)gbw);
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, ACCEL_CONFIG_2, (unsigned char)abw);
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, GYRO_CONFIG, (unsigned char)(gfs << 3));
    MPU_Write_Data(GYROSCOPE_DEV_ADDR, ACCEL_CONFIG, (unsigned char)(afs << 3));
    switch(afs)
    {
    case MPU_FS_2G:
        mpu_acc_inv = (float)2.0 * 9.8 / 32768.0;       // ���ٶȼ�����Ϊ:��2g
        break;
    case MPU_FS_4G:
        mpu_acc_inv = (float)4.0 * 9.8 / 32768.0;       // ���ٶȼ�����Ϊ:��4g
        break;
    case MPU_FS_8G:
        mpu_acc_inv = (float)8.0 * 9.8 / 32768.0;       // ���ٶȼ�����Ϊ:��8g
        break;
    case MPU_FS_16G:
        mpu_acc_inv = (float)16.0 * 9.8 / 32768.0;      // ���ٶȼ�����Ϊ:��16g
        break;
    default:
        mpu_acc_inv = 1;                                // ��ת��Ϊʵ������
        break;
    }
    switch(gfs)
    {
    case MPU_FS_250DPS:
        mpu_gyro_inv = (float)250.0 / 32768.0;          // ����������Ϊ:��250dps
        break;
    case MPU_FS_500DPS:
        mpu_gyro_inv = (float)500.0 / 32768.0;          // ����������Ϊ:��500dps
        break;
    case MPU_FS_1000DPS:
        mpu_gyro_inv = (float)1000.0 / 32768.0;         // ����������Ϊ:��1000dps
        break;
    case MPU_FS_2000DPS:
        mpu_gyro_inv = (float)2000.0 / 32768.0;         // ����������Ϊ:��2000dps
        break;
    default:
        mpu_gyro_inv = 1;                               // ��ת��Ϊʵ������
        break;
    }
}

/**
*
* @brief    MPU���IIC��ʼ
* @param    void
* @return   void
* @notes    �ڲ�����
* Example:  MPU_Start();
*
**/
static void MPU_Start(void)
{
    MPU_SCL_LEVEL(1);
    MPU_SDA_LEVEL(1);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_SDA_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_SCL_LEVEL(0);
}

/**
*
* @brief    MPU���IICֹͣ
* @param    void
* @return   void
* @notes    �ڲ�����
* Example:  MPU_Stop();
*
**/
static void MPU_Stop(void)
{
    MPU_SCL_LEVEL(0);
    MPU_SDA_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_SCL_LEVEL(1);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_SDA_LEVEL(1);
    MPU_DELAY_MS(MPU_DELAY);
}

/**
*
* @brief    ��������豸����Ӧ��/��Ӧ���ź� 1/0
* @param    ack            	�����ź�
* @return   void
* @notes    �ڲ�����
* Example:  MPU_Sendack(1);
*
**/
static void MPU_Sendack(unsigned char ack)
{
    MPU_SCL_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
    if(ack)
        MPU_SDA_LEVEL(0);
    else
        MPU_SDA_LEVEL(1);
    MPU_SCL_LEVEL(1);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_SCL_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
}

/**
*
* @brief    �����ȴ����豸Ӧ���ź�
* @param    void
* @return   int
* @notes    �ڲ�����
* Example:  MPU_Waitack();
*
**/
static int MPU_Waitack(void)
{
    MPU_SCL_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_SCL_LEVEL(1);
    MPU_DELAY_MS(MPU_DELAY);
    if(MPU_SDA)
    {
        MPU_SCL_LEVEL(1);
        return 0;
    }
    MPU_SCL_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
    return 1;
}

/**
*
* @brief    ���͵����ֽ�
* @param    ch                  �ֽ�
* @return   void
* @notes    �ڲ�����
* Example:  MPU_Write_Char(0X66);
*
**/
static void MPU_Write_Char(unsigned char ch)
{
    unsigned char i = 8;
    while(i--)
    {
        if(ch & 0x80)
            MPU_SDA_LEVEL(1);
        else
            MPU_SDA_LEVEL(0);
        ch <<= 1;
        MPU_DELAY_MS(MPU_DELAY);
        MPU_SCL_LEVEL(1);
        MPU_DELAY_MS(MPU_DELAY);
        MPU_SCL_LEVEL(0);
    }
    MPU_Waitack();
}

/**
*
* @brief    ����һ���ֽ�����
* @param    ack               	Ӧ���ź�
* @return   unsigned char       �����ֽ�
* @notes    �ڲ�����
* Example:  MPU_Read_Char(0);
*
**/
static unsigned char MPU_Read_Char(unsigned char ack)
{
    unsigned char i;
    unsigned char c = 0;
    MPU_SCL_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_SDA_LEVEL(1);
    for(i = 0; i < 8; i++)
    {
        MPU_DELAY_MS(MPU_DELAY);
        MPU_SCL_LEVEL(0);
        MPU_DELAY_MS(MPU_DELAY);
        MPU_SCL_LEVEL(1);
        MPU_DELAY_MS(MPU_DELAY);
        c <<= 1;
        if(MPU_SDA)
        {
            c += 1;
        }
    }
    MPU_SCL_LEVEL(0);
    MPU_DELAY_MS(MPU_DELAY);
    MPU_Sendack(ack);
    return c;
}

/**
*
* @brief    д���ݵ��豸�Ĵ�����
* @param    addr         		�豸��ַ
* @param    reg         		�豸�Ĵ�����ַ
* @param    dat          		���ݵ�ַ
* @return   void
* @notes		�ڲ�����
* Example:  MPU_Write_Data(addr,reg,data)
*
**/
static void MPU_Write_Data(unsigned char addr, unsigned char reg, unsigned char dat)
{
    MPU_Start();
    MPU_Write_Char(addr << 1);
    MPU_Write_Char(reg);
    MPU_Write_Char(dat);
    MPU_Stop();
}

/**
*
* @brief    ���豸�Ĵ�����ȡ���ֽ�����
* @param    addr         		�豸��ַ
* @param    reg         		�豸�Ĵ�����ַ
* @param    dat          		���ݵ�ַ
* @param    num           	���ݳ���
* @return   void
* @notes		�ڲ�����
* Example:  MPU_Read_Datas(addr,reg,data,num)
*
**/
static void MPU_Read_Datas(unsigned char addr, unsigned char reg, unsigned char *dat, unsigned char num)
{
    MPU_Start();
    MPU_Write_Char(addr << 1 | 0x00);
    MPU_Write_Char(reg);
    MPU_Start();
    MPU_Write_Char((addr << 1) | 0x01);
    while(--num)
    {
        *dat = MPU_Read_Char(1);
        dat++;
    }
    *dat = MPU_Read_Char(0);
    MPU_Stop();
}
