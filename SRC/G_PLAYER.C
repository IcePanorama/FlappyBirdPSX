#include <libetc.h>
#include <libpad.h>
#include <stdio.h>

#include "G_PLAYER.H"
#include "S_HBCNTR.H"

// Approximation for (9.8 / 2.0) m/s
#define GRAVITY -49
#define JUMP_VELOCITY 98
#define VELOCITY_LERP_VAL 7

Player_t g_create_player(short start_x, short start_y, u_char *controller) {
  Player_t p;

  SetPolyF4(&p.sprite);

  SetShadeTex(&p.sprite, 1);
  setRGB0(&p.sprite, 0, 255, 0);

  p.position.vx = start_x;
  p.position.vy = start_y;

  p.controller = controller;

  p.velocity = GRAVITY;

  p.is_dead = 0;

  return p;
}

void g_update_player_sprite_pos(Player_t *p) {
  // FIXME: uncomment this later.
  /*
  p->position.vy -= p->velocity * s_delta_time () / 4096;
  if (p->velocity > GRAVITY)
    p->velocity -= VELOCITY_LERP_VAL;

  if (p->position.vy > 240 + 8) p->is_dead = 1;
  */

  // TODO: sprite dimensions shouldn't be hardcoded.
  setXY4(&p->sprite, (p->position.vx) - 8, (p->position.vy) + 8,
         (p->position.vx) + 8, (p->position.vy) + 8, (p->position.vx) - 8,
         (p->position.vy) - 8, (p->position.vx) + 8, (p->position.vy) - 8);
}

#define JUMP_BUTTON_MASK PADRdown
void g_handle_player_input(Player_t *p) {
  static int last_padd;
  int padd;

  if (p->controller[0] != 0) {
    FntPrint("Padd error!\n");
    return;
  }

  padd = ~((p->controller[2] << 8) | (p->controller[3]));

  if ((padd & JUMP_BUTTON_MASK) && !(last_padd & JUMP_BUTTON_MASK))
    p->velocity = JUMP_VELOCITY;

  last_padd = padd;
}

void g_player_loop(Player_t *p) {
  g_handle_player_input(p);
  g_update_player_sprite_pos(p);
}
