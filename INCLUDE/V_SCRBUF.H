#ifndef _V_SCREEN_BUFFER_H_
#define _V_SCREEN_BUFFER_H_

#include <libgpu.h>
#include <libgte.h>
#include <sys/types.h>

// Max possible value is 4096.
// TODO: adjust me!
#define OT_MAX_LEN (4096)
#define MAX_NUM_SPRITES (16)
#define MAX_NUM_WIRES (64)

typedef struct ScreenBuffer_s {
  DRAWENV draw_env;
  DISPENV disp_env;
  u_long ordering_table[OT_MAX_LEN];
  POLY_F4 *sprites[MAX_NUM_SPRITES];
  u_long num_sprites;
#ifdef WIREFRAME
  LINE_F2 *wireframes[MAX_NUM_WIRES];
  u_long num_wires;
#endif /* WIREFRAME */
} ScreenBuffer_t;

#endif /* _V_SCREEN_BUFFER_H_ */
