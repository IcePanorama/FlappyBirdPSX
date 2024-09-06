#ifndef _V_SCREEN_BUFFER_H_
#define _V_SCREEN_BUFFER_H_

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

// Max possible value is 4096.
#define OT_MAX_LEN (2)

typedef struct ScreenBuffer_s {
  DRAWENV draw_env;
  DISPENV disp_env;
  u_long ordering_table[OT_MAX_LEN];
  // todo: add all our scene verts to this.
  //POLY_F4 sprite;
} ScreenBuffer_t;

#endif /* _V_SCREEN_BUFFER_H_ */
