#include "compnts/sprites.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#include <assert.h>
#endif /* DEBUG_BUILD */

SpriteCompnt_t sp_sprite_pool[SPRITES_MAX_NUM_SPRITES] = {{0}};
uint8_t sp_num_sprites = 0;

void
create_new_sprite (int8_t i8_id)
{
#ifdef DEBUG_BUILD
  assert((sp_num_sprites + 1) < SPRITES_MAX_NUM_SPRITES);
#endif /* DEBUG_BUILD */

  sp_sprite_pool[sp_num_sprites].i8_parent_id = i8_id;
  sp_num_sprites++;
}

void
destroy_sprite (int8_t i8_id)
{
  SpriteCompnt_t sc_tmp;
  uint8_t i;
  uint8_t u8_idx = -1;

  for (i = 0; i < sp_num_sprites; i++)
  {
    if (sp_sprite_pool[i].i8_parent_id == i8_id)
    {
      u8_idx = i;
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(u8_idx != (uint8_t)-1);
#endif /* DEBUG_BUILD */

  /* if the given sprite is last active sprite, swapping isn't necessary. */
  if (u8_idx != (sp_num_sprites - 1))
  {
    sc_tmp = sp_sprite_pool[u8_idx];
    sp_sprite_pool[u8_idx] = sp_sprite_pool[sp_num_sprites - 1];
    sp_sprite_pool[sp_num_sprites - 1] = sc_tmp;
  }

  sp_num_sprites--;
}

SpriteCompnt_t *
get_sprite_with_id (int8_t i8_id)
{
  uint8_t i;

  for (i = 0; i < sp_num_sprites; i++)
  {
    if (sp_sprite_pool[i].i8_parent_id == i8_id)
    {
      return &sp_sprite_pool[i];
    }
  }

#ifdef DEBUG_BUILD
  printf ("Sprite with id, %d, not found!\n", i8_id);
  assert(0);
#endif /* DEBUG_BUILD */

  return 0;
}
