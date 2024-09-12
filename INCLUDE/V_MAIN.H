#ifndef _V_MAIN_H_
#define _V_MAIN_H_

#include "V_SCRBUF.H"

/** Inits entire video subsystem. */
void v_init(void);

/** Updates entire video subsystem. */
void v_loop(void);

void add_sprite_to_sprites(POLY_F4 *sprite);

#endif /* _V_MAIN_H_ */
