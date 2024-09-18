#ifndef _V_MAIN_H_
#define _V_MAIN_H_

#include "V_SCRBUF.H"

/** Inits entire video subsystem. */
int v_init(void);

/** Updates entire video subsystem. */
void v_loop(void);

void add_sprite_to_sprites(POLY_F4 *sprite);

#ifdef WIREFRAME
void v_add_wire_to_wireframes(LINE_F2 *wire);
#endif /* WIREFRAME */

#endif /* _V_MAIN_H_ */
