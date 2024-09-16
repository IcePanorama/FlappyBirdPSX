#include "S_CONTRL.H"
#include "S_GSTATE.H"
#include "S_MAIN.H"

void s_init(void) {
  s_curr_game_state = S_GSTATE_GAME_PAUSED;
  s_init_controllers();
}
