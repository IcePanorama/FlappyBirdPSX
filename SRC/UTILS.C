#include "utils.h"
#include "sys/fb_defs.h"

void
v2_convert_world_space_to_camera_space (Vec2_t *v2_input, Vec2_t *v2_output)
{
  v2_output->x =
    (v2_input->x >> WORLD_TO_CAMERA_SPACE_NUM_SHIFTS) + (HALF_SCREEN_WIDTH);
  v2_output->y =
    (v2_input->y >> WORLD_TO_CAMERA_SPACE_NUM_SHIFTS) + (HALF_SCREEN_HEIGHT);
}
