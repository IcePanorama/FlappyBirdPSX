#ifndef _SPRITE_COMPONENT_H_
#define _SPRITE_COMPONENT_H_

/* Don't touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"

#define SPRITES_MAX_NUM_SPRITES (32)

typedef struct SpriteCompnt_s
{
  uint8_t i8_parent_id;
  POLY_F4 p4_sprite;
  uint8_t u8_width;
  uint8_t u8_height;
  // Sprite update function
  void (* update)(struct SpriteCompnt_s *s, Vec2_t *v2_pos);
} SpriteCompnt_t;

extern SpriteCompnt_t sp_sprite_pool[(SPRITES_MAX_NUM_SPRITES)];
//FIXME: add type to name
extern uint8_t sp_num_sprites;

void sc_init_sprite_compnt_pool (void);
void create_new_sprite (uint8_t u8_id);
void destroy_sprite (uint8_t u8_id);
SpriteCompnt_t *get_sprite_with_id (uint8_t u8_id);

#endif /* _SPRITE_COMPONENT_H_ */
