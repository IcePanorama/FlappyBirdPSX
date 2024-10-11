/** SPRITES.H - defines `Sprite_t` data type, global sprite_pool. */
#ifndef _SPRITE_COMPONENT_H_
#define _SPRITE_COMPONENT_H_

#include "stdbool.h"
#include "stdint.h"

/* Don't touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

/** A simple wrapper for the `POLY_F4` data type. */
typedef struct Sprite_s
{
  bool_t in_use;
  POLY_F4 sprite;
} Sprite_t;

/* I don't necessarily love that this is global. */
#define SPRITE_POOL_MAX_NUM_SPRITES (32)
extern Sprite_t sprite_pool[SPRITE_POOL_MAX_NUM_SPRITES];

/**
 *  Returns a pointer to an available sprite from sprite_pool. If no sprites are
 *  available, an assert will fail if DEBUG_BUILD is defined, otherwise it
 *  returns NULL.
 */
Sprite_t *get_new_sprite_from_pool (void);

/** Initializes a sprite as a `POLY_F4` and sets its RGB0 values to `r,g,b`. */
void init_sprite_w_rgb (POLY_F4 *s, uint8_t r, uint8_t g, uint8_t b);

/**
 *  Marks the given sprite as being no longer in use, enabling other entities to
 *  reuse it.
 */
void free_sprite (Sprite_t *s);

#endif /* _SPRITE_COMPONENT_H_ */
