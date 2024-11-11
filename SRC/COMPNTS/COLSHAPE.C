#include "compnts/colshape.h"

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

void
csc_create_new_col_shape (uint8_t u8_id)
{
#ifdef DEBUG_BUILD
  assert((u8_csc_num_col_shapes + 1) < COLSHP_MAX_NUM_COL_SHAPES);
#endif /* DEBUG_BUILD */

  csc_col_shape_pool[u8_csc_num_col_shapes].u8_parent_id = u8_id;
  u8_csc_num_col_shapes++;
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
  assert(u8_idx >= COLSHP_MAX_NUM_COL_SHAPES);
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
  printf ("Sprite with id, %d, not found!\n", u8_id);
  assert(0);
#endif /* DEBUG_BUILD */

  return 0;
}
