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

#define SPRITES_MAX_NUM_SPRITES (32)

typedef struct SpriteCompnt_s
{
  int8_t i8_parent_id;
  POLY_F4 p4_sprite;
} SpriteCompnt_t;

extern SpriteCompnt_t sp_sprite_pool[(SPRITES_MAX_NUM_SPRITES)];
//FIXME: add type to name
extern uint8_t sp_num_sprites;

void create_new_sprite (int8_t i8_id);
void destroy_sprite (int8_t i8_id);
SpriteCompnt_t *get_sprite_with_id (int8_t i8_id);

#endif /* _SPRITE_COMPONENT_H_ */
