#ifndef _V_SCREEN_BUFFER_H_
#define _V_SCREEN_BUFFER_H_

#include <libgpu.h>
#include <libgte.h>
#include <sys/types.h>

// Max possible value is 4096.
#define OT_MAX_LEN (2)
#define MAX_NUM_SPRITES (10)

typedef struct ScreenBuffer_s {
  DRAWENV draw_env;
  DISPENV disp_env;
  u_long ordering_table[OT_MAX_LEN];
  POLY_F4 *sprites[MAX_NUM_SPRITES];
  u_long curr_sprite;  // index of the last sprite in `sprites`.
} ScreenBuffer_t;

#endif /* _V_SCREEN_BUFFER_H_ */
