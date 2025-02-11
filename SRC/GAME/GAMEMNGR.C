#include "game/gamemngr.h"
#include "compnts/colshape.h"
#include "compnts/wiframe.h"
#include "compnts/physics.h"
#include "compnts/sprites.h"
#include "game/colmngr.h"
#include "game/pipemngr.h"
#include "game/score.h"
#include "game/signals.h"
#include "game_obj/pipes.h"
#include "game_obj/player.h"
#include "input/cmdlist.h"
#include "input/controlr.h"
#include "sys/fb_defs.h"
#include "sys/fb_ints.h"
#include "video/envirnmt.h"

// tmp
#define DEBUG_BUILD

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
static void update_col_shapes (Vec2_t *v2_input_pos);
static void init_compnt_pools (void);
static void normal_update (void);
static void end_game (void);

// tmp
static long _card_event (unsigned long ev0, unsigned long ev1, unsigned long ev2, unsigned long ev3);

void
gm_init_game (void)
{
  init_compnt_pools ();
  player = pe_create_player_entity ();
  s_init_scoring (&player);
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

void
gm_update_game (void)
{
  GameState_t curr_game_state = gs_get_curr_game_state ();
  ctrl_handle_user_input (cmdl_command_lists[curr_game_state],
                          (void *)&player);

  if (curr_game_state == GSTATE_NORMAL && !pe_is_alive(&player))
  {
    end_game ();
  }

  switch (curr_game_state)
  {
    case GSTATE_NORMAL:
      normal_update ();
      break;
    default:
      return;
  }
}

void
end_game (void)
{
  s_process_high_scores ();
  gs_set_game_state (GSTATE_GAME_OVER);
}

void
normal_update (void)
{
  static Vec2_t v2_entity_pos[MAX_NUM_ENTITIES] = {{0}};

  cm_handle_collisions ();
  pe_update_player (&player);

  pm_manage_pipes ();

  update_physics_compnts (v2_entity_pos);
  update_col_shapes (v2_entity_pos);
  update_sprites (v2_entity_pos);
}

void
update_physics_compnts (Vec2_t *v2_output_pos)
{
  uint8_t i;

  for (i = 0; i < u8_pc_num_physics; i++)
    update_physics_compnt (&pc_physics_pool[i], &v2_output_pos[i]);
}

void
update_sprites (Vec2_t *v2_input_pos)
{
  uint8_t i;

   for (i = 0; i < sprite_pools[TID_GAME_OBJ_TEXTURE].u8_num_sprites; i++)
     sc_update_sprite_xy (&sprite_pools[TID_GAME_OBJ_TEXTURE].sprites[i],
                          &v2_input_pos[i]);
}

void
update_col_shapes (Vec2_t *v2_input_pos)
{
  uint8_t i;

  for (i = 0; i < u8_csc_num_col_shapes; i++)
    csc_update_col_shape(&csc_col_shape_pool[i], &v2_input_pos[i]);
}

void
gm_destroy_game (void)
{
  pe_destroy_player_entity (&player);
}

void
gm_restart_game (void)
{
  gm_destroy_game ();
  gm_init_game ();
  gs_set_game_state (GSTATE_GAME_START);
  ev_reset ();
}

void
gm_pause_game (void)
{
  gs_set_game_state (GSTATE_GAME_PAUSED);
}

void
gm_unpause_game (void)
{
  gs_set_game_state (GSTATE_NORMAL);
}

//tmp
#include <libapi.h>
#include <sys/file.h>
#include <libpad.h>

void
gm_save_game (void)
{
  unsigned long ev0, ev1, ev2, ev3;
  long ret; //, file_desc;

  printf("%s\n", __func__);

  ev0 = OpenEvent (SwCARD, EvSpIOE, EvMdNOINTR, NULL);
  ev1 = OpenEvent (SwCARD, EvSpERROR, EvMdNOINTR, NULL);
  ev2 = OpenEvent (SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
  ev3 = OpenEvent (SwCARD, EvSpNEW, EvMdNOINTR, NULL);

  InitCARD (0);
  PadStopCom ();
  StartCARD ();
  _bu_init ();

#ifdef DEBUG_BUILD
  assert (_card_info (0x00) == 1);
  ret = _card_event (ev0, ev1, ev2, ev3);
  printf ("ret: %ld\n", ret);

  if (ret == 1 || ret == 2)
  {
    printf("NO CARD or comm. err.\n");
    assert (0);
  }
  else if (ret == 3) // NEWCARD
  {
    // Clear event
    TestEvent (ev0);
    TestEvent (ev1);
    TestEvent (ev2);
    TestEvent (ev3);

    assert (_card_clear (0x00) == 1);
    ret = _card_event (ev0, ev1, ev2, ev3);
    printf ("ret: %ld\n", ret);
  }

  // Clear event
  TestEvent (ev0);
  TestEvent (ev1);
  TestEvent (ev2);
  TestEvent (ev3);

  // Deliver a TEST FORMAT request.
  assert (_card_load (0x00) == 1);
  if (ret == 3) // NEWCARD
  {
    printf ("Formatting card...\n");
    assert (_card_format (0x00) == 1);
    printf ("Done!\n");
  }
#endif /* DEBUG_BUILD */

  StopCARD ();
  StartPAD ();
  PadStartCom ();
}

// FIXME: this func never returns!
long
_card_event (unsigned long ev0, unsigned long ev1, unsigned long ev2, unsigned long ev3)
{
  printf("%s\n", __func__);
  while (1)
  {
    if (TestEvent (ev0) == 1) // IOE
      return 0;
    if (TestEvent (ev1) == 1) // Error
      return 1;
    if (TestEvent (ev2) == 1) // Timeout
      return 2;
    if (TestEvent (ev3) == 1) // New card
      return 3;
  }
}
