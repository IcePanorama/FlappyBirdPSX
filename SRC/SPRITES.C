#include "sprites.h"
#include "stdint.h"

#ifdef DEBUG_BUILD
#include <assert.h>
#include <stdio.h>
#endif /* DEBUG_BUILD */

Sprite_t sprite_pool[SPRITE_POOL_MAX_NUM_SPRITES] = {{0}};

Sprite_t *
get_new_sprite_from_pool (void)
{
  uint16_t i;

  /*
   *  So long as `SPRITE_POOL_MAX_NUM_SPRITES` is low, this shouldn't be too
   *  inefficient. At the very least, it's the best solution I could come up
   *  with for the time being.
   */
  for (i = 0; i < SPRITE_POOL_MAX_NUM_SPRITES; i++)
  {
    if (!sprite_pool[i].in_use)
    {
      sprite_pool[i].in_use = TRUE;
      return &sprite_pool[i];
    }
  }

#ifdef DEBUG_BUILD
  printf ("ERROR: Sprite pool full!");
  assert(FALSE);
#endif /* DEBUG_BUILD */

  return 0;
}

void
init_sprite_w_rgb (POLY_F4 *s, uint8_t r, uint8_t g, uint8_t b)
{
  SetPolyF4 (s);
  SetShadeTex (s, 1);
  setRGB0(s, r, g, b);
}

void
free_sprite (Sprite_t *s)
{
  uint16_t i;

  // See note above.
  for (i = 0; i < SPRITE_POOL_MAX_NUM_SPRITES; i++)
  {
    if (&sprite_pool[i] == s)
    {
      sprite_pool[i].in_use = FALSE;
      return;
    }
  }
}

#undef MAX_NUM_SPRITES
