#include <stdio.h>

#include "G_MANGER.H"
#include "G_PIPE.H"
#include "V_MAIN.H"

/** Maximum number of pipes which can exist at the same time. */
#define MAX_NUM_PIPES (1)

static void create_new_pipe(void);
// static void destroy_pipe (void);

static Pipe_t pipes[MAX_NUM_PIPES];
static volatile int pipes_head;
static volatile int pipes_tail;

void g_manager_init(void) {
  pipes_head = 0;
  pipes_tail = 0;
}

void g_manager_loop(void) {
  int i;

  // printf ("pipe cnt: %d\n", pipes_tail - pipes_head);
  printf("head: %d, tail: %d\n", pipes_head, pipes_tail);
  if (pipes_tail - pipes_head < MAX_NUM_PIPES) create_new_pipe();

  for (i = pipes_head; i < pipes_tail; i++) {
    // printf ("vx pos: %d\n", pipes[i].position.vx);
    if (pipes[i].position.vx < -PIPE_WIDTH) {
      pipes_head++;
      //     pipes_head %= MAX_NUM_PIPES;
    }

    g_pipe_loop(&pipes[i]);
    add_sprite_to_sprites(&pipes[i].top_sprite);
    add_sprite_to_sprites(&pipes[i].bot_sprite);
  }
}

void create_new_pipe(void) {
  pipes[pipes_tail] = g_create_pipe();
  pipes_tail++;
  // pipes_tail %= MAX_NUM_PIPES;
}
