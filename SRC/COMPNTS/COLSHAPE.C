#include "compnts/colshape.h"
#include "compnts/wiframe.h"
#include "sys/fb_bools.h"
#include "sys/fb_defs.h"
#include "utils.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

ColShapeCompnt_t csc_col_shape_pool[(COLSHP_MAX_NUM_COL_SHAPES)] = {{0}};
uint8_t u8_csc_num_col_shapes = 0;

void
csc_init_col_shape_compnt_pool (void)
{
  memset (csc_col_shape_pool, 0,
          sizeof (ColShapeCompnt_t) * COLSHP_MAX_NUM_COL_SHAPES);
  u8_csc_num_col_shapes = 0;
}

ColShapeCompnt_t *
csc_create_new_col_shape (uint8_t u8_id, uint8_t u8_width, uint8_t u8_height)
{
#ifdef DEBUG_BUILD
  assert((u8_csc_num_col_shapes + 1) < COLSHP_MAX_NUM_COL_SHAPES);
#endif /* DEBUG_BUILD */

  csc_col_shape_pool[u8_csc_num_col_shapes].u8_parent_id = u8_id;
  csc_col_shape_pool[u8_csc_num_col_shapes].u8_width = u8_width;
  csc_col_shape_pool[u8_csc_num_col_shapes].u8_height = u8_height;

  u8_csc_num_col_shapes++;
  return &csc_col_shape_pool[u8_csc_num_col_shapes - 1];
}

//TODO: make macro version of this function.
void
csc_update_col_shape (ColShapeCompnt_t *csc, Vec2_t *v2_pos)
{
  csc->v2_pos = v2_pos;
}

ColShapeCompnt_t *
get_col_shape_with_id (uint8_t u8_id)
{
  uint8_t i;

  for (i = 0; i < u8_csc_num_col_shapes; i++)
  {
    if (csc_col_shape_pool[i].u8_parent_id == u8_id)
    {
      return &csc_col_shape_pool[i];
    }
  }

#ifdef DEBUG_BUILD
  printf ("Collision shape with id, %d, not found!\n", u8_id);
  assert(FALSE);
#endif /* DEBUG_BUILD */

  return 0;
}

/*
 *  FIXME: player appears 1 pixel away from pipe, yet collision is still
 *  detected. This doesn't always happen; it must be a weird timing issue.
 *  Probably won't really matter all that much in practice, but might be worth
 *  tracking down nonetheless.
 *  TODO: collision detection should be calculated in world space, not camera
 *  space. That should probably fix this bug.
 */
bool_t
csc_detect_collision (ColShapeCompnt_t *a, ColShapeCompnt_t *b)
{
  Vec2_t v2_a_cs_pos;
  Vec2_t v2_b_cs_pos;
  int16_t i16_A_MIN_X;
  int16_t i16_A_MAX_X;
  int16_t i16_A_MIN_Y;
  int16_t i16_A_MAX_Y;
  int16_t i16_B_MIN_X;
  int16_t i16_B_MAX_X;
  int16_t i16_B_MIN_Y;
  int16_t i16_B_MAX_Y;
  const uint8_t u8_A_HALF_WIDTH = a->u8_width >> 1;
  const uint8_t u8_A_HALF_HEIGHT = a->u8_height >> 1;
  const uint8_t u8_B_HALF_WIDTH = b->u8_width >> 1;
  const uint8_t u8_B_HALF_HEIGHT = b->u8_height >> 1;

  v2_convert_world_space_to_camera_space (a->v2_pos, &v2_a_cs_pos);
  v2_convert_world_space_to_camera_space (b->v2_pos, &v2_b_cs_pos);

  i16_A_MIN_X = v2_a_cs_pos.x - u8_A_HALF_WIDTH;
  i16_A_MAX_X = v2_a_cs_pos.x + u8_A_HALF_WIDTH;
  i16_A_MIN_Y = v2_a_cs_pos.y - u8_A_HALF_HEIGHT;
  i16_A_MAX_Y = v2_a_cs_pos.y + u8_A_HALF_HEIGHT;
  i16_B_MIN_X = v2_b_cs_pos.x - u8_B_HALF_WIDTH;
  i16_B_MAX_X = v2_b_cs_pos.x + u8_B_HALF_WIDTH;
  i16_B_MIN_Y = v2_b_cs_pos.y - u8_B_HALF_HEIGHT;
  i16_B_MAX_Y = v2_b_cs_pos.y + u8_B_HALF_HEIGHT;

  return (
    i16_A_MIN_X <= i16_B_MAX_X &&
    i16_A_MAX_X >= i16_B_MIN_X &&
    i16_A_MIN_Y <= i16_B_MAX_Y &&
    i16_A_MAX_Y >= i16_B_MIN_Y
  );
}

void
csc_destroy_col_shape (uint8_t u8_id)
{
  ColShapeCompnt_t csc_tmp;
  uint8_t i;
  uint8_t u8_idx = -1;

  for (i = 0; i < u8_csc_num_col_shapes; i++)
  {
    if (csc_col_shape_pool[i].u8_parent_id == u8_id)
    {
      u8_idx = i;
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(u8_idx != (uint8_t)(-1));
#endif /* DEBUG_BUILD */

  /* if the given sprite is last active sprite, swapping isn't necessary. */
  if (u8_idx != (u8_csc_num_col_shapes - 1))
  {
    csc_tmp = csc_col_shape_pool[u8_idx];
    csc_col_shape_pool[u8_idx] = csc_col_shape_pool[u8_csc_num_col_shapes - 1];
    csc_col_shape_pool[u8_csc_num_col_shapes - 1] = csc_tmp;
  }

  u8_csc_num_col_shapes--;
}
