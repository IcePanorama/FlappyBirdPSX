#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "sys/fb_ints.h"

extern uint32_t gm_curr_score;

void gm_init_game (void);
void gm_update_game (void);
void gm_destroy_game (void);
void gm_restart_game (void);
void gm_increase_score (void);

#endif /* _GAME_MANAGER_H_ */
