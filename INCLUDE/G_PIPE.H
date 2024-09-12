#ifndef _G_PIPE_H_
#define _G_PIPE_H_

#include <libgpu.h>
#include <libgte.h>
#include <sys/types.h>

typedef struct Pipe_s {
  POLY_F4 sprite;
  SVECTOR position;
  int height;
} Pipe_t;

/** Inits and returns a `Pipe_t` instance. */
Pipe_t g_create_pipe(void);

#endif /* _G_PIPE_H_ */
