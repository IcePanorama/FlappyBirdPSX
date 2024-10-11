#include "poscshap.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

PosColShape_t pos_col_shape_pool[POSCSHAPE_POOL_MAX_NUM_POS_CSHAPES] = {{0}};

PosColShape_t *
get_new_pos_col_shape_from_pool (void)
{
  uint16_t i;
  uint16_t j;
  PosColShape_t *output = 0;

  for (i = 0; i < POSCSHAPE_POOL_MAX_NUM_POS_CSHAPES; i++)
  {
    if (!pos_col_shape_pool[i].in_use)
    {
      pos_col_shape_pool[i].in_use = TRUE;

      for (j = 0; j < COLSHP_NUM_WIREFRAME_LNS; j++)
      {
        SetLineF2 (&pos_col_shape_pool[i].wireframe_lines[j]);
        setRGB0(&pos_col_shape_pool[i].wireframe_lines[j], 255, 255, 255);
      }

      output = &pos_col_shape_pool[i];
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(output != 0);
#endif /* DEBUG_BUILD */

  return output;
}

void
free_pos_col_shape (PosColShape_t *cs)
{
  uint16_t i;

  for (i = 0; i < POSCSHAPE_POOL_MAX_NUM_POS_CSHAPES; i++)
  {
    if (&pos_col_shape_pool[i] == cs)
    {
      pos_col_shape_pool[i].in_use = FALSE;
      return;
    }
  }

#ifdef DEBUG_BUILD
  printf ("Given positional collision shape not found in pool!");
  assert(FALSE);
#endif /* DEBUG_BUILD */
}

void
update_pos_col_shape_xy (PosColShape_t *cs, Vec2_t new_pos)
{
  uint16_t half_width  = (cs->width >> 1);
  uint16_t half_height = (cs->height >> 1);
  uint16_t left_x;
  uint16_t right_x;
  uint16_t top_y;
  uint16_t bot_y;

  cs->position = new_pos;

  left_x  = cs->position.x - half_width;
  right_x = cs->position.x + half_width;
  top_y   = cs->position.y - half_height;
  bot_y   = cs->position.y + half_height;

  setXY2(&cs->wireframe_lines[0],  left_x, top_y, right_x, top_y);
  setXY2(&cs->wireframe_lines[1], right_x, top_y, right_x, bot_y);
  setXY2(&cs->wireframe_lines[2], right_x, bot_y,  left_x, bot_y);
  setXY2(&cs->wireframe_lines[3],  left_x, bot_y,  left_x, top_y);
}
