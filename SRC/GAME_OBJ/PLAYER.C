#include "game_obj/player.h"
#include "compnts/colshape.h"
#include "compnts/physics.h"
#include "compnts/sprites.h"
#include "game/signals.h"
#include "game_obj/entityid.h"
#include "game_obj/pipes.h"
#include "sys/fb_defs.h"
#include "video/textlkup.h"
#include "utils.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

#define PLAYER_SIZE (16)
#define PLAYER_JUMP_VELOCITY (256)

/* Player Flags */
#define FL_ALIVE     (0)
#define FL_DESTROYED (1)
/****************/

static void init_player_physics_compnt (PlayerEntity_t *pe,
                                        Vec2_t *v2_output_pos);
static void init_player_sprite_compnt (PlayerEntity_t *pe);

PlayerEntity_t
pe_create_player_entity (void)
{
  PlayerEntity_t pe;
  Vec2_t v2_pos;

  pe.u8_eid = EID_PLAYER_ID;
  pe.u8_flags = 0;  // probably unnecessary, but just to be safe.
  pe.u8_flags |= (1 << (FL_ALIVE));
  pe.u8_width = pe.u8_height = (PLAYER_SIZE);

  init_player_physics_compnt (&pe, &v2_pos);

  init_player_sprite_compnt (&pe);
  pe_update_player_sprite_xy (pe.psc_sprite_compnt, &v2_pos);
  pe.psc_sprite_compnt = 0;  // set this to NULL when we're done w/ it.

  pe.pcsc_col_shape =
    csc_create_new_col_shape (pe.u8_eid, pe.u8_width, pe.u8_height);
  csc_update_col_shape (pe.pcsc_col_shape, &v2_pos);

  return pe;
}

//TODO: make macro version of this function.
void
init_player_physics_compnt (PlayerEntity_t *pe, Vec2_t *v2_output_pos)
{
  pe->ppc_physics_compnt = pc_create_new_physics_compnt (pe->u8_eid);

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
  pe->psc_sprite_compnt = sc_create_new_sprite (&sprite_pools[TEXTID_PIPE_BOT_TEXTURE], pe->u8_eid);
#ifdef DEBUG_BUILD
  assert(pe->psc_sprite_compnt != 0);
#endif /* DEBUG_BUILD */

  pe->psc_sprite_compnt->u8_width = pe->u8_width;
  pe->psc_sprite_compnt->u8_height = pe->u8_height;

  setWH(&pe->psc_sprite_compnt->sprite, pe->u8_width, pe->u8_height);
  setUV0(&pe->psc_sprite_compnt->sprite, 0, 0);

  pe->psc_sprite_compnt->update = pe_update_player_sprite_xy;
}

void
pe_update_player_sprite_xy (SpriteCompnt_t *sc, Vec2_t *v2_pos)
{
  Vec2_t v2_cs_pos;  // camera-space position

  if (sc == 0 || v2_pos == 0)
    return;

  v2_convert_world_space_to_camera_space (v2_pos, &v2_cs_pos);

  setXY0(&sc->sprite, v2_cs_pos.x - (sc->u8_width >> 1), v2_cs_pos.y - (sc->u8_height >> 1));
}

void
pe_update_player (PlayerEntity_t *pe)
{
  if (si_check_inbox (pe->u8_eid) == SIG_BELOW_SCREEN)
  {
    pe_kill_player(pe);
  }
}

void
pe_destroy_player_entity (PlayerEntity_t *pe)
{
  if (pe == 0 || (pe->u8_flags & (1 << (FL_DESTROYED)))) return;

  csc_destroy_col_shape (pe->u8_eid);
  destroy_sprite (&sprite_pools[TEXTID_PIPE_BOT_TEXTURE], pe->u8_eid);
  destroy_physics_compnt (pe->u8_eid);

  pe->u8_flags |= (1 << (FL_DESTROYED));
}

void
pe_make_player_jump (void *e)
{
  PlayerEntity_t *pe = (PlayerEntity_t *)e;
  if (pe == 0)
     return;

  pe->ppc_physics_compnt = get_physics_compnt_with_id(pe->u8_eid);
  if (pe->ppc_physics_compnt == 0)
    return;

  pe->ppc_physics_compnt->v2_velocity.y = (PLAYER_JUMP_VELOCITY);
  pe->ppc_physics_compnt = 0;
}

void
pe_kill_player (PlayerEntity_t *pe)
{
  //TODO: play death animation/sound
  pe->u8_flags &= ~(1 << (FL_ALIVE));
}

bool_t
pe_is_alive (PlayerEntity_t *pe)
{
  return (pe->u8_flags & (1 << (FL_ALIVE)));
}
