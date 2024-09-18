#include <kernel.h>
#include <libetc.h>
#include <libpad.h>
#include <rand.h>
#include <stdio.h>

#include "FB_DEFS.H"
#include "G_AABB.H"
#include "G_PLAYER.H"
#include "S_GSTATE.H"
#include "S_HBCNTR.H"

// Approximation for (9.8 / 2.0) m/s
#define GRAVITY -49
#define JUMP_VELOCITY 98
#define VELOCITY_LERP_VAL 7

static void update_player_sprite_xy(Player_t *p);

Player_t g_create_player(short start_x, short start_y, u_char *controller) {
  Player_t p;

  SetPolyF4(&p.sprite);

  SetShadeTex(&p.sprite, 1);
  setRGB0(&p.sprite, 0, 255, 0);

  p.position.vx = start_x;
  p.position.vy = start_y;

  p.col_shape.position.vx = p.position.vx;
  p.col_shape.position.vy = p.position.vy;
  p.col_shape.width = 16;
  p.col_shape.height = 16;

#ifdef WIREFRAME
  g_init_AABB_wireframe(&p.col_shape);
#endif /* WIREFRAME */

  update_player_sprite_xy(&p);

  p.controller = controller;

  p.velocity = GRAVITY;

  p.is_dead = 0;

  return p;
}

void g_update_player_sprite_pos(Player_t *p) {
  p->position.vy -= p->velocity * s_delta_time() / 4096;
  if (p->velocity > GRAVITY) p->velocity -= VELOCITY_LERP_VAL;

  // FIXME: sprite size shouldn't be hardcoded
  if (p->position.vy > SCREEN_HEIGHT + 8) p->is_dead = 1;

  p->col_shape.position.vx = p->position.vx;
  p->col_shape.position.vy = p->position.vy;
  update_player_sprite_xy(p);
}

#define JUMP_BUTTON_MASK PADRdown
void g_handle_player_input(Player_t *p) {
  static int last_padd;
  int padd;
  long tmp;

  if (p->controller[0] != 0) {
    FntPrint("Padd error!\n");
    return;
  }

  padd = ~((p->controller[2] << 8) | (p->controller[3]));

  if (s_curr_game_state == S_GSTATE_GAME_PAUSED && (padd & JUMP_BUTTON_MASK)) {
    // TODO: MOVE SEEDING TO A BETTER PLACE.
    // Currently placed here to make it easy.
    // Remove kernel.h and rand.h when you remove this from the script.
    tmp = GetRCnt(RCntCNT0);
    printf("Seed: %ld\n", tmp);
    srand(tmp);
    s_curr_game_state = S_GSTATE_NORMAL;
  } else if ((padd & JUMP_BUTTON_MASK) && !(last_padd & JUMP_BUTTON_MASK))
    p->velocity = JUMP_VELOCITY;

  last_padd = padd;
}

void g_player_loop(Player_t *p) {
  g_handle_player_input(p);
  g_update_player_sprite_pos(p);

#ifdef WIREFRAME
  g_update_AABB_wireframe_xy(&p->col_shape);
#endif /* WIREFRAME */
}

void update_player_sprite_xy(Player_t *p) {
  // TODO: sprite dimensions shouldn't be hardcoded.
  setXY4(&p->sprite, (p->position.vx) - 8, (p->position.vy) + 8,
         (p->position.vx) + 8, (p->position.vy) + 8, (p->position.vx) - 8,
         (p->position.vy) - 8, (p->position.vx) + 8, (p->position.vy) - 8);
}
