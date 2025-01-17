/**
 *  FIXME: rename file
 *  NEXT TIME:
 *  + Adjust pipe opening positioning spawning
 *  + Adjust pipe speed/work on parallax scrolling.
 *    + og game doesn't have parallax scrolling, but I figure, why not?
 */
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

enum BGTiles_e
{
 BGT_TOP_LEFT,
 BGT_TOP_RIGHT,
 BGT_BOT_LEFT,
 BGT_BOT_RIGHT,
 BGT_NUM_TILES
};

extern SPRT bg_background_sprites[BGT_NUM_TILES];

void bg_init_background (void);
void bg_draw_background (u_long *ot, u_long *ot_idx);
void bg_draw_foreground (u_long *ot, u_long *ot_idx);
void bg_scroll_background (void);

#endif /* _BACKGROUND_H_ */
