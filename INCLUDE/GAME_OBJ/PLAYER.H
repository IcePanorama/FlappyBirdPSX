/**
 *  player.h - The player entity and its related functions. PlayerEntity
 *  functions are signified by the acronym 'pe.'
 */
#ifndef _PLAYER_ENTITY_H_
#define _PLAYER_ENTITY_H_

#include "sys/fb_bools.h"
#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"

typedef struct PlayerEntity_s
{
  uint8_t u8_eid;
  uint8_t u8_flags;
  uint8_t u8_width;
  uint8_t u8_height;
  struct ColShapeCompnt_s *pcsc_col_shape;
  struct PhysicsCompnt_s *ppc_physics_compnt;
  struct SpriteCompnt_s *psc_sprite_compnt;
} PlayerEntity_t;

PlayerEntity_t pe_create_player_entity (void);
void pe_destroy_player_entity (PlayerEntity_t *pe);
void pe_update_player_sprite_xy (struct SpriteCompnt_s *sc, Vec2_t *v2_pos);
void pe_make_player_jump (void *e); // voidptr for command pattern
void pe_kill_player (PlayerEntity_t *pe);
void pe_update_player (PlayerEntity_t *pe);
bool_t pe_is_alive (PlayerEntity_t *pe);

#endif /* _PLAYER_ENTITY_H_ */
