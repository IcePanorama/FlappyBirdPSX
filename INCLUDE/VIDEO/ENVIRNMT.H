/** envirnmt.h - the game environment (foreground/background). Prefix: `ev`.*/
#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

enum EVTiles_e
{
 EVT_TOP_LEFT,
 EVT_TOP_RIGHT,
 EVT_BOT_LEFT,
 EVT_BOT_RIGHT,
 EVT_NUM_TILES
};

extern SPRT ev_background_sprites[EVT_NUM_TILES];

void ev_init_background (void);
void ev_draw_background (u_long *ot, u_long *ot_idx);
void ev_draw_foreground (u_long *ot, u_long *ot_idx);
void ev_scroll_background (void);
/** Resets all uv back to their origin. */
void ev_reset (void);

#endif /* _ENVIRONMENT_H_ */
