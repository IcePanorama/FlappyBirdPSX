#include "FB_DEFS.H"
#include "G_MANGER.H"
#include "G_PIPE.H"
#include "V_MAIN.H"

/** Maximum number of pipes that can exist at the same time. */
#define MAX_NUM_PIPES (2)

static void create_new_pipe(void);

static Pipe_t pipes[MAX_NUM_PIPES];
/** Most recently generated pipe. */
static Pipe_t *last_pipe;
static int num_pipes;

/**
 *  The factor (in terms of `PIPE_WIDTH`) by which one pipe should be separated
 *  from the next.
 */
static int gap_factor;

void g_manager_init(void) {
  num_pipes = 0;
  last_pipe = 0;
  gap_factor = 10;
}

void g_manager_loop(void) {
  int i;
#ifdef WIREFRAME
  int j;
#endif /* WIREFRAME */

  if (num_pipes < MAX_NUM_PIPES) {
    create_new_pipe();
  }

  for (i = 0; i < num_pipes; i++) {
    if (pipes[i].position.vx < -PIPE_WIDTH) {
      pipes[i] = g_create_pipe();
      last_pipe = &pipes[i];
    }

    g_pipe_loop(&pipes[i]);
    add_sprite_to_sprites(&pipes[i].top_sprite);
    add_sprite_to_sprites(&pipes[i].bot_sprite);

#ifdef WIREFRAME
    for (j = 0; j < NUM_WIREFRAME_LNS; j++) {
      v_add_wire_to_wireframes(&pipes[i].col_shape[0].wireframe[j]);
      v_add_wire_to_wireframes(&pipes[i].col_shape[1].wireframe[j]);
    }
#endif /* WIREFRAME */
  }
}

void create_new_pipe(void) {
  if (last_pipe != 0 &&
      (SCREEN_WIDTH - (*last_pipe).position.vx) < (PIPE_WIDTH * gap_factor))
    return;

  pipes[num_pipes] = g_create_pipe();
  last_pipe = &pipes[num_pipes];
  num_pipes++;
}
