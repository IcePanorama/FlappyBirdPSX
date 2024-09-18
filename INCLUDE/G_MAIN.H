#ifndef _G_MAIN_H_
#define _G_MAIN_H_

#include "G_PLAYER.H"

Player_t player;

/** Init entire gameplay subsystem. */
void g_init(void);

/** Updates entire gameplay subsystem. */
void g_loop(void);

#endif /* _G_MAIN_H_ */
