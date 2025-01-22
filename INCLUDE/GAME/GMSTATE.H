#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

typedef enum GameState_e
{
  GSTATE_GAME_START,
  GSTATE_NORMAL,
  GSTATE_GAME_PAUSED,
  GSTATE_GAME_OVER,
  GSTATE_NUM_STATES
} GameState_t;

GameState_t gs_get_curr_game_state (void);
void gs_set_game_state (GameState_t new_state);

#endif /* _GAME_STATE_H_ */
