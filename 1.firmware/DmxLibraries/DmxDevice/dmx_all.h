#ifndef _DMX_ALL_H_
#define _DMX_ALL_H_

// C语言库文件
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

// 呆萌侠二次封装驱动头文件
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

// 呆萌侠外围设备头文件
#include "dmx_icm20602.h"
#include "dmx_icm42688.h"
#include "dmx_ips.h"
#include "dmx_mpu.h"
#include "dmx_oled.h"
#include "dmx_tft180.h"
#include "dmx_tof400c.h"
#include "dmx_tsl1401.h"

// USER头文件
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
#include "gui.h"
#include "button.h"
#include "sht41.h"

#endif