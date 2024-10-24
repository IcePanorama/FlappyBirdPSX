#ifndef _PLAYER_ENTITY_H_
#define _PLAYER_ENTITY_H_

#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"  // FIXME: remove, unused in this header.

typedef struct PlayerEntity_s
{
  uint8_t eid_id;
  struct PhysicsCompnt_s *ppc_physics_compnt;
  uint8_t u8_width;
  uint8_t u8_height;
  struct SpriteCompnt_s *psc_sprite_compnt;
  uint8_t flags;  // FIXME: currently unused, remove.
} PlayerEntity_t;

PlayerEntity_t create_player_entity (void);
void destroy_player_entity (PlayerEntity_t *pe);
void update_player_sprite_xy (struct SpriteCompnt_s *sc, Vec2_t *v2_pos);
void make_player_jump (void *e); // voidptr for command pattern

#endif /* _PLAYER_ENTITY_H_ */
