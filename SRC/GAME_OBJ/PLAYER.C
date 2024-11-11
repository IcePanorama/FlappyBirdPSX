#include "game_obj/player.h"
#include "compnts/sprites.h"
#include "compnts/physics.h"
#include "game_obj/entityid.h"
#include "sys/fb_defs.h"
#include "utils.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

#define PLAYER_JUMP_VELOCITY (256)

static void init_player_physics_compnt (PlayerEntity_t *pe,
                                        Vec2_t *v2_output_pos);
static void init_player_sprite_compnt (PlayerEntity_t *pe);

PlayerEntity_t
create_player_entity (void)
{
  PlayerEntity_t pe;
  Vec2_t v2_pos;

  pe.eid_id = EID_PLAYER_ID;
  pe.u8_width = pe.u8_height = 16;

  init_player_physics_compnt (&pe, &v2_pos);

  init_player_sprite_compnt (&pe);
  if (&pe.psc_sprite_compnt != 0)
    update_player_sprite_xy (pe.psc_sprite_compnt, &v2_pos);
  pe.psc_sprite_compnt = 0;  // set this to NULL when we're done w/ it.

  return pe;
}

//TODO: make macro version of this function.
void
init_player_physics_compnt (PlayerEntity_t *pe, Vec2_t *v2_output_pos)
{
  create_new_physics_compnt (pe->eid_id);
  pe->ppc_physics_compnt = get_physics_compnt_with_id(pe->eid_id);

#ifdef DEBUG_BUILD
  assert(pe->ppc_physics_compnt != 0);
#endif /* DEBUG_BUILD */

  pe->ppc_physics_compnt->v2_position.x = 0;
  pe->ppc_physics_compnt->v2_position.y = 0;
  pe->ppc_physics_compnt->v2_velocity.x = 0;
  pe->ppc_physics_compnt->v2_velocity.y = 0;

  pe->ppc_physics_compnt->b_use_gravity = TRUE;

  v2_output_pos->x = pe->ppc_physics_compnt->v2_position.x;
  v2_output_pos->y = pe->ppc_physics_compnt->v2_position.y;

  pe->ppc_physics_compnt = 0; // set this to NULL when we're done w/ it.
}

//TODO: make macro version of this function.
void
init_player_sprite_compnt (PlayerEntity_t *pe)
{
  create_new_sprite (pe->eid_id);
  pe->psc_sprite_compnt = get_sprite_with_id(pe->eid_id);

  pe->psc_sprite_compnt->u8_width = pe->psc_sprite_compnt->u8_height = 16;

#ifdef DEBUG_BUILD
  assert(pe->psc_sprite_compnt != 0);
#endif /* DEBUG_BUILD */

  SetPolyF4 (&pe->psc_sprite_compnt->p4_sprite);
  SetShadeTex (&pe->psc_sprite_compnt->p4_sprite, 1);
  setRGB0(&pe->psc_sprite_compnt->p4_sprite, 0, 255, 0);

  pe->psc_sprite_compnt->update = update_player_sprite_xy;
}

void
update_player_sprite_xy (SpriteCompnt_t *sc, Vec2_t *v2_pos)
{
  Vec2_t v2_cs_pos;  // camera-space position
  uint8_t u8_half_width;
  uint8_t u8_half_height;
  uint16_t u16_left_x;
  uint16_t u16_right_x;
  uint16_t u16_top_y;
  uint16_t u16_bot_y;

  if (sc == 0 || v2_pos == 0)
    return;

  u8_half_width = sc->u8_width >> 1;
  u8_half_height = sc->u8_height >> 1;

  v2_convert_world_space_to_camera_space (v2_pos, &v2_cs_pos);

  u16_left_x  = v2_cs_pos.x - u8_half_width;
  u16_right_x = v2_cs_pos.x + u8_half_width;
  u16_top_y   = v2_cs_pos.y - u8_half_height;
  u16_bot_y   = v2_cs_pos.y + u8_half_height;

  setXY4(&sc->p4_sprite,
          u16_left_x, u16_top_y,
         u16_right_x, u16_top_y,
          u16_left_x, u16_bot_y,
         u16_right_x, u16_bot_y);
}

void
destroy_player_entity (PlayerEntity_t *pe)
{
  destroy_sprite (pe->eid_id);
  destroy_physics_compnt (pe->eid_id);
}

void
make_player_jump (void *e)
{
  PlayerEntity_t *pe = (PlayerEntity_t *)e;
  if (pe == 0)
     return;

  pe->ppc_physics_compnt = get_physics_compnt_with_id(pe->eid_id);
  if (pe->ppc_physics_compnt == 0)
    return;

  pe->ppc_physics_compnt->v2_velocity.y = (PLAYER_JUMP_VELOCITY);
  pe->ppc_physics_compnt = 0;
}

#undef PLAYER_JUMP_VELOCITY
