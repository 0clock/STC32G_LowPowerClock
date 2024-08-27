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
 * @file       dmx_isr.c
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

#include "dmx_all.h"

// �ⲿ�ж�0
void INT0_ISR(void) interrupt 0
{
    if (P32) // �����ش���
    {
    }
    else // �½��ش���
    {
    }
}

// �ⲿ�ж�1
void INT1_ISR(void) interrupt 2
{
    if (P33) // �����ش���
    {
    }
    else // �½��ش���
    {
    }
}

// �ⲿ�ж�2
void INT2_ISR(void) interrupt 10
{
    // �½��ش���
}

// �ⲿ�ж�3
void INT3_ISR(void) interrupt 11
{
    // �½��ش���
}

// �ⲿ�ж�4
void INT4_ISR(void) interrupt 16
{
    // �½��ش���,P30��ΪRXD����ʱ����ʹ�ø��ж�
}
// ��ʱ��0�ж� 1ms
void TIME0_ISR(void) interrupt 1
{
    static unsigned int time0_isr_count = 0;
    static unsigned char i = 1;
    time0_isr_count++;
    if (time0_isr_count % 16 == 0 && oled_refresh_flag == 1)
    {
#if 0 // ��ʱ����
        oled_draw_int(0, 56, time0_isr_count, 5, oled_gram, 1, Show6x8);
        oled_draw_string(0, 48, "voltage:", oled_gram, 1, Show6x8);
        oled_draw_int(50, 48, battery.voltage, 5, oled_gram, 1, Show6x8);
        oled_draw_string(0, 40, "current:", oled_gram, 1, Show6x8);
        oled_draw_int(50, 40, battery.current, 5, oled_gram, 1, Show6x8);
#endif
        oled_refresh(oled_gram);
        oled_refresh_flag = 0;
    }

    if (time0_isr_count % 10 == 0)
    {
        buttons_scan();
    }

    if (time0_isr_count % 1000 == 0)
    {
    }

    // ������
    //  if (time0_isr_count % i == 0)
    //  {
    //      Buzzer_Close();
    //  }
    //  else
    //  {
    //      Buzzer_Open();
    //  }
    //  if (time0_isr_count % 1000 == 0)
    //  {
    //      i++;
    //      if (i >= 10)
    //      {
    //          i = 0;
    //      }
    //  }
}
// ��ʱ��1�ж� 5ms
void TIME1_ISR(void) interrupt 3
{
    static unsigned int time1_isr_count = 0;
    time1_isr_count++;
    if (time1_isr_count % 100 == 0)
    {
        bms_main_task_500ms();
        test_update_sht41_500ms();
    }

    if (time1_isr_count % 200 == 0)
    {
        test_time_update_1s();
    }
}

// ��ʱ��2�ж�
void TIME2_ISR(void) interrupt 12
{
}

// ��ʱ��3�ж�
void TIME3_ISR(void) interrupt 19
{
}

// ��ʱ��4�ж�
void TIME4_ISR(void) interrupt 20
{
}

// ����1�ж�
void UART1_ISR(void) interrupt 4
{
    if (TI) // ��ȡ����1���ͱ�־
    {
        TI = 0; // ������1���ͱ�־
        TX_BUSY[1] = 0;
    }
    if (RI) // ��ȡ����1���ձ�־
    {
        RI = 0; // ������1���ձ�־
    }
}

// ����2�ж�
void UART2_ISR(void) interrupt 8
{
    if (S2TI) // ��ȡ����2���ͱ�־
    {
        S2TI = 0; // ������2���ͱ�־
        TX_BUSY[2] = 0;
    }
    if (S2RI) // ��ȡ����2���ձ�־
    {
        S2RI = 0; // ������2���ձ�־
    }
}

// ����3�ж�
void UART3_ISR(void) interrupt 17
{
    if (S3TI) // ��ȡ����3���ͱ�־
    {
        S3TI = 0; // ������3���ͱ�־
        TX_BUSY[3] = 0;
    }
    if (S3RI) // ��ȡ����3���ձ�־
    {
        S3RI = 0; // ������3���ձ�־
    }
}

// ����4�ж�
void UART4_ISR(void) interrupt 18
{
    if (S4TI) // ��ȡ����4���ͱ�־
    {
        S4TI = 0; // ������4���ͱ�־
        TX_BUSY[4] = 0;
    }
    if (S4RI) // ��ȡ����4���ձ�־
    {
        S4RI = 0; // ������4���ձ�־
    }
}

/**
 *
 * @brief    �ж����ȼ�����
 * @param    isr_nvic				�ж���,����isr.h��ö�ٲ鿴
 * @param    priority				���ȼ�(0~3,3��Ϊ��߼�)
 * @return   void
 * @notes    ���ڷ�������ʱ,�����ж����ȼ�Ҫ���ڶ�ʱ���ж����ȼ�
 * Example:  init_nvic(TIME1_PRIORITY,3);	// ��ʱ��1�ж����ȼ�����Ϊ��߼���3��
 *
 **/
void init_nvic(ISR_nvic_enum isr_nvic, unsigned char priority)
{
    switch (isr_nvic)
    {
    // �ⲿ�ж����ȼ�����
    case INT0_PRIORITY:
        switch (priority)
        {
        case 0:
            PX0H = 0;
            PX0 = 0;
            break;
        case 1:
            PX0H = 0;
            PX0 = 1;
            break;
        case 2:
            PX0H = 1;
            PX0 = 0;
            break;
        case 3:
            PX0H = 1;
            PX0 = 1;
            break;
        }
        break;
    case INT1_PRIORITY:
        switch (priority)
        {
        case 0:
            PX1H = 0;
            PX1 = 0;
            break;
        case 1:
            PX1H = 0;
            PX1 = 1;
            break;
        case 2:
            PX1H = 1;
            PX1 = 0;
            break;
        case 3:
            PX1H = 1;
            PX1 = 1;
            break;
        }
        break;
    case INT4_PRIORITY:
        switch (priority)
        {
        case 0:
            PX4H = 0;
            PX4 = 0;
            break;
        case 1:
            PX4H = 0;
            PX4 = 1;
            break;
        case 2:
            PX4H = 1;
            PX4 = 0;
            break;
        case 3:
            PX4H = 1;
            PX4 = 1;
            break;
        }
        break;
    // ��ʱ���ж����ȼ�����
    case TIME0_PRIORITY:
        switch (priority)
        {
        case 0:
            PT0H = 0;
            PT0 = 0;
            break;
        case 1:
            PT0H = 0;
            PT0 = 1;
            break;
        case 2:
            PT0H = 1;
            PT0 = 0;
            break;
        case 3:
            PT0H = 1;
            PT0 = 1;
            break;
        }
        break;
    case TIME1_PRIORITY:
        switch (priority)
        {
        case 0:
            PT1H = 0;
            PT1 = 0;
            break;
        case 1:
            PT1H = 0;
            PT1 = 1;
            break;
        case 2:
            PT1H = 1;
            PT1 = 0;
            break;
        case 3:
            PT1H = 1;
            PT1 = 1;
            break;
        }
        break;
    // �����ж����ȼ�����
    case UART1_PRIORITY:
        switch (priority)
        {
        case 0:
            PSH = 0;
            PS = 0;
            break;
        case 1:
            PSH = 0;
            PS = 1;
            break;
        case 2:
            PSH = 1;
            PS = 0;
            break;
        case 3:
            PSH = 1;
            PS = 1;
            break;
        }
        break;
    case UART2_PRIORITY:
        switch (priority)
        {
        case 0:
            PS2H = 0;
            PS2 = 0;
            break;
        case 1:
            PS2H = 0;
            PS2 = 1;
            break;
        case 2:
            PS2H = 1;
            PS2 = 0;
            break;
        case 3:
            PS2H = 1;
            PS2 = 1;
            break;
        }
        break;
    case UART3_PRIORITY:
        switch (priority)
        {
        case 0:
            PS3H = 0;
            PS3 = 0;
            break;
        case 1:
            PS3H = 0;
            PS3 = 1;
            break;
        case 2:
            PS3H = 1;
            PS3 = 0;
            break;
        case 3:
            PS3H = 1;
            PS3 = 1;
            break;
        }
        break;
    case UART4_PRIORITY:
        switch (priority)
        {
        case 0:
            PS4H = 0;
            PS4 = 0;
            break;
        case 1:
            PS4H = 0;
            PS4 = 1;
            break;
        case 2:
            PS4H = 1;
            PS4 = 0;
            break;
        case 3:
            PS4H = 1;
            PS4 = 1;
            break;
        }
        break;
    }
}
