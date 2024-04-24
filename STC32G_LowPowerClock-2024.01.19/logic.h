#ifndef __logic_H
#define __logic_H	
#include "STC32G.h"


void SET_key();     //主菜单 按下确认件

void ESC_key();     //二级菜单 按下退出

void Add_key(u8 a);    //主菜单 按下+ -

void key_thread1_new();

void Buzzer_task();

#endif