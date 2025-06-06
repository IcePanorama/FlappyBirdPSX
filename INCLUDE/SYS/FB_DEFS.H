#ifndef _FLAPPY_BIRD_PSX_DEFS_H_
#define _FLAPPY_BIRD_PSX_DEFS_H_

// FIXME: should this be in `video/vid_defs.h` instead?
#define FB_SCREEN_WIDTH (320)
#define FB_SCREEN_HEIGHT (240)
#define FB_HALF_SCREEN_WIDTH ((FB_SCREEN_WIDTH) >> 1)
#define FB_HALF_SCREEN_HEIGHT ((FB_SCREEN_HEIGHT) >> 1)

#define FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS (5)
#define FB_MOVEMENT_SPEED (45)

#define FB_FOREGROUND_HEIGHT (40)

#define FB_PLAYER_SIZE (20)

#endif /* _FLAPPY_BIRD_PSX_DEFS_H_ */
