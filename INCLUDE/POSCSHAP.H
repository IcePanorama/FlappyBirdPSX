#ifndef _POSITIONAL_COLISION_SHAPES_H_
#define _POSITIONAL_COLISION_SHAPES_H_

#include "fb_defs.h"
#include "stdbool.h"
#include "stdint.h"
#include "vec2.h"

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

#define POSCSHAPE_POOL_MAX_NUM_POS_CSHAPES (32)  // TODO: adjust this value.

/**
 *  Positional collision shapes. Have their own independent position/size.
 *  FIXME: size of pool for these and pool for pipes should be (2:1). There
 *  will never be more than 2 collision shapes per 1 pipe. This number should be
 *  shared between the two somehow.
 */
typedef struct PosColShape_s
{
  bool_t in_use;
  LINE_F2 wireframe_lines[COLSHP_NUM_WIREFRAME_LNS];
} PosColShape_t;

extern PosColShape_t pos_col_shape_pool[POSCSHAPE_POOL_MAX_NUM_POS_CSHAPES];

PosColShape_t *get_new_pos_col_shape_from_pool (void);
void free_pos_col_shape (PosColShape_t *cs);
void update_pos_col_shape_xy (PosColShape_t *cs, Vec2_t new_pos);

#endif /* _POSITIONAL_COLISION_SHAPES_H_ */
