# Changelog

## 2024-09-12
+ Created `add_sprite_to_sprites`.
+ Added `sprites`, `curr_sprite` to `ScreenBuffer_t` datatype.
+ Created `Pipe_t` ctor.
+ Created `S_MAIN.H`.
+ Created `V_MAIN.H`, removed `V_VIDEO.H`.
+ Created `FB_DEFS.H` for global defines.
+ Created `Pipe_t` datatype.
+ Displaying "Game Over" message when the player falls below the screen.
+ Created `GameState_t` datatype, global `s_curr_game_state` variable.
+ Players can no longer hold down the jump button to continuously jump.
+ Created `VELOCITY_LERP_VAL` to make jumping more floaty.
+ Created `velocity` data field on `Player_t` datatype.

## 2024-09-09
+ Basic user input is working, need to make it player velocity parabolic/more floaty.
+ Set up controller support, linking `Player_t` instances to specific controllers for later.
+ Player now drops due to gravity, using delta time to do so smoothly.
+ Created `s_delta_time` and its associated functions.
+ Created `Player_t` ctor.
+ Updating player sprite position in G_PLAYER.
+ Drawing player sprite using player position.

## 2024-09-05
+ Created `display_video`.
+ Created `init_video_subsystem`.
+ Created `ScreenBuffer_t`, `ScreenBuffer_s` datatypes.
+ Created project.
