#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

typedef enum GameState_e
{
  GSTATE_GAME_START,
  GSTATE_NORMAL,
  GSTATE_GAME_PAUSED,
  GSTATE_GAME_OVER,
  NUM_GAME_STATES
} GameState_t;

#endif /* _GAME_STATE_H_ */
