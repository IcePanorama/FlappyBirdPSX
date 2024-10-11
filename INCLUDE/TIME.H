/** TIME - time related utilities. */
#ifndef _TIME_H_
#define _TIME_H_

#include <libapi.h>
#include <kernel.h>

/* Bits 16-31 of RCntCNT1 are garbage. @see: psx-spx.consoledev.net/timers/ */
#define TIME_GET_CURRENT_TIME() (GetRCnt(RCntCNT1) & 0xFF)

#endif /* _TIME_H_ */
