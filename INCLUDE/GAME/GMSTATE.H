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

extern GameState_t gs_curr_game_state;

#endif /* _GAME_STATE_H_ */
