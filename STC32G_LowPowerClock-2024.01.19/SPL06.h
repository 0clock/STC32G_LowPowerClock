#ifndef __SPL06_H
#define __SPL06_H	
#include "STC32G.h"

#define     c0       202
#define     c1       -256

#define     c00       79481
#define     c10       51015 //-51224,方便计算变+

#define     c01       3388 //-2644,方便计算变+
#define     c11       962

#define     c20       8711  //-8708,方便计算变+
#define     c21       16     //-51,方便计算变+

#define     c30       1107  //-1072,方便计算变+

#define     k_pt_1    524288










void SPL06_Init();
void SPL06_C0R();
void SPL06_PR(u8 cmd,u8 dat);
void SPL06_Math_P(u32 dat3,u32 dat2);



void SPL06_PR_R();


void SPL06_PR_R_flag0008();


void SPL06_PR_P_flag0010();


#endif