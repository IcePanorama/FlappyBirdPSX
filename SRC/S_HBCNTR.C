#include "S_HBCNTR.H"

#include <stdio.h>

// Counter resets every 240 HBlanks
#define HBLANK_COUNTER_MAX 240

static int counter;
static int prev_counter;

void
s_init_hblanks_counter (void)
{
  counter = prev_counter = 0;
  ResetRCnt (RCntCNT1);
  SetRCnt (RCntCNT1, HBLANK_COUNTER_MAX, RCntMdINTR);
  StartRCnt (RCntCNT1);
}

void
s_sys_start (void)
{
  ResetRCnt (RCntCNT1);
  prev_counter = GetRCnt (RCntCNT1);
}

void
s_sys_end (void)
{
  counter = GetRCnt (RCntCNT1) - prev_counter;
}

int
s_delta_time (void)
{
  return counter;
}
