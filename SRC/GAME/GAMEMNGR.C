#include "game/gamemngr.h"
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

#define MAX_NUM_ENTITIES (32)

PlayerEntity_t player;

static void update_physics_compnts (Vec2_t *v2_output_pos);
static void update_sprites (Vec2_t *v2_input_pos);
static void init_compnt_pools (void);

void
init_game (void)
{
  init_compnt_pools ();
  ctrl_init_controllers ();
  player = create_player_entity ();

  pm_init_pipe_manager ();
  pm_spawn_pipe_entity ();
}

void
update_game (void)
{
  static Vec2_t v2_entity_pos[MAX_NUM_ENTITIES] = {{0}};

  ctrl_handle_user_input (cmdl_command_lists[gs_curr_game_state],
                          (void *)&player);

  if (si_check_inbox (player.eid_id) == SIG_BELOW_SCREEN)
  {
    player.ppc_physics_compnt = get_physics_compnt_with_id(player.eid_id);
    player.ppc_physics_compnt->v2_position.y =
      ((-(HALF_SCREEN_HEIGHT) - player.u8_height)
      << (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS));
    player.ppc_physics_compnt->v2_velocity.y = 256;
    player.ppc_physics_compnt = 0;
  }

  pm_manage_pipes ();

  update_physics_compnts (v2_entity_pos);
  update_sprites (v2_entity_pos);
}

void
destroy_game (void)
{
  destroy_player_entity (&player);
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
init_compnt_pools (void)
{
  pc_init_physics_compnt_pool ();
  sc_init_sprite_compnt_pool ();
  si_init_signal_inboxes ();
}

#undef MAX_NUM_ENTITIES
