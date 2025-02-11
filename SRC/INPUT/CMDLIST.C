#include "utils.h"
#include "input/cmdlist.h"
#include "game/gamemngr.h"
#include "game_obj/player.h"

#include <sys/types.h>
#include <libetc.h>

#define X_BUTTON     (PADRdown)
#define O_BUTTON     (PADRright)

#define START_BUTTON (PADstart)

/** Control scheme during `GSTATE_GAME_START`. */
Command_t cmd_seed_rng = { 0xFFFF, &u_seed_rng };
CommandList_t cmdl_gstart = {
  {&cmd_seed_rng},
  1
};

/** Control scheme during `GSTATE_NORMAL`. */
Command_t cmd_player_jump = { (X_BUTTON), &pe_make_player_jump };
Command_t cmd_pause_game = { (START_BUTTON), &gm_pause_game };
CommandList_t cmdl_gnormal = {
  {&cmd_player_jump, &cmd_pause_game},
  2
};

/** Control scheme during `GSTATE_GAME_PAUSED`. */
Command_t cmd_unpause_game = { (START_BUTTON), &gm_unpause_game };
Command_t cmd_save_game = { (O_BUTTON), &gm_save_game };
CommandList_t cmdl_gpaused = {
  {&cmd_unpause_game, &cmd_save_game},
  2
};

/** Control scheme during `GSTATE_GAME_OVER`. */
Command_t cmd_restart_game = { (START_BUTTON), &gm_restart_game };
CommandList_t cmdl_gover = {
  {&cmd_restart_game},
  1
};

CommandList_t *cmdl_command_lists[GSTATE_NUM_STATES] = {
  &cmdl_gstart,  // GSTATE_GAME_START
  &cmdl_gnormal, // GSTATE_NORMAL
  &cmdl_gpaused, // GSTATE_GAME_PAUSED
  &cmdl_gover    // GSTATE_GAME_OVER
};
