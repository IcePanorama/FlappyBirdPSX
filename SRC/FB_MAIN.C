#include "FB_DEFS.H"
#include "G_MAIN.H"
#include "S_GSTATE.H"
#include "S_HBCNTR.H"
#include "S_MAIN.H"
#include "V_MAIN.H"

int main(void) {
#ifdef WIREFRAME
  int i;
#endif /* WIREFRAME */

  v_init();
  s_init();
  g_init();

  while (1) {
    s_sys_start();

    switch (s_curr_game_state) {
      case S_GSTATE_NORMAL:
        g_loop();

        if (player.is_dead) s_curr_game_state = S_GSTATE_GAME_OVER;
        break;
      case S_GSTATE_GAME_OVER:
        FntPrint("Game Over!\n");
        break;
      case S_GSTATE_GAME_PAUSED:
        FntPrint("Game Paused...\n");
        g_handle_player_input(&player);
        break;
      default:
        FntPrint("Unhandled game state: %d\n", s_curr_game_state);
        break;
    }

    add_sprite_to_sprites(&player.sprite);

#ifdef WIREFRAME
    for (i = 0; i < NUM_WIREFRAME_LNS; i++)
      v_add_wire_to_wireframes(&player.col_shape.wireframe[i]);
#endif /* WIREFRAME */

    v_loop();

    FntPrint("dt: %d\n", s_delta_time());
    s_sys_end();
  }

  return 0;
}

/*
 *  TODO:
 *    * Collision detection
 *      * Use `AABB_t` to detect collisions.
 *      * Go to `S_GSTATE_GAME_OVER` if `Player_t` collides with `Pipe_t`.
 *    * Scoring
 */
