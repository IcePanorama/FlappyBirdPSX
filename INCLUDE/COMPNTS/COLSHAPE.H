#ifndef _COLLISION_SHAPE_H_
#define _COLLISION_SHAPE_H_

#include "sys/fb_ints.h"

#define COLSHP_MAX_NUM_COL_SHAPES (32)

typedef struct ColShapeCompnt_s
{
  uint8_t u8_parent_id;
  uint8_t u8_width;
  uint8_t u8_height;
} ColShapeCompnt_t;

extern ColShapeCompnt_t csc_col_shape_pool[(COLSHP_MAX_NUM_COL_SHAPES)];
extern uint8_t u8_csc_num_col_shapes;

void csc_init_col_shape_compnt_pool (void);
void csc_create_new_col_shape (uint8_t u8_id);
void csc_destroy_col_shape (uint8_t u8_id);
ColShapeCompnt_t *get_col_shape_with_id (uint8_t u8_id);

#endif /* _COLLISION_SHAPE_H_ */
