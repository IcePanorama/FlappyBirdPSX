# Changelog

## 2024-09-?? to 2024-10-??
+ Reimplemented collision shapes, created separate positional collision shapes for pipes.
+ Reimplemented game state, game state management.
+ Reimplemented spawning/despawning of pipes, now through a pipes pool.
+ Reimplemented `Pipes_t` data type.
+ Reimplemented `Player_t` data type, reimplemented user input handling using commands.
+ Created `Sprite_t` data type, implemented sprite pool.
+ Implemented 'command' design pattern for handling user input.
+ Implemented 'game loop' design pattern.
+ Reworked delta time implementation.
+ Created `STDINT.H`.
+ Refactored entire code base.

## 2024-09-18
+ Refactored code.
+ Verified that compiling without WIREFRAME still works.
+ Renamed DEBUG compile flag to WIREFRAME.
+ Added debug-only functionality to draw wireframes on top of sprites.
+ Added debug-only function `v_add_wire_to_wireframes` to `V_MAIN`.
+ Added debug-only wireframe to `AABB_t`, created functions to update wireframe `LINE_F2` positions.
+ Removed `_GET_RAND_VY` macro function.
+ Added `AABB_t` to `Player_t` datatype.
+ Created `AABB_t`, `AABB_s` datatype for collision detection.
+ Moved global variable `player` and related functions to `G_MAIN`, refactored code.
+ Created `G_MAIN.H`.
+ Implemented basic game manager/auto-pipe generator.

## 2024-09-16
+ Started work on game manager/auto-pipe generation.
+ Created `G_MANGER.H`.
+ Got dev environment set up in Windows 98.

## 2024-09-13
+ Randomly setting the height of the pipe gap.
+ Srand gets seeded using dot clock + user input.
+ Pipes now spawn off screen.
+ Player now presses the jump button at the start of the game to unpause it.
+ Game now starts in a `S_GSTATE_GAME_PAUSED` state for psuedorandomness purposes.
+ Created `update_player_sprite_xy`, refactored G_PLAYER.C.
+ Pipes move from left to right across the screen.
+ Drawing the tops and bottom of pipes sprites.

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
