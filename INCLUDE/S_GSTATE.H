#ifndef _S_GAME_STATE_H_
#define _S_GAME_STATE_H_

typedef enum GameState_e {
  S_GSTATE_NORMAL,
  S_GSTATE_GAME_OVER,
  S_GSTATE_GAME_PAUSED,
  S_NUM_GAME_STATES
} GameState_t;

GameState_t s_curr_game_state;

#endif /* _S_GAME_STATE_H_ */
