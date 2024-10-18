#include "input/cmdlist.h"
#include "game_obj/player.h"

#include <sys/types.h>
#include <libetc.h>

/* COMMANDS */
Command_t jump_command = { PADRdown, &make_player_jump };
/************/

/* COMMAND LISTS */
CommandList_t gstate_normal_cmd_list = {
  {&jump_command},
  1
};
/*****************/

/** Global command list array */
CommandList_t *cmdl_command_lists[GSTATE_NUM_STATES] = {
  0,                        // GSTATE_GAME_START
  &gstate_normal_cmd_list,  // GSTATE_NORMAL
  0,                        // GSTATE_GAME_PAUSED
  0                         // GSTATE_GAME_OVER
};
