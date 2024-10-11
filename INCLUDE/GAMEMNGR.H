#ifndef _GAME_STATE_MANAGER_H_
#define _GAME_STATE_MANAGER_H_

#include "player.h"

// This really shouldn't be global, need to rework input stuff at some point.
Player_t player;

void init_game_state (void);
void update_game_state (void);

#endif /* _GAME_STATE_MANAGER_H_ */
