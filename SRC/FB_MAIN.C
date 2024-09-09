#include <stdio.h>

#include "G_PLAYER.H"
#include "S_CONTRL.H"
#include "S_HBCNTR.H"
#include "V_VIDEO.H"

static ScreenBuffer_t screen_buffer[2];

int main(void) {
  Player_t player;

  // TODO: create inits for all the various subsystems
  // e.g., video_init, sys_init, etc.
  v_init_video_subsystem(screen_buffer);
  s_init_controllers();

  player = g_create_player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, controller_1);

  while (1) {
    s_sys_start();

    g_handle_player_input(&player);
    g_update_player_sprite_pos(&player);
    v_display_video(screen_buffer, &player.sprite);
    FntPrint("dt: %d\n", s_delta_time());

    s_sys_end();
  }

  return 0;
}
