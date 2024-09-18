#ifndef _G_AXIS_ALIGNED_BOUNDING_BOX_H_
#define _G_AXIS_ALIGNED_BOUNDING_BOX_H_

#include <libgpu.h>
#include <libgte.h>
#include <sys/types.h>

#include "FB_DEFS.H"

typedef struct AABB_s {
  VECTOR position;
  u_int width;
  u_int height;
#ifdef WIREFRAME
  LINE_F2 wireframe[NUM_WIREFRAME_LNS];
#endif /* WIREFRAME */
} AABB_t;

/** Detects if two `AABB_t`s are colliding. Returns 1 if true, else 0. */
int g_are_colliding(AABB_t *a, AABB_t *b);

#ifdef WIREFRAME
void g_init_AABB_wireframe(AABB_t *a);
void g_update_AABB_wireframe_xy(AABB_t *a);
#endif /* WIREFRAME */

#endif /* _G_AXIS_ALIGNED_BOUNDING_BOX_H_ */
