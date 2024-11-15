#include "game/gamemngr.h"
#include "compnts/colshape.h"
#include "compnts/wiframe.h"
#include "compnts/physics.h"
#include "compnts/sprites.h"
#include "game/pipemngr.h"
#include "game_obj/pipes.h"
#include "game_obj/player.h"
#include "input/cmdlist.h"
#include "input/controlr.h"
#include "sys/fb_defs.h"

// tmp (maybe)
#include "game/signals.h"
#include "game_obj/entityid.h"
#include "sys/fb_time.h"

#include <stdio.h>
#include <assert.h>

#define MAX_NUM_ENTITIES (32)

PlayerEntity_t player;

static void update_physics_compnts (Vec2_t *v2_output_pos);
static void update_sprites (Vec2_t *v2_input_pos);
static void update_col_shapes (Vec2_t *v2_input_pos);
static void init_compnt_pools (void);
static void detect_collisions (void);
static bool_t x_pos_in_red_zone (int16_t x);

//TODO: detect collisions

void
init_game (void)
{
  init_compnt_pools ();
  ctrl_init_controllers ();
  player = pe_create_player_entity ();

  pm_init_pipe_manager ();
  pm_spawn_pipe_entity ();
}

void
update_game (void)
{
  static Vec2_t v2_entity_pos[MAX_NUM_ENTITIES] = {{0}};

  ctrl_handle_user_input (cmdl_command_lists[gs_curr_game_state],
                          (void *)&player);

  if (si_check_inbox (player.u8_eid) == SIG_BELOW_SCREEN)
  {
    player.ppc_physics_compnt = get_physics_compnt_with_id(player.u8_eid);
    player.ppc_physics_compnt->v2_position.y =
      ((-(HALF_SCREEN_HEIGHT) - player.u8_height)
      << (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS));
    player.ppc_physics_compnt->v2_velocity.y = 256;
    player.ppc_physics_compnt = 0;
  }

  pm_manage_pipes ();

  update_physics_compnts (v2_entity_pos);
  update_sprites (v2_entity_pos);
  update_col_shapes (v2_entity_pos);
  detect_collisions ();
}

void
destroy_game (void)
{
  pe_destroy_player_entity (&player);
}

void
update_physics_compnts (Vec2_t *v2_output_pos)
{
  uint8_t i;

  for (i = 0; i < u8_pc_num_physics; i++)
  {
    update_physics_compnt (&pc_physics_pool[i], &v2_output_pos[i]);
  }
}

void
update_sprites (Vec2_t *v2_input_pos)
{
  uint8_t i;

  for (i = 0; i < sp_num_sprites; i++)
  {
    (*sp_sprite_pool[i].update)(&sp_sprite_pool[i], &v2_input_pos[i]);
  }
}

void
update_col_shapes (Vec2_t *v2_input_pos)
{
  uint8_t i;

  for (i = 0; i < u8_csc_num_col_shapes; i++)
  {
    csc_update_col_shape(&csc_col_shape_pool[i], &v2_input_pos[i]);
  }
}

void
init_compnt_pools (void)
{
  csc_init_col_shape_compnt_pool ();
  pc_init_physics_compnt_pool ();
  sc_init_sprite_compnt_pool ();
  si_init_signal_inboxes ();
  wfc_init_wireframe_compnt_pool ();
}

// Most basic form of collision detection is working!
// TODO: Clean up/finish later.
void
detect_collisions (void)
{
  uint8_t i;
  int16_t i16_red_zone_y;
  ColShapeCompnt_t *csc_pipe;
  //  uint16_t u16_height;

  for (i = 1; i < u8_pc_num_physics; i++)
  {
    if (x_pos_in_red_zone (pc_physics_pool[i].v2_position.x))
    {
      csc_pipe = get_col_shape_with_id(pc_physics_pool[i].i8_parent_id);

      i16_red_zone_y = csc_pipe->v2_pos->y + (csc_pipe->u8_height >> 1);

      // Player should be first physics component
      assert(pc_physics_pool[0].i8_parent_id == EID_PLAYER_ID);
      printf("y: %d\tpy: %d\n", i16_red_zone_y, pc_physics_pool[0].v2_position.y);

      if (pc_physics_pool[i].i8_parent_id % 2 == 1) // top pipe
      { assert(i16_red_zone_y < pc_physics_pool[0].v2_position.y); }
      else { assert(pc_physics_pool[0].v2_position.y < i16_red_zone_y); }
    }
  }
}

bool_t
x_pos_in_red_zone (int16_t x)
{
  const int16_t i16_RED_ZONE_X = ((player.u8_width >> 1) + (PIE_HALF_PIPE_WIDTH)) << (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

  return -i16_RED_ZONE_X < x && x < i16_RED_ZONE_X;
}

#undef MAX_NUM_ENTITIES
