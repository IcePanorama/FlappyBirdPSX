#include "utils.h"
#include "input/cmdlist.h"
#include "game/gamemngr.h"
#include "game_obj/player.h"

#include <sys/types.h>
#include <libetc.h>

#define X_BUTTON (PADRdown)
#define START_BUTTON (PADstart)

/* COMMANDS */
Command_t cmd_player_jump = { (X_BUTTON), &pe_make_player_jump };
Command_t cmd_restart_game = { (START_BUTTON), &gm_restart_game };
/* Works on any button press for the time being. */
Command_t cmd_seed_rng = { 0xFFFF, &u_seed_rng };
/************/

/* COMMAND LISTS */
CommandList_t cmdl_gstate_game_start = {
  {&cmd_seed_rng},
  1
};
CommandList_t cmdl_gstate_normal = {
  {&cmd_player_jump},
  1
};
CommandList_t cmdl_gstate_game_over = {
  {&cmd_restart_game},
  1
};
/*****************/

/** Global command list array */
CommandList_t *cmdl_command_lists[GSTATE_NUM_STATES] = {
  &cmdl_gstate_game_start,  // GSTATE_GAME_START
  &cmdl_gstate_normal,      // GSTATE_NORMAL
  0,                        // GSTATE_GAME_PAUSED
  &cmdl_gstate_game_over    // GSTATE_GAME_OVER
};
