#include "compnts/sprites.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

SpritePool_t sprite_pools[(SPRITES_NUM_POOLS)];

void
sc_init_sprite_compnt_pool (void)
{
  uint8_t i, j;

  memset (sprite_pools, 0, sizeof (SpritePool_t) * (SPRITES_NUM_POOLS));

//CLUT is incorrect for one of these?
  sprite_pools[TEXTID_PIPE_BOT_TEXTURE].texture_id = TEXTID_PIPE_BOT_TEXTURE;
  sprite_pools[TEXTID_TMP].texture_id = TEXTID_TMP;

  for (i = 0; i < (SPRITES_NUM_POOLS); i++)
  {
    sprite_pools[i].u8_num_sprites = 0;
    for (j = 0; j < (SPRITES_MAX_NUM_SPRITES); j++)
    {
      SetSprt (&sprite_pools[i].sprites[j].sprite);
      // FIXME: sprites look blown out?
      setRGB0(&sprite_pools[i].sprites[j].sprite, 0xFF, 0xFF, 0xFF);
      sprite_pools[i].sprites[j].sprite.clut =
        texture_clut_lookup[sprite_pools[i].texture_id];
    }
  }
}

SpriteCompnt_t *
sc_create_new_sprite (SpritePool_t *sp, uint8_t u8_id)
{
#ifdef DEBUG_BUILD
  assert((sp->u8_num_sprites + 1) < (SPRITES_MAX_NUM_SPRITES));
#endif /* DEBUG_BUILD */

  sp->sprites[sp->u8_num_sprites].u8_parent_id = u8_id;
  sp->u8_num_sprites++;
  return &sp->sprites[sp->u8_num_sprites - 1];
}

void
destroy_sprite (SpritePool_t *sp, uint8_t u8_id)
{
  SpriteCompnt_t sc_tmp;
  uint8_t i;
  uint8_t u8_idx = -1;

  for (i = 0; i < sp->u8_num_sprites; i++)
  {
    if (sp->sprites[i].u8_parent_id == u8_id)
    {
      u8_idx = i;
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(u8_idx < sp->u8_num_sprites);
#endif /* DEBUG_BUILD */

  /* if the given sprite is last active sprite, swapping isn't necessary. */
  if (u8_idx != (sp->u8_num_sprites - 1))
  {
    sc_tmp = sp->sprites[u8_idx];
    sp->sprites[u8_idx] = sp->sprites[sp->u8_num_sprites - 1];
    sp->sprites[sp->u8_num_sprites - 1] = sc_tmp;
  }

  sp->u8_num_sprites--;
}

SpriteCompnt_t *
get_sprite_with_id (SpritePool_t *sp, uint8_t u8_id)
{
  uint8_t i;

  for (i = 0; i < sp->u8_num_sprites; i++)
  {
    if (sp->sprites[i].u8_parent_id == u8_id)
    {
      return &sp->sprites[i];
    }
  }

#ifdef DEBUG_BUILD
  printf ("Sprite with id, %d, not found!\n", u8_id);
  assert(0);
#endif /* DEBUG_BUILD */

  return 0;
}
