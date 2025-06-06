#ifndef _SPRITE_COMPONENT_H_
#define _SPRITE_COMPONENT_H_

/* Don't touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "sys/fb_bools.h"
#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"
#include "video/textlkup.h"

#define SPRITES_MAX_NUM_SPRITES (32)
#define SPRITES_NUM_POOLS (TID_NUM_TEXTURES)

typedef struct SpriteCompnt_s
{
  uint8_t u8_parent_id;
  SPRT sprite;
  uint8_t u8_width;
  uint8_t u8_height;
/*
  bool_t is_animated;
  uint8_t u8_num_anim_frames;
*/
} SpriteCompnt_t;

typedef struct SpritePool_s
{
  TextureID_t texture_id;
  SpriteCompnt_t sprites[(SPRITES_MAX_NUM_SPRITES)];
  uint8_t u8_num_sprites;
} SpritePool_t;

extern SpritePool_t sprite_pools[(SPRITES_NUM_POOLS)];

void sc_init_sprite_compnt_pool (void);
SpriteCompnt_t *sc_create_new_sprite (SpritePool_t *sp, uint8_t u8_id);
void destroy_sprite (SpritePool_t *sp, uint8_t u8_id);
SpriteCompnt_t *get_sprite_with_id (SpritePool_t *sp, uint8_t u8_id);
void sc_update_sprite_xy (SpriteCompnt_t *sc, Vec2_t *v2_pos);

#endif /* _SPRITE_COMPONENT_H_ */
