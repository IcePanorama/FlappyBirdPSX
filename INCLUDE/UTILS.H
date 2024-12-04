#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "sys/fb_vecs.h"

/*
 *  TODO:
 *  + create macro function for release builds.
 *  + rename this to fit w/ current naming scheme
 *  Should this be in fb_vecs.h instead??
 */
void v2_convert_world_space_to_camera_space (Vec2_t *v2_input,
					     Vec2_t *v2_output);

void u_seed_rng (void);

#endif /* _UTILITIES_H_ */
