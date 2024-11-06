#ifndef _PIPES_ENTITY_H_
#define _PIPES_ENTITY_H_

#include "sys/fb_bools.h"
#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"

typedef struct PipesEntity_s
{
  /*
   *  Can't really use the EntityID_t type while hashing, need to rework how
   *  this is used.
   */
  uint8_t u8_eid;
  // In camera-space, mark this somehow.
  Vec2_t v2_origin;
  struct PhysicsCompnt_s *ppc_physics_compnts[2];
  struct SpriteCompnt_s *psc_sprite_compnts[2];
} PipesEntity_t;

PipesEntity_t pie_create_pipes_entity (void);
void pie_destroy_pipes_entity (PipesEntity_t *pe);
void pie_update_pipes_sprite_xy (struct SpriteCompnt_s *sc, Vec2_t *v2_pos);
//FIXME: remove? unused.
bool_t pie_should_be_destroyed (PipesEntity_t *pe);

#endif /* _PIPES_ENTITY_H_ */
