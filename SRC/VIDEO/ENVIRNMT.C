#include "video/envirnmt.h"
#include "video/textlkup.h"
#include "video/textmngr.h"
#include "sys/fb_defs.h"
#include "sys/fb_ints.h"

/* Do not touch! CC seems really particular about the order of these. */
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#ifdef DEBUG_BUILD
  #include "video/vid_defs.h"

  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define TEXTURE_WIDTH             (255)
#define TEXTURE_HEIGHT            (255)
#define FOREGROUND_TEXTURE_HEIGHT (40)

#define FOREGROUND_V0 ((TEXTURE_HEIGHT) - (FB_FOREGROUND_HEIGHT))
#define BACKGROUND_V0 \
  ((FOREGROUND_V0) - ((FB_SCREEN_HEIGHT) - (FOREGROUND_TEXTURE_HEIGHT)))

enum EVTiles_e
{
 EVT_TOP_LEFT,
 EVT_TOP_RIGHT,
 EVT_BOT_LEFT,
 EVT_BOT_RIGHT,
 EVT_NUM_TILES
};

static SPRT environment_sprites[EVT_NUM_TILES] = {{0}};

void
ev_init_background (void)
{
  uint8_t i;
  for (i = 0; i < EVT_NUM_TILES; i++)
  {
    SetSprt(&environment_sprites[i]);
    SetShadeTex(&environment_sprites[i], 1);
    environment_sprites[i].clut = texture_clut_lookup[TID_ENV_TEXTURE];
  }

  setWH(&environment_sprites[0], (TEXTURE_WIDTH),
        (FB_SCREEN_HEIGHT) - (FB_FOREGROUND_HEIGHT));
  setWH(&environment_sprites[1], (FB_SCREEN_WIDTH) - (TEXTURE_WIDTH),
        (FB_SCREEN_HEIGHT) - (FB_FOREGROUND_HEIGHT));
  for (i = 0; i < EVT_BOT_LEFT; i++)
  {
    setXY0(&environment_sprites[i], (TEXTURE_WIDTH) * i, 0);
    setUV0(&environment_sprites[i], 0, (BACKGROUND_V0));
  }

  setWH(&environment_sprites[2], (TEXTURE_WIDTH), (FB_FOREGROUND_HEIGHT));
  setWH(&environment_sprites[3], (FB_SCREEN_WIDTH) - (TEXTURE_WIDTH),
        (FB_FOREGROUND_HEIGHT));
  for (i = EVT_BOT_LEFT; i < EVT_NUM_TILES; i++)
  {
    setXY0(&environment_sprites[i], (TEXTURE_WIDTH) * (i - EVT_BOT_LEFT),
           (FB_SCREEN_HEIGHT) - (FB_FOREGROUND_HEIGHT));
    setUV0(&environment_sprites[i], 0, (FOREGROUND_V0));
  }
}

void
ev_scroll_environment (void)
{

  uint8_t i;
  for (i = 0; i < EVT_BOT_LEFT; i++)
    environment_sprites[i].u0++;

  /* Processed separately for parallax scrolling effect. */
  for (i = EVT_BOT_LEFT; i < EVT_NUM_TILES; i++)
    environment_sprites[i].u0 += 2;
}

void
ev_reset (void)
{

  uint8_t i;
  for (i = 0; i < EVT_NUM_TILES; i++)
    environment_sprites[i].u0 = 0;
}

void
ev_draw_environment (u_long *ot, u_long *ot_idx)
{
  uint8_t i;

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TID_ENV_TEXTURE]);
  (*ot_idx)++;

  for (i = 0; i < EVT_NUM_TILES; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
    AddPrim(&ot[(*ot_idx)], &environment_sprites[i]);
    (*ot_idx)++;
  }
}
