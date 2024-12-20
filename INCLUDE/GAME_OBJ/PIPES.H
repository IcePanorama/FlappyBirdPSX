/**
 *  pipes.h - Pipe entities and their related functions.
 *  Abbreviated as PIE for (PI)pe (E)ntity.
 */
#ifndef _PIPES_ENTITY_H_
#define _PIPES_ENTITY_H_

#include "sys/fb_bools.h"
#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"

#define PIE_PIPE_WIDTH (32)
#define PIE_HALF_PIPE_WIDTH ((PIE_PIPE_WIDTH) >> 1)

typedef struct PipesEntity_s
{
  uint8_t u8_eid;
  // In camera-space, mark this somehow.
  Vec2_t v2_origin;
  struct ColShapeCompnt_s *pcsc_col_shape_compnts[2];
  struct PhysicsCompnt_s *ppc_physics_compnts[2];
  struct SpriteCompnt_s *psc_sprite_compnts[2];
  bool_t b_was_scored;
} PipesEntity_t;

PipesEntity_t pie_create_pipes_entity (void);
void pie_destroy_pipes_entity (PipesEntity_t *pe);
void pie_update_pipes_sprite_xy (struct SpriteCompnt_s *sc, Vec2_t *v2_pos);
//FIXME: remove? unused.
bool_t pie_should_be_destroyed (PipesEntity_t *pe);

#endif /* _PIPES_ENTITY_H_ */
