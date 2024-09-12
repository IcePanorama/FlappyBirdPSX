#ifndef _G_PLAYER_H_
#define _G_PLAYER_H_

#include <libgpu.h>
#include <libgte.h>
#include <sys/types.h>

typedef struct Player_s {
  POLY_F4 sprite;
  SVECTOR position;    // FIXME: We really only need the player's y position
  u_char *controller;  // Controller associated w/ this player.
  int velocity;
  short is_dead;
} Player_t;

/** Initializes and returns a `Player_t`. */
Player_t g_create_player(short start_x, short start_y, u_char *controller);

/**
 *  Sets the vertex coordinate positions for `sprite` using the player's (`p`)
 *  position data.
 */
void g_update_player_sprite_pos(Player_t *p);

// Not sure that I love this being in this file.
/** Handles user input for the given player, `p`. */
void g_handle_player_input(Player_t *p);

/** Main player loop during normal execution. */
void g_player_loop(Player_t *p);

#endif /* _G_PLAYER_H_ */
