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
 * @file       dmx_delay.c
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

#include "dmx_board.h"
#include "dmx_delay.h"

/**
*
* @brief    软件延时函数
* @param    ms					需要延时的时间
* @return   void
* @notes    单位:ms
* Example:  delay_ms(10);	// 软件延时10ms
*
**/
void delay_ms(int ms)
{
    unsigned int i;
    while(ms--)
    {
        i = MAIN_FOSC / 6030;
        while(--i);
    }
}

/**
 * @brief    软件延时函数（微秒级）
 * 
 * @param    us 需要延时的时间，单位:微秒
 * @return   void
 * @notes    使用软件循环实现的微秒级延时，延时时间可能会因编译器优化或CPU频率变化有所误差。
 * @example  delay_us(100);  // 软件延时100微秒
 */
void delay_us(int us)
{
    unsigned int i;
    while(us--)
    {
        i = MAIN_FOSC / 6030 / 1000;  // 根据主频和目标延时计算循环次数
        while(--i);
    }
}
