#include "game/gmstate.h"

static GameState_t gs_curr_state = GSTATE_GAME_START;

GameState_t
gs_get_curr_game_state (void)
{
  return gs_curr_state;
}

//TODO: add toggle functions here.
void
gs_set_game_state (GameState_t new_state)
{
  gs_curr_state = new_state;
}
