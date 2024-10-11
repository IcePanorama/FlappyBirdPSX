#include "colshape.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

ColShape_t col_shape_pool[COLSHAPE_POOL_MAX_NUM_COLSHAPES] = {{0}};

ColShape_t *
get_new_col_shape_from_pool (void)
{
  uint16_t i;
  uint16_t j;

  for (i = 0; i < COLSHAPE_POOL_MAX_NUM_COLSHAPES; i++)
  {
    if (!col_shape_pool[i].in_use)
    {
      col_shape_pool[i].in_use = TRUE;

      for (j = 0; j < COLSHP_NUM_WIREFRAME_LNS; j++)
      {
        SetLineF2 (&col_shape_pool[i].wireframe_lines[j]);
        setRGB0(&col_shape_pool[i].wireframe_lines[j], 255, 255, 255);
      }

      return &col_shape_pool[i];
    }
  }

#ifdef DEBUG_BUILD
  printf ("ERROR: Collision shape pool full!");
  assert(FALSE);
#endif /* DEBUG_BUILD */

  return 0;
}

void
free_col_shape (ColShape_t *cs)
{
  uint16_t i;

  for (i = 0; i < COLSHAPE_POOL_MAX_NUM_COLSHAPES; i++)
  {
    if (&col_shape_pool[i] == cs)
    {
      col_shape_pool[i].in_use = FALSE;
      return;
    }
  }

#ifdef DEBUG_BUILD
  printf ("Given collision shape not found in pool!");
  assert(FALSE);
#endif /* DEBUG_BUILD */
}

void
update_col_shape_xy_from_center (ColShape_t *cs, Vec2_t pos, uint16_t width,
                                 uint16_t height)
{
  uint16_t half_width  = width >> 1;
  uint16_t half_height = height >> 1;
  uint16_t left_x      = pos.x - half_width;
  uint16_t right_x     = pos.x + half_width;
  uint16_t top_y       = pos.y - half_height;
  uint16_t bot_y       = pos.y + half_height;

  //TODO: make this into a macro function for release builds.
//  update_col_shape_xy_directly (cs, left_x, right_x, top_y, bot_y);

  setXY2(&cs->wireframe_lines[0],  left_x, top_y, right_x, top_y);
  setXY2(&cs->wireframe_lines[1], right_x, top_y, right_x, bot_y);
  setXY2(&cs->wireframe_lines[2], right_x, bot_y,  left_x, bot_y);
  setXY2(&cs->wireframe_lines[3],  left_x, bot_y,  left_x, top_y);
}

void
update_col_shape_xy_directly (ColShape_t *cs, uint16_t left_x, uint16_t right_x,
                              uint16_t top_y, uint16_t bot_y)
{
 // printf ("%d, %d, %d, %d\n", left_x, right_x, top_y, bot_y);
  setXY2(&cs->wireframe_lines[0],  left_x, top_y, right_x, top_y);
  setXY2(&cs->wireframe_lines[1], right_x, top_y, right_x, bot_y);
  setXY2(&cs->wireframe_lines[2], right_x, bot_y,  left_x, bot_y);
  setXY2(&cs->wireframe_lines[3],  left_x, bot_y,  left_x, top_y);
}
