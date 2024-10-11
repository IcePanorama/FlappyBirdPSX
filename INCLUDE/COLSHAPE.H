#ifndef _COLLISION_SHAPES_H_
#define _COLLISION_SHAPES_H_

#include "fb_defs.h"
#include "stdbool.h"
#include "stdint.h"
#include "vec2.h"

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

#define COLSHAPE_POOL_MAX_NUM_COLSHAPES (21)  // TODO: adjust this value.

/** Collision shapes. Centered around its parent, sharing data with them. */
typedef struct ColShape_s
{
  bool_t in_use;
  LINE_F2 wireframe_lines[COLSHP_NUM_WIREFRAME_LNS];
} ColShape_t;

extern ColShape_t col_shape_pool[COLSHAPE_POOL_MAX_NUM_COLSHAPES];

ColShape_t *get_new_col_shape_from_pool (void);
void free_col_shape (ColShape_t *cs);
void update_col_shape_xy_from_center (ColShape_t *cs, Vec2_t center,
                                      uint16_t width, uint16_t height);
void update_col_shape_xy_directly (ColShape_t *cs, uint16_t left_x,
                                   uint16_t right_x, uint16_t top_y,
                                   uint16_t bot_y);

#endif /* _COLLISION_SHAPES_H_ */
