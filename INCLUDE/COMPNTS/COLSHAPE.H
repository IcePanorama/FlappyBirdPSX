#ifndef _COLLISION_SHAPE_H_
#define _COLLISION_SHAPE_H_

#include "sys/fb_bools.h"
#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"

#define COLSHP_MAX_NUM_COL_SHAPES (32)

typedef struct ColShapeCompnt_s
{
  uint8_t u8_parent_id;
  uint8_t u8_width;
  uint8_t u8_height;
  struct Vec2_s *v2_pos;
//  struct WireframeCompnt_s *wfc;
} ColShapeCompnt_t;

extern ColShapeCompnt_t csc_col_shape_pool[(COLSHP_MAX_NUM_COL_SHAPES)];
extern uint8_t u8_csc_num_col_shapes;

void csc_init_col_shape_compnt_pool (void);
//void csc_create_new_col_shape (uint8_t u8_id, uint8_t u8_width, uint8_t u8_height);
ColShapeCompnt_t *csc_create_new_col_shape (uint8_t u8_id, uint8_t u8_width, uint8_t u8_height);
void csc_destroy_col_shape (uint8_t u8_id);
//FIXME: fix name.
ColShapeCompnt_t *get_col_shape_with_id (uint8_t u8_id);
void csc_update_col_shape (ColShapeCompnt_t *csc, Vec2_t *v2_pos);
bool_t csc_detect_collision (ColShapeCompnt_t *a, ColShapeCompnt_t *b);

#endif /* _COLLISION_SHAPE_H_ */
