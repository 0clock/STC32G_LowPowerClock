/*
 * @Author: 0clock
 * @Date: 2024-08-25 23:26:57
 * @LastEditors: 0clock 3075814634@qq.com
 * @LastEditTime: 2024-08-25 23:50:30
 * @FilePath: \STC32_LowPowerClock\1.firmware\Code\button.h
 * @Description:
 *
 * Copyright (c) 2024 by 0clock, All Rights Reserved.
 */
#ifndef BUTTON_H
#define BUTTON_H

#include "stc32g.h"

// 定义按键对应的IO口，根据实际连接的IO口进行修改
#define BUTTON_SET_PIN P46
#define BUTTON_UP_PIN P45
#define BUTTON_DOWN_PIN P27
#define BUTTON_ESC_PIN P26

// 按键标识符枚举
typedef enum
{
    BUTTON_SET,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_ESC,
    BUTTON_COUNT // 按键数量
} button_id;

// 按键状态枚举
typedef enum
{
    BUTTON_IDLE,
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    BUTTON_LONG_PRESSED
} button_state;

// 按键事件枚举
typedef enum
{
    BUTTON_EVENT_NONE,
    BUTTON_EVENT_CLICK,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_DOUBLE_CLICK
} button_event;

// 按键结构体
typedef struct
{
    button_state state;
    unsigned int press_time;   // 按下的持续时间
    unsigned int release_time; // 释放的持续时间
    button_event event;        // 记录按键事件
} button;

// 按键库API函数声明
void buttons_init();
void buttons_scan();
button_event button_get_event(button_id id);

#endif // BUTTON_H
