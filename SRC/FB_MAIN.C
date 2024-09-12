#include "FB_DEFS.H"
#include "G_PIPE.H"
#include "G_PLAYER.H"
#include "S_CONTRL.H"
#include "S_GSTATE.H"
#include "S_HBCNTR.H"
#include "S_MAIN.H"
#include "V_MAIN.H"

int main(void) {
  Player_t player;
  Pipe_t pipe;

  v_init();
  s_init();

  player = g_create_player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, controller_1);
  pipe = g_create_pipe();
  add_sprite_to_sprites(&pipe.sprite);
  add_sprite_to_sprites(&player.sprite);

  while (1) {
    s_sys_start();

    switch (s_curr_game_state) {
      case S_GSTATE_NORMAL:
        g_player_loop(&player);

        if (player.is_dead) s_curr_game_state = S_GSTATE_GAME_OVER;
        break;
      case S_GSTATE_GAME_OVER:
        FntPrint("Game Over!\n");
        break;
      default:
        FntPrint("Unhandled game state: %d\n", s_curr_game_state);
        break;
    }

    v_loop();

    FntPrint("dt: %d\n", s_delta_time());
    s_sys_end();
  }

  return 0;
}
