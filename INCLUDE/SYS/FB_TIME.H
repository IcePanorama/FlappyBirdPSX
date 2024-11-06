#ifndef _FLAPPY_BIRD_PSX_TIME_H_
#define _FLAPPY_BIRD_PSX_TIME_H_

#include <libapi.h>
#include <kernel.h>

#define TIME_GET_CURRENT_TIME() (GetRCnt (RCntCNT1) & 0xFFFF)

#endif /* _FLAPPY_BIRD_PSX_TIME_H_ */
