#include <libetc.h>
#include <libpad.h>

#include "G_PLAYER.H"
#include "S_HBCNTR.H"

// Approximation for 9.8 m/s / 2.0
#define GRAVITY 49
#define JUMP_VELOCITY 98

Player_t g_create_player(short start_x, short start_y, u_char *controller) {
  Player_t p;

  SetPolyF4(&p.sprite);

  SetShadeTex(&p.sprite, 1);
  setRGB0(&p.sprite, 0, 255, 0);

  p.position.vx = start_x;
  p.position.vy = start_y;

  p.controller = controller;

  return p;
}

void g_update_player_sprite_pos(Player_t *p) {
  p->position.vy += GRAVITY * s_delta_time() / 4096;

  // tmp, just put the player back on screen if they're too far gone.
  if (p->position.vy > 480) p->position.vy = 0;

  // TODO: sprite size shouldn't be hardcoded.
  setXY4(&p->sprite, (p->position.vx) - 8, (p->position.vy) + 8,
         (p->position.vx) + 8, (p->position.vy) + 8, (p->position.vx) - 8,
         (p->position.vy) - 8, (p->position.vx) + 8, (p->position.vy) - 8);
}

void g_handle_player_input(Player_t *p) {
  int padd;

  if (p->controller[0] != 0) {
    FntPrint("Padd error!\n");
    return;
  }

  padd = ~((p->controller[2] << 8) | (p->controller[3]));

  if (padd & PADRdown) p->position.vy -= JUMP_VELOCITY * s_delta_time() / 4096;
}
