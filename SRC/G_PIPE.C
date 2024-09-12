#include "FB_DEFS.H"
#include "G_PIPE.H"

#define PIPE_WIDTH (16)

Pipe_t g_create_pipe(void) {
  Pipe_t p;

  SetPolyF4(&p.sprite);
  SetShadeTex(&p.sprite, 1);
  setRGB0(&p.sprite, 255, 0, 0);

  p.position.vx = SCREEN_WIDTH - PIPE_WIDTH;
  p.position.vy = SCREEN_HEIGHT / 2;

  p.height = 200;

  setXY4(&p.sprite, p.position.vx - (PIPE_WIDTH / 2),
         p.position.vy + (p.height / 2), p.position.vx + (PIPE_WIDTH / 2),
         p.position.vy + (p.height / 2), p.position.vx - (PIPE_WIDTH / 2),
         p.position.vy - (p.height / 2), p.position.vx + (PIPE_WIDTH / 2),
         p.position.vy - (p.height / 2));

  return p;
}
