#include "button.h"

#define KEY_SCAN_INTERVAL 10       // 按键扫描间隔(ms)
#define LONG_PRESS_THRESHOLD 500   // 长按阈值(ms)
#define DOUBLE_CLICK_THRESHOLD 200 // 双击阈值(ms)

button buttons[BUTTON_COUNT]; // 按键数组

// 初始化单个按键
void button_init(button *btn)
{
    btn->state = BUTTON_IDLE;
    btn->press_time = 0;
    btn->release_time = 0;
    btn->event = BUTTON_EVENT_NONE;
}

// 初始化所有按键
void buttons_init()
{
    int i;
    for (i = 0; i < BUTTON_COUNT; i++)
    {
        button_init(&buttons[i]);
    }
}

// 读取按键状态的函数
bit read_button_pin(button_id id)
{
    switch (id)
    {
    case BUTTON_SET:
        return BUTTON_SET_PIN; // 读取P4.6状态
    case BUTTON_UP:
        return BUTTON_UP_PIN; // 读取P4.5状态
    case BUTTON_DOWN:
        return BUTTON_DOWN_PIN; // 读取P2.7状态
    case BUTTON_ESC:
        return BUTTON_ESC_PIN; // 读取P2.6状态
    default:
        return 1; // 默认返回高电平（未按下）
    }
}

// 按键扫描函数
void button_scan(button *btn, bit is_pressed)
{
    switch (btn->state)
    {
    case BUTTON_IDLE:
        if (!is_pressed)
        { // 低电平表示按下
            btn->state = BUTTON_PRESSED;
            btn->event = BUTTON_EVENT_CLICK;
            btn->press_time = 0;
        }
        break;
    case BUTTON_PRESSED:
        if (!is_pressed)
        {
            
            btn->press_time += KEY_SCAN_INTERVAL;
            if (btn->press_time >= LONG_PRESS_THRESHOLD)
            {
                btn->state = BUTTON_LONG_PRESSED;
                btn->event = BUTTON_EVENT_LONG_PRESS;
            }
        }
        else
        {
            btn->state = BUTTON_RELEASED;
            btn->release_time = 0;
        }
        break;
    case BUTTON_RELEASED:
        if (!is_pressed)
        {
            if (btn->release_time <= DOUBLE_CLICK_THRESHOLD)
            {
                btn->event = BUTTON_EVENT_DOUBLE_CLICK;
            }
            else
            {
                btn->event = BUTTON_EVENT_CLICK;
            }
            btn->state = BUTTON_PRESSED;
            btn->press_time = 0;
        }
        else
        {
            btn->release_time += KEY_SCAN_INTERVAL;
            if (btn->release_time >= DOUBLE_CLICK_THRESHOLD)
            {
                btn->state = BUTTON_IDLE;
            }
        }
        break;
    case BUTTON_LONG_PRESSED:
        if (is_pressed)
        {
            btn->state = BUTTON_IDLE;
        }
        break;
    }
}

// 扫描所有按键
void buttons_scan()
{
    int i;
    for (i = 0; i < BUTTON_COUNT; i++)
    {
        button_scan(&buttons[i], read_button_pin((button_id)i));
    }
}

// 获取单个按键事件
button_event button_get_event(button_id id)
{
    button_event event;

    if (id >= BUTTON_COUNT)
    {
        return BUTTON_EVENT_NONE;
    }
    event = buttons[id].event;
    buttons[id].event = BUTTON_EVENT_NONE; // 清除事件
    return event;
}
