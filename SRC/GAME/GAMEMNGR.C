#include "game/gamemngr.h"
#include "compnts/colshape.h"
#include "compnts/wiframe.h"
#include "compnts/physics.h"
#include "compnts/sprites.h"
#include "game/colmngr.h"
#include "game/pipemngr.h"
#include "game/score.h"
#include "game/scrwritr.h"
#include "game/signals.h"
#include "game_obj/pipes.h"
#include "game_obj/player.h"
#include "input/cmdlist.h"
#include "input/controlr.h"
#include "sys/fb_defs.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

#define MAX_NUM_ENTITIES (32)

#define SCORE_MSG_Y_POS ((SW_FONT_SPRITE_HEIGHT) << 1)
#define GAME_OVER_MSG_Y_POS ((SW_FONT_SPRITE_HEIGHT) * 3)
#define NEW_HIGH_SCORE_MSG_Y_POS ((SW_FONT_SPRITE_HEIGHT) * 4)

PlayerEntity_t player;

static void update_physics_compnts (Vec2_t *v2_output_pos);
static void update_sprites (Vec2_t *v2_input_pos);
static void update_pipe_sprites (Vec2_t *v2_input_pos);
static void update_col_shapes (Vec2_t *v2_input_pos);
static void init_compnt_pools (void);
static void normal_update (void);
static void end_game (void);

uint32_t gm_curr_score = 0;
//TODO: save and load this off of memory cards.
static uint32_t u32_high_score = 0;

static uint8_t u8_score_msg_id;
static uint8_t u8_game_over_msg_id;
static uint8_t u8_high_score_msg_id;
static bool_t b_new_high_score = FALSE;

void
gm_init_game (void)
{
  init_compnt_pools ();
  player = pe_create_player_entity ();
  s_init_scoring (&player);
  u8_score_msg_id = sw_print("0", (SCREEN_WIDTH) >> 1, (SCORE_MSG_Y_POS));
  cm_init_collision_manager (&player);

  pm_init_pipe_manager ();
  /*
   *  FIXME: this pipe always has its gap in the same exact place since we're
   *  spawning it before rand is seeded. This shouldn't be a problem in the
   *  final game as we'll likely seed rand at the main menu!
   */
//  pm_spawn_pipe_entity ();
}

void
init_compnt_pools (void)
{
  csc_init_col_shape_compnt_pool ();
  pc_init_physics_compnt_pool ();
  sc_init_sprite_compnt_pool ();
  si_init_signal_inboxes ();
}

/*
 *  TODO: start work on visuals
 *    - Replace player with texture/sprite.
 *    - Add scrolling background
 */
void
gm_update_game (void)
{
  ctrl_handle_user_input (cmdl_command_lists[gs_curr_game_state],
                          (void *)&player);

  if (gs_curr_game_state == GSTATE_NORMAL && !pe_is_alive(&player))
  {
    end_game ();
  }

  switch (gs_curr_game_state)
  {
    case GSTATE_NORMAL:
      normal_update ();
    case GSTATE_GAME_START:
    case GSTATE_GAME_PAUSED:
    case GSTATE_GAME_OVER:
    default:
      return;
  }
}

void
end_game (void)
{
  u8_game_over_msg_id = sw_print("Game over!", (SCREEN_WIDTH) >> 1,
                                 (GAME_OVER_MSG_Y_POS));
  if (gm_curr_score > u32_high_score)
  {
    u8_high_score_msg_id = sw_print("New high score!", (SCREEN_WIDTH) >> 1,
             (NEW_HIGH_SCORE_MSG_Y_POS));
    b_new_high_score = TRUE;
    u32_high_score = gm_curr_score;
    gm_curr_score = 0;
  }
  gs_curr_game_state = GSTATE_GAME_OVER;
}

void
normal_update (void)
{
  static Vec2_t v2_entity_pos[MAX_NUM_ENTITIES] = {{0}};

  pm_manage_pipes ();
  pe_update_player (&player);

  cm_handle_collisions ();
  update_physics_compnts (v2_entity_pos);
  update_sprites (v2_entity_pos);
  update_col_shapes (v2_entity_pos);
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
  /* Update player sprite */
  (*sprite_pools[TEXTID_PIPES_TEXTURE].sprites[0].update)(&sprite_pools[TEXTID_PIPES_TEXTURE].sprites[0], &v2_input_pos[0]);

  update_pipe_sprites(v2_input_pos);
}

void
update_pipe_sprites (Vec2_t *v2_input_pos)
{
  uint8_t i;

   for (i = 1; i < sprite_pools[TEXTID_PIPES_TEXTURE].u8_num_sprites; i++)
   {
     (*sprite_pools[TEXTID_PIPES_TEXTURE].sprites[i].update)(
       &sprite_pools[TEXTID_PIPES_TEXTURE].sprites[i], &v2_input_pos[i]);
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
gm_increase_score (void)
{
  char score_buffer[7] = {0}; // max val: 999999\0
  gm_curr_score++;
  sprintf(score_buffer, "%d", gm_curr_score);
  sw_destroy_text_output (u8_score_msg_id);
  u8_score_msg_id = sw_print(score_buffer, (SCREEN_WIDTH) >> 1, (SCORE_MSG_Y_POS));
}

void
gm_destroy_game (void)
{
  pe_destroy_player_entity (&player);
}

void
gm_restart_game (void)
{
  sw_destroy_text_output (u8_score_msg_id);
  sw_destroy_text_output (u8_game_over_msg_id);
  if (b_new_high_score)
  {
    sw_destroy_text_output (u8_high_score_msg_id);
    b_new_high_score = FALSE;
  }
  gm_destroy_game ();
  gm_init_game ();
  gs_curr_game_state = GSTATE_GAME_START;
}
