#ifndef _SCREEN_BUFFER_H_
#define _SCREEN_BUFFER_H_

#include "video/vid_defs.h"

typedef struct ScreenBuffer_s
{
  DRAWENV draw_env;
  DISPENV disp_env;
  u_long ordering_table[(FB_ORDERING_TABLE_MAX_LENGTH)];
} ScreenBuffer_t;

extern ScreenBuffer_t screen_buffers[2];

void sb_init_screen_buffers (void);

#endif /* _SCREEN_BUFFER_H_ */
