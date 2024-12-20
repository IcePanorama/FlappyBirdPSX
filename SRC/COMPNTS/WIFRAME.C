#include "compnts/wiframe.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

WireframeCompnt_t wfc_wireframe_pool[(WIFRAME_MAX_NUM_WIREFRAMES)];
uint8_t u8_wfc_num_wireframes;

void
wfc_init_wireframe_compnt_pool (void)
{
  memset (wfc_wireframe_pool, 0,
          sizeof (WireframeCompnt_t) * (WIFRAME_MAX_NUM_WIREFRAMES));
  u8_wfc_num_wireframes = 0;
}

void
wfc_create_new_wireframe (uint8_t u8_id)
{
  uint8_t i;
#ifdef DEBUG_BUILD
  assert((u8_wfc_num_wireframes + 1) < (WIFRAME_MAX_NUM_WIREFRAMES));
#endif /* DEBUG_BUILD */

  wfc_wireframe_pool[u8_wfc_num_wireframes].u8_parent_id = u8_id;

  for (i = 0; i < WIFRAME_NUM_WIRES; i++)
  {
    SetLineF2(&wfc_wireframe_pool[u8_wfc_num_wireframes].wires[i]);
    setRGB0(&wfc_wireframe_pool[u8_wfc_num_wireframes].wires[i], 255, 255, 255);
  }

  u8_wfc_num_wireframes++;
}

void
wfc_destroy_wireframe (uint8_t u8_id)
{
  WireframeCompnt_t wfc_tmp;
  uint8_t i;
  uint8_t u8_idx = -1;

  for (i = 0; i < u8_wfc_num_wireframes; i++)
  {
    if (wfc_wireframe_pool[i].u8_parent_id == u8_id)
    {
      u8_idx = i;
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(u8_idx != (uint8_t)(-1));
#endif /* DEBUG_BUILD */

  /* if the given sprite is last active sprite, swapping isn't necessary. */
  if (u8_idx != (u8_wfc_num_wireframes - 1))
  {
    wfc_tmp = wfc_wireframe_pool[u8_idx];
    wfc_wireframe_pool[u8_idx] = wfc_wireframe_pool[u8_wfc_num_wireframes - 1];
    wfc_wireframe_pool[u8_wfc_num_wireframes - 1] = wfc_tmp;
  }

  u8_wfc_num_wireframes--;
}

WireframeCompnt_t *
wfc_get_wireframe_with_id (uint8_t u8_id)
{
  uint8_t i;

  for (i = 0; i < u8_wfc_num_wireframes; i++)
  {
    if (wfc_wireframe_pool[i].u8_parent_id == u8_id)
    {
      return &wfc_wireframe_pool[i];
    }
  }

#ifdef DEBUG_BUILD
  printf ("Sprite with id, %d, not found!\n", u8_id);
  assert(0);
#endif /* DEBUG_BUILD */

  return 0;
}

void
update_wireframe_xy (WireframeCompnt_t *w, int16_t i16_left_x,
                     int16_t i16_right_x, int16_t i16_top_y, int16_t i16_bot_y)
{
  // Top left to top right
  setXY2(&w->wires[0],  i16_left_x, i16_top_y, i16_right_x, i16_top_y);
  // Top right to bot left
  setXY2(&w->wires[1], i16_right_x, i16_top_y,  i16_left_x, i16_bot_y);
  // Bot left to top left
  setXY2(&w->wires[2],  i16_left_x, i16_bot_y,  i16_left_x, i16_top_y);
  // Top right to bot right
  setXY2(&w->wires[3], i16_right_x, i16_top_y, i16_right_x, i16_bot_y);
  // Bot right to bot left
  setXY2(&w->wires[4], i16_right_x, i16_bot_y,  i16_left_x, i16_bot_y);
}
