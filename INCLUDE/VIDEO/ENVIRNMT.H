/** envirnmt.h - the game environment (foreground/background). Prefix: `ev`.*/
#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <sys/types.h>

void ev_init_background (void);
void ev_draw_environment (u_long *ot, u_long *ot_idx);
void ev_scroll_environment (void);
/** Resets all uv back to their origin. */
void ev_reset (void);

#endif /* _ENVIRONMENT_H_ */
