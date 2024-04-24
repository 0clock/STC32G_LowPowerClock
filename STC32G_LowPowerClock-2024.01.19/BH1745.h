#ifndef __BH1745_H
#define __BH1745_H	
#include "STC32G.h"

void BH1745_Init();

void BH1745_Rdat(u8 adds);

void BH1745_RdatX4();

void BH1745R_flag0002();
#endif