#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

void gm_init_game (void);
void gm_update_game (void);
void gm_restart_game (void);
void gm_destroy_game (void);

void gm_pause_game (void);
void gm_unpause_game (void);

void gm_save_game (void);

#endif /* _GAME_MANAGER_H_ */
