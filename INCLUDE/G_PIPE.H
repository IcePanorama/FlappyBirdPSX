#ifndef _G_PIPE_H_
#define _G_PIPE_H_

#include <libgpu.h>
#include <libgte.h>
#include <sys/types.h>

#include "G_AABB.H"

#define PIPE_WIDTH (16)

typedef struct Pipe_s {
  POLY_F4 top_sprite;
  POLY_F4 bot_sprite;
  SVECTOR position;
  AABB_t col_shape[2];  // one for the top, one for bot
} Pipe_t;

/** Inits and returns a `Pipe_t` instance. */
Pipe_t g_create_pipe(void);

void g_pipe_loop(Pipe_t *p);

#endif /* _G_PIPE_H_ */
