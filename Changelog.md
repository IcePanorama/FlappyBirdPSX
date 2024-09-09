# Changelog

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
